#ifndef Rio_Map_h
#define Rio_Map_h

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// This class stores a (key,value) pair using an external hash.         //
// The (key,value) are Long_t's and therefore can contain object        //
// pointers or any longs. The map uses an open addressing hashing       //
// method (linear probing).                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

namespace Rio {

class IPrinter;
class Assoc;

class Map {
public:
   Map(IPrinter&,int = 100);
   ~Map();
   void add(unsigned long,long,long);
   void add(long,long);
   long findValue(unsigned long,long);
   long findValue(long);
   void remove(unsigned long,long);
   void remove(long);
   int size() const;
   /*FIXME
   void delete(Option_t *opt = "");
   Int_t     Capacity() const { return fSize; }

   Long_t   &operator()(ULong_t hash, Long_t key);
   Long_t   &operator()(Long_t key) { return operator()(key, key); }
   */
private:
   void expand(int);
   int findElement(unsigned long,long);
   bool highWaterMark();
private:
   IPrinter& fPrinter;
   Assoc** fTable;
   int fSize;
   int fTally;
   void fixCollisions(int);
};

}

#endif




