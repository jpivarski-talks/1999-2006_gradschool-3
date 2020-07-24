#ifndef Rio_Out_h
#define Rio_Out_h

#include <string>

namespace Rio {

class IPrinter;
              
class Out;
typedef Out& (*OutFunc)(Out&);

class Out {
public:
  Out& operator<<(const char*);
  Out& operator<<(const std::string&);
  Out& operator<<(int);
  Out& operator<<(double);
  Out& operator<<(float);
  Out& operator<<(long);
  Out& operator<<(unsigned int);
  Out& operator<<(unsigned long);
  Out& operator<<(const void*);
  Out& operator<<(OutFunc);
public:
  Out(IPrinter&);
  virtual ~Out();
private:
  IPrinter& fPrinter;
};

Out& endl(Out&);
Out& tab(Out&);

}

#endif
