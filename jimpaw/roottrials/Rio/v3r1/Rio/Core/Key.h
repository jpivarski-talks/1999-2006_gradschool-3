#ifndef Rio_Key_h
#define Rio_Key_h

#include <string>

#include <Rio/Interfaces/IKey.h>
#include <Rio/Core/Date.h>
#include <Rio/Core/Seek.h>

namespace Rio {

class IObject;
class IBuffer;
class File;
class Directory;
class Buffer;

class Key : public virtual IKey {
public:
  virtual const std::string& name() const;
  virtual const std::string& title() const;
  virtual const std::string& className() const;
  virtual short cycle() const;
public:
  Key(File&);
  Key(File&,Seek,const std::string&,const std::string&,const std::string&,int);
  Key(File&,Seek,int);
  Key(File&,Directory&,IObject&,const std::string&,int);
  virtual ~Key();
  // Get/set :
  int numberOfBytes() const;
  int objectSize() const;
  int version() const;
  Seek seekKey() const;
  Seek seekParentDirectory() const;
  int keyLength() const;
  Buffer* bufferRef() const;
  // Else :
  virtual char* buffer() const;
  virtual int writeFile(int=1);
  virtual void readFile();
  virtual void readBuffer(char*&);
  virtual void fillBuffer(char*&);
  virtual IObject* readObject(Directory&);
  virtual void destroy();
private:
  Key(const Key&);
  void operator=(const Key&);
  int findRecordSize() const;
  void deleteBuffer();
protected:
  void initialize(int);
  void stream(IBuffer&);
protected:
  File& fFile;
private:
  int fLeft;           //Number of bytes left in current segment
protected: // Basket inherits Key.
  char* fBuffer;       //Object buffer
  Buffer* fBufferRef;  //Pointer to the Buffer object
  // Record (stored in file) :
  int fNumberOfBytes;        //Number of bytes for the object on file
  int fVersion;              //Key version identifier
  int fObjectSize;           //Length of uncompressed object in bytes
  Date fDate;                //Date/Time of insertion in file
  short fKeyLength;          //Number of bytes for the key itself
  short fCycle;              //Cycle number
  Seek fSeekKey;             //Location of object on file
  Seek fSeekParentDirectory; //Location of parent directory on file
  std::string fObjectClassName;   //Object Class name.
  std::string fObjectName;        //name of the object.
  std::string fObjectTitle;       //title of the object.
};

}

#endif
