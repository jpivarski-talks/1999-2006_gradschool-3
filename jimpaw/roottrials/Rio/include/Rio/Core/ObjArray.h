#ifndef Rio_ObjArray_h
#define Rio_ObjArray_h

#include <Rio/Core/Arguments.h>
#include <Rio/Core/Object.h>

#include <list>

namespace Rio {

class ObjArray : public Object, public std::list<IObject*> {
  RIO_META_HEADER(ObjArray)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public:
  ObjArray();
  ObjArray(int);
  virtual ~ObjArray();
  IObject*& operator[](int);
  void setReadArguments(const Arguments&,bool = true);
private:
  Arguments fArgs;
  bool fNoNull;
};

}

#endif
