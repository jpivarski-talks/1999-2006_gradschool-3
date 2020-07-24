// this :
#include <Rio/Tree/TreeDictionary.h>

#include <Rio/Interfaces/IDirectory.h>

#include <Rio/Core/Class.h>
#include <Rio/Core/StreamerInfo.h>
#include <Rio/Core/StreamerElement.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/Out.h>

#include <Rio/Tree/Tree.h>
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/BranchObject.h>
#include <Rio/Tree/Basket.h>
#include <Rio/Tree/Leaf.h>
#include <Rio/Tree/LeafObject.h>

namespace Rio {

class TreeClass : public Class {
public: //Rio::IClass
  virtual IObject* create(const Arguments& aArgs) {
    IDirectory* directory = (IDirectory*)Args_findDirectory(aArgs);
    if(!directory) {
      Out out(fPrinter);
      out << "Rio::TreeClass::create : no directory given." << endl;
      return 0;
    }
    return new Tree(*directory);
  }
public:
  TreeClass(IPrinter& aPrinter):Class(aPrinter,"TTree",5){}
  virtual ~TreeClass(){}
};

class BranchClass : public Class {
public: //Rio::IClass
  virtual IObject* create(const Arguments& aArgs) {
    Tree* tree = (Tree*)Args_findTree(aArgs);
    if(!tree) {
      Out out(fPrinter);
      out << "Rio::BranchClass::create : no tree given." << endl;
      return 0;
    }
    return new Branch(*tree);
  }
public:
  BranchClass(IPrinter& aPrinter):Class(aPrinter,"TBranch",6){}
  virtual ~BranchClass(){}
};

class BranchObjectClass : public Class {
public: //Rio::IClass
  virtual IObject* create(const Arguments& aArgs) {
    Tree* tree = (Tree*)Args_findTree(aArgs);
    if(!tree) {
      Out out(fPrinter);
      out << "Rio::BranchObjectClass::create : no tree given." << endl;
      return 0;
    }
    return new BranchObject(*tree);
  }
public:
  BranchObjectClass(IPrinter& aPrinter):Class(aPrinter,"TBranchObject",1){}
  virtual ~BranchObjectClass(){}
};

class BasketClass : public Class {
public: //Rio::IClass
  virtual IObject* create(const Arguments& aArgs) {
    IDirectory* directory = (IDirectory*)Args_findDirectory(aArgs);
    if(!directory) {
      Out out(fPrinter);
      out << "Rio::BasketClass::create : no directory given." << endl;
      return 0;
    }
    Branch* branch = (Branch*)Args_findBranch(aArgs);
    if(!branch) {
      Out out(fPrinter);
      out << "Rio::BasketClass::create : no branch given." << endl;
      return 0;
    }
    return new Basket(*directory,*branch);
  }
public:
  BasketClass(IPrinter& aPrinter):Class(aPrinter,"TBasket",2){}
  virtual ~BasketClass(){}
};

class LeafFloatClass : public Class {
public: //Rio::IClass
  virtual IObject* create(const Arguments& aArgs) {
    Branch* branch = (Branch*)Args_findBranch(aArgs);
    if(!branch) {
      Out out(fPrinter);
      out << "Rio::LeafFloatClass::create : no branch given." << endl;
      return 0;
    }
    return new LeafFloat(*branch);
  }
public:
  LeafFloatClass(IPrinter& aPrinter):Class(aPrinter,"TLeafF",1){}
  virtual ~LeafFloatClass(){}
};

class LeafIntegerClass : public Class {
public: //Rio::IClass
  virtual IObject* create(const Arguments& aArgs) {
    Branch* branch = (Branch*)Args_findBranch(aArgs);
    if(!branch) {
      Out out(fPrinter);
      out << "Rio::LeafIntegerClass::create : no branch given." << endl;
      return 0;
    }
    return new LeafInteger(*branch);
  }
public:
  LeafIntegerClass(IPrinter& aPrinter):Class(aPrinter,"TLeafI",1){}
  virtual ~LeafIntegerClass(){}
};

class LeafObjectClass : public Class {
public: //Rio::IClass
  virtual IObject* create(const Arguments& aArgs) {
    Branch* branch = (Branch*)Args_findBranch(aArgs);
    if(!branch) {
      Out out(fPrinter);
      out << "Rio::LeafObjectClass::create : no branch given." << endl;
      return 0;
    }
    return new LeafObject(*branch);
  }
public:
  LeafObjectClass(IPrinter& aPrinter):Class(aPrinter,"TLeafObject",4){}
  virtual ~LeafObjectClass(){}
};

#define RIO_CLASS_DECL(aName,aClassName,aVersion) \
class aName : public Class {\
public:\
  virtual IObject* create(const Arguments&) {return 0;}\
public:\
  aName(IPrinter& aPrinter):Class(aPrinter,aClassName,aVersion){}\
  virtual ~aName(){}\
}

RIO_CLASS_DECL(BaseLeafClass,"TLeaf",2);
RIO_CLASS_DECL(AttLineClass,"TAttLine",1);
RIO_CLASS_DECL(AttFillClass,"TAttFill",1);
RIO_CLASS_DECL(AttMarkerClass,"TAttMarker",1);

}

//////////////////////////////////////////////////////////////////////////////
Rio::TreeDictionary::TreeDictionary(
 IPrinter& aPrinter
)
:CoreDictionary(aPrinter)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ///////////////////////////////////////////////////////////////////////
  // Classes :
  ///////////////////////////////////////////////////////////////////////
  IClass* clTree = new TreeClass(fPrinter);
  Tree::setClass(clTree);
  fClasses.push_back(clTree);

  IClass* clBranch = new BranchClass(fPrinter);
  Branch::setClass(clBranch);
  fClasses.push_back(clBranch);

  IClass* clBO = new BranchObjectClass(fPrinter);
  BranchObject::setClass(clBO);
  fClasses.push_back(clBO);

  IClass* clBL = new BaseLeafClass(fPrinter);
  BaseLeaf::setClass(clBL);
  fClasses.push_back(clBL);

  IClass* clLI = new LeafIntegerClass(fPrinter);
  LeafInteger::setClass(clLI);
  fClasses.push_back(clLI);

  IClass* clLF = new LeafFloatClass(fPrinter);
  LeafFloat::setClass(clLF);
  fClasses.push_back(clLF);

  IClass* clLO = new LeafObjectClass(fPrinter);
  LeafObject::setClass(clLO);
  fClasses.push_back(clLO);

  IClass* clBasket = new BasketClass(fPrinter);
  Basket::setClass(clBasket);
  fClasses.push_back(clBasket);
  // Has no streamer info. 

  IClass* clAttLine = new AttLineClass(fPrinter);
  fClasses.push_back(clAttLine);
  IClass* clAttFill = new AttFillClass(fPrinter);
  fClasses.push_back(clAttFill);
  IClass* clAttMarker = new AttMarkerClass(fPrinter);
  fClasses.push_back(clAttMarker);

  ///////////////////////////////////////////////////////////////////////
  // StreamerInfos :
  ///////////////////////////////////////////////////////////////////////
  StreamerInfo* streamerInfo = new StreamerInfo(*clTree,"",-1049922452);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase
("TNamed","The basis for a named object (name, title)",0,Named::is().version()));
  streamerInfo->add(new StreamerBase
("TAttLine","Line attributes",28,clAttLine->version()));
  streamerInfo->add(new StreamerBase
("TAttFill","Fill area attributes",40,clAttFill->version()));
  streamerInfo->add(new StreamerBase
("TAttMarker","Marker attributes",48,clAttMarker->version()));
  streamerInfo->add(new StreamerBasicType
("fEntries","Number of entries",60,StreamerInfo::DOUBLE,"Stat_t"));
  streamerInfo->add(new StreamerBasicType
("fTotBytes","Total number of bytes in all branches before compression",68,StreamerInfo::DOUBLE,"Stat_t"));
  streamerInfo->add(new StreamerBasicType
("fZipBytes","Total number of bytes in all branches after compression",76,StreamerInfo::DOUBLE,"Stat_t"));
  streamerInfo->add(new StreamerBasicType
("fSavedBytes","Number of autosaved bytes",84,StreamerInfo::DOUBLE,"Stat_t"));
  streamerInfo->add(new StreamerBasicType
("fTimerInterval","Timer interval in milliseconds",92,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fScanField","Number of runs before prompting in Scan",96,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fUpdate","Update frequency for EntryLoop",100,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fMaxEntryLoop","Maximum number of entries to process",104,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fMaxVirtualSize","Maximum total size of buffers kept in memory",108,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fAutoSave","Autosave tree when fAutoSave bytes produced",112,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fEstimate","Number of entries to estimate histogram limits",116,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerObject
("fBranches","List of Branches",144,"TObjArray"));
  streamerInfo->add(new StreamerObject
("fLeaves","Direct pointers to individual branch leaves",184,"TObjArray"));
  streamerInfo->add(new StreamerObjectAny
("fIndexValues","Sorted index values",228,"TArrayD"));
  streamerInfo->add(new StreamerObjectAny
("fIndex","Index of sorted values",240,"TArrayI"));

  streamerInfo = new StreamerInfo(*clBranch,"",2056727376);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase
("TNamed","The basis for a named object (name, title)",0,Named::is().version()));
  streamerInfo->add(new StreamerBasicType
("fCompress","(=1 branch is compressed, 0 otherwise)",28,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fBasketSize","Initial Size of  Basket Buffer",32,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fEntryOffsetLen","Initial Length of fEntryOffset table in the basket buffers",36,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fWriteBasket","Last basket number written",40,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fEntryNumber","Current entry number (last one filled in this branch)",44,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fOffset","Offset of this branch",48,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fMaxBaskets","Maximum number of Baskets so far",52,StreamerInfo::COUNTER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fEntries","Number of entries",68,StreamerInfo::DOUBLE,"Stat_t"));
  streamerInfo->add(new StreamerBasicType
("fTotBytes","Total number of bytes in all leaves before compression",76,StreamerInfo::DOUBLE,"Stat_t"));
  streamerInfo->add(new StreamerBasicType
("fZipBytes","Total number of bytes in all leaves after compression",84,StreamerInfo::DOUBLE,"Stat_t"));
  streamerInfo->add(new StreamerObject
("fBranches","-> List of Branches of this branch",92,"TObjArray"));
  streamerInfo->add(new StreamerObject
("fLeaves","-> List of leaves of this branch",132,"TObjArray"));
  streamerInfo->add(new StreamerObject
("fBaskets","-> List of baskets of this branch",172,"TObjArray"));
  streamerInfo->add(new StreamerBasicPointer
("fBasketBytes","[fMaxBaskets] Lenght of baskets on file",220,StreamerInfo::INTEGER,"fMaxBaskets","TBranch",6,"Int_t*"));
  streamerInfo->add(new StreamerBasicPointer
("fBasketEntry","[fMaxBaskets] Table of first entry in eack basket",224,StreamerInfo::INTEGER,"fMaxBaskets","TBranch",6,"Int_t*"));
  streamerInfo->add(new StreamerBasicPointer
("fBasketSeek","[fMaxBaskets] Addresses of baskets on file",228,StreamerInfo::INTEGER,"fMaxBaskets","TBranch",6,"Seek_t*"));
  streamerInfo->add(new StreamerString
("fFileName","Name of file where buffers are stored (\"\" if in same file as Tree header)",244));

  streamerInfo = new StreamerInfo(*clBO,"",-1437088761);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase
("TBranch","Branch descriptor",0,Branch::is().version()));
  streamerInfo->add(new StreamerString
("fClassName","Class name of referenced object",256));

  streamerInfo = new StreamerInfo(*clBL,"",727988519);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase
("TNamed","The basis for a named object (name, title)",0,Named::is().version()));
  streamerInfo->add(new StreamerBasicType
("fLen","Number of fixed length elements",32,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fLenType","Number of bytes for this data type",36,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fOffset","Offset in ClonesArray object (if one)",40,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fIsRange","(=kTRUE if leaf has a range, kFALSE otherwise)",44,StreamerInfo::UNSIGNED_CHAR,"Bool_t"));
  streamerInfo->add(new StreamerBasicType
("fIsUnsigned","(=kTRUE if unsigned, kFALSE otherwise)",45,StreamerInfo::UNSIGNED_CHAR,"Bool_t"));
  streamerInfo->add(new StreamerObjectPointer
("fLeafCount","Pointer to Leaf count if variable length",48,"TLeaf*"));

  streamerInfo = new StreamerInfo(*clLI,"",-799765899);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase
("TLeaf","Leaf: description of a Branch data type",0,BaseLeaf::is().version()));
  streamerInfo->add(new StreamerBasicType
("fMinimum","Minimum value if leaf range is specified",56,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fMaximum","Maximum value if leaf range is specified",60,StreamerInfo::INTEGER,"Int_t"));

  streamerInfo = new StreamerInfo(*clLF,"",1366318032);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase
("TLeaf","Leaf: description of a Branch data type",0,BaseLeaf::is().version()));
  streamerInfo->add(new StreamerBasicType
("fMinimum","Minimum value if leaf range is specified",56,StreamerInfo::FLOAT,"Float_t"));
  streamerInfo->add(new StreamerBasicType
("fMaximum","Maximum value if leaf range is specified",60,StreamerInfo::FLOAT,"Float_t"));
 
  streamerInfo = new StreamerInfo(*clLO,"",-1982305487);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase
("TLeaf","Leaf: description of a Branch data type",0,BaseLeaf::is().version()));
  streamerInfo->add(new StreamerBasicType
("fVirtual","Support for Virtuality",64,StreamerInfo::UNSIGNED_CHAR,"Bool_t"));
 
  // Beurk ; but the ROOT TTree compells indirectly the below.
  streamerInfo = new StreamerInfo(*clAttLine,"",1369587346);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBasicType
("fLineColor","line color",0,StreamerInfo::SHORT,"Color_t"));
  streamerInfo->add(new StreamerBasicType
("fLineStyle","line style",2,StreamerInfo::SHORT,"Style_t"));
  streamerInfo->add(new StreamerBasicType
("fLineWidth","line width",4,StreamerInfo::SHORT,"Width_t"));

  streamerInfo = new StreamerInfo(*clAttFill,"",1204118360);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBasicType
("fFillColor","fill area color",0,StreamerInfo::SHORT,"Color_t"));
  streamerInfo->add(new StreamerBasicType
("fFillStyle","fill area style",2,StreamerInfo::SHORT,"Style_t"));

  streamerInfo = new StreamerInfo(*clAttMarker,"",-87219836);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBasicType
("fMarkerColor","Marker color index",0,StreamerInfo::SHORT,"Color_t"));
  streamerInfo->add(new StreamerBasicType
("fMarkerStyle","Marker style",2,StreamerInfo::SHORT,"Style_t"));
  streamerInfo->add(new StreamerBasicType
("fMarkerSize","Marker size",4,StreamerInfo::FLOAT,"Size_t"));

}
//////////////////////////////////////////////////////////////////////////////
Rio::TreeDictionary::~TreeDictionary(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}



/* ROOT tag v3-00-06 :
debug : TStreamerInfo : +++++ TTree -1049922452
debug : TStreamerInfo : +++++ TAttLine 1369587346
debug : TStreamerInfo : +++++ TAttFill 1204118360
debug : TStreamerInfo : +++++ TAttMarker -87219836
debug : TStreamerInfo : +++++ TBranch 2056727376
debug : TStreamerInfo : +++++ TLeafI -799765899
debug : TStreamerInfo : +++++ TLeaf 727988519
debug : TStreamerInfo : +++++ TLeafF 1366318032
*/
