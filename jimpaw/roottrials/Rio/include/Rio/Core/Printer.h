#ifndef Rio_Printer_h
#define Rio_Printer_h

#include <Rio/Interfaces/IPrinter.h>

namespace Rio {

class IPrinter;
              
class Printer : public virtual IPrinter {
public: //Rio::IPrinter
  virtual void write(const char*);
  virtual void disable();
  virtual void enable();
  virtual bool enabled() const;
  virtual void indent();
  virtual void increment();
  virtual void decrement();
public:
  Printer();
  virtual ~Printer();
private:
  int fLevel;
};

}

#endif
