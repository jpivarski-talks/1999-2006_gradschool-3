#ifndef Rio_IKey_h
#define Rio_IKey_h

#include <string>

namespace Rio {

/**
 * @class IKey
 *
 *  IKey is the interface to a "key".
 *  Mainly a "key" holds the position of an object within a file.
 *
 * @author Guy Barrand
 * @date 2002
 */

class IKey {
public:
  virtual ~IKey(){}

  /** @return The name of the key. */
  virtual const std::string& name() const = 0;

  /** @return The title of the key. */
  virtual const std::string& title() const = 0;

  /** @return The class name of the object pointed by the key. */
  virtual const std::string& className() const = 0;

  /** @return The "cycle" of the key. */
  virtual short cycle() const = 0;
};

}

#endif
