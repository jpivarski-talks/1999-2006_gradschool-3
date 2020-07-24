#ifndef Rio_Class_h
#define Rio_Class_h

#include <Rio/Interfaces/IClass.h>

namespace Rio {

class IPrinter;

class Class : public virtual IClass {
public: //Rio::IClass:
  virtual const std::string& name() const;
  virtual short version() const;
public:
  Class(IPrinter&,const std::string&,short);
  virtual ~Class();
private:
  std::string fName;
  short fVersion;
protected:
  IPrinter& fPrinter;
};

}

#endif
