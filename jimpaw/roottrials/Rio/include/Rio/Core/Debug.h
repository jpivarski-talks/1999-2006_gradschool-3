#ifndef Rio_Debug_h
#define Rio_Debug_h

namespace Rio {

class IPrinter;

class Debug {
public:
  Debug();
  virtual ~Debug();
  static void check(IPrinter&);
  static void increment();
  static void decrement();
private:
  static int fCount;
};

}

#endif
