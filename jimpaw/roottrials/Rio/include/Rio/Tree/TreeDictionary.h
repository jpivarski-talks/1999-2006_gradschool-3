#ifndef Rio_TreeDictionary_h
#define Rio_TreeDictionary_h

#include <Rio/Core/CoreDictionary.h>

namespace Rio {

class TreeDictionary : public CoreDictionary {
public:
  TreeDictionary(IPrinter&);
  virtual ~TreeDictionary();
};

}

#endif

