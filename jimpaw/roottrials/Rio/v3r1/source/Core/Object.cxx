// this :
#include <Rio/Core/Object.h>

// Rio :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IPrinter.h>
#include <Rio/Interfaces/IBuffer.h>

#include <Rio/Core/Out.h>
#include <Rio/Core/Debug.h>

RIO_META_SOURCE(Rio::Object)
//////////////////////////////////////////////////////////////////////////////
Rio::Object::Object(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment();
}
//////////////////////////////////////////////////////////////////////////////
Rio::Object::~Object(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::decrement();
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Object::name(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return isA().name();
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Object::title(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return isA().name();
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::Object::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (Object*)this;
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Object::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME if (IsA()->CanIgnoreTObjectStreamer()) return;
  if (aBuffer.isReading()) {
    aBuffer.readVersion();
    unsigned int uniqueID;
    aBuffer.read(uniqueID);
    unsigned int bits;
    aBuffer.read(bits);
    //FIXME fBits |= kIsOnHeap;// by definition de-serialized object is on heap
  } else {
    aBuffer.writeVersion(sClass->version());
    //debug aBuffer.write((unsigned int)317);
    aBuffer.write((unsigned int)0);
    aBuffer.write((unsigned int)0);
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Object::print(
 IPrinter& aPrinter
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aPrinter.indent();
  Out out(aPrinter);
  out << "OBJ: " << isA().name() << tab << name() << tab << title() << endl;
}
