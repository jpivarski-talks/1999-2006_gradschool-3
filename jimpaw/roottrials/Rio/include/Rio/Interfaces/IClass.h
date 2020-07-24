#ifndef Rio_IClass_h
#define Rio_IClass_h

#include <string>
#include <vector>

namespace Rio {

class IObject;

/**
 * @class IClass 
 *
 *  IClass is the interface for the Rio class description.
 *
 * @author Guy Barrand
 * @date 2002
 */

class IClass {
public:
  virtual ~IClass() {}

  /** @return The name of the described class. */
  virtual const std::string& name() const = 0;

  /** @return The version of the described class. */
  virtual short version() const = 0;

  /** Object factory for the described class.
   * @param aArgs : A vector of pair(int,void*) to pass arguments 
   *                to the constructor. */
  virtual IObject* create(const std::vector< std::pair<int,void*> >& aArgs) = 0;
};

}

#endif
