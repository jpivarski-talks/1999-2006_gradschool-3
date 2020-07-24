// this :
#include <Rio/Tree/Tree.h>

// Core :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDictionary.h>
#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IFile.h>

#include <Rio/Core/Arguments.h>
#include <Rio/Core/smanip.h>
#include <Rio/Core/Out.h>

// Tree :
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/BranchObject.h>
#include <Rio/Tree/BaseLeaf.h>

#define MINIMUM(a,b) ((a)<(b)?a:b) 

RIO_META_SOURCE(Rio::Tree)

//////////////////////////////////////////////////////////////////////////////
Rio::Tree::Tree(
 IDirectory& aDirectory
)
:Named("","")
,fDirectory(aDirectory)
,fBranches(aDirectory.printer())
,fLeaves(aDirectory.printer())
,fEntries(0)
,fTotBytes(0)
,fZipBytes(0)
,fSavedBytes(0)
,fAutoSave(100000000)
,fReadEntry(-1)
,fTotalBuffers(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fDirectory.file().verboseLevel()) {
    Out out(printer());
    out << "Rio::Tree::Tree1 : called for \"" << fName << "\" dir \"" 
	<< fDirectory.name() << "\"." << endl;
  }

  fDirectory.appendObject(this);
}
//////////////////////////////////////////////////////////////////////////////
Rio::Tree::Tree(
 IDirectory& aDirectory
,const std::string& aName
,const std::string& aTitle
)
:Named(aName,aTitle)
,fDirectory(aDirectory)
,fBranches(aDirectory.printer())
,fLeaves(aDirectory.printer())
,fEntries(0)
,fTotBytes(0)
,fZipBytes(0)
,fSavedBytes(0)
,fAutoSave(100000000)
,fReadEntry(-1)
,fTotalBuffers(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fDirectory.file().verboseLevel()) {
    Out out(printer());
    out << "Rio::Tree::Tree2 : called for \"" << fName << "\" dir \"" 
	<< fDirectory.name() << "\"." << endl;
  }

  fDirectory.appendObject(this);
}
//////////////////////////////////////////////////////////////////////////////
Rio::Tree::~Tree(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fDirectory.file().verboseLevel()) {
    Out out(printer());
    out << "Rio::Tree::~Tree : called for \"" << fName << "\"." << endl;
  }

  if(!fDirectory.isClosing()) fDirectory.removeObject(this);
    
  std::list<Branch*>::iterator it;
  for(it=fBranches.begin();it!=fBranches.end();it=fBranches.erase(it)) 
    delete (*it);

  fLeaves.clear(); // Leaves deleted by the Branches.

}
//////////////////////////////////////////////////////////////////////////////
Rio::IDirectory& Rio::Tree::directory(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fDirectory;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IPrinter& Rio::Tree::printer(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fDirectory.printer();
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::Tree::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (Tree*)this;
  else return Named::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
std::list<Rio::BaseLeaf*>& Rio::Tree::leaves(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fLeaves;
}
//////////////////////////////////////////////////////////////////////////////
double Rio::Tree::entries(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fEntries;
}
//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf* Rio::Tree::leaf(
 const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<BaseLeaf*>::const_iterator it;
  for(it=fLeaves.begin();it!=fLeaves.end();++it) {
    if((*it)->name()==aName) return (*it);
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Tree::incrementTotalBuffers(
 int aNumberOfBytes
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fTotalBuffers += aNumberOfBytes;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Tree::addTotBytes(
 int aTotal
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fTotBytes += aTotal;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Tree::addZipBytes(
 int aZIP
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fZipBytes += aZIP;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Branch* Rio::Tree::createBranch(
 const std::string& aName
,void* aAddress
,const std::string& aLeafList
,int aBufferSize
)
//////////////////////////////////////////////////////////////////////////////
// Create a new TTree Branch :
//     This Branch constructor is provided to support non-objects in
//     a Tree. The variables described in leaflist may be simple variables
//     or structures.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Branch* branch = new Branch(*this,
			      aName,
			      fName, // Branch title is tree name.
			      aAddress,aLeafList,aBufferSize,
			      fDirectory.file().compressionLevel());
  /*FIXME
    if (branch->IsZombie()) {
    delete branch;
    return 0;
    }
  */
  fBranches.push_back(branch);
  return branch;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Branch* Rio::Tree::createBranch(
 const std::string& aName
,IClass& aClass
,IObject** aAddObj
,int aBufSize
)
//////////////////////////////////////////////////////////////////////////////
// Create a new TTree BranchObject
//
//    Build a TBranchObject for an object of class classname.
//    addobj is the address of a pointer to an object of class classname.
//    IMPORTANT: classname must derive from TObject.
//    The class dictionary must be available (ClassDef in class header).
//
//    This option requires access to the library where the corresponding class
//    is defined. Accessing one single data member in the object implies
//    reading the full object.
//    See the next Branch constructor for a more efficient storage
//    in case the entry consists of arrays of identical objects.
//
//    By default the branch buffers are stored in the same file as the Tree.
//    use TBranch::SetFile to specify a different file
//
//      IMPORTANT NOTE about branch names
//    In case two or more master branches contain subbranches with
//    identical names, one must add a "." (dot) character at the end
//    of the master branch name. This will force the name of the subbranch
//    to be master.subbranch instead of simply subbranch.
//    This situation happens when the top level object (say event)
//    has two or more members referencing the same class.
//    For example, if a Tree has two branches B1 and B2 corresponding
//    to objects of the same class MyClass, one can do:
//       tree.Branch("B1.","MyClass",&b1,8000,1);
//       tree.Branch("B2.","MyClass",&b2,8000,1);
//    if MyClass has 3 members a,b,c, the two instructions above will generate
//    subbranches called B1.a, B1.b ,B1.c, B2.a, B2.b, B2.c
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Branch* branch = new BranchObject(*this,
				    aName,
				    fName, // Branch title is tree name.
				    aClass,aAddObj,aBufSize);
  fBranches.push_back(branch);

  /* FIXME splitting not yet supported by Rio.
  if (!aSplitLevel) return branch;
  TObjArray *blist = branch->GetListOfBranches();
  const char *rdname;
  const char *dname;
  char branchname[128];
  char **apointer = (char**)(addobj);
  TObject *obj = (TObject*)(*apointer);
  Bool_t delobj = kFALSE;
  if (!obj) {
    obj = (TObject*)cl->New();
    delobj = kTRUE;
  }
  //build the StreamerInfo if first time for the class
  BuildStreamerInfo(cl,obj);
   
  // Loop on all public data members of the class and its base classes
  Int_t lenName = strlen(name);
  Int_t isDot = 0;
  if (name[lenName-1] == '.') isDot = 1;
  TBranch *branch1 = 0;
  TRealData *rd, *rdi;
  TIter      nexti(cl->GetListOfRealData());
  TIter      next(cl->GetListOfRealData());
  while ((rd = (TRealData *) next())) {
    TDataMember *dm = rd->GetDataMember();
    if (!dm->IsPersistent()) continue; //do not process members with a ! as the first
    // character in the comment field
    if (rd->IsObject()) {
      TClass *clm = gROOT->GetClass(dm->GetFullTypeName());
      if (clm) BuildStreamerInfo(clm,(char*)obj+rd->GetThisOffset());
      continue;
    }
    rdname = rd->GetName();
    dname  = dm->GetName();
    if (cl->CanIgnoreTObjectStreamer()) {
      if (strcmp(dname,"fBits") == 0) continue;
      if (strcmp(dname,"fUniqueID") == 0) continue;
    }
    
    TDataType *dtype = dm->GetDataType();
    Int_t code = 0;
    if (dtype) code = dm->GetDataType()->GetType();
    
    // Encode branch name. Use real data member name
    sprintf(branchname,"%s",rdname);
    if (isDot) {
      if (dm->IsaPointer()) sprintf(branchname,"%s%s",name,&rdname[1]);
      else                  sprintf(branchname,"%s%s",name,&rdname[0]);
    }
    char leaflist[128];
    Int_t offset    = rd->GetThisOffset();
    char *pointer   = (char*)obj + offset;
    if (dm->IsaPointer()) {
      TClass *clobj = 0;
      if (!dm->IsBasic()) clobj = gROOT->GetClass(dm->GetTypeName());
      if (clobj && clobj->InheritsFrom("TClonesArray")) {
	char *cpointer  =(char*)pointer;
	char **ppointer =(char**)cpointer;
	TClonesArray *list = (TClonesArray*)(*ppointer);
	if (splitlevel != 2) {
	  if (isDot) branch1 = new TBranchClones(&branchname[0],pointer,bufsize);
	  else       branch1 = new TBranchClones(&branchname[1],pointer,bufsize);
	  blist->Add(branch1);
	} else {
	  if (isDot) branch1 = new TBranchObject(&branchname[0],list->ClassName(),pointer,bufsize);
	  else       branch1 = new TBranchObject(&branchname[1],list->ClassName(),pointer,bufsize);
	  blist->Add(branch1);
	}
      } else {
	if (!clobj) {
	  const char * index = dm->GetArrayIndex();
	  if (strlen(index)!=0) {
	    //check that index is a valid data member name
	    //if member is part of an object (eg fA and index=fN)
	    //index must be changed from fN to fA.fN
	    char aindex[128];
	    strcpy(aindex,rd->GetName());
	    char *rdot = strrchr(aindex,'.');
	    if (rdot) strcpy(rdot+1,index);
	    nexti.Reset();
	    while ((rdi = (TRealData *) nexti())) {
	      if (strcmp(rdi->GetName(),index) == 0) break;
	      if (strcmp(rdi->GetName(),aindex) == 0) {index = rdi->GetName(); break;}
	    }
	    if      (code ==  1) 
	      // Note that we differentiate between strings and
	      // char array by the fact that there is NO specified
	      // size for a string (see next if (code == 1)
	      sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"B");
	    else if (code == 11) 
	      sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"b");
	    else if (code ==  2) 
	      sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"S");
	    else if (code == 12) 
	      sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"s");
	    else if (code ==  3) 
	      sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"I");
	    else if (code == 13) 
	      sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"i");
	    else if (code ==  5) 
	      sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"F");
	    else if (code ==  8) 
	      sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"D");
	    else {
	      printf("Cannot create branch for rdname=%s, code=%d\n",branchname, code);
	      leaflist[0] = 0;
	    }
	  } else {
	    if (code == 1) {
	      sprintf(leaflist,"%s/%s",dname,"C");
	    } else {
	      continue;
	    }
	  }
	  
	  // there are '*' in both the branchname and rdname
	  char bname[128];
	  UInt_t cursor,pos;
	  for(cursor = 0, pos = 0; 
	      cursor< strlen(branchname); 
	      cursor++ ) {
	    if (branchname[cursor]!='*') {
	      bname[pos++]=branchname[cursor];
	    }
	  }
	  bname[pos] = '\0';
	  for(cursor = 0, pos = 0; cursor< strlen(leaflist); cursor++ ) {
	    if (leaflist[cursor]!='*') {
	      leaflist[pos++]=leaflist[cursor];
	    }
	  }
	  leaflist[pos] = '\0';
	  
	  // Add the branch to the tree and indicate that the address
	  // is that of a pointer to be dereferenced before using.
	  branch1 = new TBranch(bname,*(void**)pointer,leaflist,bufsize);
	  TLeaf *leaf = (TLeaf*) branch1->GetListOfLeaves()->At(0);
	  leaf->SetBit(TLeaf::kIndirectAddress);
	  leaf->SetAddress((void**)pointer);
	  blist->Add(branch1);
	} else {
	  if (!clobj->InheritsFrom(TObject::Class())) continue;
	  branch1 = new TBranchObject(dname,clobj->GetName(),pointer,bufsize,0);
	  if (isDot) branch1->SetName(&branchname[0]);
	  else       branch1->SetName(&branchname[1]);  //do not use the first character (*)
	  blist->Add(branch1);
	}
      }
    } else {
      //-------------Data Member is a basic data type----------
      if (dm->IsBasic()) {
	if      (code ==  1) sprintf(leaflist,"%s/%s",rdname,"B");
	else if (code == 11) sprintf(leaflist,"%s/%s",rdname,"b");
	else if (code ==  2) sprintf(leaflist,"%s/%s",rdname,"S");
	else if (code == 12) sprintf(leaflist,"%s/%s",rdname,"s");
	else if (code ==  3) sprintf(leaflist,"%s/%s",rdname,"I");
	else if (code == 13) sprintf(leaflist,"%s/%s",rdname,"i");
	else if (code ==  5) sprintf(leaflist,"%s/%s",rdname,"F");
	else if (code ==  8) sprintf(leaflist,"%s/%s",rdname,"D");
	else {
	  printf("Cannot create branch for rdname=%s, code=%d\n",branchname, code);
	  leaflist[0] = 0;
	}
	branch1 = new TBranch(branchname,pointer,leaflist,bufsize);
	branch1->SetTitle(rdname);
	blist->Add(branch1);
      }
    }
    if (branch1) branch1->SetOffset(offset);
    else Warning("Branch","Cannot process member:%s",rdname);
    
  }
  if (delobj) delete obj;
  */

  return branch;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Tree::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Stream a class object.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s, c;
    short v = aBuffer.readVersion(&s, &c);
    if (v > 4) {
      /*
      TTree::Class()->ReadBuffer(b, this, v, s, c);
      if (fEstimate <= 10000) fEstimate = 1000000;
      fSavedBytes = fTotBytes;
      fDirectory = gDirectory;
      gDirectory->Append(this);
      return;
      */
    }
    //====process old versions before automatic schema evolution

    short dummy_short;
    float dummy_float;
    double dummy_double;
    int dummy_int;

    Named::stream(aBuffer);

    //TAttLine::Streamer(b);
    {unsigned int s, c;
     aBuffer.readVersion(&s, &c);
     aBuffer.read(dummy_short); //fLineColor
     aBuffer.read(dummy_short); //fLineStyle
     aBuffer.read(dummy_short); //fLineWidth
     aBuffer.checkByteCount(s, c,"TAttLine");}

    //TAttFill::Streamer(b);
    {unsigned int s, c;
     aBuffer.readVersion(&s, &c);
     aBuffer.read(dummy_short); //fFillColor
     aBuffer.read(dummy_short); //fFillStyle
     aBuffer.checkByteCount(s, c,"TAttFill");}

    //TAttMarker::Streamer(b);
    {unsigned int s, c;
     aBuffer.readVersion(&s, &c);
     aBuffer.read(dummy_short); //fMarkerColor
     aBuffer.read(dummy_short); //fMarkerStyle
     aBuffer.read(dummy_float); //fMarkerWidth
     aBuffer.checkByteCount(s, c,"TAttMarker");}

    if(v<5) {
      aBuffer.read(dummy_int); //fScanField
      aBuffer.read(dummy_int); //fMaxEntryLoop
      aBuffer.read(dummy_int); //fMaxVirtualSize
      aBuffer.read(fEntries);
      aBuffer.read(fTotBytes);
      aBuffer.read(fZipBytes);
      aBuffer.read(fAutoSave);
      aBuffer.read(dummy_int); //fEstimate
    } else {
      aBuffer.read(fEntries);
      aBuffer.read(fTotBytes);
      aBuffer.read(fZipBytes);
      aBuffer.read(dummy_double); //fSavedBytes
      aBuffer.read(dummy_int); //fTimerInterval
      aBuffer.read(dummy_int); //fScanField (25)
      aBuffer.read(dummy_int); //fUpdate
      aBuffer.read(dummy_int); //fMaxEntryLoop (1 000 000 000)
      aBuffer.read(dummy_int); //fMaxVirtualSize (0)
      aBuffer.read(fAutoSave);  // (100 000 000))
      aBuffer.read(dummy_int); //fEstimate (1 000 000)
    }
    //FIXME if (fEstimate <= 10000) fEstimate = 1000000;

    Arguments args;
    Args_insertDirectory(args,&fDirectory);
    Args_insertTree(args,this);

    fBranches.read(aBuffer,args);

    // We read leaves in order to keep streaming synchronisation.
    // In fact leaves are references to existing leaves read by 
    // the branches in the upper line of code.
    ObjArrayT<BaseLeaf, std::list<BaseLeaf*>::iterator > leaves(printer());
    leaves.read(aBuffer,args);

    fSavedBytes = fTotBytes;
    /*
    fDirectory = gDirectory;
    gDirectory->Append(this);
    */
    double* dummy_double_pointer;
    aBuffer.readArray(dummy_double_pointer); 
    int* dummy_int_pointer;
    aBuffer.readArray(dummy_int_pointer); 

    if(v>=6) { //TList* fFriends
      Arguments args;
      aBuffer.readObject(args);
    } 

    aBuffer.checkByteCount(s, c,sClass->name());
    //====end of old versions

  } else {
    //TTree::Class()->WriteBuffer(b,this);
    //====process old versions before automatic schema evolution
    unsigned int c = aBuffer.writeVersion(sClass->version(),true);

    Named::stream(aBuffer);

    IDictionary& dico = fDirectory.file().dictionary();
    //TAttLine::Streamer(b);  //Beurk.
    {unsigned int c = 
       aBuffer.writeVersion(dico.findClass("TAttLine")->version(),true);
     aBuffer.write((short)1); //fLineColor
     aBuffer.write((short)1); //fLineStyle
     aBuffer.write((short)1); //fLineWidth
     aBuffer.setByteCount(c,true);}
    
    //TAttFill::Streamer(b); //Beurk.
    {unsigned int c = 
       aBuffer.writeVersion(dico.findClass("TAttFill")->version(),true);
     aBuffer.write((short)0);    //fFillColor
     aBuffer.write((short)1001); //fFillStyle
     aBuffer.setByteCount(c,true);}

    //TAttMarker::Streamer(b); //Beurk.
   {unsigned int c = 
      aBuffer.writeVersion(dico.findClass("TAttMarker")->version(),true);
    aBuffer.write((short)1); //fMarkerColor
    aBuffer.write((short)1); //fMarkerStyle
    aBuffer.write((float)1); //fMarkerWidth
    aBuffer.setByteCount(c,true);}

    if(sClass->version()<5) {
      aBuffer.write((int)25);         //fScanField
      aBuffer.write((int)1000000000); //fMaxEntryLoop
      aBuffer.write((int)0);          //fMaxVirtualSize
      aBuffer.write(fEntries);
      aBuffer.write(fTotBytes);
      aBuffer.write(fZipBytes);
      aBuffer.write(fAutoSave);
      aBuffer.write((int)1000000);    //fEstimate;
    } else {
      aBuffer.write(fEntries);
      aBuffer.write(fTotBytes);
      aBuffer.write(fZipBytes);
      aBuffer.write((double)0); //fSavedBytes
      aBuffer.write((int)0);    //fTimerInterval
      aBuffer.write((int)25);   //fScanField (25)
      aBuffer.write((int)0);    //fUpdate
      aBuffer.write((int)1000000000); //fMaxEntryLoop
      aBuffer.write((int)0);          //fMaxVirtualSize
      aBuffer.write(fAutoSave);
      aBuffer.write((int)1000000);    //fEstimate;
    }
    fBranches.write(aBuffer);
    fLeaves.write(aBuffer);

    aBuffer.writeArray((double*)0,0);  // fIndexValues (TArrayD).
    aBuffer.writeArray((int*)0,0);  // fIndex (TArrayI).

    aBuffer.setByteCount(c,true);
    //====end of old versions
  }

}
//////////////////////////////////////////////////////////////////////////////
int Rio::Tree::fill(
)
//////////////////////////////////////////////////////////////////////////////
// Fill all branches of a Tree :
//   This function loops on all the branches of this tree.
//   For each branch, it copies to the branch buffer (basket) the current
//   values of the leaves data types.
//   If a leaf is a simple data type, a simple conversion to a machine
//   independent format has to be done.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*FIXME
  //case of one single super branch. Automatically update
  // all the branch addresses if a new object was created
  if (nb == 1) {
    branch = (TBranch*)fBranches.UncheckedAt(0);
    branch->UpdateAddress();
    }*/
  
  int nbytes = 0;
  std::list<Branch*>::const_iterator it;
  for(it=fBranches.begin();it!=fBranches.end();++it) {
    //FIXME if ((*it)->testBit(kDoNotProcess)) continue;
    nbytes += (*it)->fill();
  }

  fEntries++;
  if (fTotBytes - fSavedBytes > fAutoSave) autoSave();

  return nbytes;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Tree::setAutoSave(
 int aAutoSave
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fAutoSave = aAutoSave;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Tree::autoSave(
)
//////////////////////////////////////////////////////////////////////////////
// AutoSave tree header every fAutoSave bytes :
//
//   When large Trees are produced, it is safe to activate the AutoSave
//   procedure. Some branches may have buffers holding many entries.
//   AutoSave is automatically called by TTree::Fill when the number of bytes
//   generated since the previous AutoSave is greater than fAutoSave bytes.
//   This function may also be invoked by the user, for example every
//   N entries.
//   Each AutoSave generates a new key on the file.
//   Once the key with the tree header has been written, the previous cycle
//   (if any) is deleted.
//
//   Note that calling TTree::AutoSave too frequently (or similarly calling
//   TTree::SetAutoSave with a small value) is an expensive operation.
//   You should make tests for your own application to find a compromize
//   between speed and the quantity of information you may loose in case of
//   a job crash.
//
//   In case your program crashes before closing the file holding this tree,
//   the file will be automatically recovered when you will connect the file
//   in UPDATE mode.
//   The Tree will be recovered at the status corresponding to the 
//   last AutoSave.
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //   printf("AutoSave Tree:%s after %g bytes written\n",GetName(),fTotBytes);
  fSavedBytes = fTotBytes;
  fDirectory.saveObject(*this);
  // save StreamerInfo
  //FIXME TFile *file = fDirectory->GetFile();
  //FIXME if (file) file->WriteStreamerInfo();
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Tree::entry(
 int aEntry
,int aGetAll
)
//////////////////////////////////////////////////////////////////////////////
// Read all branches of entry and return total number of bytes :
//     getall = 0 : get only active branches
//     getall = 1 : get all branches
//
//  The function returns the number of bytes read from the input buffer.
//  If entry does not exist or an I/O error occurs, the function returns 0.
//  if entry is the same as the previous call, the function returns 1.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aEntry < 0 || aEntry >= fEntries) return 0;
  int nbytes = 0;
  fReadEntry = aEntry;
  std::list<Branch*>::const_iterator it;
  for(it=fBranches.begin();it!=fBranches.end();++it) {
    nbytes += (*it)->entry(aEntry,aGetAll);
  }
  return nbytes;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Tree::show(
 int aEntry
)
//////////////////////////////////////////////////////////////////////////////
// Print values of all active leaves for entry :
// if entry==-1, print current entry (default)
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aEntry != -1) entry(aEntry);
  Out out(printer());
  out << "======> EVENT:" << fReadEntry << endl;
  std::list<BaseLeaf*>::const_iterator it;
  for(it=fLeaves.begin();it!=fLeaves.end();++it) {
    BaseLeaf* leaf = *it;
    //Branch& branch = leaf->branch();
    //FIXME if (branch->TestBit(kDoNotProcess)) continue;
    //FIXME if (branch.GetListOfBranches()->GetEntriesFast() > 0) continue;
    int len = leaf->length();
    if (len <= 0) continue;
    //FIXME if (leaf->IsA() == TLeafF::Class()) len = TMath::Min(len,5);
    //FIXME if (leaf->IsA() == TLeafD::Class()) len = TMath::Min(len,5);
    len = MINIMUM(len,10);
    smanip smanip;
    std::string s;
    smanip.printf(s,32," %-15s = ",leaf->name().c_str());
    out << s;
    for (int i=0;i<len;i++) {
      leaf->printValue(printer(),i);
      if (i == len-1) out << endl;
      else            out << ", ";
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
Rio::Branch* Rio::Tree::findBranch(
 const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<Branch*>::const_iterator it;
  for(it=fBranches.begin();it!=fBranches.end();++it) {
    Branch* branch = *it;
    if (branch->name()==aName) return branch;
    /*FIXME
    TObjArray *lb = branch->GetListOfBranches();
    nb1 = lb->GetEntriesFast();
    for (j=0;j<nb1;j++) {
      b1 = (TBranch*)lb->UncheckedAt(j);
      if (!strcmp(b1->GetName(),name)) return b1;
      lb1 = b1->GetListOfBranches();
      nb2 = lb1->GetEntriesFast();
      for (k=0;k<nb2;k++) {
	b2 = (TBranch*)lb1->UncheckedAt(k);
	if (!strcmp(b2->GetName(),name)) return b2;
      }
    }
    */
  }

  std::list<BaseLeaf*>::const_iterator itl;
  for(itl=fLeaves.begin();itl!=fLeaves.end();++itl) {
    if((*itl)->branch().name()==aName) return &((*itl)->branch());
  }

  return 0;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Tree::loadTree(
 int aEntry
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // this function is overloaded in TChain
  fReadEntry = aEntry;
  return fReadEntry;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Tree::readEntry(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fReadEntry;
}
