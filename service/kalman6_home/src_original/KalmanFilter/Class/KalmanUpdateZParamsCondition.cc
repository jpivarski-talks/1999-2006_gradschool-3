// -*- C++ -*-
//
// Package:     <KalmanFilter>
// Module:      KalmanUpdateZParamsCondition
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Apr  5 02:17:47 EDT 1999
// $Id: KalmanUpdateZParamsCondition.cc,v 1.7 1999/08/30 21:55:52 wsun Exp $
//
// Revision history
//
// $Log: KalmanUpdateZParamsCondition.cc,v $
// Revision 1.7  1999/08/30 21:55:52  wsun
// Check for nonzero USED hits on track.
//
// Revision 1.6  1999/08/27 20:43:08  wsun
// New constructor.
//
// Revision 1.5  1999/08/03 22:33:52  wsun
// Disabled z-parameter update.
//
// Revision 1.4  1999/07/12 07:08:02  wsun
// Reenabled flipping of update flag.
//
// Revision 1.3  1999/06/18 19:28:02  wsun
// More improvements to low momentum fitting.
//
// Revision 1.2  1999/06/02 23:07:46  wsun
// Improved low-momentum fitting.
//
// Revision 1.1  1999/05/29 20:21:04  wsun
// First submission.
//
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
#include "KalmanFilter/KalmanUpdateZParamsCondition.h"
#include "KalmanFilter/KalmanHelix.h"

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

static const char* const kFacilityString = "KalmanUpdateZParamsCondition" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: KalmanUpdateZParamsCondition.cc,v 1.7 1999/08/30 21:55:52 wsun Exp $";
static const char* const kTagString = "$Name: v12_04_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
KalmanUpdateZParamsCondition::KalmanUpdateZParamsCondition(
   double aErr2Z0,
   double aErr2CotTheta,
   double aDFracCotTheta )
   : m_err2Z0( aErr2Z0 ),
     m_err2CotTheta( aErr2CotTheta ),
     m_dFracCotTheta( aDFracCotTheta )
{
}

// KalmanUpdateZParamsCondition::KalmanUpdateZParamsCondition( const KalmanUpdateZParamsCondition& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

KalmanUpdateZParamsCondition::~KalmanUpdateZParamsCondition()
{
}

//
// assignment operators
//
// const KalmanUpdateZParamsCondition& KalmanUpdateZParamsCondition::operator=( const KalmanUpdateZParamsCondition& rhs )
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
KalmanUpdateZParamsCondition::satisfied(
   const HIHelix& aHelix,
   HIIntersectionSurface& aSurface ) const
{
   // Cast the aHelixAfterOperation into a local KalmanHelix.
   KalmanHelix* kalmanHelix ;
   if( aHelix.helixType() == KalmanHelix::kHelixType )
   {
      kalmanHelix = ( KalmanHelix* ) &aHelix ;
   }
   else
   {
      report( ERROR, kFacilityString )
	 << "Helix in use is not a KalmanHelix." << endl ;
      return false ;
   }


   DABoolean updateParams = kalmanHelix->updateZTrackParametersFlag() ;

   if( kalmanHelix->numberOfNewHitsInFit() > 0 )
   {
      // Flip update parameters flag if necessary.
      // The criteria to flip from
      // false to true are more stingent than vice versa.

      double newDeltaCotTheta =
	 kalmanHelix->newTrackParameterCorrections()(
	    KalmanHelix::kCotTheta ) ;
      double newDeltaZ0 =
	 kalmanHelix->newTrackParameterCorrections()(
	    KalmanHelix::kZ0 ) ;

      double newSigma2CotTheta =
	 kalmanHelix->newErrorMatrix()(
	    KalmanHelix::kCotTheta, KalmanHelix::kCotTheta ) ;
      double newSigma2Z0 =
	 kalmanHelix->newErrorMatrix()(
	    KalmanHelix::kZ0, KalmanHelix::kZ0 ) ;

      double cotTheta = kalmanHelix->cotTheta() ;
      double z0   = kalmanHelix->z0() ;

//       cout << "z params " << cotTheta << " " << z0 << endl
// 	   << "Dparams " << newDeltaCotTheta << " " << newDeltaZ0 << endl
// 	   << "error2 " << newSigma2CotTheta<< " " << newSigma2Z0 << endl
// 	   << "err2z0 " << m_err2Z0 << " err2CotTheta " << m_err2CotTheta
// 	   << " dFracCotTheta " << m_dFracCotTheta << endl ;

      if( updateParams )
      {
 	 if( newSigma2CotTheta > m_err2CotTheta * sqr( sqr( cotTheta ) ) )
// //	 if( newSigma2Z0 > m_err2Z0 ) // ||
// // 	     newSigma2CotTheta > m_err2CotTheta ||
// // 	     fabs( newDeltaCotTheta / cotTheta ) > m_dFracCotTheta )
	 {
	    return false ;
	 }
      }
      // If a track reaches the first few stereo layers, its error on
      // z0 will be about 10cm before reaching the Si.  However, its
      // error on cotTheta will still be large ~2.

      else if( ( newSigma2CotTheta < m_err2CotTheta * sqr( sqr( cotTheta ) ) &&
		 newSigma2Z0 < 1.5e-4 &&
		 kalmanHelix->numberOfHitsInFit() >= 10 ) ||
	       newSigma2Z0 < m_err2Z0 )
//      else if( newSigma2Z0 < m_err2Z0 ) //&&
// 	       newSigma2CotTheta < m_err2CotTheta &&
// 	       fabs( newDeltaCotTheta / cotTheta ) < m_dFracCotTheta )
      {
//	 return true ;
      }
   }

   return updateParams ;
}

//
// static member functions
//
