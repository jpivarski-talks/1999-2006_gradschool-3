#ifndef Rio_LeafObject_h
#define Rio_LeafObject_h

#include <Rio/Tree/BaseLeaf.h>

namespace Rio {

class LeafObject : public BaseLeaf {
  RIO_META_HEADER(LeafObject)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public : // Rio::ILeaf 
  virtual double value(int aIndex) const;
  virtual void* valuePointer() const;
  virtual void fillBasket(IBuffer&);
  virtual void readBasket(IBuffer&);
  virtual void setAddress(void*);
  virtual void printValue(IPrinter&,int = 0) const;
  IObject* object() const;
public:
  LeafObject(Branch&);
  LeafObject(Branch&,const std::string&,IClass&);
  virtual ~LeafObject();
private:
  IClass* fClass;        //! pointer to class
  IObject** fObjAddress; //! Address of Pointer to object
  bool fVirtual;         // Support for Virtuality
};
 
}

#endif
