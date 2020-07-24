// This :
#include <Rio/Core/Map.h>

// Core :
#include <Rio/Core/Out.h>
#include <Rio/Core/Lib.h>

#include <string.h> //memset

namespace Rio {
class Assoc {
public:
  Assoc(unsigned long,long,long);
public:
  unsigned long fHash;
  long fKey;
  long fValue;
};
}

//////////////////////////////////////////////////////////////////////////////
Rio::Assoc::Assoc(
 unsigned long aHash
,long aKey
,long aValue
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fHash = aHash; 
  fKey = aKey; 
  fValue = aValue; 
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Rio::Map::Map(
 IPrinter& aPrinter
,int aMapSize
)
:fPrinter(aPrinter)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fSize  = (int)nextPrime(aMapSize);
  fTable = new Assoc* [fSize];
  ::memset(fTable, 0, fSize*sizeof(Assoc*));
  fTally = 0;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Map::~Map(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  for (int i = 0; i < fSize; i++) delete fTable[i];
  delete [] fTable; 
  fTable = 0;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Map::size(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fTally; 
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Map::add(
 unsigned long aHash
,long aKey
,long aValue
)
//////////////////////////////////////////////////////////////////////////////
// Add an (key,value) pair to the table. The key should be unique.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (!fTable) return;
  int slot = findElement(aHash,aKey);
  if (fTable[slot] == 0) {
    fTable[slot] = new Assoc(aHash, aKey, aValue);
    fTally++;
    if (highWaterMark()) expand(2 * fSize);
  } else {
    Out out(fPrinter);
    out << "Rio::Map::add : key " << aKey << " is not unique." << endl;
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Map::add(
 long aKey
,long aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  add((unsigned long)aKey,aKey,aValue); 
}
//////////////////////////////////////////////////////////////////////////////
long Rio::Map::findValue(
 unsigned long aHash
,long aKey
)
//////////////////////////////////////////////////////////////////////////////
// Return the value belonging to specified key and hash value. If key not
// found return 0.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (!fTable) return 0;

  int slot = int(aHash % fSize);
  for (int n = 0; n < fSize; n++) {
    if (!fTable[slot]) return 0;
    if (aKey == fTable[slot]->fKey) return fTable[slot]->fValue;
    if (++slot == fSize) slot = 0;
  }
  
  if (fTable[slot])
    return fTable[slot]->fValue;
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
long Rio::Map::findValue(
 long aKey
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return findValue(aKey,aKey); 
}

/*
//______________________________________________________________________________
Long_t &Rio::Map::operator()(ULong_t hash, Long_t key)
{
   // Return a reference to the value belonging to the key with the
   // specified hash value. If the key does not exist it will be added.

   static Long_t err;
   if (!fTable) {
      Error("operator()", "fTable==0, should never happen");
      return err;
   }

   Int_t slot = FindElement(hash, key);
   if (fTable[slot] == 0) {
      fTable[slot] = new Assoc_t(hash, key, 0);
      fTally++;
      if (HighWaterMark()) {
         Expand(2 * fSize);
         slot = FindElement(hash, key);
      }
   }
   return fTable[slot]->fValue;
}

//______________________________________________________________________________
void Rio::Map::Delete(Option_t *)
{
   // Delete all entries stored in the Rio::Map.

   for (int i = 0; i < fSize; i++) {
      if (fTable[i]) {
         delete fTable[i];
         fTable[i] = 0;
      }
   }
   fTally = 0;
}
*/
//////////////////////////////////////////////////////////////////////////////
void Rio::Map::remove(
 unsigned long aHash
,long aKey
)
//////////////////////////////////////////////////////////////////////////////
// Remove entry with specified key from the Rio::Map.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (!fTable) return;

  int i = findElement(aHash, aKey);
  if (fTable[i] == 0) {
    Out out(fPrinter);
    out << "Rio::Map::Remove : key " 
	<< aKey << " not found at " << i << endl;
    for (int j = 0; j < fSize; j++) {
      if (fTable[j] && fTable[j]->fKey == aKey) {
	out << "Rio::Map::remove : " 
	    << aKey << " found at " << j << " !!!" << endl;
	i = j;
      }
    }
  }
  
  if (fTable[i]) {
    delete fTable[i];
    fTable[i] = 0;
    fixCollisions(i);
    fTally--;
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Map::remove(
 long aKey
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  remove(aKey, aKey); 
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Map::fixCollisions(
 int aIndex
)
//////////////////////////////////////////////////////////////////////////////
// Rehash the map in case an entry has been removed.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int oldIndex, nextIndex;
  Assoc* nextObject;

  for (oldIndex = aIndex+1; ;oldIndex++) {
    if (oldIndex >= fSize) oldIndex = 0;
    nextObject = fTable[oldIndex];
    if (nextObject == 0) break;
    nextIndex = findElement(nextObject->fHash, nextObject->fKey);
    if (nextIndex != oldIndex) {
      fTable[nextIndex] = nextObject;
      fTable[oldIndex] = 0;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Map::highWaterMark(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return (bool) (fTally >= ((3*fSize)/4)); 
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Map::findElement(
 unsigned long aHash
,long aKey
)
//////////////////////////////////////////////////////////////////////////////
// Find an entry with specified hash and key in the Rio::Map.
// Returns the slot of the key or the next empty slot.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (!fTable) return 0;
  int slot = int(aHash % fSize);
  for (int n = 0; n < fSize; n++) {
    if (!fTable[slot]) return slot;
    if (aKey == fTable[slot]->fKey) return slot;
    if (++slot == fSize) slot = 0;
  }
  return slot;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Map::expand(
 int aNewSize
)
//////////////////////////////////////////////////////////////////////////////
// Expand the Rio::Map.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Assoc** oldTable = fTable;
  int oldsize = fSize;
  aNewSize = (int)nextPrime(aNewSize);
  fTable  = new Assoc* [aNewSize];
  ::memset(fTable, 0, aNewSize * sizeof(Assoc*));
  fSize = aNewSize;
  for (int i = 0; i < oldsize; i++) {
    Assoc* op;
    if ((op = oldTable[i])) {
      int slot = findElement(op->fHash, op->fKey);
      if (fTable[slot] == 0) {
	fTable[slot] = op;
      } else {
	Out out(fPrinter);
	out << "Rio::Map::expand : slot " 
	    << slot << " not empty (should never happen)." << endl;
      }
    }
  }
  delete [] oldTable;
}
