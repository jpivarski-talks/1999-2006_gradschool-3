// this :
#include <Rio/Tree/Branch.h>

// Core :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IFile.h>

#include <Rio/Core/Buffer.h>
#include <Rio/Core/smanip.h>
#include <Rio/Core/Lib.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/Out.h>

// Tree :
#include <Rio/Tree/Basket.h>
#include <Rio/Tree/Tree.h>
#include <Rio/Tree/BaseLeaf.h>
#include <Rio/Tree/Leaf.h>

#define MAXIMUM(a,b) ((a)>(b)?a:b) 

const unsigned int Rio_kNewClassTag   = 0xFFFFFFFF;
// OR the byte count with this
const unsigned int Rio_kByteCountMask = 0x40000000;  
const int Rio_kMaxRAM = 10;
const int Rio_kInitCapacity = 16;

RIO_META_SOURCE(Rio::Branch)

//////////////////////////////////////////////////////////////////////////////
Rio::Branch::Branch(
 Tree& aTree
)
:Named("","")
,fTree(aTree)
,fAutoDelete(false)
,fBranches(fTree.printer())
,fLeaves(fTree.printer())
,fBaskets(fTree.printer(),Rio_kInitCapacity,0)
,fCompress(0)
,fBasketSize(32000)
,fEntryOffsetLen(1000)
,fWriteBasket(0)
,fEntryNumber(0)
,fMaxBaskets(1000)
,fOffset(0)
,fNleaves(0)
,fReadBasket(0)
,fReadEntry(-1)
,fEntries(0)
,fTotBytes(0)
,fZipBytes(0)
,fNBasketRAM(0)
,fBasketRAM(0)
,fBasketBytes(0)
,fBasketEntry(0)
,fBasketSeek(0)
,fAddress(0)
,fEntryBuffer(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fNBasketRAM = Rio_kMaxRAM+1;
  fBasketRAM = new int[Rio_kMaxRAM]; 
  for (int i=0;i<Rio_kMaxRAM;i++) fBasketRAM[i] = -1;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Branch::Branch(
 Tree& aTree
,const std::string& aName
,const std::string& aTitle
,void* aAddress
,const std::string& aLeafList
,int aBasketSize
,int aCompress
)
:Named(aName,aLeafList)
,fTree(aTree)
,fAutoDelete(false)
,fBranches(fTree.printer())
,fLeaves(fTree.printer())
,fBaskets(fTree.printer(),Rio_kInitCapacity,0)
,fCompress(aCompress)
,fBasketSize(0)
,fEntryOffsetLen(0)
,fWriteBasket(0)
,fEntryNumber(0)
,fMaxBaskets(1000)
,fOffset(0)
,fNleaves(0)
,fReadBasket(0)
,fReadEntry(-1)
,fEntries(0)
,fTotBytes(0)
,fZipBytes(0)
,fNBasketRAM(0)
,fBasketRAM(0)
,fBasketBytes(0)
,fBasketEntry(0)
,fBasketSeek(0)
,fAddress(0)
,fEntryBuffer(0)
//////////////////////////////////////////////////////////////////////////////
// Create a Branch :
//       * address is the address of the first item of a structure
//         or the address of a pointer to an object (see example).
//       * leaflist is the concatenation of all the variable names and types
//         separated by a colon character :
//         The variable name and the variable type are separated by 
//         a slash (/).
//         The variable type may be 0,1 or 2 characters. If no type is given,
//         the type of the variable is assumed to be the same as the previous
//         variable. If the first variable does not have a type, it is assumed
//         of type F by default. The list of currently supported 
//         types is given below:
//            - C : a character string terminated by the 0 character
//            - B : an 8 bit signed integer (Char_t)
//            - b : an 8 bit unsigned integer (UChar_t)
//            - S : a 16 bit signed integer (Short_t)
//            - s : a 16 bit unsigned integer (UShort_t)
//            - I : a 32 bit signed integer (Int_t)
//            - i : a 32 bit unsigned integer (UInt_t)
//            - F : a 32 bit floating point (Float_t)
//            - D : a 64 bit floating point (Double_t)
//
//         By default, a variable will be copied to the buffer with 
//         the number of
//         bytes specified in the type descriptor character. However, 
//         if the type
//         consists of 2 characters, the second character is an integer that
//         specifies the number of bytes to be used when copying the variable
//         to the output buffer. Example:
//             X    : variable X, type Float_t
//             Y/I  : variable Y, type Int_t
//             Y/I2 : variable Y, type Int_t converted to a 16 bits integer
//
//   See an example of a Branch definition in the TTree constructor.
//
//   Note that in case the data type is an object, this branch can contain
//   only this object.
//
//    Note that this function is invoked by TTree::Branch
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBasketSize < 100) aBasketSize = 100;
  fBasketSize     = aBasketSize;
   /*
   fSplitLevel     = 0;
   */
  fAddress        = (char*)aAddress;
  fNBasketRAM     = Rio_kMaxRAM+1;
  fBasketRAM      = new int[Rio_kMaxRAM]; 
  int i;
  for (i=0;i<Rio_kMaxRAM;i++) fBasketRAM[i] = -1;
  fBasketEntry    = new int[fMaxBaskets];
  fBasketBytes    = new int[fMaxBaskets];
  fBasketSeek     = new Seek[fMaxBaskets];
  fBasketEntry[0] = fEntryNumber;
  for (i=0;i<fMaxBaskets;i++) {
    fBasketBytes[i] = 0;
    fBasketEntry[i] = 0;
    fBasketSeek[i]  = 0;
  }
  // Have at least one valid basket :
  fBaskets[0] = new Basket(fTree.directory(),*this,aName,aTitle);
  
  // Decode the leaflist (search for : as separator)
  std::vector<std::string> leaves;
  Rio::smanip smanip;
  smanip.words(aLeafList,":",leaves);
  unsigned int leafn = leaves.size();
  int offset = 0;
  for(unsigned int index=0;index<leafn;index++) {
    std::vector<std::string> words;
    smanip.words(leaves[index],"/",words);
    if(words.size()==2) {
      std::string& leafName = words[0];
      std::string& leafType = words[1];
      BaseLeaf* leaf = 0;
      if( (leafType == "F") || (leafType == "f") ) {
	leaf = new LeafFloat(*this,leafName);
      } else if( (leafType == "I") || (leafType == "i") ) {
	leaf = new LeafInteger(*this,leafName);
      } else {
	Out out(fTree.printer());
	out << "Rio::Branch::Branch : Illegal data type \"" 
	    << leafType << "\"." << endl;
	return;
      }
      /*FIXME 
	if (leaf->IsZombie()) {
	delete leaf;
	Error("TBranch","Illegal leaf:%s/%s",name,leaflist);
	MakeZombie();
	return;
	}*/
      
      leaf->setAddress((char*)(fAddress+offset));
      leaf->setOffset(offset);
      if (leaf->leafCount()) fEntryOffsetLen = 1000;
      //FIXME if (leaf->InheritsFrom("TLeafC")) fEntryOffsetLen = 1000;
      fNleaves++;
      fLeaves.push_back(leaf);
      fTree.leaves().push_back(leaf);
      offset += leaf->lengthType() * leaf->length();
    } else {
      Out out(fTree.printer());
      out << "Rio::Branch::Branch : Bad format for \"" 
	  << leaves[index] << "\". Should be \"<name>/[F,f]\"." << endl;
      return;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
Rio::Branch::~Branch(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (fBasketRAM)   delete [] fBasketRAM;
  if (fBasketEntry) delete [] fBasketEntry;
  if (fBasketBytes) delete [] fBasketBytes;
  if (fBasketSeek)  delete [] fBasketSeek;
  fBasketRAM   = 0;
  fBasketEntry = 0;
  fBasketBytes = 0;
  fBasketSeek  = 0;
  
 {std::list<BaseLeaf*>::iterator it;
  for(it=fLeaves.begin();it!=fLeaves.end();it=fLeaves.erase(it)) 
    delete (*it);}
  
 {std::list<Basket*>::iterator it;
  for(it=fBaskets.begin();it!=fBaskets.end();it=fBaskets.erase(it)) 
    delete (*it);}

 /*
   // Warning. Must use FindObject by name instead of fDirectory->GetFile()
   // because two branches<may point to the same file and the file
   // already deleted in the previous branch
   if (fDirectory && fDirectory != fTree->GetDirectory()) {
   TFile *file = (TFile*)gROOT->GetListOfFiles()->FindObject(GetFileName());
   if (file ) delete file;
   }
   fTree        = 0;
   fDirectory   = 0;
 */
}
//////////////////////////////////////////////////////////////////////////////
double Rio::Branch::entries(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fEntries;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Tree& Rio::Branch::tree(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fTree;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IPrinter& Rio::Branch::printer(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fTree.directory().printer();
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Branch::compressionLevel(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fCompress;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Branch::basketSize(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fBasketSize;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Branch::writeBasket(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fWriteBasket;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Branch::entryOffsetLength(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fEntryOffsetLen;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Branch::setEntryOffsetLength(
 int aEntryOffsetLen
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fEntryOffsetLen = aEntryOffsetLen;
}
//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf* Rio::Branch::leaf(
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
int Rio::Branch::fill(
)
//////////////////////////////////////////////////////////////////////////////
// Loop on all leaves of this branch to fill Basket buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME if (TestBit(kDoNotProcess)) return 0;

  Basket* basket = getBasket(fWriteBasket);
  if (!basket) return 0;
  Buffer* buf = basket->bufferRef();
  
  // Fill basket buffer
  if (buf->isReading()) {
    basket->setWriteMode();
  }

  buf->resetMap();
  int lold = buf->length();
  int objectStart = 0;
  if ( fEntryBuffer ) {
    /*FIXME
    if ( fEntryBuffer->IsA() == TMessage::Class() ) {
      objectStart = 8;
    }
    */
    // We are required to copy starting at the version number (so not
    // including the class name.
    // See if byte count is here, if not its class still be a newClass
    unsigned int startpos = fEntryBuffer->length();
    fEntryBuffer->setBufferOffset(objectStart);

    unsigned int tag;
    fEntryBuffer->read(tag);

    if ( tag & Rio_kByteCountMask ) {
      fEntryBuffer->read(tag);
    }
    if ( tag == Rio_kNewClassTag ) {
      char s[80];
      fEntryBuffer->readString(s, 80);
    } else {
      fEntryBuffer->setBufferOffset(objectStart);
    }
    objectStart = fEntryBuffer->length();
    fEntryBuffer->setBufferOffset(startpos);
    
    basket->update(lold, objectStart - fEntryBuffer->displacement());
  } else  
    basket->update(lold,lold);

  fEntries++;
  fEntryNumber++;

  if (fEntryBuffer) {
    
    unsigned int len;
    unsigned int startpos = fEntryBuffer->length();
    if (startpos > (unsigned int)(objectStart)) {
      // We assume this buffer have just been directly filled
      // the current position in the buffer indicates the end of the object!
      len = fEntryBuffer->length() - objectStart;
    } else {
      // The buffer have been acquired either via TSocket or via
      // TBuffer::SetBuffer(newloc,newsize)
      // Only the actual size of the memory buffer gives us an hint about where
      // the object ends.
      len = fEntryBuffer->bufferSize() - objectStart;
    }
    buf->writeBuffer( fEntryBuffer->buffer() + objectStart , len );
    
  } else {
    fillLeaves(*buf);
  }

  int lnew   = buf->length();
  int nbytes = lnew - lold;
  int nsize  = 0;
  if (fEntryOffsetLen) {
    int nevbuf = basket->nevBuf();
    nsize = nevbuf * sizeof(int); //total size in bytes of EntryOffset table
  } else {
    if (!basket->nevBufSize()) basket->setNevBufSize(nbytes);
  }
  
  // Should we create a new basket?
  if ( (lnew +2*nsize +nbytes >= fBasketSize) ) { 
    int nout  = basket->writeBuffer();    //  Write buffer
    fBasketBytes[fWriteBasket]  = basket->numberOfBytes();
    fBasketSeek[fWriteBasket]   = basket->seekKey();
    int addbytes = basket->objectSize() + basket->keyLength() ;
    if (fTree.directory().isWritable()) {
      delete basket;
      fBaskets[fWriteBasket] = 0;
    }
    fZipBytes += nout;
    fTotBytes += addbytes;
    fTree.addTotBytes(addbytes);
    fTree.addZipBytes(nout);
    basket = new Basket(fTree.directory(),*this,fName,fTree.name());
    fWriteBasket++;
    //FIXME fBaskets.AddAtAndExpand(basket,fWriteBasket);
    if(fWriteBasket>=(int)fBaskets.size())  //FIXME : the same than upper ?
      fBaskets.resize(fWriteBasket+1,0);
    fBaskets[fWriteBasket] = basket;
    if (fWriteBasket >= fMaxBaskets) {
      //Increase BasketEntry buffer of a minimum of 10 locations
      // and a maximum of 50 per cent of current size
      int newsize = MAXIMUM(10,int(1.5*fMaxBaskets));
      fBasketEntry  = (int*)Rio::realloc(fBasketEntry, 
					 newsize * sizeof(int), 
					 fMaxBaskets * sizeof(int));
      fBasketBytes  = (int*)Rio::realloc(fBasketBytes, 
					 newsize * sizeof(int), 
					 fMaxBaskets * sizeof(int));
      fBasketSeek = (Seek*)Rio::realloc(fBasketSeek, 
					newsize * sizeof(Seek), 
					fMaxBaskets * sizeof(Seek));
      fMaxBaskets   = newsize;
    }
    fBasketEntry[fWriteBasket] = fEntryNumber;
    fBasketBytes[fWriteBasket] = 0;
    fBasketSeek[fWriteBasket]  = 0;
  }
  return nbytes;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Basket* Rio::Branch::getBasket(
 int aBasketIndex
)
//////////////////////////////////////////////////////////////////////////////
// Return pointer to basket aBasketIndex in this Branch.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  static int nerrors = 0;

  // reference to an existing basket in memory ?
  if (aBasketIndex <0 || aBasketIndex > fWriteBasket) return 0;

  Basket* basket = fBaskets[aBasketIndex];
  if (basket) return basket;
  
  // create/decode basket parameters from buffer
  basket = new Basket(fTree.directory(),*this);
  if (fBasketBytes[aBasketIndex] == 0) {
    fBasketBytes[aBasketIndex] = 
     basket->readBasketBytes(fBasketSeek[aBasketIndex]);
  }
  int badread = 
    basket->readBasketBuffers(fBasketSeek[aBasketIndex],
			      fBasketBytes[aBasketIndex]);
  if (badread || basket->seekKey() != fBasketSeek[aBasketIndex]) {
    Out out(fTree.printer());
    nerrors++;
    if (nerrors > 10) return 0;
    if (nerrors == 10) {
      out << "Rio::Branch::getBasket : " 
	  << "file probably overwritten: "
	  << "stopping reporting error messages." << endl;
      if (fBasketSeek[aBasketIndex] > 2000000000) {
	out << "Rio::Branch::getBasket : " 
	    << "===>File is more than 2 Gigabytes." << endl;
	return 0;
      }
      if (fBasketSeek[aBasketIndex] > 1000000000) {
	out << "Rio::Branch::getBasket : " 
	    << "===>Your file is may be bigger than the "
	    << "maximum file size allowed on your system." << endl;
	out << "Rio::Branch::getBasket :     " 
	    << "Check your AFS maximum file size limit for example." << endl;
	return 0;
      }
    }
    out << "Rio::Branch::getBasket : File: \"" 
	<< fTree.directory().file().name() 
	<< "\" at byte:" << basket->seekKey() 
	<< ", branch:" << fName << ", entry:" << fReadEntry << endl;
    return 0;
  }
  
  fBaskets[aBasketIndex] = basket;
  if (fNBasketRAM < Rio_kMaxRAM) fBasketRAM[fNBasketRAM] = aBasketIndex;
  fNBasketRAM++;
  return basket;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Branch::fillLeaves(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<BaseLeaf*>::iterator it;
  for(it=fLeaves.begin();it!=fLeaves.end();++it) {
    (*it)->fillBasket(aBuffer);
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Branch::readLeaves(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<BaseLeaf*>::iterator it;
  for(it=fLeaves.begin();it!=fLeaves.end();++it) {
    (*it)->readBasket(aBuffer);
  }
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Branch::entry( 
 int aEntry
,int /*aGetAll*/
)
//////////////////////////////////////////////////////////////////////////////
// Read all leaves of entry and return total number of bytes :
// The input argument entry is the entry serial number in the current tree.
// In case of a TChain, the entry number in the current Tree must be found
//  before calling this function. example with TChain *chain;
//  Int_t localEntry = chain->LoadTree(entry);
//  branch->GetEntry(localEntry);
//
//  The function returns the number of bytes read from the input buffer.
//  If entry does not exist or an I/O error occurs, the function returns 0.
//  if entry is the same as the previous call, the function returns 1.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME if (TestBit(kDoNotProcess) && !aGetAll) return 0;
  if (fReadEntry == aEntry) return 1;
  if (aEntry < 0 || aEntry >= fEntryNumber) return 0;
  int nbytes;
  int first = fBasketEntry[fReadBasket];
  int last;
  if (fReadBasket == fWriteBasket) last = fEntryNumber - 1;
  else                             last = fBasketEntry[fReadBasket+1] - 1;

  //
  //      Are we still in the same ReadBasket?
  if (aEntry < first || aEntry > last) {
    fReadBasket = binarySearch(fWriteBasket+1, fBasketEntry, aEntry);
    first       = fBasketEntry[fReadBasket];
  }

  //     We have found the basket containing this entry.
  //     make sure basket buffers are in memory.
  Basket* basket = fBaskets[fReadBasket];
  if (!basket) {
    basket = getBasket(fReadBasket);
    if (!basket) return 0;
  }

  Buffer* buf = basket->bufferRef();
  //     This test necessary to read very old Root files (NvE)
  if (!buf) {
    //File* file = GetFile(0);
    basket->readBasketBuffers(fBasketSeek[fReadBasket],
			      fBasketBytes[fReadBasket]);
    buf = basket->bufferRef();
  }
  //     Set entry offset in buffer and read data from all leaves
  buf->resetMap();
  if (!buf->isReading()) {
    basket->setReadMode();
  }
  
  int bufbegin;
  int* entryOffset = basket->entryOffset();
  if (entryOffset) 
    bufbegin = entryOffset[aEntry-first];
  else             
    bufbegin = basket->keyLength() + (aEntry-first) * basket->nevBufSize();
  buf->setBufferOffset(bufbegin);
  int* displacement = basket->displacement();
  if (displacement) buf->setDisplacement(displacement[aEntry-first]);
  else buf->setDisplacement();

  readLeaves(*buf);
  
  nbytes = buf->length() - bufbegin;
  fReadEntry = aEntry;
  return nbytes;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Branch::reset(
)
//////////////////////////////////////////////////////////////////////////////
// Reset a Branch :
//    Existing buffers are deleted
//    Entries, max and min are reset
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {std::list<Basket*>::iterator it;
  for(it=fBaskets.begin();it!=fBaskets.end();it=fBaskets.erase(it)) 
    delete (*it);}

  if (fBasketEntry) delete [] fBasketEntry;
  if (fBasketBytes) delete [] fBasketBytes;
  if (fBasketSeek)  delete [] fBasketSeek;
  fBasketEntry    = 0;
  fReadBasket     = 0;
  fReadEntry      = -1;
  fWriteBasket    = 0;
  fEntries        = 0;
  fTotBytes       = 0;
  fZipBytes       = 0;
  fEntryNumber    = 0;
  fBasketEntry    = new int[fMaxBaskets];
  fBasketBytes    = new int[fMaxBaskets];
  fBasketSeek     = new Seek[fMaxBaskets];
  fBasketEntry[0] = fEntryNumber;
  for (int i=0;i<fMaxBaskets;i++) {
    fBasketBytes[i] = 0;
    fBasketEntry[i] = 0;
    fBasketSeek[i]  = 0;
  }
  fBaskets.resize(Rio_kInitCapacity,0);
  fBaskets[0] = new Basket(fTree.directory(),*this,fName,fTitle);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Branch::setAddress(
 void* aAddress
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fReadEntry = -1;
  fAddress = (char*)aAddress;
  std::list<BaseLeaf*>::const_iterator it;
  for(it=fLeaves.begin();it!=fLeaves.end();++it) {
    int offset = (*it)->offset();
    //FIXME if (TestBit(kIsClone)) offset = 0;
    (*it)->setAddress(fAddress+offset);
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void* Rio::Branch::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (Branch*)this;
  else return Named::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Branch::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s, c;
    fAddress = 0;
    //FIXME gROOT->SetReadingObject(kTRUE);
    short v = aBuffer.readVersion(&s, &c);
    if (v > 5) {
      /*FIXME 
      TBranch::Class()->ReadBuffer(b, this, v, s, c);
      gBranch = branchSave;
      fDirectory = gDirectory;
      fNleaves = fLeaves.GetEntriesFast();
      if (fFileName.Length() != 0) fDirectory = 0;
      gROOT->SetReadingObject(kFALSE);
      return;
	*/
    }
    //====process old versions before automatic schema evolution

    Named::stream(aBuffer);

    if (v <6 ) {
      aBuffer.read(fCompress);
      aBuffer.read(fBasketSize);
      aBuffer.read(fEntryOffsetLen);
      aBuffer.read(fMaxBaskets);
      aBuffer.read(fWriteBasket);
      aBuffer.read(fEntryNumber);
      aBuffer.read(fEntries);
      aBuffer.read(fTotBytes);
      aBuffer.read(fZipBytes);
      aBuffer.read(fOffset);
    } else {
      aBuffer.read(fCompress);
      aBuffer.read(fBasketSize);
      aBuffer.read(fEntryOffsetLen);
      aBuffer.read(fWriteBasket);
      aBuffer.read(fEntryNumber);
      aBuffer.read(fOffset);
      aBuffer.read(fMaxBaskets);
      if(v>=7) {
	int dummy_int;
	aBuffer.read(dummy_int); //fSplitLevel
      }
      aBuffer.read(fEntries);
      aBuffer.read(fTotBytes);
      aBuffer.read(fZipBytes);
    }

    Arguments args;
    Args_insertDirectory(args,&fTree.directory());
    Args_insertTree(args,&fTree);
    Args_insertBranch(args,this);

    fBranches.read(aBuffer,args);
    fLeaves.read(aBuffer,args);
    fBaskets.read(aBuffer,args,false);

    if(fBaskets.size()<=0) {
      Out out(fTree.printer());
      out << "Rio::Branch::stream : error : fBaskets now empty." << endl;
    } else {
      /* debug
      Out out(fTree.printer());
      out << "Rio::Branch::stream : read : baskets " 
	  << fBaskets.size() << " :" << endl;
      std::list<Basket*>::iterator it;
      int index = 0;
      for(it=fBaskets.begin();it!=fBaskets.end();++it,++index) {
	if(*it)
	  out << "Rio::Branch::stream : read :   " << index << " " << *it << endl;
      } 
      */
    }

    fNleaves = fLeaves.size();

    fBasketEntry = new int[fMaxBaskets];
    fBasketBytes = new int[fMaxBaskets];
    fBasketSeek = new Seek[fMaxBaskets];

    if (v <6 ) {
      aBuffer.readArray(fBasketEntry);
      if (v <= 4) {
	for (int n=0;n<fMaxBaskets;n++) fBasketBytes[n] = 0;
      } else {
	aBuffer.readArray(fBasketBytes);
      }
      if (v < 2) {
	for (int n=0;n<fWriteBasket;n++) {
	  fBasketSeek[n] = getBasket(n)->seekKey();
	}
      } else {
	aBuffer.readArray(fBasketSeek);
      }
    } else {
      // See TStreamerInfo::ReadBuffer::ReadBasicPointer
      char isArray;
      aBuffer.read(isArray);
      if(isArray!=0) aBuffer.readFastArray(fBasketBytes,fMaxBaskets);
      aBuffer.read(isArray);
      if(isArray!=0) aBuffer.readFastArray(fBasketEntry,fMaxBaskets);
      aBuffer.read(isArray);
      aBuffer.readFastArray(fBasketSeek,fMaxBaskets);
    }

    if (v > 2) {
      std::string fileName;
      aBuffer.read(fileName);
    }
    //FIXME if (v < 4) SetAutoDelete(kTRUE);
    //FIXME gROOT->SetReadingObject(kFALSE);
    aBuffer.checkByteCount(s, c,sClass->name());
    //====end of old versions
  } else {
    /*FIXME
    TBranch::Class()->WriteBuffer(b,this);
    // if branch is in a separate file save this branch
    // as an independent key
    if (fDirectory && fDirectory != fTree->GetDirectory()) {
    TDirectory *cursav = gDirectory;
    fDirectory->cd();
    fDirectory = 0;  // to avoid recursive calls
    Write();
    fDirectory = gDirectory;
    cursav->cd();
    }
    */
    //====process old versions before automatic schema evolution
    unsigned int c = aBuffer.writeVersion(sClass->version(),true);

    Named::stream(aBuffer);

    if(sClass->version()<6) {
      aBuffer.write(fCompress);
      aBuffer.write(fBasketSize);
      aBuffer.write(fEntryOffsetLen);
      aBuffer.write(fMaxBaskets);
      aBuffer.write(fWriteBasket);
      aBuffer.write(fEntryNumber);
      aBuffer.write(fEntries);
      aBuffer.write(fTotBytes);
      aBuffer.write(fZipBytes);
      aBuffer.write(fOffset);
    } else {
      aBuffer.write(fCompress);
      aBuffer.write(fBasketSize);
      aBuffer.write(fEntryOffsetLen);
      aBuffer.write(fWriteBasket);
      aBuffer.write(fEntryNumber);
      aBuffer.write(fOffset);
      aBuffer.write(fMaxBaskets);
      aBuffer.write(fEntries);
      aBuffer.write(fTotBytes);
      aBuffer.write(fZipBytes);
    }

    fBranches.write(aBuffer);
    fLeaves.write(aBuffer);
    fBaskets.write(aBuffer);

    if(sClass->version()<6) {
      aBuffer.writeArray(fBasketEntry,fMaxBaskets);
      aBuffer.writeArray(fBasketBytes,fMaxBaskets);
      aBuffer.writeArray(fBasketSeek, fMaxBaskets);
    } else {
      // See TStreamerInfo::ReadBuffer::WriteBasicPointer
      aBuffer.write((char)1);
      aBuffer.writeFastArray(fBasketBytes,fMaxBaskets);
      aBuffer.write((char)1);
      aBuffer.writeFastArray(fBasketEntry,fMaxBaskets);
      aBuffer.write((char)1);
      aBuffer.writeFastArray(fBasketSeek,fMaxBaskets);
    }

    aBuffer.write(std::string("")); // fFileName

    /*
    // if branch is in a separate file save this branch
    // as an independent key
    if (fDirectory && fDirectory != fTree->GetDirectory()) {
      TDirectory *cursav = gDirectory;
      fDirectory->cd();
      fDirectory = 0;  // to avoid recusive calls
      Write();
      fDirectory = gDirectory;
      cursav->cd();
    }
    */
    aBuffer.setByteCount(c,true);
  }
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Branch::isAutoDelete(
) const
//////////////////////////////////////////////////////////////////////////////
// Return TRUE if an existing object in a TBranchObject must be deleted.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAutoDelete;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Branch::setAutoDelete(
 bool aAutoDelete
)
//////////////////////////////////////////////////////////////////////////////
// Set the AutoDelete bit
// this bit is used by TBranchObject::ReadBasket to decide if an object
// referenced by a TBranchObject must be deleted or not before reading
// a new entry
// if autodel is kTRUE, this existing object will be deleted, a new object
//    created by the default constructor, then object->Streamer
// if autodel is kFALSE, the existing object is not deleted. Root assumes
//    that the user is taking care of deleting any internal object or array
//    This can be done in Streamer itself.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
   fAutoDelete = aAutoDelete;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Branch::updateAddress(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Branch::setBasketSize(
 int aBufferSize
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fBasketSize = aBufferSize;
}
