// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      KalmanUpdateParamsOperation
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Aug 17 13:57:41 EDT 1999
// $Id: KalmanUpdateParamsOperation.cc,v 1.7 2001/03/27 06:37:23 wsun Exp $
//
// Revision history
//
// $Log: KalmanUpdateParamsOperation.cc,v $
// Revision 1.7  2001/03/27 06:37:23  wsun
// Added flag to always update.
//
// Revision 1.6  2000/03/02 08:36:46  wsun
// More informative debug output.
//
// Revision 1.5  2000/02/27 17:41:03  dsr
// removing single argument calls to report
//
// Revision 1.4  1999/11/22 18:50:11  wsun
// Unused hits were not being added to the list.
//
// Revision 1.3  1999/09/10 21:52:34  wsun
// Fixed debug output.
//
// Revision 1.2  1999/08/30 21:55:51  wsun
// Check for nonzero USED hits on track.
//
// Revision 1.1  1999/08/27 20:32:33  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"

#include "CalibratedData/CalibratedHit.h"

#include "KalmanFilter/KalmanHelix.h"
#include "KalmanFilter/KalmanUpdateParamsOperation.h"

#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HICondition.h"
#include "HelixIntersection/HIDedxOperation.h"
#include "HelixIntersection/HICompositeOperation.h"
#include "HelixIntersection/HIMultScatOperation.h"

// TEMPORARY!!
#include "HelixIntersection/HISiSensorPlane.h"

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Geometry/Normal3D.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"KalmanUpdateParamsOperation" ;

//
// static data member definitions
//

//
// constructors and destructor
//
KalmanUpdateParamsOperation::KalmanUpdateParamsOperation(
   HICondition* aUpdateRphiParamsCondition,
   HICondition* aUpdateZParamsCondition )
   : m_updateRphiParamsCondition( aUpdateRphiParamsCondition ),
     m_updateZParamsCondition( aUpdateZParamsCondition ),
     m_alwaysUpdate( false )
{
}

// KalmanUpdateParamsOperation::KalmanUpdateParamsOperation( const KalmanUpdateParamsOperation& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

KalmanUpdateParamsOperation::~KalmanUpdateParamsOperation()
{
   delete m_updateRphiParamsCondition ;
   delete m_updateZParamsCondition ;
}

//
// assignment operators
//
// const KalmanUpdateParamsOperation& KalmanUpdateParamsOperation::operator=( const KalmanUpdateParamsOperation& rhs )
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
KalmanUpdateParamsOperation::perform(
   HIHelix& aHelixAfterOperation,
   const HIHelix& aHelixBeforeOperation,
   const HIHelix& aHelixAtLastIntersection,
   HIIntersectionSurface& aSurface,
   const HIIntersectionSurface& aLastSurface ) const
{
   // Do nothing if the helix does not have an error matrix.
   if( aHelixAfterOperation.hasNullErrorMatrix() )
   {
      return true ;
   }

   // Cast the aHelixAfterOperation into a local KalmanHelix.
   KalmanHelix* kalmanHelix ;
   if( aHelixAfterOperation.helixType() == KalmanHelix::kHelixType )
   {
      kalmanHelix = ( KalmanHelix* ) &aHelixAfterOperation ;
   }
   else
   {
      report( ERROR, kFacilityString )
	 << "Helix in use is not a KalmanHelix." << endl ;
      return false ;
   }

   int nNewHits = kalmanHelix->numberOfNewHitsInFit() ;
   if( nNewHits > 0 )
   {
      DABoolean updateRphi = m_alwaysUpdate ||
	 m_updateRphiParamsCondition->satisfied( *kalmanHelix, aSurface ) ;
      kalmanHelix->setUpdateRphiTrackParametersFlag( updateRphi ) ;

      DABoolean updateZ = m_alwaysUpdate ||
	 m_updateZParamsCondition->satisfied( *kalmanHelix, aSurface ) ;
      kalmanHelix->setUpdateZTrackParametersFlag( updateZ ) ;

      kalmanHelix->updateFitInfo() ;

      if( printDiagnostics() )
      {
	 report( DEBUG, kFacilityString )
	    << nNewHits
	    << " hits added. Update flags = "
	    << ( updateRphi ? 1 : 0 )
	    << "/" << ( updateZ ? 1 : 0 ) << "\n" << endl ;
      }
   }
   else
   {
      if( printDiagnostics() )
      {
	 report( DEBUG, kFacilityString )
	    << "No new hits in fit,"
	    << kalmanHelix->newHitsOnTrack().size()
	    << " unused hits linked.\n" << endl ;
      }

      kalmanHelix->updateHitsOnTrack() ; // Unused hits should be linked.
      kalmanHelix->clearFitInfoChanges() ;
   }

   return true ;
}

//
// static member functions
//
