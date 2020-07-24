#ifndef Rio_BranchObject_h
#define Rio_BranchObject_h

#include <Rio/Tree/Branch.h>

namespace Rio {

class BranchObject : public Branch {
  RIO_META_HEADER(BranchObject)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public:
  BranchObject(Tree&);
  BranchObject(Tree&,const std::string&,const std::string&,
	       IClass&,IObject**,
	       int = 32000,int = -1);
  virtual ~BranchObject();
  virtual void setAutoDelete(bool = true);
  virtual int fill();
  virtual void updateAddress();
  virtual int entry(int = 0,int = 0);
  virtual void reset();
  virtual void setAddress(void*);
  virtual void setBasketSize(int);
private:
  std::string fClassName;
  IObject* fOldObject;
  bool fWarn;
};

}

#endif
