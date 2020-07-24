#ifndef Rio_Buffer_h
#define Rio_Buffer_h

//////////////////////////////////////////////////////////////////////////
// Buffer base class used for serializing objects.                      //
//////////////////////////////////////////////////////////////////////////

#include <string>

#include <Rio/Interfaces/IBuffer.h>

namespace Rio {

class IDictionary;
class IPrinter;
class IObject;
class IClass;
class Map;

class Buffer : public virtual IBuffer  {
public: //Rio::IBuffer
  virtual void read(char&);
  virtual void read(bool&);
  virtual void read(short&);
  virtual void read(int&);
  virtual void read(long&);
  virtual void read(float&);
  virtual void read(double&);
  virtual void read(unsigned char&);
  virtual void read(unsigned short&);
  virtual void read(unsigned int&);
  virtual void read(unsigned long&);
  virtual void read(std::string&);
  virtual int readArray(int*&);
  virtual int readArray(double*&);
  virtual int readArray(char*&);
  virtual void readFastArray(char*,int);
  virtual void readFastArray(int*,int);
  virtual void readFastArray(float*,int);

  virtual void write(char);
  virtual void write(bool);
  virtual void write(short);
  virtual void write(int);
  virtual void write(long);
  virtual void write(float);
  virtual void write(double);
  virtual void write(unsigned char);
  virtual void write(unsigned short);
  virtual void write(unsigned int);
  virtual void write(unsigned long);
  virtual void write(const std::string&);
  virtual void writeArray(const int*,int);
  virtual void writeArray(const double*,int);
  virtual void writeFastArray(const char*,int);
  virtual void writeFastArray(const float*,int);
  virtual void writeFastArray(const int*,int);

  virtual bool isReading() const;
  virtual short readVersion(unsigned int* = 0, unsigned int* = 0);
  virtual unsigned int writeVersion(short,bool = false);
  virtual int checkByteCount(unsigned int ,unsigned int,const std::string&);
  virtual void setByteCount(unsigned int,bool = false);
  virtual IObject* readObject(const std::vector<std::pair<int,void*> >&);
  virtual void writeObject(IObject*);
  virtual void setBufferOffset(int = 0);
  virtual char* buffer() const;
  virtual int length() const;
public:
  enum Mode { 
    READ = 0, 
    WRITE = 1 
  };
  enum Size { 
    INITIAL = 1024, 
    MINIMAL = 128 
  };
  Buffer(IDictionary&,IPrinter&,Mode,int = INITIAL,char* = 0);
  virtual ~Buffer();
  // Get/set :
  bool isWriting() const;
  int displacement() const;
  int bufferSize() const;
  void setDisplacement(int);
  void setDisplacement();
  // Else :
  void setBuffer(void*,unsigned int = 0);
  void setReadMode();
  void setWriteMode();
  void resetMap();
  void mapObject(const void*,unsigned int = 1);

  char* readString(char*,int);
  void writeString(const char*);

  void writeBuffer(const void*,int);

private:
  // Buffer objects cannot be copied or assigned
  Buffer(const Buffer&);           // not implemented
  void operator=(const Buffer &);  // not implemented
  void expand(int);  //Expand buffer to newsize
  int readBuffer(void*,int);
  void writeClass(const IClass&);
  IClass* readClass(const IClass*,unsigned int&);
  void initMap();
  void checkCount(unsigned int);
  unsigned int checkObject(unsigned int,const IClass*,bool = false);
private:
  IDictionary& fDictionary;
  IPrinter& fPrinter;
  Mode fMode;         //Read or write mode
  //
  int fVersion;       //Buffer format version
  int fBufSize;       //Size of buffer
  char* fBuffer;      //Buffer used to store objects
  char* fBufCur;      //Current position in buffer
  char* fBufMax;      //End of buffer
  int fMapCount;      //Number of objects or classes in map
  int fMapSize;       //Default size of map
  int fDisplacement;  //Value to be added to the map offsets
  // Maps :
  union {
    Map* fReadMap;    //Map containing id,object references during reading
    Map* fWriteMap;   //Map containing object,id pairs during writing
  };
};

}

#endif
