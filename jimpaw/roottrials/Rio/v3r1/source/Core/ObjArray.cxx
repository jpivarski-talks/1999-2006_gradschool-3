// this :
#include <Rio/Core/ObjArray.h>

// Rio :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>

RIO_META_SOURCE(Rio::ObjArray)

//////////////////////////////////////////////////////////////////////////////
Rio::ObjArray::ObjArray(
)
:fNoNull(true)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::ObjArray::ObjArray(
 int aSize
)
:std::list<IObject*>((std::list<IObject*>::size_type)aSize,0)
,fNoNull(true)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::ObjArray::~ObjArray(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::ObjArray::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (ObjArray*)this;
  else return Object::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::ObjArray::setReadArguments(
 const Arguments& aArgs
,bool aNoNull
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fArgs = aArgs;
  fNoNull = aNoNull;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::ObjArray::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Stream all objects in the collection to or from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {

    unsigned int s, c;
    aBuffer.readVersion(&s, &c);
    Object::stream(aBuffer);
    std::string name;
    aBuffer.read(name);
    int nobjects;
    aBuffer.read(nobjects);
    int lowerBound;
    aBuffer.read(lowerBound);
    clear();
    for (int i = 0; i < nobjects; i++) {
      IObject* obj = aBuffer.readObject(fArgs);
      if(fNoNull) {
	if(obj) push_back(obj);
      } else {
	push_back(obj);
      }
    }
    aBuffer.checkByteCount(s, c,sClass->name());

  } else {

    unsigned int c = aBuffer.writeVersion(sClass->version(),true);
    Object::stream(aBuffer);
    aBuffer.write(std::string(""));
    int nobjects = size();
    aBuffer.write(nobjects);
    //debug int lowerBound = 211;
    int lowerBound = 0;
    aBuffer.write(lowerBound);
    
    std::list<IObject*>::const_iterator it;
    for(it=begin();it!=end();++it) {
      aBuffer.writeObject(*it);
    }
    aBuffer.setByteCount(c,true);

  }
}
//////////////////////////////////////////////////////////////////////////////
Rio::IObject*& Rio::ObjArray::operator[](
 int aIndex
)
//////////////////////////////////////////////////////////////////////////////
// Assume that aIndex is in range.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<IObject*>::iterator it;
  int index = 0;
  for(it=begin();it!=end();++it,++index) {
    if(index==aIndex) {
      return *it;
    }
  }
  //FIXME : Should issue a warning (later an exception).
  return *begin();
}
