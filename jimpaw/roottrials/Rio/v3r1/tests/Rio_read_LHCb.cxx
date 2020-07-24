/**
 *  An LHCb root event file has TTrees in the main directory.
 * KEY: TTree     _Event_Rec_Tr_OTClusters;29     /Event/Rec/Tr/OTClusters
 * KEY: TTree     _Event_Rec_Tr_OTClusters;28     /Event/Rec/Tr/OTClusters
 * KEY: TTree     _Event_Rec_Tr_StatesP;21        /Event/Rec/Tr/StatesP
 * KEY: TTree     _Event_Rec_Tr_StatesP;20        /Event/Rec/Tr/StatesP
 * KEY: TTree     _Event_MC_Particles;12  /Event/MC/Particles
 * KEY: TTree ...
 *
 *  Each TTree has four branches :
 * ID, OBJSIZE, NUMLINK, NUMSYMB of type integer and
 * BUFFER of type TBlob.
 *
 */

#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Interfaces/IDirectory.h>

#include <Rio/Core/Class.h>
#include <Rio/Core/Printer.h>
#include <Rio/Core/File.h>
#include <Rio/Core/tobuf.h>
#include <Rio/Core/StreamerInfo.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/Debug.h>
#include <Rio/Core/Out.h>

#include <Rio/Tree/TreeDictionary.h>
#include <Rio/Tree/Tree.h>
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/BaseLeaf.h>
#include <Rio/Tree/LeafObject.h>

#include <iostream>

class Blob : public Rio::Object {
  RIO_META_HEADER(Blob)
public: //Rio::IObject
  void* cast(const Rio::IClass& aClass) const {
    if(&aClass==sClass) return (Blob*)this;
    else return Object::cast(aClass);
  }
  void stream(Rio::IBuffer& buf)   {
    Object::stream(buf);
    if ( buf.isReading() )    {
      // Avoid copy of data when reading
      buf.read(m_size);
      if ( m_size > 0 )   {
	long len = buf.length();
	m_buffer = buf.buffer() + len;
	buf.setBufferOffset(len+m_size);
      }
    }
    else  {
      buf.write(m_size);
      if ( m_size > 0 )   {
	buf.writeFastArray(m_buffer, m_size);
      }
    }
  }
public:
  long  m_size;
  char* m_buffer;   //[m_size]  
  Blob():m_size(0),m_buffer(0) {}
  virtual ~Blob(){}
};
RIO_META_SOURCE(Blob)


class BlobClass : public Rio::Class {
public: //Rio::IClass
  virtual Rio::IObject* create(const Rio::Arguments& aArgs) {
    Rio::IDirectory* directory = 
      (Rio::IDirectory*)Rio::Args_findDirectory(aArgs);
    if(!directory) {
      Rio::Out out(fPrinter);
      out << "BlobClass::create : no directory given." << Rio::endl;
      return 0;
    }
    Rio::IObject* object = new Blob();
    directory->appendObject(object);
    return object;
  }
public:
  BlobClass(Rio::IPrinter& aPrinter):Rio::Class(aPrinter,"TBlob",0){}
  virtual ~BlobClass(){}
};

int main(int aArgc,char* aArgv[]) {

  /*
   * Create a printer (concreete instance of the Rio::IPrinter)
   * that will be declared to the system. It will be used 
   * for error messages, debug informations, etc...
   */
  Rio::Printer printer;
 {Rio::Out out(printer);

  std::string arg1 = (aArgc>=2) ? aArgv[1] : "";

  /*
   * Instanciate the dictionary.
   */
  Rio::TreeDictionary dico(printer);
  Rio::IClass* clBlob = new BlobClass(printer);
  Blob::setClass(clBlob);
  dico.addClass(clBlob);

  if(arg1=="-verbose") dico.print();
  
  Rio::File file("LHCb.root",Rio::File::READ,dico,printer);
  if(arg1=="-verbose") file.setVerboseLevel(1);
  file.directory().print(printer);
  
  out << " Try to find \"_Event_MC_Particles\" object..." << Rio::endl;
  Rio::Tree* tree = 
    Rio::cast<Rio::Tree>(file.directory().find("_Event_MC_Particles"));
  if(!tree) {
    out << " Object \"_Event_MC_Particles\" not found." << Rio::endl;
  } else {
    int number = (int)tree->entries();
    out << " Object \"_Event_MC_Particles\" found (" << number 
	<< " entries)." << Rio::endl;
    /* 
    int index;
    out << "Head :" << Rio::endl;
    for(index=0;index<10;index++) tree->show(index);
	
    out << "Tail :" << Rio::endl;
    for(index=number-10;index<number;index++) tree->show(index);
    */

    Rio::BaseLeaf* leaf_ID = tree->leaf("ID");
    if(!leaf_ID) {
      out << " Leaf \"ID\" not found." << Rio::endl;
    } else {
      out << " Leaf \"ID\" found." << Rio::endl;
      Rio::Branch& branch_ID = leaf_ID->branch();
      for(int index=0;index<number;index++) {
	if(branch_ID.entry(index)==0) continue;
	int val = (int)leaf_ID->value();
	out << "ID " << index << " = " << val << Rio::endl;
      }
    }
    
    Rio::BaseLeaf* leaf_blob = tree->leaf("BUFFER");
    if(!leaf_blob) {
      out << " Leaf \"BUFFER\" not found." << Rio::endl;
    } else {
      out << " Leaf \"BUFFER\" found." << Rio::endl;
      Rio::Branch& branch_blob = leaf_blob->branch();
      for(int index=0;index<number;index++) {
	if(branch_blob.entry(index)==0) continue;
	Rio::IObject* obj = ((Rio::LeafObject*)leaf_blob)->object();
	Blob* blob = Rio::cast<Blob>(obj);
	out << "blob.m_size " << blob->m_size << Rio::endl;
      }
    }
    
  }    
  file.directory().print(printer);

  file.close();}

  /*
  * To do a simple instance counting. It should produce zero message.
  */
  Rio::Debug::check(printer);

  return 0;
}
