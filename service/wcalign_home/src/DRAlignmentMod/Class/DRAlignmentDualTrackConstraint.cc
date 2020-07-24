// -*- C++ -*-
//
// Package:     <DRAlignmentMod>
// Module:      DRAlignmentDualTrackConstraint
// 
// Description: see DRAlignmentMod.cc
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Dec  8 11:51:15 EST 2003
// $Id:
//
// Revision history
//
// $Log:
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
#include "DRAlignmentMod/DRAlignmentDualTrackConstraint.h"
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

static const char* const kFacilityString = "DRAlignmentMod.DRAlignmentDualTrackConstraint" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRAlignmentDualTrackConstraint.cc,v 1.1.1.1 2003/12/15 18:05:02 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DRAlignmentDualTrackConstraint::DRAlignmentDualTrackConstraint(
   DABoolean aPointConstraint,
   DABoolean aMomentumConstraint,
   double aMagneticFieldZ,
   const HepVector3D& aVirtualPhotonMomentum)
   : m_pointConstraint(aPointConstraint)
   , m_momentumConstraint(aMomentumConstraint)
   , m_Bz(aMagneticFieldZ)
   , m_Ptot(aVirtualPhotonMomentum)
{
   assert(m_pointConstraint || m_momentumConstraint);
}

// DRAlignmentDualTrackConstraint::DRAlignmentDualTrackConstraint( const DRAlignmentDualTrackConstraint& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DRAlignmentDualTrackConstraint::~DRAlignmentDualTrackConstraint()
{
}

//
// assignment operators
//
// const DRAlignmentDualTrackConstraint& DRAlignmentDualTrackConstraint::operator=( const DRAlignmentDualTrackConstraint& rhs )
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
DRAlignmentDualTrackConstraint::constraint(
   const STL_VECTOR( HIFitHelix* )& aHelices ) const
{
   assert( aHelices.size() == 2 ) ;
   assert( aHelices[0]->referencePoint() == aHelices[1]->referencePoint() );

   HepVector constraints(numberOfConstraints(), 0);

   if (m_pointConstraint) {
      const int offset = 0;
      constraints[offset + kD0] = aHelices[0]->d0() + aHelices[1]->d0();
      constraints[offset + kZ0] = aHelices[0]->z0() - aHelices[1]->z0();
   }

   if (m_momentumConstraint) {
      const int offset = (m_pointConstraint ? 2 : 0);
      const double k = fabs(-0.0299792458 * m_Bz / 2.);

      constraints[offset + kPx] = 
	 k * (cos(aHelices[0]->phi0()) / fabs(aHelices[0]->curvature()) +
	      cos(aHelices[1]->phi0()) / fabs(aHelices[1]->curvature()))
	 - m_Ptot.x();
      constraints[offset + kPy] = 
	 k * (sin(aHelices[0]->phi0()) / fabs(aHelices[0]->curvature()) +
	      sin(aHelices[1]->phi0()) / fabs(aHelices[1]->curvature()))
	 - m_Ptot.y();
      constraints[offset + kPz] = 
	 k * (aHelices[0]->cotTheta() / fabs(aHelices[0]->curvature()) +
	      aHelices[1]->cotTheta() / fabs(aHelices[1]->curvature()))
	 - m_Ptot.z();
   }

   return constraints;
}

HepMatrix
DRAlignmentDualTrackConstraint::constraintDerivatives( 
   const STL_VECTOR( HIFitHelix* )& aHelices ) const
{
   assert( aHelices.size() == 2 ) ;
   assert( aHelices[0]->referencePoint() == aHelices[1]->referencePoint() );

   HepMatrix derivs(2*HIHelix::kNTrackParameters, numberOfConstraints(), 0);
   
   if (m_pointConstraint) {
      const int offset = 1;
      derivs(HIHelix::kD0,                              offset + kD0) =  1.;
      derivs(HIHelix::kNTrackParameters + HIHelix::kD0, offset + kD0) =  1.;
      derivs(HIHelix::kZ0,                              offset + kZ0) =  1.;
      derivs(HIHelix::kNTrackParameters + HIHelix::kZ0, offset + kZ0) = -1.;
   }

   if (m_momentumConstraint) {
      const int offset = 1 + (m_pointConstraint ? 2 : 0);
      const double k = fabs(-0.0299792458 * m_Bz / 2.);

      derivs(HIHelix::kPhi0,                              offset + kPx) =
	 -k * sin(aHelices[0]->phi0()) / fabs(aHelices[0]->curvature());
      derivs(HIHelix::kNTrackParameters + HIHelix::kPhi0, offset + kPx) =
	 -k * sin(aHelices[1]->phi0()) / fabs(aHelices[1]->curvature());

      derivs(HIHelix::kPhi0,                              offset + kPy) =
	 k * cos(aHelices[0]->phi0()) / fabs(aHelices[0]->curvature());
      derivs(HIHelix::kNTrackParameters + HIHelix::kPhi0, offset + kPy) =
	 k * cos(aHelices[1]->phi0()) / fabs(aHelices[1]->curvature());

      derivs(HIHelix::kCotTheta,                              offset + kPz) =
	 k * 1. / fabs(aHelices[0]->curvature());
      derivs(HIHelix::kNTrackParameters + HIHelix::kCotTheta, offset + kPz) =
	 k * 1. / fabs(aHelices[1]->curvature());

      double dabs0 = (aHelices[0]->curvature() > 0. ? 1. : -1.);
      double dabs1 = (aHelices[1]->curvature() > 0. ? 1. : -1.);

      derivs(HIHelix::kCurvature,                              offset + kPx) =
	 -k * cos(aHelices[0]->phi0()) / sqr(aHelices[0]->curvature()) * dabs0;
      derivs(HIHelix::kNTrackParameters + HIHelix::kCurvature, offset + kPx) =
	 -k * cos(aHelices[1]->phi0()) / sqr(aHelices[1]->curvature()) * dabs1;

      derivs(HIHelix::kCurvature,                              offset + kPy) =
	 -k * sin(aHelices[0]->phi0()) / sqr(aHelices[0]->curvature()) * dabs0;
      derivs(HIHelix::kNTrackParameters + HIHelix::kCurvature, offset + kPy) =
	 -k * sin(aHelices[1]->phi0()) / sqr(aHelices[1]->curvature()) * dabs1;

      derivs(HIHelix::kCurvature,                              offset + kPz) =
	 -k * aHelices[0]->cotTheta() / sqr(aHelices[0]->curvature()) * dabs0;
      derivs(HIHelix::kNTrackParameters + HIHelix::kCurvature, offset + kPz) =
	 -k * aHelices[1]->cotTheta() / sqr(aHelices[1]->curvature()) * dabs1;
   }

   return derivs ;
}

//
// static member functions
//
