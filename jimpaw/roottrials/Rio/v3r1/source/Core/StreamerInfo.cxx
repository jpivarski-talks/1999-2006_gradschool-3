// this :
#include <Rio/Core/StreamerInfo.h>

// Rio :
#include <Rio/Interfaces/IDictionary.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>

#include <Rio/Core/Out.h>
#include <Rio/Core/StreamerElement.h>
#include <Rio/Core/ObjArray.h>
#include <Rio/Core/Arguments.h>

static int fgCount = 0;

RIO_META_SOURCE(Rio::StreamerInfo)

//////////////////////////////////////////////////////////////////////////////
Rio::StreamerInfo::StreamerInfo(
 const IClass& aStreamedClass
,const std::string& aTitle
,int aCheckSum
)
:Named(aStreamedClass.name(),aTitle)
,fStreamedClass(aStreamedClass)
,fCheckSum(aCheckSum)
,fStreamedClassVersion(fStreamedClass.version())
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fgCount++;  // Then start at 1.
  fNumber   = fgCount;

  fElements  = new ObjArray();
  /*
  fType     = 0;
  fNewType  = 0;
  fOffset   = 0;
  fLength   = 0;
  fElem     = 0;
  fMethod   = 0;
  fNdata    = 0;
  fOptimized = kFALSE;
  */
  
  //FIXME if (info) BuildUserInfo(info);
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerInfo::~StreamerInfo(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*
  if (fNdata) {
    delete [] fType;
    delete [] fNewType;
    delete [] fOffset;
    delete [] fLength;
    delete [] fElem;
    delete [] fMethod;
  }
  */
  if(fElements) {
    std::list<IObject*>::iterator it;
    for(it=fElements->begin();
	it!=fElements->end();
	it=fElements->erase(it)) 
      delete (*it);
    delete fElements;
  }
}
//////////////////////////////////////////////////////////////////////////////
int Rio::StreamerInfo::identifier(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fNumber;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerInfo::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (StreamerInfo*)this;
  else return Named::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerInfo::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s, c;
    /*short v = */ aBuffer.readVersion(&s, &c);
    /*FIXME
    if (v > 1) {
      TStreamerInfo::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      return;
      }*/
    //====process old versions before automatic schema evolution
    Named::stream(aBuffer);
    aBuffer.read(fCheckSum);
    aBuffer.read(fStreamedClassVersion);

    fElements = Rio::cast<ObjArray>(aBuffer.readObject(Arguments()));

    aBuffer.checkByteCount(s,c,sClass->name());
  } else {
    unsigned int c = aBuffer.writeVersion(sClass->version(),true);
    //FIXME TStreamerInfo::Class()->WriteBuffer(R__b,this);

    Named::stream(aBuffer);
    aBuffer.write(fCheckSum);
    aBuffer.write(fStreamedClassVersion);

    aBuffer.writeObject(fElements);

    aBuffer.setByteCount(c,true);
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerInfo::add(
 StreamerElement* aElement
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fElements) return;
  fElements->push_back(aElement);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerInfo::print(
 IPrinter& aPrinter
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Out out(aPrinter);
  out << endl;
  out << "StreamerInfo for class: " 
      << name() << ", version=" << fStreamedClassVersion << endl;

  if(fElements) {
    std::list<IObject*>::const_iterator it;
    for(it=fElements->begin();it!=fElements->end();++it) { 
      (*it)->print(aPrinter);
    }
  }
  /*FIXME
  for (Int_t i=0;i<fNdata;i++) {
    TStreamerElement *element = (TStreamerElement*)fElem[i];
    printf("   i=%2d, %-15s type=%3d, offset=%3d, len=%d, method=%ld\n",i,element->GetName(),fType[i],fOffset[i],fLength[i],fMethod[i]);
  }
  */
}
