// -*- C++ -*-
//
// Package:     <McCannFilter>
// Module:      McCannUpdateParamsCondition
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Apr  5 02:17:47 EDT 1999
// $Id: McCannUpdateParamsCondition.cc,v 1.7 2001/03/27 06:37:02 wsun Exp $
//
// Revision history
//
// $Log: McCannUpdateParamsCondition.cc,v $
// Revision 1.7  2001/03/27 06:37:02  wsun
// Added flag to ignore number of hits -- used in removal of hits from fit.
//
// Revision 1.6  1999/09/10 21:52:07  wsun
// Cleaned up code.
//
// Revision 1.5  1999/08/30 21:55:50  wsun
// Check for nonzero USED hits on track.
//
// Revision 1.4  1999/06/02 23:07:45  wsun
// Improved low-momentum fitting.
//
// Revision 1.3  1999/05/09 21:38:36  wsun
// Updated code to new Lattice interface.
//
// Revision 1.2  1999/04/30 23:06:38  wsun
// Add cotTheta precision criterion.
//
// Revision 1.1  1999/04/27 22:20:53  wsun
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
#include "McCannFilter/McCannUpdateParamsCondition.h"
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

static const char* const kFacilityString = "McCannFilter.McCannUpdateParamsCondition" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: McCannUpdateParamsCondition.cc,v 1.7 2001/03/27 06:37:02 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
McCannUpdateParamsCondition::McCannUpdateParamsCondition()
   : m_ignoreNumberOfHits( false )
{
}

// McCannUpdateParamsCondition::McCannUpdateParamsCondition( const McCannUpdateParamsCondition& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

McCannUpdateParamsCondition::~McCannUpdateParamsCondition()
{
}

//
// assignment operators
//
// const McCannUpdateParamsCondition& McCannUpdateParamsCondition::operator=( const McCannUpdateParamsCondition& rhs )
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
McCannUpdateParamsCondition::satisfied( const HIHelix& aHelix,
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


   DABoolean updateParams = kalmanHelix->updateRphiTrackParametersFlag() ;

   if( kalmanHelix->numberOfNewHitsInFit() > 0 )
   {
      double newDeltaCurv =
	 kalmanHelix->newTrackParameterCorrections()(
	    McCannHelix::kCurvature ) ;
      double newDeltaPhi0 =
	 kalmanHelix->newTrackParameterCorrections()(
	    McCannHelix::kPhi0 ) ;
      double newDeltaD0 =
	 kalmanHelix->newTrackParameterCorrections()(
	    McCannHelix::kD0 ) ;

      double newSigma2Curv =
	 kalmanHelix->newErrorMatrix()(
	    McCannHelix::kCurvature, McCannHelix::kCurvature ) ;
      double newSigma2Phi0 =
	 kalmanHelix->newErrorMatrix()(
	    McCannHelix::kPhi0, McCannHelix::kPhi0 ) ;
      double newSigma2D0 =
	 kalmanHelix->newErrorMatrix()(
	    McCannHelix::kD0, McCannHelix::kD0 ) ;

      double curv = kalmanHelix->curvature() ;
      double phi0 = kalmanHelix->phi0() ;
      double d0   = kalmanHelix->d0() ;

      if( updateParams )
      {
	 // If curvature changes by more than 10%, flip.
	 if( fabs( newDeltaCurv / curv ) > 0.1 )
	 {
	    return false ;
	 }
      }
      else if( curv != 0. &&
	       newSigma2Curv / sqr( curv ) < 0.01 &&
	       fabs( newDeltaCurv / curv ) < 0.1 &&
	       ( m_ignoreNumberOfHits ||
		 kalmanHelix->numberOfHitsInFit() >= 10 ) )
      {
	 return true ;
      }
   }

   return updateParams ;
}

//
// static member functions
//
