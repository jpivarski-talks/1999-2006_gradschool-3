#ifndef Rio_List_h
#define Rio_List_h

#include <Rio/Core/Object.h>

#include <list>

namespace Rio {

class List : public Object, public std::list<IObject*> {
  RIO_META_HEADER(List)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public:
  List();
  virtual ~List();
private:
  std::string fName;
};

}

#endif
