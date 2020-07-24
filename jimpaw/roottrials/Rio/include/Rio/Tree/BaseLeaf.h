#ifndef Rio_BaseLeaf_h
#define Rio_BaseLeaf_h

#include <string>

#include <Rio/Interfaces/ILeaf.h>
#include <Rio/Core/Named.h>

namespace Rio {

class Branch;

class BaseLeaf : public Named, public virtual ILeaf {
  RIO_META_HEADER(BaseLeaf)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public:
  BaseLeaf(Branch&);
  BaseLeaf(Branch&,const std::string&);
  virtual ~BaseLeaf();
  // Get/set :
  bool isRange() const;
  void setRange(bool = true);
  BaseLeaf* leafCount() const;
  int lengthType() const;
  bool isIndirectAddress() const;
  void setIndirectAddress(bool);
  void setOffset(int = 0);
  Branch& branch() const;
  // Else :
  virtual int maximum() const;
  virtual int minimum() const;
  virtual int length() const;
  virtual BaseLeaf* getLeafCounter(int&) const;
  int resetAddress(void*,bool = false);
  virtual int offset() const;
private:
  Branch& fBranch;
  bool fNewValue;
  bool fIndirectAddress;
protected:
  int fNdata;           //! Number of elements in fAddress data buffer
  int fLength;          //  Number of fixed length elements
  int fLengthType;      //  Number of bytes for this data type
  int fOffset;          //  Offset in ClonesArray object (if one)
  bool fIsRange;        //  (=true if leaf has a range, false otherwise)
  bool fIsUnsigned;     //  (=kTRUE if unsigned, kFALSE otherwise)
  BaseLeaf* fLeafCount; //  Pointer to Leaf count if variable length
};

}

#endif
