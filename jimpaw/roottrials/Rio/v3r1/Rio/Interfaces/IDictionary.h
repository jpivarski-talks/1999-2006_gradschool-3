#ifndef Rio_IDictionary_h
#define Rio_IDictionary_h

#include <string>
#include <list>

namespace Rio {

class IClass;
class IObject;

/**
 * @class IDictionary
 *
 *  IDictionary is the interface to handle class dictionary informations.
 *
 * @author Guy Barrand
 * @date 2002
 */

class IDictionary {
public:
  virtual ~IDictionary() {}
public:

  /** Find a class describer given its name.
   * @param aName : The name of the class to find.
   * @return The class describer (an IClass). 
   *         It returns 0 in case of failure. */
  virtual IClass* findClass(const std::string& aName) = 0;

  /** Add a class describer to the dictionary.
   * @param aClass : The class describer. */
  virtual void addClass(IClass* aClass) = 0;

  /** @return The streamer informations. */
  virtual const std::list<IObject*>& streamerInfos() const = 0;

};

}

#endif
