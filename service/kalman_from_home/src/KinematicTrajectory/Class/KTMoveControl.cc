// -*- C++ -*-
//
// Package:     KinematicTrajectory
// Module:      KTMoveControl
// 
// Description: Controls how moves requiring iteration are handled for
//              KTHelix and KTKinematicMoveable objects
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Paul Avery
// Created:     Sat May 16 14:07:08 EDT 1998
// $Id: KTMoveControl.cc,v 1.9 2000/06/15 20:16:49 pappas Exp $
//
// Revision history
//
// $Log: KTMoveControl.cc,v $
// Revision 1.9  2000/06/15 20:16:49  pappas
// Double checked and committed Paul Avery's modifications eliminating
// references to Toolbox classes. Should be completely transparent since
// Toolbox is just typedefs.
//
// Revision 1.8  1999/10/08 22:18:44  marsh
// Changed TBThreeVector to TBThreePoint.
//
// Revision 1.7  1998/12/09 22:09:14  avery
// Added notion of a "sticky" distance when moving. Fixed some small errors
// in moving KTHelix and KTKinematicData objects over large arcs. Implemented
// some missing set functions.
//
// Revision 1.6  1998/11/09 15:05:52  avery
// Replace abs(...) by fabs(...). Fix logic in KTMoveControl::chooseArcLength.
//
// Revision 1.5  1998/07/09 21:32:51  avery
// Bug fixes, finished implementation of move routines, updated documentation.
//
// Revision 1.4  1998/06/01 22:19:24  cdj
// gave initial values to static data members
//
// Revision 1.3  1998/05/27 17:10:36  avery
// Fixed algorithm
//
// Revision 1.2  1998/05/26 17:44:51  mkl
// provide definitions for Paul's static variables
//
// Revision 1.1  1998/05/23 17:21:09  avery
// Add KTMoveControl.cc
//

#include "Experiment/Experiment.h"
#include "Experiment/types.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#if defined(EXCEPTION_CLASH_WITH_MATH_H_BUG)
#define exception _math_bug_
#include <math.h>  // required for trigonometry and fabs
#undef exception
#else
#include <math.h>  // For 'sqrt'
#endif /* EXCEPTION_CLASH_WITH_MATH_H_BUG */

#include <assert.h>

// user include files
//#include "Experiment/report.h"
#include "KinematicTrajectory/KTMoveControl.h"
#include "KinematicTrajectory/KTKinematicMoveable.h"

// STL classes

//
// constants, enums and typedefs
//

const double       kBFieldConstant = -0.0299792458;

const unsigned int kDefaultMaxIterations =150;
const double       kDefaultMaxTurns      = 2.0;
const double       kDefaultMaxArcLength  = 20.;
const double       kDefaultMinStepSize   = 1.e-10;
const double       kLarge = 1.e20;
const double       kDefaultSafeFraction  = 0.9;

static const char* const kFacilityString = "KinematicTrajectory.KTMoveControl";

//
// static data member definitions
//

static inline double
sign(double a, double b)
{
// Returns |a| * sign(b)

   if(0. <= b) {
      return fabs(a);
   } else {
      return -fabs(a);
   }
}

KTMoveControl::ValueType KTMoveControl::m_maxErrorPerStep = 0.000100;

unsigned int KTMoveControl::m_storedMaxIterations = kDefaultMaxIterations;
double       KTMoveControl::m_storedMaxTurns      = kDefaultMaxTurns;
double       KTMoveControl::m_storedMaxArcLength  = kDefaultMaxArcLength;

double       KTMoveControl::m_storedMinStepSize   = kDefaultMinStepSize;
double       KTMoveControl::m_storedSafeFraction  = kDefaultSafeFraction;

double       KTMoveControl::m_stickyDistance  = 1.e-12;


//
// constructors and destructor
//

KTMoveControl::KTMoveControl( ) :
  m_maxIterations( m_storedMaxIterations ),
  m_maxTurns( m_storedMaxTurns ),
  m_maxArcLength( m_storedMaxArcLength ),
  m_iterations( 0 ),
  m_turns( 0. ),
  m_arcLength( 0. ),
  m_minStepSize( 0.),
  m_maxStepSize( 0.)
{
  m_minStepSize = m_storedMinStepSize;
  m_safeFraction = m_storedSafeFraction;
}


KTMoveControl::KTMoveControl( const unsigned int aMaxIterations,
			      const ValueType    aMaxTurns,
			      const ValueType    aMaxArcLength) :
  m_maxIterations( aMaxIterations ),
  m_maxTurns( aMaxTurns ),
  m_maxArcLength( aMaxArcLength ),
  m_iterations( 0 ),
  m_turns( 0. ),
  m_arcLength( 0. ),
  m_minStepSize( 0.),
  m_maxStepSize( 0.)
{
  m_minStepSize = m_storedMinStepSize;
  m_safeFraction = m_storedSafeFraction;
}


KTMoveControl::~KTMoveControl()
{
}

//
// assignment operators
//
// const KTMoveControl& KTMoveControl::operator=( const KTMoveControl& )
// {
// }

//
// member functions
//

void
KTMoveControl::setMinStepSize( const ValueType aMinStep) {
  m_minStepSize = aMinStep;
}


void
KTMoveControl::setMaxStepSize( const ValueType aMaxStep) {
  m_maxStepSize = aMaxStep;
}


void
KTMoveControl::setMaxStepSizeFromRho( const ValueType aRho,
				      const ValueType aSinTheta)
{
  m_maxStepSize = KTMoveControl::calculateMaxStepSize( aRho, aSinTheta);
}


void
KTMoveControl::setPossibleStepSize( const ValueType aRho,
				    const ValueType aSinTheta,
				    const ValueType aStep)
{
   const double step = KTMoveControl::calculateMaxStepSize( aRho, aSinTheta);
   if(step < aStep) {
      m_maxStepSize = step;
   }
   else {
      m_maxStepSize = aStep;
   }
}


void
KTMoveControl::setStoredMaxIterations( unsigned int aStoredMaxIterations) {
  m_storedMaxIterations = aStoredMaxIterations;
}


void
KTMoveControl::setStoredMaxTurns( const ValueType aStoredMaxTurns) {
  m_storedMaxTurns = aStoredMaxTurns;
}


void
KTMoveControl::setStoredMaxArcLength( const ValueType aStoredMaxArcLength) {
  m_storedMaxArcLength = aStoredMaxArcLength;
}


void
KTMoveControl::setStickyDistance( const ValueType aStickyDistance) {
  m_stickyDistance = aStickyDistance;
}


KTMoveControl::MoveStatus
KTMoveControl::update( const ValueType aRho,
		       const ValueType aStep)
{

  // Update the track iteration by one step. Return an error if too many
  // iterations, or too many turns or moved too far.

  ++m_iterations;
  m_turns     += aStep * (0.5/M_PI) * aRho;
  m_arcLength += aStep;

  // Do the check and quit if we have gone too far
  if(m_iterations       >= m_maxIterations ||
     fabs(m_turns)      >  m_maxTurns ||
     fabs(m_arcLength)  >  m_maxArcLength) {
    return kMoveAlgorithmFailed;
  }

  // If step size falls below a minimum, say we are done
  if(fabs(aStep) <= m_minStepSize) {
    return kMoveOK;
  }

  // Otherwise we have to do another step
  return kMoveNotDone;

}


//
// const member functions
//
KTMoveControl::ValueType
KTMoveControl::chooseArcLength( const ApproxParameters& aParameters,
				const MoveDirection aDirection) const
{
  // For the indicial equation
  //
  //      aQuadratic*s^2 + 2.*aLinear*s + aConstant
  //
  // solve for the arc length s. Choose the root depending on the
  // specified direction of motion and whether the roots satisfy
  //
  //      |s1| < maxStepSize()  or |s2| < maxStepSize
  //
  // The maximum size step returned is maxStepSize


  // Default direction is forward, unless backward is specified
  int sDirection = 1;
  if(aDirection == KTMoveControl::kDirectionBackward) {
     sDirection = -1;
  }

  //  ---------------------------------------------------------------------
  // Choose the solution encountered first in the direction of motion.
  // |s1| < |s2| always
  //  ---------------------------------------------------------------------

  // Get the solutions of the indicial equation

  // If no solution, step by full step in correct direction
  double s1 = 0.;
  double s2 = 0.;
  const double sBound = maxStepSize();

  if(!solveQuadratic(aParameters.quadratic(), aParameters.linear(),
		     aParameters.constant(),  s1, s2)) {
     return sBound * sDirection;
  }

  // If first time, check to see if we are pinned to the surface
  if(iterations() == 0  &&  aDirection != kDirectionEither) {
     if(fabs(s1) < stickyDistance()) {
	s1 = sBound * sDirection;
     }
     if(fabs(s2) < stickyDistance()) {
	s2 = sBound * sDirection;
     }
  }

  // resort the roots
  if(fabs(s1) > fabs(s2)) {
     double temp = s1;
     s1 = s2;
     s2 = temp;
  }

  // No preferred direction, find closest root
  if(aDirection == kDirectionEither) {

     if(fabs(s1) >= safeFraction()*sBound) { // Both roots outside safe region
	return sign(sBound, s1); // Step in direction of closest one
     }
 
  } else {    // Direction specified as forward or backward

     if(fabs(s1) >= safeFraction()*sBound) {// Both roots outside safe region,
	                                    // return bound
	return sBound * sDirection;
     }

     // At least one root is inside bound
     // s1d and s2d are positive when roots are in correct direction
     const double s1d = s1 * sDirection;
     const double s2d = s2 * sDirection;
    
     // Smallest root is in opposite direction to requested direction.
     // If total step (including this one) is in wrong direction,
     // take full step in correct direction
     if(s1d < 0.) {
        if(s1d + fabs( arcLength() ) < 0.) {
	   if(s2d < 0. || s2d > sBound) {
	      return sBound * sDirection;
	   } else {
	      return s2;
	   }
	}
     }
  }

  // Default return value
  return s1;
}


//
// static member functions
//

DABoolean
KTMoveControl::solveQuadratic(const ValueType aQuadratic,
			      const ValueType aLinear,
			      const ValueType aConstant,
			      ValueType& aS1,
			      ValueType& aS2)
{
  // Solve the equation aConstant + 2.*aLinear*s + aQuadratic*s^2 = 0
  // and return both roots sorted so that
  //
  //   |aS1| <= |aS2|


  aS1 = 0.;
  aS2 = 0.;

  double disc = aLinear*aLinear - aQuadratic*aConstant;

  // No solution, move forward by max arc length and return
  if(disc < 0.) {
    return false;
  }

  disc = sqrt(disc);
  double sign;   // sign = sign of linear term
  if(aLinear >= 0.) {
    sign = 1.;
  }
  else {
    sign = -1.;
  }

  // Quadratic term non-zero
  if(aQuadratic != 0.) {
    aS1 = (-aLinear - sign*disc) / aQuadratic;
    aS2 = aConstant / (-aLinear - sign*disc);
    if(fabs(aS1) > fabs(aS2)) {
      double temp = aS1;
      aS1 = aS2;
      aS2 = temp;
    }

  }

  // Quadratic term zero
  else {
    if(aLinear == 0.) {
      return false;   // No linear or quad. terms  ==> no solution
    }
    else {
      aS1 = -0.5*aConstant / aLinear;  // Linear term only  ==> 1 solution
      aS2 = aS1;
    }
  }

  return true;
}

int
KTMoveControl::directionInteger( const MoveDirection aDirection)
{
   // Return a simple integer for forward (+1), backward (-1) and either (0)

   if(kDirectionForward == aDirection) {
      return +1;
   }
   else if(kDirectionBackward == aDirection) {
      return -1;
   }
   else {
      return 0;
   }
}


KTMoveControl::ValueType
KTMoveControl::calculateMaxStepSize( const ValueType aRho,
				     const ValueType aSinTheta)
{
  // Calculate the maximum arc length a track can move in one step,
  // depending on its curvature and the max perpendicular error it
  // can make in one step. The distance is set by looking at
  // the first neglected term in sin(rho*s)/rho and making its
  // value less than the error per step, i.e.
  //
  //        |p x eta|/p * (s - sin(rho*s)/rho) < errstep
  // Then use the fact that sin(theta) = |p x eta| / p
  //
  // step = [6*errstep/sin(theta)/rho^2]^(1/3)

  if(aRho == 0.) {
    return kLarge;   // Zero curvature ==> return large value
  }
  else {
    return pow(6. * maxErrorPerStep() / (aSinTheta*aRho*aRho), 1./3.) ;
  }

}


void
KTMoveControl::setMaxErrorPerStep( const ValueType aMaxErrorPerStep)
{
  // Set maximum perpendicular error per step. This value is used to
  // set the maximum arc length a track can move in one step, depending
  // on its curvature

  m_maxErrorPerStep = aMaxErrorPerStep;
}


KTMoveControl::ApproxParameters
KTMoveControl::Point3D::calcParameters( const KTKinematicMoveable& aParticle)
                                const
{
  // Calculate the coefficients of the indicial equation
  //
  //     aQuadratic*s^2 + 2*aLinear*s + aConstant = 0
  //
  // which is used to determine the approximate distance s to the point of
  // closest approach to a 3-D point. The indicial equation is obtained
  // by substituting the equations of motion of the helix into the equation
  // which defines the point of closest approach and truncating to second
  // order in s.


  // Get the B field information
  const HepVector3D bFieldVector = aParticle.bField();
  const double bFieldMagnitude = bFieldVector.mag();
  HepVector3D bFieldDir;
  if(0. == bFieldMagnitude) {
    bFieldDir = HepVector3D(0., 0., 0.);
  }
  else {
    bFieldDir = (1. / bFieldMagnitude) * bFieldVector;
  }

  // Dot and cross products needed by the coefficients
  const double ptot = aParticle.pmag();  // Total momentum
  const double rho = kBFieldConstant * bFieldMagnitude * aParticle.charge()
                            / ptot;
  const HepVector3D momentum = aParticle.momentum();

  const HepPoint3D delta       = aParticle.position() - point();
  const HepVector3D ph         = momentum.cross(bFieldDir);
  const double        pDotDel  = momentum.dot(delta);
  const double        phDotDel = ph.dot(delta);
  const double        delDotH  = delta.dot(bFieldDir);
  const double        pDotH    = momentum.dot(bFieldDir);

  // Calculate the coefficients of the indicial equation which determines
  // the arc length to move in this step.
  const double constant  =  pDotDel;
  const double linear    =  0.5 * (ptot - rho * phDotDel);
  const double quadratic = -0.5 * rho*rho * (pDotDel - delDotH*pDotH);

  return ApproxParameters(quadratic, linear, constant);

}


KTMoveControl::ApproxParameters
KTMoveControl::Point2D::calcParameters( const KTKinematicMoveable& aParticle)
                                const
{
  // Calculate the coefficients of the indicial equation
  //
  //     aQuadratic*s^2 + 2*aLinear*s + aConstant = 0
  //
  // which is used to determine the approximate distance s to the point of
  // closest approach to a point in the bend plane.
  // The indicial equation is obtained
  // by substituting the equations of motion of the helix into the equation
  // which defines the point of closest approach and truncating to second
  // order in s.


  // Get the B field information
  const HepVector3D bFieldVector = aParticle.bField();
  const double bFieldMagnitude = bFieldVector.mag();
  HepVector3D bFieldDir;
  if(0. == bFieldMagnitude) {
    bFieldDir = HepVector3D(0., 0., 0.);
  }
  else {
    bFieldDir = (1. / bFieldMagnitude) * bFieldVector;
  }

  // Dot and cross products needed by the coefficients
  const double ptot = aParticle.pmag();  // Total momentum
  const double rho = kBFieldConstant * bFieldMagnitude * aParticle.charge()
                            / ptot;
  const HepVector3D momentum = aParticle.momentum();

  const HepPoint3D delta        = aParticle.position() - point();
  const HepVector3D ph          = momentum.cross(bFieldDir);
  const double        pDotDel   = momentum.dot(delta);
  const double        phDotDel  = ph.dot(delta);
  const double        delDotH   = delta.dot(bFieldDir);
  const double        pDotH     = momentum.dot(bFieldDir);

  // Calculate the coefficients of the indicial equation which determines
  // the arc length to move in this step.
  const double constant  =  pDotDel - delDotH * pDotH;
  const double linear    =  0.5 * (ptot - pDotH*pDotH/ptot - rho * phDotDel);
  const double quadratic = -0.5 * rho*rho * (pDotDel - delDotH*pDotH);

  return ApproxParameters(quadratic, linear, constant);

}


KTMoveControl::ApproxParameters
KTMoveControl::Line::calcParameters( const KTKinematicMoveable& aParticle)
                               const
{
  // Calculate the coefficients of the indicial equation
  //
  //     aQuadratic*s^2 + 2*aLinear*s + aConstant = 0
  //
  // which is used to determine the approximate distance s to the point of
  // closest approach to a line. The indicial equation is obtained
  // by substituting the equations of motion of the helix into the equation
  // which defines the point of closest approach and truncating to second
  // order in s.


  // Get the B field information
  const HepVector3D bFieldVector = aParticle.bField();
  const double bFieldMagnitude = bFieldVector.mag();
  HepVector3D bFieldDir;
  if(0. == bFieldMagnitude) {
    bFieldDir = HepVector3D(0., 0., 0.);
  }
  else {
    bFieldDir = (1. / bFieldMagnitude) * bFieldVector;
  }

  // Dot and cross products needed by the coefficients
  const double ptot = aParticle.pmag();  // Total momentum
  const double rho = kBFieldConstant * bFieldMagnitude * aParticle.charge()
                            / ptot;
  const HepVector3D momentum = aParticle.momentum();
  const HepVector3D eta      = dirCosine();

  const HepPoint3D    delta     = aParticle.position() - point();
  const HepVector3D   ph        = momentum.cross(bFieldDir);
  const double        pDotDel   = momentum.dot(delta);
  const double        phDotDel  = ph.dot(delta);
  const double        delDotH   = delta.dot(bFieldDir);
  const double        pDotH     = momentum.dot(bFieldDir);

  const double        delDotEta = delta.dot(eta);
  const double        pDotEta   = momentum.dot(eta);
  const double        phDotEta  = ph.dot(eta);
  const double        etaDotH   = eta.dot(bFieldDir);

  // Calculate the coefficients of the indicial equation which determines
  // the arc length to move in this step.
  const double constant  = pDotDel - delDotEta * pDotEta;
  const double linear    = 0.5 * (ptot - pDotEta*pDotEta / ptot
                               - rho * (phDotDel - delDotEta * phDotEta));
  const double quadratic = 0.5 * rho * (3.*pDotEta/ptot*phDotEta
                           + rho * (-pDotDel + delDotEta * pDotEta
                                   + delDotH * pDotH
				   - delDotEta * pDotH * etaDotH) );

  return ApproxParameters(quadratic, linear, constant);

}


KTMoveControl::ApproxParameters
KTMoveControl::Plane::calcParameters( const KTKinematicMoveable& aParticle)
                               const
{
  // Calculate the coefficients of the indicial equation
  //
  //     aQuadratic*s^2 + 2*aLinear*s + aConstant = 0
  //
  // which is used to determine the approximate distance s to the point of
  // intersection with a plane. The indicial equation is obtained
  // by substituting the equations of motion of the helix into the equation
  // which defines the plane and truncating to second order in s.


  // Get the B field information
  const HepVector3D bFieldVector = aParticle.bField();
  const double bFieldMagnitude = bFieldVector.mag();
  HepVector3D bFieldDir;
  if(0. == bFieldMagnitude) {
    bFieldDir = HepVector3D(0., 0., 0.);
  }
  else {
    bFieldDir = (1. / bFieldMagnitude) * bFieldVector;
  }

  // Dot and cross products needed by the coefficients
  const double ptot = aParticle.pmag();  // Total momentum
  const double rho = kBFieldConstant * bFieldMagnitude * aParticle.charge()
                            / ptot;
  const HepVector3D momentum = aParticle.momentum();

  const HepPoint3D delta     = aParticle.position() - point();
  const HepVector3D ph       = momentum.cross(bFieldDir);

  const double        delDotEta = delta.dot(normal());
  const double        pDotEta   = momentum.dot(normal());
  const double        phDotEta  = ph.dot(normal());

  // Calculate the coefficients of the indicial equation which determines
  // the arc length to move in this step.
  const double constant  = delDotEta;
  const double linear    = 0.5 * pDotEta / ptot;
  const double quadratic = -0.5 * rho * phDotEta / ptot;

  return ApproxParameters(quadratic, linear, constant);

}


KTMoveControl::ApproxParameters
KTMoveControl::Cylinder::calcParameters( const KTKinematicMoveable& aParticle)
                               const
{
  // Calculate the coefficients of the indicial equation
  //
  //     aQuadratic*s^2 + 2*aLinear*s + aConstant = 0
  //
  // which is used to determine the approximate distance s to the point of
  // intersection with a cylinder. The indicial equation is obtained
  // by substituting the equations of motion of the helix into the equation
  // which defines the cylinder and truncating to second order in s.


  // Get the B field information
  const HepVector3D bFieldVector = aParticle.bField();
  const double bFieldMagnitude = bFieldVector.mag();
  HepVector3D bFieldDir;
  if(0. == bFieldMagnitude) {
    bFieldDir = HepVector3D(0., 0., 0.);
  }
  else {
    bFieldDir = (1. / bFieldMagnitude) * bFieldVector;
  }

  // Dot and cross products needed by the coefficients
  const double ptot = aParticle.pmag();  // Total momentum
  const double rho = kBFieldConstant * bFieldMagnitude * aParticle.charge()
                            / ptot;
  const HepVector3D momentum = aParticle.momentum();

  const HepPoint3D  delta = aParticle.position() - center();
  const HepVector3D ph    = momentum.cross(bFieldDir);
  const HepVector3D eta   = dirCosine();

  const double        pDotDel   = delta.dot(momentum);
  const double        phDotDel   = delta.dot(ph);
  const double        delDotEta = delta.dot(eta);
  const double        pDotEta   = momentum.dot(eta);
  const double        phDotEta  = ph.dot(eta);

  // Calculate the coefficients of the indicial equation which determines
  // the arc length to move in this step.
  const double constant  = delta.dot(delta) - delDotEta*delDotEta
                         - radius()*radius() ;
  const double linear    = (pDotDel - delDotEta*pDotEta) / ptot;
  const double quadratic = 1. - pDotEta*pDotEta/ptot/ptot
                          -rho * (phDotDel/ptot - delDotEta*phDotEta/ptot);

  return ApproxParameters(quadratic, linear, constant);

}


KTMoveControl::MoveSpec::~MoveSpec()
{
}
