#ifndef Rio_Leaf_h
#define Rio_Leaf_h

#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Core/Out.h>

#include <Rio/Tree/BaseLeaf.h>

namespace Rio {

template <class T> class Leaf : public BaseLeaf {
public : // Rio::ILeaf 
  virtual double value(int aIndex) const { return fValue[aIndex]; }
  virtual void* valuePointer() const { return fValue; }
  virtual void fillBasket(IBuffer& aBuffer){
    int len = length();
    if (fPointer) fValue = *fPointer;
    aBuffer.writeFastArray(fValue,len);
  }
  virtual void readBasket(IBuffer& aBuffer){
    if (!fLeafCount && fNdata == 1) {
      aBuffer.read(fValue[0]);
    }else {
      if (fLeafCount) {
	int len = int(fLeafCount->value());
	if (len > fLeafCount->maximum()) {
	  Out out(branch().printer());
	  out << "Rio::Leaf::readBasket : \"" 
	      << name() << "\", len = " << len << " and max = " 
	      << fLeafCount->maximum() << endl;
	  len = fLeafCount->maximum();
	}
	fNdata = len * fLength;
	aBuffer.readFastArray(fValue,len * fLength);
      } else {
	aBuffer.readFastArray(fValue,fLength);
      }
    }
  }
  virtual void setAddress(void* aAddress) {
    if (resetAddress(aAddress)) {
      delete [] fValue;
    }
    if (aAddress) {
      if (isIndirectAddress()) {
	fPointer = (T**) aAddress;
	int ncountmax = fLength;
	if (fLeafCount) ncountmax = fLength * (fLeafCount->maximum() + 1);
	if (ncountmax > fNdata || *fPointer == 0) {
	  if (*fPointer) delete [] *fPointer;
	  if (ncountmax > fNdata) fNdata = ncountmax;
	  *fPointer = new T[fNdata];
	}
	fValue = *fPointer;
      } else {
	fValue = (T*)aAddress;
      }
    } else {
      fValue = new T[fNdata];
      fValue[0] = 0;
    }
  }
public:
  Leaf(Branch& aBranch):BaseLeaf(aBranch),fMinimum(0),fMaximum(0),fValue(0),fPointer(0){
    fLengthType = sizeof(T);
  }
  Leaf(Branch& aBranch,const std::string& aName)
    :BaseLeaf(aBranch,aName),fMinimum(0),fMaximum(0),fValue(0),fPointer(0){
    fLengthType = sizeof(T);
  }
  virtual ~Leaf(){
    if (resetAddress(0,true)) delete [] fValue;
  }
protected:
  T fMinimum;    //Minimum value if leaf range is specified
  T fMaximum;    //Maximum value if leaf range is specified
  T* fValue;     //!Pointer to data buffer
  T** fPointer;  //!Addresss of pointer to data buffer!
};
 
class LeafFloat : public Leaf<float> {
  RIO_META_HEADER(LeafFloat)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public: //Rio::ILeaf
  virtual void printValue(IPrinter&,int = 0) const;
public:
  LeafFloat(Branch&);
  LeafFloat(Branch&,const std::string&);
  virtual ~LeafFloat();
};

class LeafInteger : public Leaf<int> {
  RIO_META_HEADER(LeafInteger)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public: //Rio::ILeaf
  virtual void printValue(IPrinter&,int = 0) const;
public:
  LeafInteger(Branch&);
  LeafInteger(Branch&,const std::string&);
  virtual ~LeafInteger();
};

}

#endif
