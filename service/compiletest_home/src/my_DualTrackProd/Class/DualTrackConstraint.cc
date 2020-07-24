// -*- C++ -*-
//
// Package:     <DualTrackProd>
// Module:      DualTrackConstraint
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Dec 10 11:49:56 EST 2003
// $Id$
//
// Revision history
//
// $Log$

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
#include <assert.h>

// user include files
//#include "Experiment/report.h"
#include "DualTrackProd/DualTrackConstraint.h"


//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DualTrackProd.DualTrackConstraint" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: skeleton.cc,v 1.6 2001/01/03 16:25:23 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DualTrackConstraint::DualTrackConstraint(double magneticField,
					 HepVector3D pVirtualPhoton,
					 DABoolean pointConstraint,
					 DABoolean ptConstraint,
					 DABoolean pzConstraint)
   : m_magneticField(magneticField)
   , m_pVirtualPhoton(pVirtualPhoton)
   , m_pointConstraint(pointConstraint)
   , m_ptConstraint(ptConstraint)
   , m_pzConstraint(pzConstraint)
{
}

// DualTrackConstraint::DualTrackConstraint( const DualTrackConstraint& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DualTrackConstraint::~DualTrackConstraint()
{
}

//
// assignment operators
//
// const DualTrackConstraint& DualTrackConstraint::operator=( const DualTrackConstraint& rhs )
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

// Curvature -> momentum constant 0.0299792458 cribbed from KTHelix.cc

HepVector
DualTrackConstraint::constraint(const vector<HIFitHelix*>& helix) const
{
   assert(helix.size() == 2);
   assert(helix[0]->referencePoint() == helix[1]->referencePoint());

   HepVector constraints(numberOfConstraints(), 0);

   if (m_pointConstraint) {
      const int offset = 0;
      constraints[offset + kD0] = helix[0]->d0() + helix[1]->d0();
      constraints[offset + kZ0] = helix[0]->z0() - helix[1]->z0();
   }

   if (m_ptConstraint) {
      const int offset = (m_pointConstraint ? 2 : 0);
      const double k = fabs(-0.0299792458 * m_magneticField / 2.);

      constraints[offset + kPx] = 
	 k * (cos(helix[0]->phi0()) / fabs(helix[0]->curvature()) +
	      cos(helix[1]->phi0()) / fabs(helix[1]->curvature()))
	 - m_pVirtualPhoton.x();
      constraints[offset + kPy] = 
	 k * (sin(helix[0]->phi0()) / fabs(helix[0]->curvature()) +
	      sin(helix[1]->phi0()) / fabs(helix[1]->curvature()))
	 - m_pVirtualPhoton.y();
   }

   if (m_pzConstraint) {
      const int offset = (m_pointConstraint ? 2 : 0) + (m_ptConstraint ? 2 : 0);
      const double k = fabs(-0.0299792458 * m_magneticField / 2.);

      constraints[offset + kPz] = 
	 k * (helix[0]->cotTheta() / fabs(helix[0]->curvature()) +
	      helix[1]->cotTheta() / fabs(helix[1]->curvature()))
	 - m_pVirtualPhoton.z();
   }

   return constraints;
}

HepMatrix
DualTrackConstraint::constraintDerivatives(const vector<HIFitHelix*>& helix) const
{
   assert(helix.size() == 2);
   assert(helix[0]->referencePoint() == helix[1]->referencePoint());

   HepMatrix derivs(2*HIHelix::kNTrackParameters, numberOfConstraints(), 0);
   
   if (m_pointConstraint) {
      const int offset = 1;
      derivs(HIHelix::kD0,                              offset + kD0) =  1.;
      derivs(HIHelix::kNTrackParameters + HIHelix::kD0, offset + kD0) =  1.;
      derivs(HIHelix::kZ0,                              offset + kZ0) =  1.;
      derivs(HIHelix::kNTrackParameters + HIHelix::kZ0, offset + kZ0) = -1.;
   }

   if (m_ptConstraint) {
      const int offset = 1 + (m_pointConstraint ? 2 : 0);
      const double k = fabs(-0.0299792458 * m_magneticField / 2.);

      derivs(HIHelix::kPhi0,                              offset + kPx) =
	 -k * sin(helix[0]->phi0()) / fabs(helix[0]->curvature());
      derivs(HIHelix::kNTrackParameters + HIHelix::kPhi0, offset + kPx) =
	 -k * sin(helix[1]->phi0()) / fabs(helix[1]->curvature());

      derivs(HIHelix::kPhi0,                              offset + kPy) =
	 k * cos(helix[0]->phi0()) / fabs(helix[0]->curvature());
      derivs(HIHelix::kNTrackParameters + HIHelix::kPhi0, offset + kPy) =
	 k * cos(helix[1]->phi0()) / fabs(helix[1]->curvature());

      double dabs0 = (helix[0]->curvature() > 0. ? 1. : -1.);
      double dabs1 = (helix[1]->curvature() > 0. ? 1. : -1.);

      derivs(HIHelix::kCurvature,                              offset + kPx) =
	 -k * cos(helix[0]->phi0()) / sqr(helix[0]->curvature()) * dabs0;
      derivs(HIHelix::kNTrackParameters + HIHelix::kCurvature, offset + kPx) =
	 -k * cos(helix[1]->phi0()) / sqr(helix[1]->curvature()) * dabs1;

      derivs(HIHelix::kCurvature,                              offset + kPy) =
	 -k * sin(helix[0]->phi0()) / sqr(helix[0]->curvature()) * dabs0;
      derivs(HIHelix::kNTrackParameters + HIHelix::kCurvature, offset + kPy) =
	 -k * sin(helix[1]->phi0()) / sqr(helix[1]->curvature()) * dabs1;
   }

   if (m_pzConstraint) {
      const int offset = 1 + (m_pointConstraint ? 2 : 0) + (m_ptConstraint ? 2 : 0);
      const double k = fabs(-0.0299792458 * m_magneticField / 2.);

      derivs(HIHelix::kCotTheta,                              offset + kPz) =
	 k * 1. / fabs(helix[0]->curvature());
      derivs(HIHelix::kNTrackParameters + HIHelix::kCotTheta, offset + kPz) =
	 k * 1. / fabs(helix[1]->curvature());

      double dabs0 = (helix[0]->curvature() > 0. ? 1. : -1.);
      double dabs1 = (helix[1]->curvature() > 0. ? 1. : -1.);

      derivs(HIHelix::kCurvature,                              offset + kPz) =
	 -k * helix[0]->cotTheta() / sqr(helix[0]->curvature()) * dabs0;
      derivs(HIHelix::kNTrackParameters + HIHelix::kCurvature, offset + kPz) =
	 -k * helix[1]->cotTheta() / sqr(helix[1]->curvature()) * dabs1;
   }

   return derivs ;
}

//
// static member functions
//
