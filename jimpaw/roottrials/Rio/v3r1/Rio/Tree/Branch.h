#ifndef Rio_Branch_h
#define Rio_Branch_h

#include <Rio/Core/Named.h>
#include <Rio/Core/Seek.h>
#include <Rio/Core/ObjArrayT.h>

namespace Rio {

class Buffer;
class Tree;
class Basket;
class BaseLeaf;

class Branch : public Named {
  RIO_META_HEADER(Branch)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public:
  Branch(Tree&);
  Branch(Tree&,const std::string&,const std::string&,
	 void*,const std::string&,int,int);
  virtual ~Branch();
  virtual int fill();
  // Get/set :
  int basketSize() const;
  int entryOffsetLength() const;
  int writeBasket() const;
  void setEntryOffsetLength(int);
  Tree& tree() const;
  int compressionLevel() const;
  BaseLeaf* leaf(const std::string&);
  double entries() const;
  // Else :
  virtual void fillLeaves(IBuffer&);
  virtual int entry(int = 0,int = 0);
  virtual void readLeaves(IBuffer&);
  virtual IPrinter& printer() const;
  virtual void reset();
  virtual void setAddress(void*);
  bool isAutoDelete() const;
  virtual void setAutoDelete(bool = true);
  virtual void updateAddress();
  virtual void setBasketSize(int);
private:
  Basket* getBasket(int);
protected:
  Tree& fTree;
private:
  bool fAutoDelete;
protected:
  ObjArrayT<Branch, std::list<Branch*>::iterator > fBranches;
  ObjArrayT<BaseLeaf, std::list<BaseLeaf*>::iterator > fLeaves;
  ObjArrayT<Basket, std::list<Basket*>::iterator > fBaskets;
  int fCompress;        //  (=1 branch is compressed, 0 otherwise)
  int fBasketSize;      //  Initial Size of  Basket Buffer
private:
  int fEntryOffsetLen;  //  Initial Length of fEntryOffset table in the basket buffers
  int fWriteBasket;     //  Last basket number written
protected:
  int fEntryNumber;     //  Current entry number (last one filled in this branch)
  int fMaxBaskets;      //  Maximum number of Baskets so far
private:
  int fOffset;          //Offset of this branch
protected:
  int fNleaves;         //! Number of leaves
private:
  int fReadBasket;      //! Current basket number when reading
protected:
  int fReadEntry;       //! Current entry number when reading
  double fEntries;      //  Number of entries
private:
  double fTotBytes;  //  Total number of bytes in all leaves before compression
  double fZipBytes;  //  Total number of bytes in all leaves after compression
  int fNBasketRAM;      //! Number of baskets in fBasketRAM
  int* fBasketRAM;      //! [fNBasketRAM] table of basket numbers in memory
protected:
  int* fBasketBytes;    //[fMaxBaskets] Lenght of baskets on file
  int* fBasketEntry;    //[fMaxBaskets] Table of first entry in eack basket
  Seek* fBasketSeek;    //[fMaxBaskets] Addresses of baskets on file
  char* fAddress;       //! Address of 1st leaf (variable or object)
private:
  Buffer* fEntryBuffer; //! Buffer used to directly pass the content without streaming
};

}

#endif
