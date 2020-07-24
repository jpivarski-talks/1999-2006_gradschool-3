#ifndef Rio_ILeaf_h
#define Rio_ILeaf_h

namespace Rio {

class IBuffer;
class IPrinter;

/**
 * @class ILeaf
 *
 *  ILeaf is the interface to a Rio "leaf".
 *  Internal usage only.
 *
 * @author Guy Barrand
 * @date 2002
 */

class ILeaf {
public:
  virtual ~ILeaf(){}
public:
  virtual double value(int = 0) const = 0;
  virtual void* valuePointer() const = 0;
  virtual void fillBasket(IBuffer&) = 0;
  virtual void readBasket(IBuffer&) = 0;
  virtual void setAddress(void* = 0) = 0;
  virtual void printValue(IPrinter&,int = 0) const = 0;
};

}

#endif
