#ifndef Rio_StreamerElement_h
#define Rio_StreamerElement_h

#include <Rio/Core/Named.h>

namespace Rio {

class StreamerElement : public Named {
  RIO_META_HEADER(StreamerElement)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
  virtual void print(IPrinter&) const;
public:
  StreamerElement(const std::string&,
		  const std::string&,
		  int,int,
		  const std::string&);
  virtual ~StreamerElement();
  virtual void setArrayDimension(int);
  virtual void setMaxIndex(int,int);
  virtual std::string fullName() const;
protected:
  int fType;            //element type
  int fSize;            //sizeof element
  int fArrayLength;     //cumulative size of all array dims
  int fArrayDim;        //number of array dimensions
  int fMaxIndex[5];     //Maximum array index for array dimension "dim"
  int fOffset;          //!element offset in class
  //FIXME Int_t         fNewType;         //!new element type when reading
  std::string fTypeName;        //Data type name of data member
};

class StreamerBase : public StreamerElement {
  RIO_META_HEADER(StreamerBase)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public:
  StreamerBase(const std::string&,const std::string&,int,int);
  virtual ~StreamerBase();
private:
  int fBaseVersion;         //version number of the base class
};

class StreamerBasicType : public StreamerElement {
  RIO_META_HEADER(StreamerBasicType)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public:
  StreamerBasicType(const std::string&,
		    const std::string&,
		    int,int,const std::string&);
  virtual ~StreamerBasicType();
};

class StreamerBasicPointer : public StreamerElement {
  RIO_META_HEADER(StreamerBasicPointer)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public:
  StreamerBasicPointer(const std::string&,
		       const std::string&, 
		       int,int,
		       const std::string&,
		       const std::string&, 
		       int,
		       const std::string&);
  virtual ~StreamerBasicPointer();
private:
  int fCountVersion;       //version number of the class with the counter
  std::string fCountName;  //name of data member holding the array count
  std::string fCountClass; //name of the class with the counter
};

class StreamerString : public StreamerElement {
  RIO_META_HEADER(StreamerString)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public:
  StreamerString(const std::string&,const std::string&,int);
  virtual ~StreamerString();
};
 

class StreamerObject : public StreamerElement {
  RIO_META_HEADER(StreamerObject)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public:
  StreamerObject(const std::string&,
		 const std::string&,
		 int,const std::string&);
  virtual ~StreamerObject();
};


class StreamerObjectPointer : public StreamerElement {
  RIO_META_HEADER(StreamerObjectPointer)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public:
  StreamerObjectPointer(const std::string&,
			const std::string&,
			int,const std::string&);
  virtual ~StreamerObjectPointer();
};

class StreamerObjectAny : public StreamerElement {
  RIO_META_HEADER(StreamerObjectAny)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public:
  StreamerObjectAny(const std::string&,
		    const std::string&,
		    int,const std::string&);
  virtual ~StreamerObjectAny();
};


} 
  
#endif
