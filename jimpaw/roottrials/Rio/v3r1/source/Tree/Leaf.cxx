// this :
#include <Rio/Tree/Leaf.h>

// Core :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>

#include <Rio/Tree/Branch.h>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::LeafFloat)

void* Rio::LeafFloat::cast(const IClass& aClass) const{
  if(&aClass==sClass) return (LeafFloat*)this;
  else return BaseLeaf::cast(aClass);
}
void Rio::LeafFloat::stream(IBuffer& aBuffer) {
  if(aBuffer.isReading()) {
    unsigned int s,c;
    aBuffer.readVersion(&s,&c);
    BaseLeaf::stream(aBuffer);
    aBuffer.read(fMinimum);
    aBuffer.read(fMaximum);
    aBuffer.checkByteCount(s,c,sClass->name());
  } else {
    unsigned int c = aBuffer.writeVersion(sClass->version(),true);
    BaseLeaf::stream(aBuffer);
    aBuffer.write(fMinimum);
    aBuffer.write(fMaximum);
    aBuffer.setByteCount(c,true);
  }
}
void Rio::LeafFloat::printValue(IPrinter& aPrinter,int aIndex) const {
  Out out(aPrinter);
  out << fValue[aIndex];
}
Rio::LeafFloat::LeafFloat(Branch& aBranch)
  :Leaf<float>(aBranch){}
Rio::LeafFloat::LeafFloat(Branch& aBranch,const std::string& aName)
  :Leaf<float>(aBranch,aName) {}
Rio::LeafFloat::~LeafFloat(){}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::LeafInteger)

void* Rio::LeafInteger::cast(const IClass& aClass) const{
  if(&aClass==sClass) return (LeafInteger*)this;
  else return BaseLeaf::cast(aClass);
}
void Rio::LeafInteger::stream(IBuffer& aBuffer) {
    if(aBuffer.isReading()) {
      unsigned int s,c;
      aBuffer.readVersion(&s,&c);
      BaseLeaf::stream(aBuffer);
      aBuffer.read(fMinimum);
      aBuffer.read(fMaximum);
      aBuffer.checkByteCount(s,c,sClass->name());
    } else {
      unsigned int c = aBuffer.writeVersion(sClass->version(),true);
      BaseLeaf::stream(aBuffer);
      aBuffer.write(fMinimum);
      aBuffer.write(fMaximum);
      aBuffer.setByteCount(c,true);
    }
  }
void Rio::LeafInteger::printValue(IPrinter& aPrinter,int aIndex) const {
  Out out(aPrinter);
  out << fValue[aIndex];
}
Rio::LeafInteger::LeafInteger(Branch& aBranch)
  :Leaf<int>(aBranch) {}
Rio::LeafInteger::LeafInteger(Branch& aBranch,const std::string& aName)
  :Leaf<int>(aBranch,aName) {}
Rio::LeafInteger::~LeafInteger(){}

