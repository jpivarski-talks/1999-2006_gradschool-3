#ifndef Rio_Arguments_h
#define Rio_Arguments_h

#include <vector>

namespace Rio {

typedef std::vector< std::pair<int,void*> > Arguments;

void Args_insertDirectory(Arguments&,void*);
void Args_insertTree(Arguments&,void*);
void Args_insertBranch(Arguments&,void*);
void* Args_findDirectory(const Arguments&);
void* Args_findTree(const Arguments&);
void* Args_findBranch(const Arguments&);
void* Args_findItem(const Arguments&,int);

}


#endif
