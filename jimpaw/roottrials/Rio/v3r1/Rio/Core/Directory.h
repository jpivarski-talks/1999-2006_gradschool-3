#ifndef Rio_Directory_h
#define Rio_Directory_h

#include <list>

#include <Rio/Interfaces/IDirectory.h>

#include <Rio/Core/Named.h>
#include <Rio/Core/Seek.h>
#include <Rio/Core/Date.h>
#include <Rio/Core/Key.h>

namespace Rio {

class File;

class Directory : public Named, public virtual IDirectory {
  RIO_META_HEADER(Directory)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
  virtual void print(IPrinter&) const;
public: //Rio::IDirectory
  virtual IDirectory* mkdir(const std::string&,const std::string& = "");
  virtual void appendObject(IObject*);
  virtual void removeObject(IObject*);
  virtual IObject* find(const std::string&);
  virtual IDirectory* findDirectory(const std::string&);
  virtual IFile& file() const;
  virtual IPrinter& printer() const;
  virtual int seekDirectory() const;
  virtual bool isWritable() const;
  virtual const std::string& name() const;
  virtual bool isClosing() const;
  virtual void saveObject(IObject&);
  virtual void remove(const std::string&);
  virtual IObject* findInMemory(const std::string&);
  virtual IDirectory* parent() const;
  virtual std::string path() const;
  virtual std::vector<IKey*> keys() const;
public:
  Directory(File&,Directory*);
  Directory(File&,Directory*,
	    const std::string&,
	    const std::string& = "");
  virtual ~Directory();
  // Get/set :
  Seek seekKeys() const;
  int nBytesName() const;
  void setNbytesName(int);
  // Else :
  virtual int recordSize() const;
  virtual void fillBuffer(char*&);
  virtual void readBuffer(char*&);
  virtual void setSeekDirectory(Seek);
  virtual void writeKeys();
  virtual int readKeys();
  virtual int appendKey(Key*);
  virtual void removeKey(Key*);
  virtual Key* findKey(const std::string&) const;
  virtual int write();
  virtual void close();
  virtual int writeObject(IObject&);
private:
  void initialize();
  Directory(const Directory&);
  void operator=(const Directory &);
  void writeHeader();
  void saveSelf(bool=false);
  void save();
  Key* getKey(const std::string&,short = 9999);
  void decodeNameCycle(const std::string&,std::string&,short&);
private:
  File& fFile;
  Directory* fParent;
  bool fIsClosing;
  //
  std::list<IObject*> fObjects; //Pointer to objects list in memory
  std::list<Key*> fKeys;        //Pointer to keys list in memory
  bool fModified;        //true if directory has been modified
  bool fWritable;        //true if directory is writable
  // Record (stored in file):
  Date fDateC;           //Date and time when directory is created
  Date fDateM;           //Date and time of last modification
  int  fNbytesKeys;      //Number of bytes for the keys
  int  fNbytesName;      //Number of bytes in TNamed at creation time
  Seek fSeekDirectory;   //Location of directory on file
  Seek fSeekParent;      //Location of parent directory on file
  Seek fSeekKeys;        //Location of Keys record on file
};

}

#endif
