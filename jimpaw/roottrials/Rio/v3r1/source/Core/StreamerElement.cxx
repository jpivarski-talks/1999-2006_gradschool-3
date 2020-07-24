// this :
#include <Rio/Core/StreamerElement.h>

// Rio :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>

#include <Rio/Core/Out.h>
#include <Rio/Core/StreamerInfo.h>
#include <Rio/Core/smanip.h>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerElement)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerElement::StreamerElement(
 const std::string& aName
,const std::string& aTitle
,int aOffset
,int aType
,const std::string& aTypeName
)
:Named(aName,aTitle)
,fType(aType)
,fSize(0)
,fArrayLength(0)
,fArrayDim(0)
,fOffset(aOffset)
,fTypeName(aTypeName)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*FIXME
    fNewType     = fType;
    fStreamer    = 0;
    fMethod      = 0;
  */
  for (int i=0;i<5;i++) fMaxIndex[i] = 0;
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerElement::~StreamerElement(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME delete fMethod;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerElement::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (StreamerElement*)this;
  else return Named::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
std::string Rio::StreamerElement::fullName(
) const
//////////////////////////////////////////////////////////////////////////////
// return element name including dimensions, if any
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string s = name();
  smanip smanip;
  for (int i=0;i<fArrayDim;i++) {
    std::string cdim;
    smanip.printf(cdim,32,"[%d]",fMaxIndex[i]);
    s += cdim;
  }
  return s;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerElement::setArrayDimension(
 int aDimension
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fArrayDim = aDimension;
  if(aDimension) fType += StreamerInfo::ARRAY;
  //fNewType = fType;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerElement::setMaxIndex(
 int aDimension
,int aMaximum
)
//////////////////////////////////////////////////////////////////////////////
//set maximum index for array with dimension dim
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aDimension < 0 || aDimension > 4) return;
  fMaxIndex[aDimension] = aMaximum;
  if (fArrayLength == 0)  fArrayLength  = aMaximum;
  else                    fArrayLength *= aMaximum;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerElement::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s, c;
    aBuffer.readVersion(&s, &c);

    Named::stream(aBuffer);
    aBuffer.read(fType);
    aBuffer.read(fSize);
    aBuffer.read(fArrayLength);
    aBuffer.read(fArrayDim);
    aBuffer.readFastArray(fMaxIndex,5);
    aBuffer.read(fTypeName);

    aBuffer.checkByteCount(s,c,sClass->name());
  } else {
    unsigned int c = 
      aBuffer.writeVersion(sClass->version(),true);

    Named::stream(aBuffer);
    aBuffer.write(fType);
    aBuffer.write(fSize);
    aBuffer.write(fArrayLength);
    aBuffer.write(fArrayDim);
    aBuffer.writeFastArray(fMaxIndex,5);
    aBuffer.write(fTypeName);

    aBuffer.setByteCount(c,true);
  }

}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerElement::print(
 IPrinter& aPrinter
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*FIXME  sprintf(includeName,GetTypeName());
  if (IsaPointer() && !fTypeName.Contains("*")) strcat(includeName,"*");
  printf("  %-14s%-15s offset=%3d type=%2d %-20s\n",includeName,GetFullName(),fOffset,fType,GetTitle());
  */

  smanip smanip;
  std::string s;
  smanip.printf(s,128,"  %-14s%-15s offset=%3d type=%2d %-20s",fTypeName.c_str(),fullName().c_str(),fOffset,fType,title().c_str());
  Out out(aPrinter);
  out << s << endl;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerBase)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerBase::StreamerBase(
 const std::string& aName
,const std::string& aTitle
,int aOffset
,int aBaseVersion
)
:StreamerElement(aName,aTitle,aOffset,StreamerInfo::BASE,"BASE")
,fBaseVersion(aBaseVersion)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aName=="TObject") fType = StreamerInfo::TOBJECT;
  if (aName=="TNamed") fType = StreamerInfo::TNAMED;
  /*FIXME
    fNewType = fType;
    fBaseClass = gROOT->GetClass(GetName());
    Init();
  */
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerBase::~StreamerBase(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerBase::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (StreamerBase*)this;
  else return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerBase::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s, c;
    aBuffer.readVersion(&s, &c);
    StreamerElement::stream(aBuffer);
    aBuffer.read(fBaseVersion);
    aBuffer.checkByteCount(s,c,sClass->name());
  } else {
    unsigned int c = aBuffer.writeVersion(sClass->version(),true);
    StreamerElement::stream(aBuffer);
    aBuffer.write(fBaseVersion);
    aBuffer.setByteCount(c,true);
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerBasicType)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerBasicType::StreamerBasicType(
 const std::string& aName
,const std::string& aTitle
,int aOffset
,int aType
,const std::string& aTypeName
)
:StreamerElement(aName,aTitle,aOffset,aType,aTypeName)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerBasicType::~StreamerBasicType(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerBasicType::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (StreamerBasicType*)this;
  else return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerBasicType::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s, c;
    aBuffer.readVersion(&s, &c);
    StreamerElement::stream(aBuffer);
    aBuffer.checkByteCount(s,c,sClass->name());
  } else {
    unsigned int c = aBuffer.writeVersion(sClass->version(),true);
    StreamerElement::stream(aBuffer);
    aBuffer.setByteCount(c,true);
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerString)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerString::StreamerString(
 const std::string& aName
,const std::string& aTitle
,int aOffset
)
:StreamerElement(aName,aTitle,aOffset,StreamerInfo::TSTRING,"TString")
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerString::~StreamerString(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerString::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (StreamerString*)this;
  else return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerString::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s, c;
    aBuffer.readVersion(&s, &c);
    StreamerElement::stream(aBuffer);
    aBuffer.checkByteCount(s,c,sClass->name());
  } else {
    unsigned int c = aBuffer.writeVersion(sClass->version(),true);
    StreamerElement::stream(aBuffer);
    aBuffer.setByteCount(c,true);
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerObject)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerObject::StreamerObject(
 const std::string& aName
,const std::string& aTitle
,int aOffset
,const std::string& aTypeName
)
:StreamerElement(aName,aTitle,aOffset,0,aTypeName)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fType = StreamerInfo::OBJECT;
  if (aName=="TObject") fType = StreamerInfo::TOBJECT;
  if (aName=="TNamed") fType = StreamerInfo::TNAMED;
  /*
  fNewType = fType;
  Init();
  */
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerObject::~StreamerObject(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerObject::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (StreamerObject*)this;
  else return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerObject::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s, c;
    aBuffer.readVersion(&s, &c);
    StreamerElement::stream(aBuffer);
    aBuffer.checkByteCount(s,c,sClass->name());
  } else {
    unsigned int c = aBuffer.writeVersion(sClass->version(),true);
    StreamerElement::stream(aBuffer);
    aBuffer.setByteCount(c,true);
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerObjectPointer)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerObjectPointer::StreamerObjectPointer(
 const std::string& aName
,const std::string& aTitle
,int aOffset
,const std::string& aTypeName
)
:StreamerElement(aName,aTitle,aOffset,StreamerInfo::OBJECT_POINTER,aTypeName)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aTitle.substr(0,2)=="->") fType = StreamerInfo::OBJECT_ARROW;
  /*FIXME
    fNewType = fType;
    Init();
  */
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerObjectPointer::~StreamerObjectPointer(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerObjectPointer::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (StreamerObjectPointer*)this;
  else return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerObjectPointer::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s, c;
    aBuffer.readVersion(&s, &c);
    StreamerElement::stream(aBuffer);
    aBuffer.checkByteCount(s,c,sClass->name());
  } else {
    unsigned int c = aBuffer.writeVersion(sClass->version(),true);
    StreamerElement::stream(aBuffer);
    aBuffer.setByteCount(c,true);
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerBasicPointer)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerBasicPointer::StreamerBasicPointer(
 const std::string& aName
,const std::string& aTitle
,int aOffset
,int aType
,const std::string& aCountName
,const std::string& aCountClass
,int aCountVersion
,const std::string& aTypeName
)
:StreamerElement(aName,aTitle,aOffset,aType,aTypeName)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fType += StreamerInfo::POINTER;
  fCountName    = aCountName;
  fCountClass   = aCountClass;
  fCountVersion = aCountVersion;
  //   Init();
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerBasicPointer::~StreamerBasicPointer(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerBasicPointer::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (StreamerBasicPointer*)this;
  else return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerBasicPointer::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s, c;
    aBuffer.readVersion(&s, &c);
    StreamerElement::stream(aBuffer);
    aBuffer.read(fCountVersion);
    aBuffer.read(fCountName);
    aBuffer.read(fCountClass);
    aBuffer.checkByteCount(s,c,sClass->name());
  } else {
    unsigned int c = aBuffer.writeVersion(sClass->version(),true);
    StreamerElement::stream(aBuffer);
    aBuffer.write(fCountVersion);
    aBuffer.write(fCountName);
    aBuffer.write(fCountClass);
    aBuffer.setByteCount(c,true);
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerObjectAny)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerObjectAny::StreamerObjectAny(
 const std::string& aName
,const std::string& aTitle
,int aOffset
,const std::string& aTypeName
)
:StreamerElement(aName,aTitle,aOffset,StreamerInfo::OBJECT_ANY,aTypeName)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*FIXME
    Init();
  */
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerObjectAny::~StreamerObjectAny(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerObjectAny::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (StreamerObjectAny*)this;
  else return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerObjectAny::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s, c;
    aBuffer.readVersion(&s, &c);
    StreamerElement::stream(aBuffer);
    aBuffer.checkByteCount(s,c,sClass->name());
  } else {
    unsigned int c = aBuffer.writeVersion(sClass->version(),true);
    StreamerElement::stream(aBuffer);
    aBuffer.setByteCount(c,true);
  }
}
