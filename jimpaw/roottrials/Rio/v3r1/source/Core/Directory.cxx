//
// A ROOT file is structured in Directories (like a file system).
// Each Directory has a list of Keys 
// and a list of objects in memory.
// A Key is a small object that describes the type and location
// of a persistent data in a file. The persistent data may be a directory.
//

// this :
#include <Rio/Core/Directory.h>

#include <Rio/Interfaces/IObject.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IPrinter.h>
#include <Rio/Core/tobuf.h>
#include <Rio/Core/File.h>
#include <Rio/Core/Key.h>
#include <Rio/Core/Buffer.h>
#include <Rio/Core/smanip.h>
#include <Rio/Core/Out.h>

#define BIT(n)       (1 << (n))

//----- Write() options
enum {
  Rio_kSingleKey = BIT(0),        // write collection with single key
  Rio_kOverWrite = BIT(1)         // overwrite existing object with same name
};

RIO_META_SOURCE(Rio::Directory)

//////////////////////////////////////////////////////////////////////////////
Rio::Directory::Directory(
 File& aFile
,Directory* aParent
)
:Named("","")
,fFile(aFile)
,fParent(aParent)
,fIsClosing(false)
,fModified(false)
,fWritable(false)
// Record :
,fNbytesKeys(0)
,fNbytesName(0)
,fSeekDirectory(0)
,fSeekParent(0)
,fSeekKeys(0)
//////////////////////////////////////////////////////////////////////////////
// Used when reading a file.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(file().verboseLevel()) {
    Out out(printer());
    out << "Rio::Directory::Directory1 : called for " 
	<< this << " \"" << name() << "\"." << endl;
  }

  aParent->appendObject(this);
}
//////////////////////////////////////////////////////////////////////////////
Rio::Directory::Directory(
 File& aFile
,Directory* aParent
,const std::string& aName
,const std::string& aTitle
)
:Named(aName,aTitle)
,fFile(aFile)
,fParent(aParent)
,fIsClosing(false)
,fModified(false)
,fWritable(false)
// Record :
,fNbytesKeys(0)
,fNbytesName(0)
,fSeekDirectory(0)
,fSeekParent(0)
,fSeekKeys(0)
//////////////////////////////////////////////////////////////////////////////
//  A new directory with name,title is created in the parent directory
//  The directory header information is immediatly saved on the file
//  A new key is added in the parent directory
//
//  FIXME : When this constructor is called from a class directly derived
//  from TDirectory, the third argument classname MUST be specified.
//  In this case, classname must be the name of the derived class.
//
//  Note that the directory name cannot contain slashes.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Out out(printer());
  if(file().verboseLevel()) {
    out << "Rio::Directory::Directory : called for " 
	<< this << " \"" << name() << "\"." << endl;
  }

  if(fName=="") {
    out << "Rio::Directory::Directory : "
	<< "directory name cannot be \"\"." << endl;
    return; //FIXME ? what to do.
  }
  if(fName.find("/")!=std::string::npos) {
    out << "Rio::Directory::Directory : "
	<< "directory name \"" << name() 
	<< "\" cannot contain a slash." << endl;
    return; //FIXME ? what to do.
  }

  if(fTitle=="") fTitle = fName;

  initialize();

  if(!aParent) {
    fWritable = true; //root directory of fFile.
    return;
  }

  if(!fFile.isWritable()) return; //*-* in case of a directory in memory
  if(aParent->getKey(fName)) {
    out << "Rio::Directory::Directory : directory \"" 
	<< name() << "\" exists already." << endl;
    return;
  }

  fWritable = true;
  fSeekParent= aParent->seekDirectory();
  int nbytes = recordSize();
  Key* key = new Key(fFile,aParent->seekDirectory(),
		     fName,fTitle,sClass->name(),nbytes);
  fNbytesName = key->keyLength();
  fSeekDirectory = key->seekKey();
  if(fSeekDirectory == 0) return;
  char* buffer = key->buffer();
  fillBuffer(buffer);
  int cycle = aParent->appendKey(key);
  key->writeFile(cycle);

  fModified = false;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Directory::Directory(
 const Directory& aDirectory
)
:Named("","")
,fFile(aDirectory.fFile)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::operator=(
 const Directory& 
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::Directory::~Directory(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Out out(printer());
  if (fFile.verboseLevel()) {
    out << "Rio::Directory::~Directory : called for \"" 
	<< name() << "\" : begin." << endl;
  }

  //TCollection::StartGarbageCollection();

  fIsClosing = true;

  std::list<IObject*>::iterator ito;
  for(ito=fObjects.begin();ito!=fObjects.end();ito=fObjects.erase(ito)) 
    delete (*ito);

  std::list<Key*>::iterator itk;
  for(itk=fKeys.begin();itk!=fKeys.end();itk=fKeys.erase(itk)) delete (*itk);

  //TCollection::EmptyGarbageCollection();

  if (fFile.verboseLevel()) {
    out << "Rio::Directory::~Directory : called for \"" 
	<< name() << "\" : end." << endl;
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::initialize(
)
//////////////////////////////////////////////////////////////////////////////
// Initialise directory to defaults :
// If directory is created via default ctor (when dir is read from file)
// don't add it here to the directory since its name is not yet known.
// It will be added to the directory in TKey::ReadObj().
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fParent && fName.size()) fParent->appendObject(this);
  fModified = true;
  fWritable = false;
  fDateC.set();
  fDateM.set();
  fNbytesKeys = 0;
  fSeekDirectory = 0;
  fSeekParent = 0;
  fSeekKeys = 0;
  //FIXME SetBit(kCanDelete);
}
//////////////////////////////////////////////////////////////////////////////
Rio::IPrinter& Rio::Directory::printer(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fFile.printer();
}
//////////////////////////////////////////////////////////////////////////////
Rio::IDirectory* Rio::Directory::parent(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fParent;
}
//////////////////////////////////////////////////////////////////////////////
std::vector<Rio::IKey*> Rio::Directory::keys(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<Rio::IKey*> keys;
  std::list<Key*>::const_iterator it;
  for(it=fKeys.begin();it!=fKeys.end();++it) keys.push_back(*it);
  return keys;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::Directory::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (Directory*)this;
  else return Named::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Directory::name(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return Named::name();
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::isWritable(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fWritable; 
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::isClosing(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fIsClosing;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IFile& Rio::Directory::file(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fFile;
}
//////////////////////////////////////////////////////////////////////////////
std::string Rio::Directory::path(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string path = "";
  const IDirectory* directory = this;
  while(directory) {
    std::string s = path;
    path = "/";
    path += directory->name();
    path += s;
    directory = directory->parent();
  }
  return path;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::setSeekDirectory(
 Seek aSeek
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fSeekDirectory = aSeek;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Seek Rio::Directory::seekKeys(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fSeekKeys;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Seek Rio::Directory::seekDirectory(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fSeekDirectory;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Directory::nBytesName(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fNbytesName;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::setNbytesName(
 int aNbytesName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fNbytesName = aNbytesName;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Key* Rio::Directory::getKey(
 const std::string& aName
,short aCycle
)
//////////////////////////////////////////////////////////////////////////////
// Return pointer to key with name, cycle
//  if aCycle = 9999 returns highest cycle
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<Key*>::iterator itk;
  for(itk=fKeys.begin();itk!=fKeys.end();++itk) {
    if((*itk)->name()==aName) {
      if(aCycle==9999) return *itk;
      if(aCycle>=(*itk)->cycle()) return *itk;
    }
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Key* Rio::Directory::findKey(
 const std::string& aName
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<Key*>::const_iterator itk;
  for(itk=fKeys.begin();itk!=fKeys.end();++itk) {
    if((*itk)->name()==aName) return *itk;
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::removeKey(
 Rio::Key* aKey
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fKeys.remove(aKey);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::appendObject(
 IObject* aObject
)
//////////////////////////////////////////////////////////////////////////////
// Append object to current directory.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aObject) return;
  if (fFile.verboseLevel()) {
    Out out(printer());
    out << "Rio::Directory::appendObject : directory " 
	<< this << " \"" << name() << "\" ; appending " 
	<< aObject << " \"" << aObject->name() << "\" of class \""
	<< aObject->isA().name() << "\"." << endl;
  }

  fObjects.push_back(aObject);

  /*FIXME
  if (!fMother) return;
  if (fMother->IsA() == TMapFile::Class()) {
    TMapFile *mfile = (TMapFile*)fMother;
    mfile->Add(obj);
    }*/
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::removeObject(
 IObject* aObject
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aObject) return;
  if (fFile.verboseLevel()) {
    Out out(printer());
    out << "Rio::Directory::removeObject : directory "
	<< this << " \"" << name() << "\" ; appending "
	<< aObject << " \"" << aObject->name() << "\" of class \""
	<< aObject->isA().name() << "\"." << endl;
  }

  fObjects.remove(aObject);

}
//////////////////////////////////////////////////////////////////////////////
int Rio::Directory::appendKey(
 Key* aKey
)
//////////////////////////////////////////////////////////////////////////////
// Insert key in the list of keys of this directory.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fModified = true;

  std::list<Key*>::iterator itk;
  for(itk=fKeys.begin();itk!=fKeys.end();++itk) {
    if((*itk)->name()==aKey->name()) {
      fKeys.insert(itk,aKey); //aKey will be before *itk.
      return ((*itk)->cycle() + 1);
    }
  }

  // Not found :
  fKeys.push_back(aKey);
  return 1;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Directory::recordSize(
) const
//////////////////////////////////////////////////////////////////////////////
// (Name, title) are stored in the (name, title) of the associated key.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int nbytes = sizeof(short);
  nbytes += fDateC.recordSize();
  nbytes += fDateM.recordSize();
  nbytes += sizeof fNbytesKeys;
  nbytes += sizeof fNbytesName;
  nbytes += sizeof fSeekDirectory;
  nbytes += sizeof fSeekParent;
  nbytes += sizeof fSeekKeys;
  return nbytes;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::fillBuffer(
 char*& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Encode directory header into output buffer.
// (Name, title) are stored in the (name, title) of the associated key.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  short version = sClass->version();
  Rio_tobuf(aBuffer, version);
  Rio_tobuf(aBuffer, fDateC.date());
  Rio_tobuf(aBuffer, fDateM.date());
  Rio_tobuf(aBuffer, fNbytesKeys);
  Rio_tobuf(aBuffer, fNbytesName);
  Rio_tobuf(aBuffer, fSeekDirectory);
  Rio_tobuf(aBuffer, fSeekParent);
  Rio_tobuf(aBuffer, fSeekKeys);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::readBuffer(
 char*& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Decode input buffer.
// (Name, title) are stored in the (name, title) of the associated key.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  short versiondir;
  Rio_frombuf(aBuffer,&versiondir);
  unsigned int date;
  Rio_frombuf(aBuffer,&date);fDateC.setDate(date);
  Rio_frombuf(aBuffer,&date);fDateM.setDate(date);
  Rio_frombuf(aBuffer, &fNbytesKeys);
  Rio_frombuf(aBuffer, &fNbytesName);
  Rio_frombuf(aBuffer, &fSeekDirectory);
  Rio_frombuf(aBuffer, &fSeekParent);
  Rio_frombuf(aBuffer, &fSeekKeys);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Stream a class object
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    initialize();
    if (fFile.isWritable()) fWritable = true;
    aBuffer.readVersion();
    unsigned int date;
    aBuffer.read(date);fDateC.setDate(date);
    aBuffer.read(date);fDateM.setDate(date);
    aBuffer.read(fNbytesKeys);
    aBuffer.read(fNbytesName);
    aBuffer.read(fSeekDirectory);
    aBuffer.read(fSeekParent);
    aBuffer.read(fSeekKeys);
    if (fSeekKeys) readKeys();
  } else {
    aBuffer.writeVersion(sClass->version());
    aBuffer.write(fDateC.date());
    aBuffer.write(fDateM.date());
    aBuffer.write(fNbytesKeys);
    aBuffer.write(fNbytesName);
    aBuffer.write(fSeekDirectory);
    aBuffer.write(fSeekParent);
    aBuffer.write(fSeekKeys);
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::writeKeys(
)
//////////////////////////////////////////////////////////////////////////////
// Write KEYS list on the file :
//  The list of keys (fKeys) is written as a single data record
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Delete the old keys structure if it exists
  if(fSeekKeys) {
    fFile.makeFreeSegment(fSeekKeys, fSeekKeys + fNbytesKeys -1);
  }
  // Write new keys record :
  int nkeys  = fKeys.size();
  int nbytes = sizeof nkeys;          // Compute size of all keys
  std::list<Key*>::iterator itk;
  for(itk=fKeys.begin();itk!=fKeys.end();++itk) {
    nbytes += (*itk)->keyLength();
  }
  Key headerkey(fFile,seekDirectory(),
		fName,fTitle,sClass->name(),nbytes);
  if (headerkey.seekKey() == 0) return;
  char *buffer = headerkey.buffer();

  Rio_tobuf(buffer, nkeys);
  std::list<Key*>::iterator it;
  for(it=fKeys.begin();it!=fKeys.end();++it) {
    (*it)->fillBuffer(buffer);
  }
  
  fSeekKeys = headerkey.seekKey();
  fNbytesKeys = headerkey.numberOfBytes();

  if(fFile.verboseLevel()) {
    Out out(printer());
    out << "Rio::Directory::writeKeys : write header key \""
	<< name() << "\" \"" << fTitle << "\" \"" 
	<< sClass->name() << "\" (" << nkeys << ", " 
	<< fKeys.size() << ", " << nbytes << ", " 
	<< fSeekKeys << ", " << fNbytesKeys << "):" << endl;
  }

  headerkey.writeFile();
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Directory::readKeys(
)
//////////////////////////////////////////////////////////////////////////////
// Read the KEYS :
//  Every directory has a list (fKeys). This list has been
//  written on the file via writeKeys as a single data record.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<Key*>::iterator it;
  for(it=fKeys.begin();it!=fKeys.end();it=fKeys.erase(it)) delete (*it);

  //In case directory was updated by another process, read new
  //position for the keys
  int nbytes = fNbytesName + recordSize();
  char *header = new char[nbytes];
  char *buffer = header;
  fFile.setPosition(fSeekDirectory);
  fFile.readBuffer(buffer,nbytes);
  buffer += fNbytesName;
  readBuffer(buffer);
  delete [] header;

  Key headerkey(fFile,fSeekKeys,fNbytesKeys);
  headerkey.readFile();
  buffer = headerkey.buffer();
  headerkey.readBuffer(buffer);
  int nkeys;
  Rio_frombuf(buffer, &nkeys);
  for (int i = 0; i < nkeys; i++) {
    Key* key = new Key(fFile);
    key->readBuffer(buffer);
    fKeys.push_back(key);
  }
  
  return nkeys;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Directory::write(
)
//////////////////////////////////////////////////////////////////////////////
// Write all objects in memory to disk.
// Loop on all objects in memory (including subdirectories).
// A new key is created in the KEYS linked list for each object.
// For allowed options see TObject::Write().
// The directory header info is rewritten on the directory header record
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fFile.verboseLevel()) {
    Out out(printer());
    out << "Rio::Directory::write : \"" << name() << "\"(" 
	<< isWritable() << ") : " << fObjects.size() << " objects." << endl;
  }

  if (!isWritable()) return 0;

  int nbytes = 0;

  // Loop on all objects (including subdirs)
  std::list<IObject*>::iterator it;
  for(it=fObjects.begin();it!=fObjects.end();++it) {
    Directory* dir = Rio::cast<Directory>(*it);
    if(dir) {
      nbytes += dir->write();
    } else {
      nbytes += writeObject(*(*it));
    }
  }

  saveSelf(true);   // force save itself
  
  return nbytes;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::saveObject(
 IObject& aObject
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Key* key = findKey(aObject.name());
  //wOK will be 0 if Write failed (disk space exceeded)
  int wOK = writeObject(aObject); 
  if (wOK && key) {
    key->destroy();
    removeKey(key);
    delete key;
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::saveSelf(
 bool aForce
)
//////////////////////////////////////////////////////////////////////////////
// Save Directory keys and header :
//  If the directory has been modified (fModified set), write the keys
//  and the directory header. This function assumes the cd is correctly set.
//
//  It is recommended to use this function in the following situation:
//  Assume a process1 using a directory in Update mode
//    -New objects or modified objects have been written to the directory
//    -You do not want to close the file
//    -You want your changes be visible from another process2 already connected
//     to this directory in read mode
//    -Call this function
//    -In process2, use TDirectory::ReadKeys to refresh the directory
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (isWritable() && (fModified || aForce)) {
    if(!fFile.freeSegments().empty()) {
      writeKeys(); // Write keys record.
      writeHeader(); // Update directory record.
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::writeHeader(
)
//////////////////////////////////////////////////////////////////////////////
// Overwrite the Directory header record.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int nbytes = recordSize();
  char* header = new char[nbytes];
  char* buffer = header;
  fDateM.set();
  fillBuffer(buffer);
  // do not overwrite the name/title part
  Seek pointer = fSeekDirectory + fNbytesName; 
  fModified = false;
  fFile.setPosition(pointer);
  fFile.writeBuffer(header, nbytes);
  fFile.synchronize();
  delete [] header;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::close(
)
//////////////////////////////////////////////////////////////////////////////
// Delete all objects from memory and directory structure itself.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME TCollection::StartGarbageCollection();

  // Save the directory key list and header
  save();

  // Delete objects from directory list, this in turn, recursively closes all
  // sub-directories (that were allocated on the heap)
  fIsClosing = true;

  std::list<IObject*>::iterator ito;
  for(ito=fObjects.begin();ito!=fObjects.end();ito=fObjects.erase(ito)) {
    if (fFile.verboseLevel()) {
      Out out(printer());
      out << "Rio::Directory::close : directory \"" << name()
	  << "\" ; deleting \"" << (*ito)->name() 
	  << "\" of class \"" << (*ito)->isA().name() << "\"." << endl;
    }

    delete (*ito);

    if (fFile.verboseLevel()) {
      Out out(printer());
      out << "Rio::Directory::close : directory \"" 
	  << name() << "\" ; deleting end." << endl;
    }
  }

  // Delete keys from key list (but don't delete the list header)
  std::list<Key*>::iterator itk;
  for(itk=fKeys.begin();itk!=fKeys.end();itk=fKeys.erase(itk)) delete (*itk);

  //FIXME : TCollection::EmptyGarbageCollection();
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::save(
)
//////////////////////////////////////////////////////////////////////////////
// Save recursively all directory keys and headers.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  saveSelf();

  // recursively save all sub-directories
  std::list<IObject*>::iterator it;
  for(it=fObjects.begin();it!=fObjects.end();++it) {
    Directory* dir = Rio::cast<Directory>(*it);
    if(dir) dir->save();
  }
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Directory::writeObject(
 IObject& aObject
)
//////////////////////////////////////////////////////////////////////////////
// Write this object to the current directory
// The data structure corresponding to this object is serialized.
// The corresponding buffer is written to the current directory
// with an associated key with name "name".
//
// Writing an object to a file involves the following steps:
//
//  -Creation of a support TKey object in the current directory.
//   The TKey object creates a TBuffer object.
//
//  -The TBuffer object is filled via the class::Streamer function.
//
//  -If the file is compressed (default) a second buffer is created to
//   hold the compressed buffer.
//
//  -Reservation of the corresponding space in the file by looking
//   in the TFree list of free blocks of the file.
//
//  -The buffer is written to the file.
//
//  Bufsize can be given to force a given buffer size to write this object.
//  By default, the buffersize will be taken from the average buffer size
//  of all objects written to the current file so far.
//
//  If a name is specified, it will be the name of the key.
//  If name is not given, the name of the key will be the name as returned
//  by GetName().
//
//  The option can be a combination of:
//    kSingleKey and kOverwrite
//  Using the kOverwrite option a previous key with the same name is
//  overwritten.
//  The kSingleKey option is only used by TCollection::Write() to write
//  a container with a single key instead of each object in the container
//  with its own key.
//
//  An object is read from the file into memory via TKey::Read() or
//  via TObject::Read().
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fFile.isWritable()) {
    Out out(printer());
    out << "Rio::Directory::writeObject : file \"" 
	<< fFile.name() << "\" is not writable." << endl;
    return 0;
  }

  int bsize = fFile.bestBuffer();

  std::string name = aObject.name();
  Rio::smanip smanip;
  smanip.strip(name);

  /*
  if ((aOption & Rio_kOverWrite)) {
    std::list<Key*>::iterator it;
    for(it=fKeys.begin();it!=fKeys.end();++it) {
      if((*it)->name()==name) {
	delete *it;
	fKeys.erase(it);
	break;
      }
    }
  }
  */

  Key* key = new Key(fFile,*this,aObject,name,bsize);
  
  if (!key->seekKey()) {
    fKeys.remove(key);
    delete key;
    return 0;
  }

  fFile.sumBuffer(key->objectSize());

  if(fFile.verboseLevel()) {
    Out out(printer());
    out << "Rio::Directory::writeObject : \"" 
	<< aObject.name() << "\"." << endl;
  }

  return key->writeFile();
}
//////////////////////////////////////////////////////////////////////////////
Rio::IDirectory* Rio::Directory::mkdir(
 const std::string& aName
,const std::string& aTitle
)
//////////////////////////////////////////////////////////////////////////////
// Create a sub-directory and return a pointer to the created directory.
// Returns 0 in case of error.
// Note that the directory name cannot contain slashes.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aName=="") {
    Out out(printer());
    out << "Rio::Directory::mkdir : directory name cannot be \"\"." << endl;
    return 0;
  }
  if(aName.find("/")!=std::string::npos) {
    Out out(printer());
    out << "Rio::Directory::mkdir : directory name \"" 
	<< aName << "\" cannot contain a slash." << endl;
    return 0;
  }
  return new Directory(fFile,this,aName,aTitle==""?aName:aTitle);
}
//////////////////////////////////////////////////////////////////////////////
Rio::IDirectory* Rio::Directory::findDirectory(
 const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  IObject* object = find(aName);
  if(!object) {
    Out out(printer());
    out << "Rio::Directory::findDirectory : object \"" 
	<< aName << "\" not found ." << endl; 
    return 0;
  }
  IDirectory* dir = Rio::cast<Directory>(object);
  if(!dir) {
    Out out(printer());
    out << "Rio::Directory::findDirectory : object \"" 
	<< aName << "\" not a directory." << endl; 
    return 0;
  }
  return dir;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IObject* Rio::Directory::find(
 const std::string& aNameCycle
)
//////////////////////////////////////////////////////////////////////////////
// Return pointer to object identified by namecycle.
//   namecycle has the format name;cycle
//   name  = * is illegal, cycle = * is illegal
//   cycle = "" or cycle = 9999 ==> apply to a memory object
//
//   examples:
//     foo   : get object named foo in memory
//             if object is not in memory, try with highest cycle from file
//     foo;1 : get cycle 1 of foo on file
//
// WARNING: Never use Directory::find when namecycle is a directory itself.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  short cycle;
  std::string name;
  decodeNameCycle(aNameCycle,name,cycle);

  if(fFile.verboseLevel()) {
    Out out(printer());
    out << "Rio::Directory::find : namecycle \"" 
	<< aNameCycle << "\" : \"" << name << "\" " << cycle << endl;
  }

  // Case of Object in memory :
  std::list<IObject*>::const_iterator ito;
  for(ito=fObjects.begin();ito!=fObjects.end();++ito) {
    if((*ito)->name()==name) {
      if (cycle == 9999) return *ito;
      /*FIXME
	if (idcur->InheritsFrom(TCollection::Class()))
	idcur->Delete();  // delete also list elements
	delete idcur; //FIXME ?
      */
      break;
    }
  }

  // Case of Key :
  std::list<Key*>::iterator itk;
  for(itk=fKeys.begin();itk!=fKeys.end();++itk) {
    if((*itk)->name()==name) {
      if ((cycle == 9999) || (cycle == (*itk)->cycle())) {
	return (*itk)->readObject(*this);
      }
    }
  }
 
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::remove(
 const std::string& aNameCycle
)
//////////////////////////////////////////////////////////////////////////////
// Delete Objects or/and keys in a directory :
//   namecycle has the format name;cycle
//   namecycle = "" same as namecycle ="T*"
//   name  = * means all
//   cycle = * means all cycles (memory and keys)
//   cycle = "" or cycle = 9999 ==> apply to a memory object
//   When name=* use T* to delete subdirectories also
//
//   To delete one directory, you must specify the directory cycle,
//      eg.  file.Delete("dir1;1");
//
//   examples:
//     foo   : delete object named foo in memory
//     foo*  : delete all objects with a name starting with foo
//     foo;1 : delete cycle 1 of foo on file
//     foo;* : delete all cycles of foo on file and also from memory
//     *;2   : delete all objects on file having the cycle 2
//     *;*   : delete all objects from memory and file
//    T*;*   : delete all objects from memory and file and all subdirectories
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  short cycle;
  std::string pattern;
  decodeNameCycle(aNameCycle,pattern,cycle);

  bool deleteall = false;
  bool deletetree = false;
  if(pattern=="*") deleteall = true;
  if(pattern=="*T") { 
    deleteall = true; 
    deletetree = true;
  }
  if(pattern=="T*") { 
    deleteall = true; 
    deletetree = true;
  }
  if(aNameCycle.size() == 0) { 
    deleteall = true; 
    deletetree = true;
  }

  //---------------------Case of Object in memory---------------------
  //                     ========================
  if (cycle >= 9999 ) {
    Rio::smanip smanip;
    std::list<IObject*>::iterator it;
    for(it=fObjects.begin();it!=fObjects.end();) {
      bool deleteOK = false;
      if (deleteall || smanip.match((*it)->name(),pattern) ) {
	deleteOK = true;
	Directory* dir = Rio::cast<Directory>(*it);
	if(dir) {
	  if (!deletetree && deleteall) deleteOK = false;
	}
      }
      if (deleteOK) {
	if(fFile.verboseLevel() && (*it)) {
	  Out out(printer());
	  out << "Rio::Directory::remove : delete object "
	      << *it << " of class \"" << (*it)->isA().name()
	      << "\" and name \"" << (*it)->name() << "\"." << endl;
	}

	delete (*it);
	it = fObjects.erase(it);
      } else {
	++it;
      }
    }
  }

  //---------------------Case of Key---------------------
  //                     ===========
  if (cycle != 9999 ) {
    if (isWritable()) {
      Rio::smanip smanip;
      std::list<Key*>::iterator itk;
      for(itk=fKeys.begin();itk!=fKeys.end();) {
	bool deleteOK = false;
	if (deleteall || smanip.match((*itk)->name(),pattern) ) {
	  if (cycle == (*itk)->cycle()) deleteOK = true;
	  if (cycle > 9999) deleteOK = true;
	  if((*itk)->className()=="TDirectory") {
	    if (!deletetree && deleteall) deleteOK = false;
	    if (cycle == (*itk)->cycle()) deleteOK = true;
	  }
	}
	if (deleteOK) {
	  if(fFile.verboseLevel() && (*itk)) {
	    Out out(printer());
	    out << "Rio::Directory::remove : delete key "
		<< *itk << " for object class \"" 
		<< (*itk)->className()
		<< "\" and name \"" << (*itk)->name() << "\"." << endl;
	  }

	  Key* key = *itk;
	  itk = fKeys.erase(itk);
	  key->destroy(); // Do a fDirectory.removeKey(key) !
	  removeKey(key);
	  delete key;
	  fModified = true;
	} else {
	  ++itk;
	}
      }
      if (fModified) {
	writeKeys();               // Write new keys structure
	writeHeader();             // Write new directory header
	fFile.writeFreeSegments(); // Write new free segments list
	fFile.writeHeader();       // Write new file header
      }
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
Rio::IObject* Rio::Directory::findInMemory(
 const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<IObject*>::const_iterator ito;
  for(ito=fObjects.begin();ito!=fObjects.end();++ito) {
    if((*ito)->name()==aName) return *ito;
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::print(
 IPrinter& aPrinter
) const
//////////////////////////////////////////////////////////////////////////////
// List Directory contents.
//  Indentation is used to identify the directory tree
//  Subdirectories are listed first, 
//   then objects in memory, then objects on the file
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aPrinter.indent();
  /*FIXME  if(fClassName=="TFile")
  out << sClass->name() << "**" << tab << tab << name() 
      << tab << fTitle << endl;
    else */
  Out out(aPrinter);
  out << sClass->name() << "*" << tab << tab << name() 
      << tab << fTitle << endl;
  aPrinter.increment();

  bool memobj  = true;
  bool diskobj = true;
  
  if(memobj) {
    std::list<IObject*>::const_iterator it;
    for(it=fObjects.begin();it!=fObjects.end();++it) {
      (*it)->print(aPrinter);
    }
  }
  
  if(diskobj) {
    std::list<Key*>::const_iterator it;
    for(it=fKeys.begin();it!=fKeys.end();++it) {
      aPrinter.indent();
      out << "KEY: " << (*it)->className() 
	  << tab << (*it)->name() << ";" << (*it)->cycle() 
	  << tab << (*it)->title() << endl;
    }
  }
  
  aPrinter.decrement();
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::decodeNameCycle(
 const std::string& aString
,std::string& aName
,short& aCycle
)
//////////////////////////////////////////////////////////////////////////////
// Decode a namecycle "aap;2" into name "aap" and cycle "2".
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aCycle = 9999;
  int nch = aString.size();
  for (int i = 0; i < nch; i++) {
    if (aString[i] != ';')
      aName += aString[i];
    else {
      if( (i < nch-1) && (aString[i+1] == '*') ) {
	aCycle = 10000;
      } else {
	std::string s = aString.substr(i+1,aString.size()-(i+1));
	smanip smanip;
	long v;
	//::sscanf(aString.c_str()+i+1, "%hd",&aCycle);
	if(!smanip.tolong(s,v)) {
	  Out out(printer());
	  out << "Rio::Directory::decodeNameCycle : in \""
	      << aString << "\", cycle is not a number." << endl;
	  aCycle = 10000;
	} else {
	  aCycle = (short)v;
	}
      }
      return;
    }
  }
}
