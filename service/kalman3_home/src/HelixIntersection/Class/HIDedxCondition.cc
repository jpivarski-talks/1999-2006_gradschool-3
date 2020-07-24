// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIDedxCondition
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Wed May 19 18:30:42 EDT 1999
// $Id: HIDedxCondition.cc,v 1.5 1999/09/16 19:38:21 wsun Exp $
//
// Revision history
//
// $Log: HIDedxCondition.cc,v $
// Revision 1.5  1999/09/16 19:38:21  wsun
// Added error flag as argument to deltaEnergy().
//
// Revision 1.4  1999/08/03 21:59:37  wsun
// abs --> fabs
//
// Revision 1.3  1999/06/24 22:24:55  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.2  1999/06/18 19:25:56  wsun
// More improvements to low momentum fitting.
//
// Revision 1.1  1999/06/02 23:05:04  wsun
// First submission.
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
#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HIDedxOperation.h"
#include "HelixIntersection/HIDedxCondition.h"
#include "HelixIntersection/HIMoveIterateOperation.h"

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

static const char* const kFacilityString = "HIDedxCondition" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIDedxCondition.cc,v 1.5 1999/09/16 19:38:21 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIDedxCondition::HIDedxCondition( double aMaxFractionalMomentumChange,
				  HIMoveIterateOperation* aOperation )
   : m_maxFracDP( aMaxFractionalMomentumChange ),
     m_moveIterateOperation( aOperation )
{
}

// HIDedxCondition::HIDedxCondition( const HIDedxCondition& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIDedxCondition::~HIDedxCondition()
{
}

//
// assignment operators
//
// const HIDedxCondition& HIDedxCondition::operator=( const HIDedxCondition& rhs )
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
HIDedxCondition::satisfied( const HIHelix& aHelix,
			    HIIntersectionSurface& aSurface ) const
{
   // Calculate energy loss.  If helix is being propagated backwards,
   // the arc length will be negative --> positive dE.

   // Determine whether to use the inner or outer material description.
   DGConstMaterialPtr material =
      aSurface.materialInLastTransport( aHelix ) ;

   DABoolean dedxOK ;
   GeV dE = HIDedxOperation::deltaEnergy( material, aHelix, dedxOK ) ;

   if( !dedxOK )
   {
      return true ;
   }

   // Calculate new curvature.
   double oldCurvature = aHelix.curvature() ;

   GeV mass = aHelix.mass() ;
   GeV absmom = aHelix.momentum().mag2() ;
   GeV energy = sqrt( absmom + sqr( mass ) ) ;
   absmom = sqrt( absmom ) ;

   GeV newAbsmom = sqrt( sqr( energy + dE ) - sqr( mass ) ) ;

   // Cast away the const on aHelix
   HIHelix* helixNonConst = const_cast< HIHelix* >( &aHelix ) ;

   double fracDP = fabs( ( newAbsmom - absmom ) / absmom ) ;

   if( fracDP > m_maxFracDP && m_moveIterateOperation != 0 )
   {
//      Meters lastArcLength = aHelix.lastArcLength() ;

      // Divide the arc length by N+1 and tell HIMoveIterateOperation to
      // move by that distance N times, leaving the last iteration for the
      // next call to HIHelixIntersector::swimToNextIntersection().
      int numberIterations = int( fracDP / m_maxFracDP ) ;

      m_moveIterateOperation->setStepArcLength(
	 aHelix.lastArcLength() / ( numberIterations + 1 ) ) ;

      m_moveIterateOperation->setNumberIterations( numberIterations );

//       helixNonConst->setArcLengthToMove( lastArcLength ) ;
//       helixNonConst->setEnergyToLose( fracDP / m_maxFracDP ) ;
//      helixNonConst->setEnergyToLose( deltaD0 / m_maxFracDP ) ;

//       if( printDiagnostics() )
//       {
// 	 cout << "arc " << lastArcLength
// 	      << " dE " << dE << " maxdE " << maxDE << endl ;
//       }

      return true ;
   }

//   helixNonConst->setEnergyToLose( dE ) ;

   return false ;
}

//
// static member functions
//
