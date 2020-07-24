#ifndef Rio_IPrinter_h
#define Rio_IPrinter_h

namespace Rio {

/**
 * @class IPrinter
 *
 *  IPrinter is the interface to handle a basic printer.
 *
 * @author Guy Barrand
 * @date 2002
 */

class IPrinter {
public:
  virtual ~IPrinter() {}

  /** Write the given buffer.
   * @param aBuffer : buffer to write. Should be null terminated. */
  virtual void write(const char* aBuffer) = 0;

  /** To enable / disable the printer. */
  virtual void disable() = 0;
  virtual void enable() = 0;
  virtual bool enabled() const = 0;

  /** To handle indentation. */
  virtual void indent() = 0;
  virtual void increment() = 0;
  virtual void decrement() = 0;
};

}

#endif




