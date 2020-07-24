// -*- C++ -*-
//
// Package:     KinematicTrajectory
// Module:      KTKinematicFixedBz
// 
// Description: Implements methods for moving Kinematic objects through a
//              fixed solenoidal magnetic field.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Paul Avery
// Created:     Wed Oct  8 16:44:33 EDT 1997
// $Id: KTKinematicFixedBz.cc,v 1.16 2000/10/14 20:58:16 wsun Exp $
//
// Revision history
//
// $Log: KTKinematicFixedBz.cc,v $
// Revision 1.16  2000/10/14 20:58:16  wsun
// Fixed bug found my lkg many moons ago in moveToZPosition().
//
// Revision 1.15  2000/09/25 15:00:29  pappas
// Small bug fix in error matrix propagation; cross partials of energy
// should be zero since energy is independent variable and mass is not
// constrained.
//
// Revision 1.14  2000/06/15 20:16:48  pappas
// Double checked and committed Paul Avery's modifications eliminating
// references to Toolbox classes. Should be completely transparent since
// Toolbox is just typedefs.
//
// Revision 1.13  1999/10/08 22:26:14  marsh
// Changed TBThreeVector to TBThreePoint.
//
// Revision 1.12  1998/12/09 22:09:07  avery
// Added notion of a "sticky" distance when moving. Fixed some small errors
// in moving KTHelix and KTKinematicData objects over large arcs. Implemented
// some missing set functions.
//
// Revision 1.11  1998/11/09 15:05:48  avery
// Replace abs(...) by fabs(...). Fix logic in KTMoveControl::chooseArcLength.
//
// Revision 1.10  1998/10/13 17:26:21  avery
// Make check on non-axial line more robust
//
// Revision 1.9  1998/07/09 21:32:47  avery
// Bug fixes, finished implementation of move routines, updated documentation.
//
// Revision 1.8  1998/06/01 18:24:10  avery
// Fixed problem with copy constructor
//
// Revision 1.7  1998/05/27 17:12:44  avery
// Improved documentation
//
// Revision 1.6  1998/05/24 21:30:32  avery
// Add move, PCA routines. Some cleanup
//
// Revision 1.5  1997/12/22 23:21:52  cleo3
// quick fix/kludge for non-compiling source from Paul Avery
//
// Revision 1.4  1997/12/22 17:49:08  avery
// Fixed error in s3d definition in KTKinematicFixedBz.cc
//
// Revision 1.3  1997/12/19 19:42:21  avery
// Changed B field constant to reflect use of KGauss instead of Tesla.
//
// Revision 1.2  1997/12/03 22:03:14  mkl
// improved documentation; fixed g++ problems (warning about unused parameters etc.)
//
// Revision 1.1.1.1  1997/11/04 23:38:46  cdj
// First submission
//
// Revision 1.1.1.1  1997/11/04 22:45:14  avery
// First submission
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include <assert.h>

// user include files
//#include "Experiment/report.h"
#include "KinematicTrajectory/KTKinematicFixedBz.h"
#include "CLHEP/Matrix/Matrix.h"

// STL classes

//
// constants, enums and typedefs
//

// This will go into a header file
const double kBFieldConstant = -0.0299792458;

static const char* const kFacilityString = "KinematicTrajectory.KTKinematicFixedBz" ;

//
// static data member definitions
//

//
// constructors and destructor
//
KTKinematicFixedBz::KTKinematicFixedBz()
{
}

KTKinematicFixedBz::KTKinematicFixedBz( const KTKinematicFixedBz& iCopyFrom,
					const DABoolean aCopyErrorMatrix) :
   KTKinematicMoveable( iCopyFrom, aCopyErrorMatrix ),
   m_BFieldMagnitude( iCopyFrom.BFieldMagnitude() )
{
}


KTKinematicFixedBz::KTKinematicFixedBz( const KTKinematicFixedBz& iCopyFrom):
   KTKinematicMoveable( iCopyFrom ),
   m_BFieldMagnitude( iCopyFrom.BFieldMagnitude() )
{
}


KTKinematicFixedBz::KTKinematicFixedBz( const KTKinematicData& aKinematicData,
					const ValueType aBFieldMagnitude,
					const DABoolean aCopyErrorMatrix) :
  KTKinematicMoveable(aKinematicData, aCopyErrorMatrix),
  m_BFieldMagnitude(aBFieldMagnitude)
{
}


KTKinematicFixedBz::KTKinematicFixedBz(const HepVector3D& aMomentum ,
				       const HepPoint3D&  aPosition ,
				       const ValueType aMass ,
				       const ValueType aCharge ,
				       const ValueType aBFieldMagnitude) :
   KTKinematicMoveable(aMomentum, aPosition, aMass, aCharge),
   m_BFieldMagnitude(aBFieldMagnitude)
{
   return ;
}

KTKinematicFixedBz::KTKinematicFixedBz(const HepVector3D& aMomentum ,
				       const HepPoint3D&  aPosition ,
				       const ValueType aMass ,
				       const ValueType aCharge ,
				       const ValueType aBFieldMagnitude ,
				       const HepSymMatrix& aErrorMatrix) :
   KTKinematicMoveable(aMomentum, aPosition, aMass, aCharge, aErrorMatrix),
   m_BFieldMagnitude(aBFieldMagnitude)
{
   return ;
}


KTKinematicFixedBz::KTKinematicFixedBz( const KTHelix& aHelix ,
					const ValueType aMass,
					const ValueType aBFieldMagnitude ,
					const DABoolean aCopyErrorMatrix) :
   KTKinematicMoveable(aHelix, aMass, aBFieldMagnitude, aCopyErrorMatrix),
   m_BFieldMagnitude(aBFieldMagnitude)
{
}


KTKinematicFixedBz::~KTKinematicFixedBz()
{
}

//
// assignment operators
//
// const KTKinematicFixedBz& KTKinematicFixedBz::operator=(
//                                               const KTKinematicFixedBz& )
// {
// }

//
// member functions
//

KTMoveControl::MoveStatus
KTKinematicFixedBz::moveByArcLength( const ValueType aArcLength)
{
  // This function translates a KinematicData object by the arc length
  // aArcLength. The transport equations are shown below:
  //
  //  px' = px*cos(rho*s) - py*sin(rho*s)
  //  py' = px*sin(rho*s) + py*cos(rho*s)
  //  pz' = pz
  //   E' = E
  //   x' = x + (px/a) * sin(rho*s) - (py/a) * (1 - cos(rho*s) )
  //   y' = y + (py/a) * sin(rho*s) + (px/a) * (1 - cos(rho*s) )
  //   z' = z + (pz/p) * s
  //
  // where
  //  s   = arc length
  //  a   = -0.0299792458 * Bfield * charge
  //  rho = a / p
  //

  // Curvature constant
  double a = kBFieldConstant * BFieldMagnitude() * charge();

  // Check if we should use the zero B field version
  if(a == 0.) {
    return moveByArcLengthBZero( aArcLength);
  }

  KTKinematicData saveTrack(*this);   // Save current values in order
                                      // to move error matrix later

  const HepVector3D pmom = momentum();
  double pmag = pmom.mag();

  double rho = a / pmag;
  double sinps = sin(rho * aArcLength);
  double cosps = cos(rho * aArcLength);

  // Transport momentum
  setMomentum( HepVector3D(pmom.x()*cosps - pmom.y()*sinps,
			     pmom.y()*cosps + pmom.x()*sinps,
			     pmom.z()) );

  // Transport position (be sure to use initial momentum)
  double dcosps = 1. - cosps;
  setPosition( position()
	       + HepPoint3D( (pmom.x()*sinps - pmom.y()*dcosps) / a,
			       (pmom.y()*sinps + pmom.x()*dcosps) / a,
			        pmom.z() * (aArcLength / pmag) ) );

  // Move error matrix and return status
  KTMoveControl::MoveStatus errorStatus = moveErrorMatrix(saveTrack);

  return errorStatus;
}

KTMoveControl::MoveStatus
KTKinematicFixedBz::moveToPoint( const HepPoint3D& aPoint,
				 const KTMoveControl::MoveDirection aDirection)
{
   // Curvature constant
   const double a = kBFieldConstant * BFieldMagnitude() * charge();

   // Check if we should use the zero B field version
   if(a == 0.) {
      return moveToPointBZero( aPoint, aDirection);
   }

   // Otherwise do the move iteratively
   return KTKinematicMoveable::moveToPointIterate( aPoint, aDirection );

}

KTMoveControl::MoveStatus
KTKinematicFixedBz::moveToPointBend( 
                                const HepPoint3D& aPoint,
				const KTMoveControl::MoveDirection aDirection)
{
   // This function translates a KinematicData object to the point closest
   // to the specified (x,y) point. The transport equations are shown below:
   //
   //  px' = px*cos(rho*s) - py*sin(rho*s)
   //  py' = px*sin(rho*s) + py*cos(rho*s)
   //  pz' = pz
   //   E' = E
   //   x' = x + (px/a) * sin(rho*s) - (py/a) * (1 - cos(rho*s) )
   //   y' = y + (py/a) * sin(rho*s) + (px/a) * (1 - cos(rho*s) )
   //   z' = z + (pz/p) * s
   //
   // where
   //  s   = arc length
   //  a   = -0.0299792458 * Bfield * charge
   //  rho = a / p
   //

   // Curvature constant
   const double a = kBFieldConstant * BFieldMagnitude() * charge();

   // Check if we should use the zero B field version
   if(a == 0.) {
      return moveToPointBendBZero( aPoint, aDirection);
   }

   KTKinematicData saveTrack(*this);   // Save current values in order
                                       // to move error matrix later

   const double px = momentum().x();
   const double py = momentum().y();
   const double pz = momentum().z();
   const double pmag = momentum().mag();

   const double delta_x = x() - aPoint.x();
   const double delta_y = y() - aPoint.y();

   const double pperp = sqrt(px*px + py*py);

   // If no pperp, then we are already at PCA and we can quit
   if(0. == pperp) {
      return KTMoveControl::kMoveOK;
   }

   const double rho = a / pmag;

   const double A1 =  (pperp*pperp - a*(delta_x*py - delta_y*px) )
                        / (pmag*pmag);
   const double A2 = (delta_x*px + delta_y*py) / pmag;

   const double denom =  1. / sqrt(A1*A1 + (rho*A2)*(rho*A2) );
   const double cosps =        A1 * denom;
   const double sinps = -(rho*A2) * denom;
   double circum = (2.*M_PI) / fabs(rho); // Circumference incl. z motion

   // Find the arc length
   double s = atan2(sinps, cosps) / rho;

   int sDirection = KTMoveControl::directionInteger(aDirection);

   // If track is already pinned to the point, add full circle if
   // direction specified
   if(fabs(s) < KTMoveControl::stickyDistance()) {
      s = circum * sDirection;
   }

   // If arc length is in wrong direction, force it to be in right
   // direction by adding circle circumference to it.
   if(s < 0. && KTMoveControl::kDirectionForward == aDirection) {
      s = s + circum;
   }
   else if(s > 0. && KTMoveControl::kDirectionBackward == aDirection) {
      s = s - circum;
   }

   // Transport momentum
   setMomentum( HepVector3D(px*cosps - py*sinps,
			      py*cosps + px*sinps,
			      pz) );

   // Transport position
   double dcosps = 1. - cosps;
   setPosition( position() + HepPoint3D( (px*sinps - py*dcosps) / a,
					   (py*sinps + px*dcosps) / a,
					    pz * (s / pmag) ) );
   // Move error matrix and return status
   KTMoveControl::MoveStatus errorStatus = moveErrorMatrix(saveTrack);

   return errorStatus;
}


KTMoveControl::MoveStatus
KTKinematicFixedBz::moveToLine( const HepPoint3D&  aPoint,
		                const HepVector3D& aDirCosine,
		                const KTMoveControl::MoveDirection aDirection)
{
   // Curvature constant
   const double a = kBFieldConstant * BFieldMagnitude() * charge();

   // Check for straight line case. If so, use special version
   if(a == 0.) {
     return moveToLineBZero(aPoint, aDirCosine, aDirection);
   }

   // Check if line is axial. If so, use special routine
   if(aDirCosine.x() == 0. && aDirCosine.y() == 0.) {
      const HepPoint3D point(aPoint);
      return moveToPointBend( point, aDirection);
   }

   // Otherwise do the move iteratively
   return KTKinematicMoveable::moveToLineIterate( aPoint, aDirCosine,
						  aDirection );

}



KTMoveControl::MoveStatus
KTKinematicFixedBz::moveToZPosition(
                                const ValueType aZPosition,
				const KTMoveControl::MoveDirection aDirection)
{
   // Move to the specified z position

   // Curvature constant
   const double a = kBFieldConstant * BFieldMagnitude() * charge();

   // Check if we should use the zero B field version
   if(a == 0.) {
      const HepPoint3D point(0., 0., aZPosition);
      const HepVector3D eta(0., 0., 1.);
      return moveToPlaneBZero( point, eta, aDirection);
   }

   KTKinematicData saveTrack(*this);   // Save current values in order
                                       // to move error matrix later

   const double px = momentum().x();
   const double py = momentum().y();
   const double pz = momentum().z();
   const double pmag = momentum().mag();

   // Quit if total momentum 0
   if(pmag == 0.) {
      return KTMoveControl::kMoveNoIntersection;
   }

   // Quit if no z component and we are not already at the correct z
   if(pz == 0.) {
      if(z() != aZPosition) {
	 return KTMoveControl::kMoveNoIntersection;
      }
      else {
	 return KTMoveControl::kMoveOK;
      }
   }

   // wsun 10-14-00: fixed bug found by lkg several months ago.
   //   double sOverP = (aZPosition - x()) / pz;
   double sOverP = (aZPosition - z()) / pz;
   double s = sOverP * pmag;

   int sDirection = KTMoveControl::directionInteger(aDirection);
   // If track is already pinned to the point, put it there exactly
   if(fabs(s) < KTMoveControl::stickyDistance()) {
      s = 0.;
      sOverP = 0.;
   }

   // Check to see if intercept is in correct direction
   if(s <= 0. && aDirection==KTMoveControl::kDirectionForward ||
      s >= 0. && aDirection==KTMoveControl::kDirectionBackward) {
      return KTMoveControl::kMoveNoIntersection;
   }

   const double rho = a / pmag;
   const double cosps = cos(rho*s);
   const double sinps = sin(rho*s);
   const double dcosps = 1. - cosps;

   // Transport momentum
   setMomentum(HepVector3D(px*cosps - py*sinps,
			     py*cosps + px*sinps,
			     pz) );
      
   // Transport position
   setPosition( position() + HepPoint3D( (px*sinps - py*dcosps) / a,
					   (py*sinps + px*dcosps) / a,
					    pz * (s / pmag) ) );

   // Move error matrix and return status
   KTMoveControl::MoveStatus errorStatus = moveErrorMatrix(saveTrack);

   return errorStatus;
}


KTMoveControl::MoveStatus
KTKinematicFixedBz::moveToXYPlane(
                                const HepPoint3D&  aPoint,
				const HepVector3D& aNormal,
				const KTMoveControl::MoveDirection aDirection)
{
   // This function translates a KinematicData object to the specified
   // plane in a fixed solenoidal B field. The normal of the plane
   // is assumed to be in the xy plane.
   //
   // The transport equations are shown below:
   //
   //  px' = px*cos(rho*s) - py*sin(rho*s)
   //  py' = px*sin(rho*s) + py*cos(rho*s)
   //  pz' = pz
   //   E' = E
   //   x' = x + (px/a) * sin(rho*s) - (py/a) * (1 - cos(rho*s) )
   //   y' = y + (py/a) * sin(rho*s) + (px/a) * (1 - cos(rho*s) )
   //   z' = z + (pz/p) * s
   //
   // where
   //  s   = arc length
   //  a   = -0.0299792458 * Bfield (Kgauss) * charge
   //  rho = a / p
   //
   //   The equation of the plane can be written
   //
   //        p_eta * sin(ps) - pt_eta * (1 - cos(ps)) + a*dx_eta = 0
   //
   //   where the rotated quantities are defined as
   //
   //   p_eta   = px0*cosb + py0*sinb
   //   pt_eta  = py0*cosb - px0*sinb
   //   dx_eta  = (x0-xp)*cosb + (y0-yp)*sinb
   //
   //   To solve, we use the auxiliary quantity w = (2/rho)*sin(ps/2)
   //
   //         sin(ps)     = rho*w*sqrt(1 - rho^2*w^2/4)
   //         1 - cos(ps) = rho^2*w^2 / 2
   //
   //    0.25*a^2*(p_eta^2+pt_eta^2)*W^4 - p^2*(p_eta^2 + a*dx_eta*pt_eta)*W^2
   //                                    + p^4*dx_eta^2 = 0
   //
   //   The solution is shown below, after making appropriate substitutions
   //   and making the solutions numerically robust.
   //
   //   Near solution
   //   1 - cos(ps) = (a*del_eta)^2 / (B + p_eta*R)
   //       sin(ps) = -a*del_eta * (p_eta + R) / (B + p_eta*R)
   //
   //   Far solution
   //   1 - cos(ps) = (B + p_eta*R) / (p_eta^2 + pt_eta^2)
   //       sin(ps) = (p_eta*(pt_eta - a*del_eta) + pt_eta*R)
   //                                              / (p_eta^2 + pt_eta^2)
   //
   //       R = sqrt(p_eta^2 + 2.*a*del_eta*pt_eta - a^2*del_eta^2)
   //       B = p_eta^2 + a*del_eta*pt_eta


   // Curvature constant
   double a = kBFieldConstant * BFieldMagnitude() * charge();

   // Check if we should use the zero B field version
   if(a == 0.) {
      return moveToPlaneBZero( aPoint, aNormal, aDirection);
   }

   KTKinematicData saveTrack(*this);   // Save current values in order
                                       // to move error matrix later

   const HepVector3D pmom = momentum();
   const double pmag = pmom.mag();

   // Quit if no p
   if(pmag == 0.) {
      return KTMoveControl::kMoveNoIntersection;
   }

   // Calculate dot products
   const HepPoint3D delta = position() - aPoint;
   const double deltaDotEta = delta.dot(aNormal);
   const double pDotEta = momentum().dot(aNormal);
   const double ptDotEta = py()*aNormal.x() - px()*aNormal.y();

   const double rho = a / pmag;

   const double temp = a * deltaDotEta;
   const double B = pDotEta*pDotEta + temp * ptDotEta;
   double disc = pDotEta*pDotEta + 2.*temp*ptDotEta - temp*temp;

   if(disc < 0.) {
      return KTMoveControl::kMoveNoIntersection;
   }

   disc = sqrt(disc);

   // "Near" and "far" solutions
   double cosps1, sinps1, cosps2, sinps2;
   const double ptsqEta = pDotEta*pDotEta + ptDotEta*ptDotEta;
   if(pDotEta >= 0.) {
      cosps1 = 1. - temp*temp / (B + pDotEta*disc);
      sinps1 = -temp * (pDotEta + disc) / (B + pDotEta*disc);
      cosps2 = 1. - (B + pDotEta*disc) / ptsqEta;
      sinps2 = (pDotEta * (ptDotEta - temp) + ptDotEta*disc) / ptsqEta;
   }
   else {
      cosps1 = 1. - temp*temp / (B - pDotEta*disc);
      sinps1 = -temp * (pDotEta - disc) / (B - pDotEta*disc);
      cosps2 = 1. - (B - pDotEta*disc) / ptsqEta;
      sinps2 = (pDotEta * (ptDotEta - temp) - ptDotEta*disc) / ptsqEta;
   }
   double s1 = atan2(sinps1, cosps1) / rho;
   double s2 = atan2(sinps2, cosps2) / rho;

   int sDirection = KTMoveControl::directionInteger(aDirection);
   // If track is already pinned to the plane, put it there exactly
   if(fabs(s1) < KTMoveControl::stickyDistance()) {
      s1 = 0.;
      sinps1 = 0.;
      cosps1 = 1.;
   }
   if(fabs(s2) < KTMoveControl::stickyDistance()) {
      s2 = 0.;
      sinps2 = 0.;
      cosps2 = 1.;
   }

   double s;    // Arc length we are trying to compute

   // Two intersections are possible. Choose the one first encountered in
   // the direction of motion.

   // Choose smallest positive arc length for forward projection
   // Choose smallest negative arc length for backward projection

   const double circum = (2.*M_PI) / fabs(rho); // Circumference incl. z motion

   double sinps;  // sin(rho*s)
   double cosps;  // cos(rho*s)

   // Move forward
   if(aDirection == KTMoveControl::kDirectionForward) {
      if(s1 < 0.) {s1 = s1 + circum;}
      if(s2 < 0.) {s2 = s2 + circum;}
      if(s1 == 0. && s2 == 0.) {
	 return KTMoveControl::kMoveNoIntersection;
      }
      if((s1 < s2 && s1 != 0.) || s2 == 0.) {
	 s = s1;
	 sinps = sinps1;
	 cosps = cosps1;
      }
      else {
	 s = s2;
	 sinps = sinps2;
	 cosps = cosps2;
      }
   }

   // Move backward
   else if(aDirection == KTMoveControl::kDirectionBackward) {
      if(s1 > 0.) {s1 = s1 - circum;}
      if(s2 > 0.) {s2 = s2 - circum;}
      if(s1 == 0. && s2 == 0.) {
	 return KTMoveControl::kMoveNoIntersection;
      }
      if((-s1 < -s2 && s1 != 0.) || s2 == 0.) {
	 s = s1;
	 sinps = sinps1;
	 cosps = cosps1;
      }
      else {
	 s = s2;
	 sinps = sinps2;
	 cosps = cosps2;
      }
   }

   // Move either direction
   else {
      if(fabs(s1) <= fabs(s2)) {
	 s = s1;
	 sinps = sinps1;
	 cosps = cosps1;
      }
      else {
	 s = s2;
	 sinps = sinps2;
	 cosps = cosps2;
      }
   }

   // Transport momentum
   setMomentum(HepVector3D(px()*cosps - py()*sinps,
			     py()*cosps + px()*sinps,
			     pz()) );
      
   // Transport position (be careful to use initial momentum)
   double dcosps = 1. - cosps;
   setPosition( position()
      + HepPoint3D( (saveTrack.px()*sinps - saveTrack.py()*dcosps) / a,
		      (saveTrack.py()*sinps + saveTrack.px()*dcosps) / a,
		       saveTrack.pz() * (s / pmag) ) );

   // Move error matrix and return status
   KTMoveControl::MoveStatus errorStatus = moveErrorMatrix(saveTrack);

   return errorStatus;
}


KTMoveControl::MoveStatus
KTKinematicFixedBz::moveToPlane( const HepPoint3D&  aPoint,
				 const HepVector3D& aNormal,
				 const KTMoveControl::MoveDirection aDirection)
{
   // Curvature constant
   double a = kBFieldConstant * BFieldMagnitude() * charge();

   // Check if we should use the zero B field version
   if(a == 0.) {
      return moveToPlaneBZero( aPoint, aNormal, aDirection);
   }

   // Check if we can use the exact case where the normal is in the XY plane
   if(aNormal.z() == 0.) {
      return moveToXYPlane( aPoint, aNormal, aDirection);
   }

   // Check if we can use the exact case where the normal is along the z axis
   if(aNormal.x()==0. && aNormal.y()==0.) {
      return moveToZPosition( aPoint.z(), aDirection);
   }

   // Otherwise do the move iteratively
   return KTKinematicMoveable::moveToPlaneIterate( aPoint, aNormal );

}


KTMoveControl::MoveStatus
KTKinematicFixedBz::moveToRadius(const ValueType aRadius,
				 const KTMoveControl::MoveDirection aDirection)
{
   // This function translates a KinematicData object to the specified
   // radius in a fixed solenoidal B field. The axis of the cylinder
   // is assumed to be along the z axis and the center is at the origin.

   // Pass the call to the routine which handles cylinders with the
   // axis along the z axis.
   HepPoint3D center(0., 0., 0.);
   return moveToXYCylinder(center, aRadius, aDirection);
}


KTMoveControl::MoveStatus
KTKinematicFixedBz::moveToXYCylinder( const HepPoint3D& aCenter,
				      const ValueType aRadius,
				      const KTMoveControl::MoveDirection aDirection)
{
   // This function translates a KinematicData object to the specified
   // cylinder in a fixed solenoidal B field. The axis of the cylinder
   // is assumed to be along the z axis.
   //
   // The transport equations are shown below:
   //
   //  px' = px*cos(rho*s) - py*sin(rho*s)
   //  py' = px*sin(rho*s) + py*cos(rho*s)
   //  pz' = pz
   //   E' = E
   //   x' = x + (px/a) * sin(rho*s) - (py/a) * (1 - cos(rho*s) )
   //   y' = y + (py/a) * sin(rho*s) + (px/a) * (1 - cos(rho*s) )
   //   z' = z + (pz/p) * s
   //
   // where
   //  s   = arc length
   //  a   = -0.0299792458 * Bfield (Kgauss) * charge
   //  rho = a / p
   //
   //   Squaring and adding the x-y equations yields
   //
   //   0 = -delta + (2/rho)*A2*sin(rho*s) + (2/rho^2)*A1*(1-cos(rho*s))
   //
   //  where delta = radius^2 - dx^2 - dy^2
   //           A1 = [pt^2 + a * (px*dy - py*dx) ] / p^2
   //           A2 = (px*x + py*y) / p
   //           dx = x0 - center_x
   //           dy = y0 - center_y
   //
   //  To solve this we introduce the variable w = (2/rho) * sin(rho*s/2),
   //  defined so that it is meaningful even when rho is zero. We have then
   //
   //            sin(rho*s) = w * rho * sqrt(1 - (w*rho)^2/4)
   //        1 - cos(rho*s) = (w*rho)^2 / 2
   //
   //  the equation we must solve is
   //
   //    0 = (A1^2 + (A2*rho)^2)*w^4 - 2*(A1*delta + 2*A2^2)*w^2 + delta^2
   //
   //  the solution for w^2 is
   //
   //    w^2 = [A1*delta + 2*A2^2 +- A2*disc] / [A1^2 + (A2*rho)^2]
   //
   //  where disc = sqrt(4*A1*delta + 4*A2^2 - (rho*delta)^2)
   //
   //  w^2 is essentially cos(rho*ps). To get a unique solution we need to
   //  find sin(rho*s). This can be found from the second equation above:
   //
   // sin(rho*s) = rho*[delta - A1*w^2] / (2*A2)
   //            = rho*[delta*A2*rho^2 - 2*A2*A1 -+ A1*disc]/2(A1^2+rho^2*A2^2)
   //
   //  where the signs are correlated with the solution for w^2. If there is
   //  more than one solution, the one closest to the current point along the
   //  forward/backward direction should be chosen.

   // Curvature constant
   const double a = kBFieldConstant * BFieldMagnitude() * charge();

   // Check if we should use the zero B field version
   if(a == 0.) {
      HepVector3D eta(0., 0., 1.);
      return moveToCylinderBZero( aCenter, eta, aRadius, aDirection);
   }

   KTKinematicData saveTrack(*this);   // Save current values in order
                                       // to move error matrix later

   const double px = momentum().x();
   const double py = momentum().y();
   const double pz = momentum().z();

   const double  delx = position().x() - aCenter.x();
   const double  dely = position().y() - aCenter.y();

   const HepVector3D pmom = momentum();
   const double pmag = pmom.mag();
   const double pperp = sqrt(px*px + py*py);

   // Quit if no p or pt
   if(pmag == 0. || pperp == 0.) {
      return KTMoveControl::kMoveNoIntersection;
   }

   const double A1 = (pperp*pperp + a * (px*dely - py*delx) ) / (pmag*pmag);
   const double A2 = (px*delx + py*dely) / pmag;
   const double delta_R = delx*delx + dely*dely - aRadius*aRadius;

   double s;    // Arc length we are trying to compute

   // Two intersections are possible. Choose the one first encountered in
   // the direction of motion.

   // Choose smallest positive arc length for forward projection
   // Choose smallest negative arc length for backward projection

   const double rho = a / pmag;

   double disc = 4. * (A2*A2 - A1*delta_R) - (rho*delta_R)*(rho*delta_R);
   if(disc < 0.) {
      return KTMoveControl::kMoveNoIntersection;
   }
   disc = sqrt(disc);

   const double denom = 1. / ( A1*A1 + (rho*A2)*(rho*A2) );

   const double circum = (2.*M_PI) / fabs(rho); // Circumference incl. z motion

   // Find the two intersection arc lengths s1 and s2
   const double ww1 = (2.*A2*A2 - A1*delta_R - A2*disc) * denom;
   const double cosps1 = 1. - 0.5 * rho*rho * ww1;
   const double sinps1 = -0.5*rho * (2.*A1*A2 + rho*rho*A2*delta_R
				  - A1*disc) * denom;
   double s1 = atan2(sinps1, cosps1) / rho;

   const double ww2 = ww1 + 2.*A2*disc*denom;
   const double cosps2 = 1. - 0.5 * rho*rho * ww2;
   const double sinps2 =  sinps1 - rho*A1*disc*denom;
   double s2 = atan2(sinps2, cosps2) / rho;

   int sDirection = KTMoveControl::directionInteger(aDirection);

   // If track is already pinned to the surface, put it there exactly
   if(fabs(s1) < KTMoveControl::stickyDistance()) {
      s1 = 0.;
   }
   if(fabs(s2) < KTMoveControl::stickyDistance()) {
      s2 = 0.;
   }

   double sinps;  // sin(rho*s)
   double cosps;  // cos(rho*s)

   // Move forward
   if(aDirection == KTMoveControl::kDirectionForward) {
      if(s1 < 0.) {s1 = s1 + circum;}
      if(s2 < 0.) {s2 = s2 + circum;}
      if(s1 == 0. && s2 == 0.) {
	 return KTMoveControl::kMoveNoIntersection;
      }
      if((s1 < s2 && s1 != 0.) || s2 == 0.) {
	 s = s1;
	 sinps = sinps1;
	 cosps = cosps1;
      }
      else {
	 s = s2;
	 sinps = sinps2;
	 cosps = cosps2;
      }
   }

   // Move backward
   else if(aDirection == KTMoveControl::kDirectionBackward) {
      if(s1 > 0.) {s1 = s1 - circum;}
      if(s2 > 0.) {s2 = s2 - circum;}
      if(s1 == 0. && s2 == 0.) {
	 return KTMoveControl::kMoveNoIntersection;
      }
      if((-s1 < -s2 && s1 != 0.) || s2 == 0.) {
	 s = s1;
	 sinps = sinps1;
	 cosps = cosps1;
      }
      else {
	 s = s2;
	 sinps = sinps2;
	 cosps = cosps2;
      }
   }

   // Move either direction
   else {
      if(fabs(s1) <= fabs(s2)) {
	 s = s1;
	 sinps = sinps1;
	 cosps = cosps1;
      }
      else {
	 s = s2;
	 sinps = sinps2;
	 cosps = cosps2;
      }
   }

   // Transport momentum
   setMomentum(HepVector3D(px*cosps - py*sinps,
			     py*cosps + px*sinps,
			     pz) );
      
   // Transport position
   double dcosps = 1. - cosps;
   setPosition( position() + HepPoint3D( (px*sinps - py*dcosps) / a,
					   (py*sinps + px*dcosps) / a,
					    pz * (s / pmag) ) );

   // Move error matrix and return status
   KTMoveControl::MoveStatus errorStatus = moveErrorMatrix(saveTrack);

   return errorStatus;
}


KTMoveControl::MoveStatus
KTKinematicFixedBz::moveToCylinder( const HepPoint3D& aCenter,
				    const HepVector3D& aDirCosine,
				    const ValueType aRadius,
		        const KTMoveControl::MoveDirection aDirection)
{
   // This function translates a KinematicData object to the specified
   // cylinder in a fixed solenoidal B field.

   // Curvature constant
   const double a = kBFieldConstant * BFieldMagnitude() * charge();

   // Check if we should use the zero B field version
   if(a == 0.) {
      return moveToCylinderBZero( aCenter, aDirCosine, aRadius, aDirection);
   }

   // Check if the cylinder axis is along the z axis
   if(aDirCosine.x() == 0. && aDirCosine.y() == 0.) {
      return moveToXYCylinder(aCenter, aRadius, aDirection);
   }

   // Otherwise do the move iteratively
   return KTKinematicMoveable::moveToCylinderIterate( aCenter, aDirCosine,
						      aRadius, aDirection );

}

KTMoveControl::MoveStatus
KTKinematicFixedBz::moveErrorMatrix(const KTKinematicData& aOldTrack)
{
   // Move the error matrix from one point to another, assuming that
   // the arc length is fixed.
   // The kinematic parameters before the move are in aOldTrack.

   // Quit if no error matrix
   if( hasNullErrorMatrix() ) {return KTMoveControl::kMoveOK;}

   // Calculate derivative matrix and apply similarity transformation
   HepMatrix R(7,7,0);
   KTMoveControl::MoveStatus matrixReturn
                          = calculateDerivativeMatrix(aOldTrack, R);
   if( KTMoveControl::kMoveCovarianceMoveFailed != matrixReturn ){

// Calculate covariance matrix of new parameters: V_c' = R * V_k * R(t).
      setErrorMatrix ( aOldTrack.errorMatrix().similarity( R ) );

   }
   return matrixReturn;
}


//
// const member functions
//

KTMoveControl::MoveStatus
KTKinematicFixedBz::calculateDerivativeMatrix(
                                   const KTKinematicData& aOldTrack,
				   HepMatrix& aDerivativeMatrix) const
{
   // Calculate the derivative matrix d(new) / d(old) of the kinematic
   // parameters.

   double a = kBFieldConstant * BFieldMagnitude() * charge();

   // Handle zero B field in a special routine
   if(0. == a) {
      return calculateDerivativeMatrixBZero(aOldTrack, aDerivativeMatrix);
   }

   // Make an alias for the derivative matrix to keep code readable
   HepMatrix& R = aDerivativeMatrix;

   double pperp = aOldTrack.pperp();
   double delta_x = x() - aOldTrack.x();
   double delta_y = y() - aOldTrack.y();

   double px_new = px();
   double py_new = py();
   double px_old = aOldTrack.px();
   double py_old = aOldTrack.py();
   double pz_old = aOldTrack.pz();
   double ptot = pmag();

   double factor1; // mkl fix for paul's screwup

   double s3d;  // Arc length in 3-D

   double cosps = 1. - a*(delta_y*px_old - delta_x*py_old) / (pperp*pperp);
   double sinps =      a*(delta_x*px_old + delta_y*py_old) / (pperp*pperp);
   double ainv = 1. / a;

   s3d = atan2(sinps, cosps) * ptot * ainv;
   factor1 = s3d / (ptot*ptot*ptot);
   double factor2 = a * factor1;

   R(kPx,kPx) =  cosps + px_old * py_new * factor2;
   R(kPx,kPy) = -sinps + py_old * py_new * factor2;
   R(kPx,kPz) =          pz_old * py_new * factor2;
      
   R(kPy,kPx) =  sinps - px_old * px_new * factor2;
   R(kPy,kPy) =  cosps - py_old * px_new * factor2;
   R(kPy,kPz) =        - pz_old * px_new * factor2;

   R(kX,kPx) = ainv *  R(kPy,kPx);
   R(kX,kPy) = ainv * (R(kPy,kPy) - 1.);
   R(kX,kPz) = ainv *  R(kPy,kPz);
   R(kX,kX) = 1.;

   R(kY,kPx) = -ainv * (R(kPx,kPx) - 1.);
   R(kY,kPy) = -ainv *  R(kPx,kPy);
   R(kY,kPz) = -ainv *  R(kPx,kPz);
   R(kY,kY) = 1.;

   R(kPz,kPz) = 1.;

   //
   //   Bug fix: There should be no cross terms between Energy and
   //   Momentum. The momentum is constant for propagation through an
   //   empty region of space...
   //
   // R(kEnergy,kPx) = px_old / energy();
   // R(kEnergy,kPy) = py_old / energy();
   // R(kEnergy,kPz) = pz_old / energy();
   //
   //   The above comes from the equation: E = (p^2 + m^2)^(1/2). Why?
   //   This was probably done in anticipation of switching from E to
   //   M in the parameterization of the kinematics and hence the
   //   error matrix. The rest of the Jacobian seems to be the same as
   //   it is in KWFit.
   //
   R(kEnergy,kEnergy) = 1.;

   R(kZ,kPx) = -pz_old * px_old * factor1;
   R(kZ,kPy) = -pz_old * py_old * factor1;
   R(kZ,kPz) = (ptot*ptot - pz_old*pz_old) * factor1;

   R(kZ,kZ) = 1.;

   return KTMoveControl::kMoveOK;

}


HepPoint3D
KTKinematicFixedBz::pointOfClosestApproach( const HepPoint3D& ) const
{

   // Return point of closest approach (in 3-D) of the track to a point.

   assert(false);   // Doesn't yet exist
   return HepPoint3D(0., 0., 0.);
}


HepPoint3D
KTKinematicFixedBz::pointOfClosestApproachBend(
					  const HepPoint3D& aPoint) const
{

   // Return point of closest approach of the track to a point
   // in the bend plane.

   // The transport equations are shown below:
   //
   //  px' = px*cos(rho*s) - py*sin(rho*s)
   //  py' = px*sin(rho*s) + py*cos(rho*s)
   //  pz' = pz
   //   E' = E
   //   x' = x + (px/a) * sin(rho*s) - (py/a) * (1 - cos(rho*s) )
   //   y' = y + (py/a) * sin(rho*s) + (px/a) * (1 - cos(rho*s) )
   //   z' = z + (pz/p) * s
   //
   // where
   //  s   = arc length
   //  a   = -0.0299792458 * Bfield * charge
   //  rho = a / p
   //
   //  Let dx = x0 - point_x
   //      dy = y0 - point_y
   //
   //     A1 =  ( w_in(8)**2 - a*(delx*py - dely*px) ) / p**2
   //     A2 = (delx*px + dely*py) / p
   //
   //     alpha =  1. / sqrt(A1**2 + (rho*A2)**2)
   //     cosps = A1 * alpha
   //     sinps = -(rho*A2) * alpha
   //
   //  Solving for the arc length s then gives
   //
   //     s = arctan(sinps/cosps) / rho     (B field non-zero)
   //       =  -p * (px*dx + py*dy) / pt^2  (B field zero)


   // If no pt, motion is along z and XY position is unchanged
   if(pperp() == 0.) {
      return HepPoint3D( x(), y(), aPoint.z() );
   }

   // Curvature constant
   double a = kBFieldConstant * BFieldMagnitude() * charge();

   const double delx = x() - aPoint.x();
   const double dely = y() - aPoint.y();

   double s;
   const double p = pmag();

   // Zero B field
   if(a == 0.) {
      s = -pmag() * ( px()*delx + py()*dely ) / pperp2();
      return HepPoint3D ( x() + (px()/p)*s,
			    y() + (py()/p)*s,
			    z() + (pz()/p)*s );
   }

   // Non-zero B field
   else {
      const double rho = a / p;
      const double A1 =  ( pperp2() - a*(delx*py() - dely*px()) ) / (p*p);
      const double A2 = (delx*px() + dely*py()) / p;

      const double alpha =  1. / sqrt(A1*A1 + (rho*A2)*(rho*A2) );
      const double cosps = A1 * alpha;
      const double sinps = -(rho*A2) * alpha;
      const double dcosps = 1. - cosps;

      s = atan2(sinps, cosps) / rho;

      return HepPoint3D( x() + (px()/a)*sinps - (py()/a)*dcosps,
			   y() + (py()/a)*sinps + (px()/a)*dcosps,
			   z() + (pz()/p)*s );
   }
}


KTKinematicData::ValueType
KTKinematicFixedBz::distanceOfClosestApproach(
                                           const HepPoint3D& ) const
{

   // Return distance of closest approach (in 3-D) of the track to
   // a point.

   assert(false);   // Doesn't yet exist
   return 0.;
}


KTKinematicData::ValueType
KTKinematicFixedBz::distanceOfClosestApproachBend(
                                           const HepPoint3D& aPoint) const
{

   // Return distance of closest approach of the track to a point in
   // the bend plane.

   // The transport equations are shown below:
   //
   //  px' = px*cos(rho*s) - py*sin(rho*s)
   //  py' = px*sin(rho*s) + py*cos(rho*s)
   //  pz' = pz
   //   E' = E
   //   x' = x + (px/a) * sin(rho*s) - (py/a) * (1 - cos(rho*s) )
   //   y' = y + (py/a) * sin(rho*s) + (px/a) * (1 - cos(rho*s) )
   //   z' = z + (pz/p) * s
   //
   // where
   //  s   = arc length
   //  a   = -0.0299792458 * Bfield * charge
   //  rho = a / p
   //
   //  Let dx = x0 - point_x
   //      dy = y0 - point_y
   //
   //     cross = (px*dy - py*dx) / pt
   //
   //  For zero B field
   //     distance = -cross
   //
   //  For non-zero B field
   //     distance = [ -2*cross + rho * (dx^2+dy^2) ] / (T + 1)
   //
   //     T = sqrt( 1 - 2*rho*cross + rho^2*(dx*dx + dy*dy) )

   const double delx = x() - aPoint.x();
   const double dely = y() - aPoint.y();
   const double pt = pperp();

   // If no pt, motion is along z and XY position is unchanged
   if(pt == 0.) {
      return sqrt( delx*delx + dely+dely );
   }

   // Curvature constant
   double a = kBFieldConstant * BFieldMagnitude() * charge();
   const double cross = ( delx*py() - dely*px() ) / pt;

   // Special version for zero B field
   if(a == 0.) {
      return -cross;
   }

   // Non-zero B field
   const double rho = a / pmag();
   const double temp = delx*delx + dely*dely;
   const double disc = sqrt(1. - 2.*rho*cross + rho*rho*temp );
   return (-2.*cross + rho*temp) / (disc + 1.);
}


HepPoint3D
KTKinematicFixedBz::pointOfClosestApproach(
                                   const HepPoint3D& aPoint,
				   const HepVector3D& aDirCosine) const
{

   // Return point of closest approach (in 3-D) of the track to a line.

   // If line is along z direction, call the XY point version
   if(aDirCosine == 1.) {
      return pointOfClosestApproachBend(aPoint);
   }

   assert(false);   // Doesn't yet exist
   return HepPoint3D(0., 0., 0.);
}


KTKinematicData::ValueType
KTKinematicFixedBz::distanceOfClosestApproach(
                                        const HepPoint3D&  aPoint,
					const HepVector3D& aDirCosine) const
{

   // Return distance of closest approach (in 3-D) of the track to
   // a line.

   // If line is along z direction, call the XY point version
   if(aDirCosine == 1.) {
      return distanceOfClosestApproachBend(aPoint);
   }

   assert(false);   // Doesn't yet exist
   return 0.;
}


//
// static member functions
//
