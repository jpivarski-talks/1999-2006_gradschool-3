// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHistDir
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Mon Jul 16 15:21:35 EDT 2001
// $Id: HIHistDir.cc,v 1.1 2001/07/21 15:43:45 cdj Exp $
//
// Revision history
//
// $Log: HIHistDir.cc,v $
// Revision 1.1  2001/07/21 15:43:45  cdj
// can now iterate through HIHistDir's contents
//

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

// user include files
//#include "Experiment/report.h"
#include "HistogramInterface/HIHistDir.h"
#include "HistogramInterface/HIHist1D.h"
#include "HistogramInterface/HIHist2D.h"
#include "HistogramInterface/HINtuple.h"
#include "HistogramInterface/HINumber.h"
#include "HistogramInterface/HINumberWithErrors.h"

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramInterface.HIHistDir" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIHistDir.cc,v 1.1 2001/07/21 15:43:45 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
//HIHistDir::HIHistDir()
//{
//}

// HIHistDir::HIHistDir( const HIHistDir& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

template<class Iter>
static
inline
void
deleteContained( Iter iBegin, Iter iEnd )
{
   for( iBegin; iBegin != iEnd; ++iBegin ) {
      delete *iBegin;
   }
}

HIHistDir::~HIHistDir()
{
   deleteContained(h1d_begin(), h1d_end() );
   deleteContained(h2d_begin(), h2d_end() );
   deleteContained(nt_begin(), nt_end() );
   deleteContained(prof_begin(), prof_end() );
   deleteContained(n_begin(), n_end() );
   deleteContained(ne_begin(), ne_end() );
   deleteContained(dir_begin(), dir_end() );
}

//
// assignment operators
//
// const HIHistDir& HIHistDir::operator=( const HIHistDir& rhs )
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


#define HIDIR_EMPTY(_prefix_) \
void HIHistDir:: _prefix_ ## Empty() {\
 deleteContained( _prefix_ ## _begin(), _prefix_ ## _end() ); \
 m_ ## _prefix_ ##List.erase( m_ ## _prefix_ ## List.begin(), m_ ## _prefix_ ## List.end()); }

HIDIR_EMPTY( h1d)
HIDIR_EMPTY( h2d)
HIDIR_EMPTY( nt)
HIDIR_EMPTY( prof)
HIDIR_EMPTY( n)
HIDIR_EMPTY( ne)
HIDIR_EMPTY( dir)

//
// const member functions
//

//
// static member functions
//
