// this :
#include <Rio/Core/Arguments.h>

#define RIO_ARGS_DIRECTORY 0
#define RIO_ARGS_TREE 1
#define RIO_ARGS_BRANCH 2

void Rio::Args_insertDirectory(Arguments& aArgs,void* aValue){ 
  aArgs.push_back(std::pair<int,void*>(RIO_ARGS_DIRECTORY,aValue));
}
void Rio::Args_insertTree(Arguments& aArgs,void* aValue){ 
  aArgs.push_back(std::pair<int,void*>(RIO_ARGS_TREE,aValue));
}
void Rio::Args_insertBranch(Arguments& aArgs,void* aValue){ 
  aArgs.push_back(std::pair<int,void*>(RIO_ARGS_BRANCH,aValue));
}
void* Rio::Args_findItem(const Arguments& aArgs,int aKey) { 
  int n = aArgs.size();
  for(int i=0;i<n;i++){
    const std::pair<int,void*>& item = aArgs[i];
    if(item.first==aKey) return item.second;
  }
  return 0;
}
void* Rio::Args_findDirectory(const Arguments& aArgs) { 
  return Args_findItem(aArgs,RIO_ARGS_DIRECTORY);
}
void* Rio::Args_findTree(const Arguments& aArgs) { 
  return Args_findItem(aArgs,RIO_ARGS_TREE);
}
void* Rio::Args_findBranch(const Arguments& aArgs) { 
  return Args_findItem(aArgs,RIO_ARGS_BRANCH);
}


