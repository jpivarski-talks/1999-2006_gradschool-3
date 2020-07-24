// this :
#include <Rio/Core/List.h>

#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>

#include <Rio/Core/Arguments.h>

RIO_META_SOURCE(Rio::List)

//////////////////////////////////////////////////////////////////////////////
Rio::List::List(
)
:fName("")
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::List::~List(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::List::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (List*)this;
  else return Object::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::List::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Stream all objects in the collection to or from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s, c;
    /*short v = */ aBuffer.readVersion(&s, &c);
    Object::stream(aBuffer);
    aBuffer.read(fName);

    int nobjects;
    aBuffer.read(nobjects);

    for (int i = 0; i < nobjects; i++) {
      IObject* obj = aBuffer.readObject(Arguments());
      unsigned char nch;
      aBuffer.read(nch);
      if (nch) {
	char readOption[256];
	aBuffer.readFastArray(readOption,nch);
	readOption[nch] = 0;
	push_back(obj); 	//Add(obj,readOption);
      } else {
	push_back(obj);
      }
    }

    aBuffer.checkByteCount(s,c,sClass->name());

  } else {
    unsigned int c = aBuffer.writeVersion(sClass->version(),true);

    Object::stream(aBuffer);

    aBuffer.write(fName);

    int nobjects = size();
    aBuffer.write(nobjects);
    
    std::list<IObject*>::const_iterator it;
    for(it=begin();it!=end();++it) {
      aBuffer.writeObject(*it);
      // Store object option :
      std::string dummy_string;
      unsigned char nch = dummy_string.size();
      aBuffer.write(nch);
      aBuffer.writeFastArray(dummy_string.c_str(),nch);
    }

    aBuffer.setByteCount(c,true);
  }
}


