// -*- C++ -*-
//
// Package:     <McCannFilter>
// Module:      McCannFitQualityCondition
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Apr  5 03:09:55 EDT 1999
// $Id: McCannFitQualityCondition.cc,v 1.3 1999/07/07 16:46:40 wsun Exp $
//
// Revision history
//
// $Log: McCannFitQualityCondition.cc,v $
// Revision 1.3  1999/07/07 16:46:40  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.2  1999/06/02 23:07:41  wsun
// Improved low-momentum fitting.
//
// Revision 1.1  1999/04/27 22:20:50  wsun
// Major reorganization and structural changes.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "McCannFilter/McCannFitQualityCondition.h"
#include "McCannFilter/McCannHelix.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "McCannFilter.McCannFitQualityCondition" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: McCannFitQualityCondition.cc,v 1.3 1999/07/07 16:46:40 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
McCannFitQualityCondition::McCannFitQualityCondition( double aChisqCut )
   : m_chisqCut( aChisqCut )
{
}

// McCannFitQualityCondition::McCannFitQualityCondition( const McCannFitQualityCondition& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

McCannFitQualityCondition::~McCannFitQualityCondition()
{
}

//
// assignment operators
//
// const McCannFitQualityCondition& McCannFitQualityCondition::operator=( const McCannFitQualityCondition& rhs )
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

//
// const member functions
//

DABoolean
McCannFitQualityCondition::satisfied( const HIHelix& aHelix,
				      HIIntersectionSurface& aSurface ) const
{
   // Cast the aHelixAfterOperation into a local McCannHelix.
   McCannHelix* kalmanHelix ;
   if( aHelix.helixType() == McCannHelix::kHelixType )
   {
      kalmanHelix = ( McCannHelix* ) &aHelix ;
   }
   else
   {
      report( ERROR, kFacilityString )
	 << "Helix in use is not a McCannHelix." << endl ;
      return false ;
   }

   return ( kalmanHelix->deltaChisq() / kalmanHelix->newHitsOnTrack().size() <
	    m_chisqCut ) ;
}

//
// static member functions
//
