#ifndef Rio_StreamerInfo_h
#define Rio_StreamerInfo_h

#include <Rio/Core/Named.h>

namespace Rio {

class IPrinter;
class StreamerElement;
class ObjArray;

// sizeof(vtbl)         = 4
// sizeof(unsigned int) = 4
// sizeof(TObject)   = 12  = 2 * (unsigned int) + vtbl.
// sizeof(TString)   = 8   = char* + vtbl.
// sizeof(TNamed)    = 28  = TObject + 2 * TString.
// sizeof(TObjArray) = 40

class StreamerInfo : public Named {
  RIO_META_HEADER(StreamerInfo)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
  virtual void print(IPrinter&) const;
public:
  enum Type {              // sizeof :
    BASE = 0,              //  x
    ARRAY = 20,            //  ?
    POINTER = 40,          //  4
    COUNTER =  6,          //  4
    SHORT =  2,            //  2
    INTEGER =  3,          //  4
    FLOAT =  5,            //  4
    DOUBLE =  8,           //  8
    UNSIGNED_CHAR =  11,   //  1
    UNSIGNED_INTEGER = 13, //  4
    OBJECT = 61,           //  ?
    OBJECT_ANY = 62,       //  ?
    OBJECT_ARROW = 63,     //  ?
    OBJECT_POINTER = 64,   //  ?
    TSTRING = 65,          //  8  
    TOBJECT = 66,          // 12
    TNAMED = 67            // 28
  };
  StreamerInfo(const IClass&,const std::string&,int = 0);
  virtual ~StreamerInfo();
  // Get/Set :
  int identifier() const;
  // Else :
  void add(StreamerElement*);
private:
  const IClass& fStreamedClass;
  unsigned int fCheckSum;    //checksum of original class
  int fStreamedClassVersion; //Class version identifier
  int fNumber;               //!Unique identifier
  ObjArray* fElements;       //Array of TStreamerElements
};

}

#endif
