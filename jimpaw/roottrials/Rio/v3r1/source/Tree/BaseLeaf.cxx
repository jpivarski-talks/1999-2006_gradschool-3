// this :
#include <Rio/Tree/BaseLeaf.h>

// Core :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IFile.h>

#include <Rio/Core/smanip.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/Out.h>

// Tree :
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/Tree.h>

RIO_META_SOURCE(Rio::BaseLeaf)

//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf::BaseLeaf(
 Branch& aBranch
)
:Named("","")
,fBranch(aBranch)
,fNewValue(false)
,fIndirectAddress(false)
,fNdata(0)
,fLength(0)
,fOffset(0)
,fIsRange(0)
,fLeafCount(0)
//////////////////////////////////////////////////////////////////////////////
// Called when reading a branch. We add the leaf
// in the tree here.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  bool found = false;
  const std::list<BaseLeaf*>& leaves = fBranch.tree().leaves();
  std::list<BaseLeaf*>::const_iterator it;
  for(it=leaves.begin();it!=leaves.end();++it) {
    if((*it)==this) {
      found = true;
      break;
    }
  }
  if(!found) fBranch.tree().leaves().push_back(this);
}
//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf::BaseLeaf(
 Branch& aBranch
,const std::string& aName
)
:Named(aName,aName)
,fBranch(aBranch)
,fNewValue(false)
,fIndirectAddress(false)
,fNdata(0)
,fLength(0)
,fOffset(0)
,fIsRange(0)
,fIsUnsigned(0)
,fLeafCount(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fLeafCount  = getLeafCounter(fLength);
  if (fLength == -1) {/*FIXME MakeZombie(); */return;}
  fLengthType = 4;
  std::string::size_type pos;
  if( fLeafCount || 
      ((pos = aName.find("[")) != std::string::npos) ) {
    fName = aName.substr(0,pos);
  }
}
//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf::~BaseLeaf(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::BaseLeaf::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (BaseLeaf*)this;
  else return Named::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::BaseLeaf::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s,c;
    /*short v = */ aBuffer.readVersion(&s,&c);
    //FIXME if (v > 1) {
    //TLeaf::Class()->ReadBuffer(b, this, R__v, R__s, R__c);
    //FIXME } else {
    //====process old versions before automatic schema evolution
    Named::stream(aBuffer);

    // Ok with v 1 & 2
    aBuffer.read(fLength);
    aBuffer.read(fLengthType);
    aBuffer.read(fOffset);
    aBuffer.read(fIsRange);
    aBuffer.read(fIsUnsigned);

    Arguments args;
    Args_insertDirectory(args,&fBranch.tree().directory());
    Args_insertTree(args,&fBranch.tree());
    Args_insertBranch(args,&fBranch);
    fLeafCount = Rio::cast<BaseLeaf>(aBuffer.readObject(args));

    aBuffer.checkByteCount(s,c,sClass->name());
    //====end of old versions
    //}
    if (fLength == 0) fLength = 1;
    fNewValue = false;
    setAddress();
  } else {
    //FIXME TLeaf::Class()->WriteBuffer(b,this);
    //====process old versions before automatic schema evolution
    unsigned int c = aBuffer.writeVersion(sClass->version(),true);
    //fBranch.tree().printer().println
    //("Rio::BaseLeaf::stream : debug : name \"%s\" title \"%s\" ",
    // fName.c_str(),fTitle.c_str());
    Named::stream(aBuffer);
    aBuffer.write(fLength);
    aBuffer.write(fLengthType);
    aBuffer.write(fOffset);
    aBuffer.write(fIsRange);
    aBuffer.write(fIsUnsigned);
    aBuffer.writeObject(fLeafCount);
    aBuffer.setByteCount(c,true);
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Rio::Branch& Rio::BaseLeaf::branch(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fBranch;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BaseLeaf::isIndirectAddress(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fIndirectAddress;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::BaseLeaf::setIndirectAddress(
 bool aIndirectAddress
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fIndirectAddress = aIndirectAddress;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::BaseLeaf::setOffset(
 int aOffset
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fOffset = aOffset; 
}
//////////////////////////////////////////////////////////////////////////////
int Rio::BaseLeaf::offset(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fOffset; 
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BaseLeaf::isRange(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fIsRange;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::BaseLeaf::setRange(
 bool aRange
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fIsRange = aRange;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::BaseLeaf::lengthType(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fLengthType;
}
//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf* Rio::BaseLeaf::leafCount(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fLeafCount;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::BaseLeaf::maximum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::BaseLeaf::minimum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::BaseLeaf::length(
) const
//////////////////////////////////////////////////////////////////////////////
// Return the number of effective elements of this leaf.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int len;
  if (fLeafCount) {
    len = int(fLeafCount->value());
    if (len > fLeafCount->maximum()) {
      Out out(fBranch.tree().printer());
      out << "Rio::BaseLeaf::length : ERROR leaf:" 
	  << fName << ", len=" << len << " and max=" 
	  << fLeafCount->maximum() << endl;
      len = fLeafCount->maximum();
    }
    return len * fLength;
  } else {
    return fLength;
  }
}
//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf* Rio::BaseLeaf::getLeafCounter(
 int& aCountValue
) const
//////////////////////////////////////////////////////////////////////////////
// Return Pointer to counter of referenced Leaf :
//  If leaf name has the forme var[nelem], where nelem is alphanumeric, then
//     If nelem is a leaf name, return countval = 1 and the pointer to 
//     the leaf named nelem.
//  If leaf name has the forme var[nelem], where nelem is a digit, then
//     return countval = nelem and a null pointer.
//  Otherwise return countval=0 and a null pointer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aCountValue = 1;
  std::string::size_type pos1;
  if((pos1 = fTitle.find("[")) == std::string::npos) return 0;
  std::string::size_type pos2;
  if((pos2 = fTitle.find("]")) == std::string::npos) return 0;
  if(pos1>=pos2) return 0;
  std::string countname = fTitle.substr(pos1+1,pos2-pos1+1);
  
  // Now search a branch name with a leave name = countname
  // We search for the leaf in the list of leaves from the Tree. 

  const std::list<BaseLeaf*>& leaves = fBranch.tree().leaves();

  BaseLeaf* leaf = 0;
  std::list<BaseLeaf*>::const_iterator it;
  for(it=leaves.begin();it!=leaves.end();++it) {
    if((*it)->name()==countname) {
      leaf = *it;
      break;
    }
  }

  //int i;
  if (leaf) {
    aCountValue = 1;
    leaf->setRange();
    /*FIXME
    if (bleft2) {
      sscanf(bleft2,"[%d]",&i);
      aCountValue *= i;
    }
    bleft = bleft2;
    while(bleft) {
      bleft2++;
      bleft = (char*)strchr(bleft2,'[');
      if (!bleft) break;
      sscanf(bleft,"[%d]",&i);
      aCountValue *= i;
      bleft2 = bleft;
    }
    */
    return leaf;
  }
  // not found in a branch/leaf :
  Rio::smanip smanip;
  long value;
  if(!smanip.tolong(countname,value)) { // Is it a numerical value?
    aCountValue = -1;
    return 0;
  }
  aCountValue = value;
  /*
  if (bleft2) {
    sscanf(bleft2,"[%d]",&i);
    aCountValue *= i;
  }
  bleft = bleft2;
  while(bleft) {
    bleft2++;
    bleft = (char*)strchr(bleft2,'[');
    if (!bleft) break;
    sscanf(bleft,"[%d]",&i);
    aCountValue *= i;
    bleft2 = bleft;
  }
  */
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::BaseLeaf::resetAddress(
 void* aAddress
,bool aDestructor
)
//////////////////////////////////////////////////////////////////////////////
// Set leaf buffer data address.
//  This function is called by all TLeafX::SetAddress
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int todelete = 0;
  if (fNewValue) todelete = 1;
  if (aDestructor) return todelete;

  if (fLeafCount) fNdata = fLength * (fLeafCount->maximum() + 1);
  else            fNdata = fLength;
  
  fNewValue = false;
  if (!aAddress) fNewValue = true;

  return todelete;
}
