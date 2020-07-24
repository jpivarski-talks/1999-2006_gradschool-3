#ifndef Rio_Object_h
#define Rio_Object_h

#include <Rio/Interfaces/IObject.h>
#include <Rio/Core/Meta.h>

namespace Rio {

class Object : public virtual IObject {
  RIO_META_HEADER(Object)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual const std::string& name() const;
  virtual const std::string& title() const;
  virtual void stream(IBuffer&);
  virtual void print(IPrinter&) const;
public:
  Object();
  virtual ~Object();
};

}

#endif
