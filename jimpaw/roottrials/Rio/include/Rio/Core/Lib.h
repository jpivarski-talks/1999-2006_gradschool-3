#ifndef Rio_Lib_h
#define Rio_Lib_h

namespace Rio {

void* realloc(void*,unsigned long,unsigned long);
unsigned long hash(const void*,int);
long nextPrime(long);
int binarySearch(int,int*,int);

}

#endif
