//
// Each file has a linked list of free segments. Each free segment
// is described by its firts and last address.
// When an object is written to a file via TObject::Write, a new Key (see TKey)
// is created. The first free segment big enough to accomodate the object
// is used.
// If the object size has a length corresponding to the size of the free 
// segment,the free segment is deleted from the list of free segments.
// When an object is deleted from a file, a new TFree object is generated.
// If the deleted object is contiguous to an already deleted object, the free
// segments are merged in one single segment.
//


// this :
#include <Rio/Core/FreeSegment.h>

#include <Rio/Core/tobuf.h>

#define Rio_FreeSegment_Class_Version 1

//////////////////////////////////////////////////////////////////////////////
Rio::FreeSegment::FreeSegment(
)
:fFirst(0)
,fLast(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::FreeSegment::FreeSegment(
 Seek aFirst
,Seek aLast
)
:fFirst(aFirst)
,fLast(aLast)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::FreeSegment::~FreeSegment(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::Seek Rio::FreeSegment::first(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fFirst;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Seek Rio::FreeSegment::last(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fLast;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::FreeSegment::setFirst(
 Seek aFirst
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fFirst = aFirst;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::FreeSegment::setLast(
 Seek aLast
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fLast = aLast;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::FreeSegment::recordSize(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return sizeof(short) +  2 * sizeof(Seek);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::FreeSegment::fillBuffer(
 char*& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Encode FREE structure into output buffer :
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  short version = Rio_FreeSegment_Class_Version;
  Rio_tobuf(aBuffer, version);
  Rio_tobuf(aBuffer, fFirst);
  Rio_tobuf(aBuffer, fLast);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::FreeSegment::readBuffer(
 char*& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Decode one FREE structure from input buffer :
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  short version;
  Rio_frombuf(aBuffer, &version);
  Rio_frombuf(aBuffer, &fFirst);
  Rio_frombuf(aBuffer, &fLast);
}


/*
TFree::TFree()
{
}

TFree::TFree(TList *lfree, Seek_t first, Seek_t last)
{
  fFirst = first;
  fLast  = last;
  lfree->Add(this);
}

TFree *TFree::AddFree(TList *lfree, Seek_t first, Seek_t last)
{
// Add a new free segment to the list of free segments :
//  If last just preceedes an existing free segment, then first becomes
//     the new starting location of the free segment.
//  if first just follows an existing free segment, then last becomes
//     the new ending location of the free segment.
//  if first just follows an existing free segment AND last just preceedes
//     an existing free segment, these two segments are merged into
//     one single segment.
//
  TFree *idcur = this;
  while (idcur) {
    Seek_t curfirst = idcur->GetFirst();
    Seek_t curlast  = idcur->GetLast();
    if (curlast == first-1) {
      idcur->SetLast(last);
      TFree *idnext = (TFree*)lfree->After(idcur);
      if (idnext == 0) return idcur;
      if (idnext->GetFirst() > last+1) return idcur;
      idcur->SetLast( idnext->GetLast() );
      lfree->Remove(idnext);
      return idcur;
    }
    if (curfirst == last+1) {
      idcur->SetFirst(first);
      return idcur;
    }
    if (first < curfirst) {
      TFree * newfree = new TFree();
      newfree->SetFirst(first);
      newfree->SetLast(last);
      lfree->AddBefore(idcur, newfree);
      return newfree;
    }
    idcur = (TFree*)lfree->After(idcur);
  }
  return 0;
}

*/
