#ifndef Rio_CoreDictionary_h
#define Rio_CoreDictionary_h

#include <Rio/Interfaces/IDictionary.h>

namespace Rio {

class IPrinter;

class CoreDictionary : public virtual IDictionary {
public: //Rio::IDictionary
  virtual IClass* findClass(const std::string&);
  virtual const std::list<IObject*>& streamerInfos() const;
  virtual void addClass(IClass*);
public:
  CoreDictionary(IPrinter&);
  virtual ~CoreDictionary();
  virtual void print() const;
protected:
  IPrinter& fPrinter;
  std::list<IClass*> fClasses;
  std::list<IObject*> fStreamerInfos;
};

}

#endif

