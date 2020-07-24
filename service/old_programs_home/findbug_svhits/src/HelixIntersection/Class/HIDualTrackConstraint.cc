// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIDualTrackConstraint
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sat Oct  9 13:23:23 EDT 1999
// $Id: HIDualTrackConstraint.cc,v 1.5 2001/08/24 21:40:21 wsun Exp $
//
// Revision history
//
// $Log: HIDualTrackConstraint.cc,v $
// Revision 1.5  2001/08/24 21:40:21  wsun
// Fixed factor of two error in virtual photon beta.
//
// Revision 1.4  2001/08/23 03:15:52  wsun
// Forgot to boost Pparallel into virtual photon rest frame.
//
// Revision 1.3  2000/10/11 05:53:17  wsun
// Corrected previous log comment.
//
// Revision 1.2  2000/10/11 05:25:57  wsun
// Added option to release p_t constraints.
//
// Revision 1.1  1999/10/14 18:51:06  wsun
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

#include <assert.h>

// user include files
//#include "Experiment/report.h"
#include "HelixIntersection/HIDualTrackConstraint.h"
#include "HelixIntersection/HIFitHelix.h"

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

static const char* const kFacilityString = "HelixIntersection.HIDualTrackConstraint" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIDualTrackConstraint.cc,v 1.5 2001/08/24 21:40:21 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIDualTrackConstraint::HIDualTrackConstraint(
   double aHalfCrossingAngleInRadians,
   GeV aBeamEnergy,
   const HepVector3D& aVirtualPhotonDir,
   DABoolean aConstrainPT,
   GeV aParticleMass )
   : m_halfAngle( aHalfCrossingAngleInRadians ),
     m_ebm( aBeamEnergy ),
     m_vPhotonDir( aVirtualPhotonDir ),
     m_constrainPT( aConstrainPT ),
     m_mass( aParticleMass )
{
   // Ensure virtual photon direction is a unit vector without a z-component.
   m_vPhotonDir.setZ( 0. ) ;
   m_vPhotonDir = m_vPhotonDir.unit() ;

   // Calculate beta and gamma of the virtual photon.
   m_beta = fabs( sin( m_halfAngle ) ) *
      sqrt( sqr( m_ebm ) - sqr( 0.000511 ) ) / m_ebm ;  // electron mass in GeV

   m_gamma = 1. / sqrt( 1. - sqr( m_beta ) ) ;
}

// HIDualTrackConstraint::HIDualTrackConstraint( const HIDualTrackConstraint& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIDualTrackConstraint::~HIDualTrackConstraint()
{
}

//
// assignment operators
//
// const HIDualTrackConstraint& HIDualTrackConstraint::operator=( const HIDualTrackConstraint& rhs )
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

HepVector
HIDualTrackConstraint::constraint(
   const STL_VECTOR( HIFitHelix* )& aHelices ) const
{
   assert( aHelices.size() == 2 ) ;
   assert( aHelices[0]->referencePoint() == aHelices[1]->referencePoint() );

   // First we need the components of the two momentum vectors parallel and
   // perpendicular to the boost direction.

   HepVector constraints( numberOfConstraints(), 0 ) ;
   constraints[ kD0 ]    = aHelices[ 0 ]->d0() + aHelices[ 1 ]->d0() ;
   constraints[ kZ0 ]    = aHelices[ 0 ]->z0() - aHelices[ 1 ]->z0() ;

   if( m_constrainPT )
   {
      GeV p0squared = aHelices[ 0 ]->momentumMag2() ;
      HepVector3D pt0 = aHelices[ 0 ]->momentum() ;
      pt0.setZ( 0. ) ;
      GeV pprll0 = m_vPhotonDir.dot( pt0 ) ;
      GeV pperp0 = m_vPhotonDir.cross( pt0 ).z() ;

      GeV p1squared = aHelices[ 1 ]->momentumMag2() ;
      HepVector3D pt1 = aHelices[ 1 ]->momentum() ;
      pt1.setZ( 0. ) ;
      GeV pprll1 = m_vPhotonDir.dot( pt1 ) ;
      GeV pperp1 = m_vPhotonDir.cross( pt1 ).z() ;

      // Boost the parallel components into the virtual photon rest frame.
      // Pprime = gamma * ( P - beta * E )
      GeV pprll0Boosted =
	 m_gamma * ( pprll0 - m_beta * sqrt( sqr( m_mass ) + p0squared ) ) ;
      GeV pprll1Boosted =
	 m_gamma * ( pprll1 - m_beta * sqrt( sqr( m_mass ) + p1squared ) ) ;

      // Now fill the constraint vector.
      constraints[ kPPrll ] = pprll0Boosted + pprll1Boosted ;
      constraints[ kPPerp ] = pperp0 + pperp1 ;
   }

   return constraints ;
}

HepMatrix
HIDualTrackConstraint::constraintDerivatives( 
   const STL_VECTOR( HIFitHelix* )& aHelices ) const
{
   assert( aHelices.size() == 2 ) ;
   assert( aHelices[0]->referencePoint() == aHelices[1]->referencePoint() );

   // First we need the components of the two momentum vectors parallel and
   // perpendicular to the boost direction.

   // Now fill the derivative matrix ;
   int nParams = HIHelix::kNTrackParameters ;
   HepMatrix derivs( 2*nParams, numberOfConstraints(), 0 ) ;

   derivs( HIHelix::kD0, kD0 + 1 )           = 1. ;
   derivs( HIHelix::kD0 + nParams, kD0 + 1 ) = 1. ;

   derivs( HIHelix::kZ0, kZ0 + 1 )           =  1. ;
   derivs( HIHelix::kZ0 + nParams, kZ0 + 1 ) = -1. ;

   if( m_constrainPT )
   {
      HepVector3D pt0 = aHelices[ 0 ]->momentum() ;
      GeV pmagsq0 = pt0.mag2() ;
      GeV pz0     = pt0.z() ;
      GeV ptmag0  = pt0.perp() ;

      pt0.setZ( 0. ) ;
      GeV pprll0 = m_vPhotonDir.dot( pt0 ) ;
      GeV pperp0 = m_vPhotonDir.cross( pt0 ).z() ;

      double K0 = aHelices[ 0 ]->curvature() ;
      double E0 = sqrt( pmagsq0 + sqr( aHelices[ 0 ]->mass() ) ) ;

      HepVector3D pt1 = aHelices[ 1 ]->momentum() ;
      GeV pmagsq1 = pt1.mag2() ;
      GeV pz1     = pt1.z() ;
      GeV ptmag1  = pt1.perp() ;

      pt1.setZ( 0. ) ;
      GeV pprll1 = m_vPhotonDir.dot( pt1 ) ;
      GeV pperp1 = m_vPhotonDir.cross( pt1 ).z() ;

      double K1 = aHelices[ 1 ]->curvature() ;
      double E1 = sqrt( pmagsq1 + sqr( aHelices[ 1 ]->mass() ) ) ;

      // Fill derivative matrix
      derivs( HIHelix::kCurvature, kPPrll + 1 ) =
	 m_gamma / K0 * ( m_beta * pmagsq0 / E0 - pprll0 ) ;
      derivs(  HIHelix::kCurvature + nParams, kPPrll + 1 ) =
	 m_gamma / K1 * ( m_beta * pmagsq1 / E1 - pprll1 ) ;

      derivs( HIHelix::kPhi0, kPPrll + 1 )           = -m_gamma * pperp0 ;
      derivs( HIHelix::kPhi0 + nParams, kPPrll + 1 ) = -m_gamma * pperp1 ;

      derivs( HIHelix::kCotTheta, kPPrll + 1 ) =
	 -m_beta * m_gamma * pz0 * ptmag0 / E0 ;
      derivs( HIHelix::kCotTheta + nParams, kPPrll + 1 ) =
	 -m_beta * m_gamma * pz1 * ptmag1 / E1 ;

      derivs( HIHelix::kCurvature, kPPerp + 1 )           = -pperp0 / K0 ;
      derivs( HIHelix::kCurvature + nParams, kPPerp + 1 ) = -pperp1 / K1 ;

      derivs( HIHelix::kPhi0, kPPerp + 1 )           = pprll0 ;
      derivs( HIHelix::kPhi0 + nParams, kPPerp + 1 ) = pprll1 ;
   }

   return derivs ;
}

//
// static member functions
//
