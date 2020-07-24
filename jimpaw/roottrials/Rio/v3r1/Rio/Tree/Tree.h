#ifndef Rio_Tree_h
#define Rio_Tree_h

#include <Rio/Core/Named.h>
#include <Rio/Core/ObjArrayT.h>

namespace Rio {

class IDirectory;
class Branch;
class BaseLeaf;

class Tree : public Named {
  RIO_META_HEADER(Tree)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual void stream(IBuffer&);
public:
  Tree(IDirectory&);
  Tree(IDirectory&,const std::string&,const std::string&);
  virtual ~Tree();
  // Get/set :
  std::list<BaseLeaf*>& leaves();
  IDirectory& directory() const;
  IPrinter& printer() const;
  BaseLeaf* leaf(const std::string&);
  double entries() const;
  virtual int readEntry()  const;
  // Else :
  virtual Branch* createBranch(const std::string&,
			       void*,
			       const std::string&,
			       int = 32000);
  virtual Branch* createBranch(const std::string&,
			       IClass&,IObject**,int = 32000);
  virtual int fill();
  virtual void addTotBytes(int);
  virtual void addZipBytes(int);
  virtual void autoSave();
  virtual int entry(int = 0,int = 0);
  virtual void show(int = -1);
  virtual void incrementTotalBuffers(int);
  virtual void setAutoSave(int = 10000000);
  virtual Branch* findBranch(const std::string&);
  virtual int loadTree(int);
private:
  IDirectory& fDirectory;
  ObjArrayT<Branch, std::list<Branch*>::iterator > fBranches;
  ObjArrayT<BaseLeaf, std::list<BaseLeaf*>::iterator > fLeaves;
  double fEntries;      //  Number of entries
  double fTotBytes;     //  Total number of bytes in all branches before compression
  double fZipBytes;     //  Total number of bytes in all branches after compression
  double fSavedBytes;   //  Number of autosaved bytes
  int fAutoSave;        //  Autosave tree when fAutoSave bytes produced
  int fReadEntry;       //! Number of the entry being processed
  int fTotalBuffers;    //! Total number of bytes in branch buffers
};

}

#endif
