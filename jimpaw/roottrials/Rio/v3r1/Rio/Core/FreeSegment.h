#ifndef Rio_FreeSegment_h
#define Rio_FreeSegment_h

//////////////////////////////////////////////////////////////////////////
// Description of free segments on a file.                              //
//////////////////////////////////////////////////////////////////////////

#include <Rio/Core/Seek.h>

namespace Rio {

class FreeSegment {
public:
  FreeSegment();
  FreeSegment(Seek,Seek);
  virtual ~FreeSegment();
  // Get/set :
  Seek first() const;
  Seek last() const;
  void setFirst(Seek);
  void setLast(Seek);
  // Else :
  virtual int recordSize() const;
  virtual void fillBuffer(char*&);
  virtual void readBuffer(char*&);
private:
  /*
  FreeSegment* AddFree(TList *lfree,Seek first,Seek last);
  virtual void FillBuffer(char *&buffer);
  FreeSegment* GetBestFree(TList *lfree, Int_t nbytes);
  virtual void     ReadBuffer(char *&buffer);
  */
private:
  Seek fFirst;            //First free word of segment
  Seek fLast;             //Last free word of segment
};

}

#endif
