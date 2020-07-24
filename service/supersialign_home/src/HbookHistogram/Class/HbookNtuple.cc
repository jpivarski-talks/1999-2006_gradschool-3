// -*- C++ -*-
//
// Package:     <package>
// Module:      HbookNtuple
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Marc Soussa
// Created:     Wed Feb 17 16:02:05 EST 1999
// $Id: HbookNtuple.cc,v 1.4 1999/10/20 19:30:59 cdj Exp $
//
// Revision history
//
// $Log: HbookNtuple.cc,v $
// Revision 1.4  1999/10/20 19:30:59  cdj
// pass hbookn_ the full path within the file
//
// Revision 1.3  1999/10/19 23:23:04  cdj
// Ntuples no longer try to write themselves out
//
// Revision 1.2  1999/09/29 17:55:49  cdj
// added histogram directories
//
// Revision 1.1.1.1  1999/09/06 22:02:30  mkl
// imported HbookHistogram source
//
// Revision 1.1.1.1  1999/02/23 22:00:59  cdj
// imported HbookHistogram source
//

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string.h>
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/Experiment.h"
#include "HbookHistogram/hbook_functions.h"
#include "Experiment/report.h"
#include "HbookHistogram/HbookNtuple.h"
//#include "HbookHistogram/HbookFile.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HbookHistogram.HbookNtuple" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HbookNtuple.cc,v 1.4 1999/10/20 19:30:59 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HbookNtuple::HbookNtuple( HbookHistDir* iParent,
                          HbookFile&    iNtupleFile,
			  Count         iId,
			  const string& iTitle,
			  unsigned int  iNumberOfVariables,
			  unsigned int  iChunkSize,
			  const char* const* iLabels ) :
  HINtuple( iId,
	    iTitle,
	    iNumberOfVariables,
	    iChunkSize,
	    iLabels ) ,
  HbookRZItem( &iNtupleFile ),
  HbookDirContents( iParent )
{
  hbookinit_() ;

  FInteger ID = id() ;
  
  if ( ( hexist_( &ID ) & 0x1 ) ) {
    return ;
  }

  //NOTE: the hbook manual says to pass the 'top directory' of the file
  //      to hbookn, but this does not work when an NTuple resides in a
  //      subdirectory.  Looking at the code for CABS, we determined that
  //      you must pass it the full path ( file top directory and subsequent
  //      subdirectories) in order for NTuples to work.
  book( iTitle,
	iParent->filePath(),
	iNumberOfVariables,
	iChunkSize,
	iLabels ) ;
}

// HbookNtuple::HbookNtuple( const HbookNtuple& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HbookNtuple::~HbookNtuple( void )
{
  close() ;
}

//
// assignment operators
//
// const HbookNtuple& HbookNtuple::operator=( const HbookNtuple& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//
void HbookNtuple::fill( const float* iEntry )
{
  setDir();
  FInteger ID = id() ;

  hfn_( &ID, iEntry ) ;
}
//
// Book a row-wise Ntuple into memory on an RZ file.  Only single 
// precision floating point numbers can be strored, and no data
// compression is provided.
//
void HbookNtuple::book( const string& iTitle,
			const string& iTopDir,
			unsigned int  iNumberOfVariables,
			unsigned int  iChunkSize,
			const char* const* iLabels )
{
  // Determine longest label and arrange FORTRAN array accordingly
  //
  FInteger* length = new FInteger[iNumberOfVariables] ;
  FInteger  maxLabelLength = 0 ;
  FInteger  index ;

  for ( index = 0 ; index < iNumberOfVariables ; index++ ) {
    length[index] = strlen( iLabels[index] ) ;
    if ( maxLabelLength < length[index] ) {
      maxLabelLength = length[index] ;
    }
  }
 
  FChar* startOfLabels = new FChar[ maxLabelLength * iNumberOfVariables ] ;
  FChar* currentLabel = startOfLabels ;

  for ( index = 0 ; index < iNumberOfVariables ; index++ ) {
    memcpy( currentLabel, iLabels[index], length[ index ] ) ;
    for ( FInteger place = length[index] ; place < maxLabelLength ; place++ ) {
      currentLabel[ place ] = ' ' ;
    }
  currentLabel += maxLabelLength ;
  }
  delete [] length ;

  FInteger ID = id() ;
  FInteger NVAR = numberOfVariables() ;
  FInteger NWBUFF = chunkSize() ;
  
  hbookinit_() ;

  hbookn_( &ID,
	   iTitle.c_str(),
	   &NVAR,
	   iTopDir.c_str(),
	   &NWBUFF,
	   startOfLabels,
	   iTitle.size(),
	   iTopDir.size(),
	   maxLabelLength ) ;
  
  delete [] startOfLabels ;
}
// 
// Close down and write an Ntuple
//
void HbookNtuple::close (void ) 
{
  if ( !HbookRZItem::isOpen() ) {
    return ;
  }
  HbookRZItem::close() ;
}
//
// const member functions
//

//
// static member functions
//
