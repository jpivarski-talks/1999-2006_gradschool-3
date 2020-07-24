// -*- C++ -*-
//
// Package:     KinematicTrajectory
// Module:      KTKinematicMoveable
// 
// Description: Abstract base class for moveable KinematicData objects
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Paul Avery
// Created:     Wed Oct 15 13:45:40 EDT 1997
// $Id: KTKinematicMoveable.cc,v 1.9 2000/06/15 20:16:48 pappas Exp $
//
// Revision history
//
// $Log: KTKinematicMoveable.cc,v $
// Revision 1.9  2000/06/15 20:16:48  pappas
// Double checked and committed Paul Avery's modifications eliminating
// references to Toolbox classes. Should be completely transparent since
// Toolbox is just typedefs.
//
// Revision 1.8  1999/10/08 22:18:43  marsh
// Changed TBThreeVector to TBThreePoint.
//
// Revision 1.7  1998/12/09 22:09:10  avery
// Added notion of a "sticky" distance when moving. Fixed some small errors
// in moving KTHelix and KTKinematicData objects over large arcs. Implemented
// some missing set functions.
//
// Revision 1.6  1998/11/09 15:05:50  avery
// Replace abs(...) by fabs(...). Fix logic in KTMoveControl::chooseArcLength.
//
// Revision 1.5  1998/07/09 21:32:49  avery
// Bug fixes, finished implementation of move routines, updated documentation.
//
// Revision 1.4  1998/06/01 18:24:11  avery
// Fixed problem with copy constructor
//
// Revision 1.3  1998/05/27 17:12:46  avery
// Improved documentation
//
// Revision 1.2  1998/05/24 21:30:33  avery
// Add move, PCA routines. Some cleanup
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

// user include files
//#include "Experiment/report.h"
#include "KinematicTrajectory/KTKinematicMoveable.h"
#include "CLHEP/Matrix/Matrix.h"

// STL classes

//
// constants, enums and typedefs
//

typedef KTMoveControl::MoveDirection MoveDirection;
typedef KTMoveControl::MoveStatus    MoveStatus;

const double kBFieldConstant = -0.0299792458;

static const char* const kFacilityString =
                               "KinematicTrajectory.KTKinematicMoveable" ;

//
// static data member definitions
//

//
// constructors and destructor
//
KTKinematicMoveable::KTKinematicMoveable()
{
}

KTKinematicMoveable::KTKinematicMoveable(
				      const KTKinematicData& aKinematicData) :
   KTKinematicData(aKinematicData)
{
}

KTKinematicMoveable::KTKinematicMoveable(
				      const KTKinematicData& aKinematicData,
				      const DABoolean aCopyErrorMatrix) :
   KTKinematicData(aKinematicData, aCopyErrorMatrix)
{
}

KTKinematicMoveable::KTKinematicMoveable( const HepVector3D& aMomentum ,
					  const HepPoint3D& aPosition,
					  const ValueType aMass,
					  const ValueType aCharge) :
   KTKinematicData(aMomentum, aPosition, aMass, aCharge)
{
}

KTKinematicMoveable::KTKinematicMoveable(const HepVector3D& aMomentum ,
					 const HepPoint3D& aPosition,
					 const ValueType aMass,
					 const ValueType aCharge,
					 const HepSymMatrix& aErrorMatrix ) :
  KTKinematicData(aMomentum, aPosition, aMass, aCharge, aErrorMatrix)
{
}

KTKinematicMoveable::KTKinematicMoveable(
                         const KTHelix& aHelix,
			 const ValueType aMass,
			 const HepVector3D& aMagneticField,
			 const DABoolean aCopyErrorMatrix) :
   KTKinematicData(aHelix, aMass, aMagneticField, aCopyErrorMatrix)
{
}

KTKinematicMoveable::KTKinematicMoveable(
                         const KTHelix& aHelix,
			 const ValueType aMass,
			 const ValueType aBFieldMagnitude,
			 const DABoolean aCopyErrorMatrix) :
   KTKinematicData(aHelix, aMass, aBFieldMagnitude, aCopyErrorMatrix)
{
}

// KTKinematicMoveable::KTKinematicMoveable( const KTKinematicMoveable& )
// {
// }

//KTKinematicMoveable::~KTKinematicMoveable()
//{
//}

//
// assignment operators
//
// const KTKinematicMoveable& KTKinematicMoveable::operator=(
//                                           const KTKinematicMoveable& )
// {
// }

//
// member functions
//

KTMoveControl::MoveStatus
KTKinematicMoveable::moveByArcLengthBZero( const ValueType aArcLength)
{
   // This function translates a KinematicData object by the specified
   // arc length in a zero magnetic field.
   //
   // The transport equations are shown below:
   //
   //  px' = px
   //  py' = py
   //  pz' = pz
   //   E' = E
   //   x' = x + (px/p) * s
   //   y' = y + (py/p) * s
   //   z' = z + (pz/p) * s
   //
   // where
   //  s   = arc length
   //

   KTKinematicData saveTrack(*this);   // Save current values in order
                                       // to move error matrix later

   // Transport position
   const double ptot = pmag();
   if(0. == ptot) {
      return KTMoveControl::kMoveNoIntersection;
   }

   setPosition( position() + (aArcLength / ptot) * momentum());


   // Move error matrix and return status
   MoveStatus errorStatus = moveErrorMatrixBZero(saveTrack);

   return errorStatus;
}


KTMoveControl::MoveStatus
KTKinematicMoveable::moveToPointBZero( 
                                const HepPoint3D& aPoint,
				const MoveDirection aDirection)
{
   // This function translates a KinematicData object to the point
   // closest (in 3D space) to the specified point in a zero magnetic
   // field.
   //
   // The transport equations are shown below:
   //
   //  px' = px
   //  py' = py
   //  pz' = pz
   //   E' = E
   //   x' = x + (px/p) * s
   //   y' = y + (py/p) * s
   //   z' = z + (pz/p) * s
   //
   // where
   //  s   = arc length
   //

   KTKinematicData saveTrack(*this);   // Save current values in order
                                       // to move error matrix later

   const double ptot = pmag();

   // Quit if no momentum
   if(0. == ptot) {
     return KTMoveControl::kMoveNoIntersection;
   }

   const HepPoint3D delta = position() - aPoint;

   // Arc length we are trying to compute
   double s = -(delta.dot( momentum() )) / ptot;

   int sDirection = KTMoveControl::directionInteger(aDirection);

  // If track is already pinned to the point, there is no other solution
  if(fabs(s) < KTMoveControl::stickyDistance()) {
     s = 0.;
  }
   if(s <= 0. && KTMoveControl::kDirectionForward == aDirection) {
      return KTMoveControl::kMoveNoIntersection;
   }
   else if(s >= 0. && KTMoveControl::kDirectionBackward == aDirection) {
      return KTMoveControl::kMoveNoIntersection;
   }

   // Transport position
   const double sOverP = s / ptot;
   setPosition( position() + sOverP * momentum());

   // Move error matrix and return status
   MoveStatus errorStatus = moveErrorMatrixBZero(saveTrack);

   return errorStatus;
}


KTMoveControl::MoveStatus
KTKinematicMoveable::moveToPointBendBZero( 
                                const HepPoint3D& aPoint,
				const MoveDirection aDirection)
{
   // This function translates a KinematicData object to the point
   // closest (in the xy plane) to the specified (x,y) point in a
   // zero magnetic field.
   //
   // The transport equations are shown below:
   //
   //  px' = px
   //  py' = py
   //  pz' = pz
   //   E' = E
   //   x' = x + (px/p) * s
   //   y' = y + (py/p) * s
   //   z' = z + (pz/p) * s
   //
   // where
   //  s   = arc length
   //

   KTKinematicData saveTrack(*this);   // Save current values in order
                                       // to move error matrix later

   const double ptot = pmag();

   const double delta_x = x() - aPoint.x();
   const double delta_y = y() - aPoint.y();

   const double pperp = sqrt(px()*px() + py()*py());

   // If no pperp, then we are already at PCA and we can quit
   if(0. == pperp) {
      return KTMoveControl::kMoveOK;
   }

   // Calculate arc length
   double sOverP = -(delta_x*px() + delta_y*py()) / (pperp*pperp);
   double s = sOverP * ptot;

   int sDirection = KTMoveControl::directionInteger(aDirection);

  // If track is already pinned to the point, there is no other solution
  if(fabs(s) < KTMoveControl::stickyDistance()) {
     s = 0.;
     sOverP = 0.;
  }
   if(s <= 0. && KTMoveControl::kDirectionForward == aDirection) {
      return KTMoveControl::kMoveNoIntersection;
   }
   else if(s >= 0. && KTMoveControl::kDirectionBackward == aDirection) {
      return KTMoveControl::kMoveNoIntersection;
   }

   // Transport position
   setPosition( position() + sOverP * momentum() );

   // Move error matrix and return status
   MoveStatus errorStatus = moveErrorMatrixBZero(saveTrack);

   return errorStatus;
}


KTMoveControl::MoveStatus
KTKinematicMoveable::moveToLineBZero( 
                                const HepPoint3D& aPoint,
				const HepVector3D& aDirCosine,
				const MoveDirection aDirection)
{
   // This function translates a KinematicData object to the
   // specified line in a zero magnetic field.
   //
   // The transport equations are shown below:
   //
   //  px' = px
   //  py' = py
   //  pz' = pz
   //   E' = E
   //   x' = x + (px/p) * s
   //   y' = y + (py/p) * s
   //   z' = z + (pz/p) * s
   //
   // where
   //  s   = arc length
   //
   // The solution is
   //
   //    s = -p * ([delta_v,p_v] - [delta_v,eta_v]*[p_v,eta_v])
   //                     / (p^2 - [p_v,eta_v]^2)

   KTKinematicData saveTrack(*this);   // Save current values in order
                                       // to move error matrix later

   const double ptot = pmag();

   //Quit if no momentum
   if(0. == ptot) {
     return KTMoveControl::kMoveNoIntersection;
   }

   // If track is parallel to line, then we are already at the PCA
   if(0 == (momentum().cross(aDirCosine)).mag() ) {
      return KTMoveControl::kMoveOK;
   }

   const HepPoint3D delta = position() - aPoint;
   const double delta_eta = delta.dot(aDirCosine);
   const double p_eta = momentum().dot(aDirCosine);
   const double deltaDotP = momentum().dot(delta);

   // Arc length we are trying to compute
   double s = -ptot * (deltaDotP - delta_eta*p_eta)
                      / (ptot*ptot - p_eta*p_eta);

   int sDirection = KTMoveControl::directionInteger(aDirection);

  // If track is already pinned to the line, there is no other solution
  if(fabs(s) < KTMoveControl::stickyDistance()) {
     s = 0.;
  }
   // Check that point is in the correct direction
   if(s <= 0. && KTMoveControl::kDirectionForward == aDirection) {
      return KTMoveControl::kMoveNoIntersection;
   }
   else if(s >= 0. && KTMoveControl::kDirectionBackward == aDirection) {
      return KTMoveControl::kMoveNoIntersection;
   }

   // Transport position
   const double sOverP = s / ptot;
   setPosition( position() + sOverP * momentum() );

   // Move error matrix and return status
   MoveStatus errorStatus = moveErrorMatrixBZero(saveTrack);

   return errorStatus;
}


KTMoveControl::MoveStatus
KTKinematicMoveable::moveToPlaneBZero(
                                const HepPoint3D& aPoint,
				const HepVector3D& aNormal,
				const MoveDirection aDirection)
{
   // This function translates a KinematicData object to the specified
   // plane in a zero B field.
   //
   // The transport equations are shown below:
   //
   //  px' = px
   //  py' = py
   //  pz' = pz
   //   E' = E
   //   x' = x + (px/p) * s
   //   y' = y + (py/p) * s
   //   z' = z + (pz/p) * s
   //
   // where
   //  s   = arc length
   //
   //  The solution defining the intersection point is
   //
   //     s = -[delta_v,eta_v] * p / [p_v,eta_v]
   //
   //  where delta_v = x_v - point_v
   //
   //  If there is more than one solution, the one closest to the
   //  current point along the forward/backward direction should be
   //  chosen.

   KTKinematicData saveTrack(*this);   // Save current values in order
                                       // to move error matrix later

   HepPoint3D delta = position() - aPoint;
   const double deltaDotEta = delta.dot(aNormal);
   const double pDotEta = momentum().dot(aNormal);

   // Quit if direction is parallel to plane
   if(0. == pDotEta) {
      return KTMoveControl::kMoveNoIntersection;
   }

   double sOverP = -deltaDotEta / pDotEta;
   double s = sOverP * pmag();

   int sDirection = KTMoveControl::directionInteger(aDirection);

  // If track is already pinned to the plane, there is no other solution
  if(fabs(s) < KTMoveControl::stickyDistance()) {
     s = 0.;
     sOverP = 0.;
  }
   // Check that point is in the correct direction
   if(s <= 0. && aDirection==KTMoveControl::kDirectionForward  ||
      s >= 0. && aDirection==KTMoveControl::kDirectionBackward) {
      return KTMoveControl::kMoveNoIntersection;
   }

   // Transport position
   setPosition( position() + sOverP * momentum() );

   // Move error matrix and return status
   MoveStatus errorStatus = moveErrorMatrixBZero(saveTrack);

   return errorStatus;
}


KTMoveControl::MoveStatus
KTKinematicMoveable::moveToCylinderBZero(
                                const HepPoint3D& aCenter,
			        const HepVector3D& aDirCosine,
				const ValueType aRadius,
				const MoveDirection aDirection)
{
   // This function translates a KinematicData object to the specified
   // cylinder in a zero B field.
   //
   // The transport equations are shown below:
   //
   //  px' = px
   //  py' = py
   //  pz' = pz
   //   E' = E
   //   x' = x + (px/p) * s
   //   y' = y + (py/p) * s
   //   z' = z + (pz/p) * s
   //
   // where
   //  s   = arc length
   //
   //  The equation defining the intersection of the line with the cylinder
   //  is
   //
   //   0 = delta_R^2 + 2*A2*s + A1*s^2
   //
   //  where delta_v = x_v - center_v
   //             A1 = 1 - [p_v,eta_v]^2 / p^2
   //             A2 = ([delta_v,p_v] - [delta_v,eta_v]*[p_v,eta_v]) / p
   //        delta_R = delta_v^2 - [delta_v,eta_v]^2 - radius^2
   //
   //  The solution is
   //
   //     s = (-A2 +- sqrt(A2^2 - delta_R*A1) / A1
   //
   //
   //  If there is more than one solution, the one closest to the
   //  current point along the forward/backward direction should be
   //  chosen.

   KTKinematicData saveTrack(*this);   // Save current values in order
                                       // to move error matrix later

   const double px = momentum().x();
   const double py = momentum().y();

   const double  delx = position().x() - aCenter.x();
   const double  dely = position().y() - aCenter.y();

   const HepVector3D pmom = momentum();
   const double ptot = pmom.mag();
   const double pperp = sqrt(px*px + py*py);

   // Quit if no momentum or no pt relative to axis
   HepVector3D pCrossEta = momentum().cross(aDirCosine);
   if(0. == ptot || 0. == pCrossEta.mag()) {
      return KTMoveControl::kMoveNoIntersection;
   }

   const double A1 = pperp*pperp / (ptot*ptot);
   const double A2 = (px*delx + py*dely) / ptot;
   const double delta_R = delx*delx + dely*dely - aRadius*aRadius;

   double s;    // Arc length we are trying to compute
   double s1;   // Arc length of first intersection
   double s2;   // Arc length of second intersection
   double disc; // Discriminant for quadratic equation we must solve

   disc = A2*A2 - A1*delta_R;
   if(disc < 0.) {
      return KTMoveControl::kMoveNoIntersection;
   }

   int sDirection = KTMoveControl::directionInteger(aDirection);

   // Find the two intersections and sort them in ascending order
   disc = sqrt(disc);
   s1 = (-A2 - disc) / A1;
   s2 = (-A2 + disc) / A1;
 
   // Check if track is already pinned to the cylinder surface
   if(fabs(s1) < KTMoveControl::stickyDistance()) {
      s1 = 0.;
   }
   if(fabs(s2) < KTMoveControl::stickyDistance()) {
      s2 = 0.;
   }
   if(s1 > s2) {
      double temp = s1;
      s1 = s2;
      s2 = temp;
   }


   // Move forward
   if(aDirection == KTMoveControl::kDirectionForward) {
      if(s2 <= 0.) {
	 return KTMoveControl::kMoveNoIntersection;
      }
      else if(s1 <= 0.) {
	 s = s2;
      }
      else {
	 s = s1;
      }
   }

   // Move backward
   else if(aDirection == KTMoveControl::kDirectionBackward) {
      if(s1 >= 0.) {
	 return KTMoveControl::kMoveNoIntersection;
      }
      else if(s2 >= 0.) {
	 s = s1;
      }
      else {
	 s = s2;
      }
   }

   // Move either direction
   else {
      if(fabs(s1) <= fabs(s2)) {
	 s = s1;
      }
      else {
	 s = s2;
      }
   }

   // Transport position
   const double sOverP = s / ptot;
   setPosition( position() + sOverP * momentum() );

   // Move error matrix and return status
   MoveStatus errorStatus = moveErrorMatrixBZero(saveTrack);

   return errorStatus;
}


KTMoveControl::MoveStatus
KTKinematicMoveable::moveErrorMatrixBZero(const KTKinematicData& aOldTrack)
{
   // Move the error matrix from one point to another, assuming that
   // the arc length is fixed and that the B field is zero.
   // The kinematic parameters before the move are in aOldTrack.

   // Quit if no error matrix
   if( hasNullErrorMatrix() ) {
      return KTMoveControl::kMoveOK;
   }

   // Calculate derivative matrix and apply similarity transformation
   HepMatrix R(7,7,0);
   KTMoveControl::MoveStatus matrixReturn
                          = calculateDerivativeMatrixBZero(aOldTrack, R);
   if( KTMoveControl::kMoveCovarianceMoveFailed != matrixReturn ){

// Calculate covariance matrix of new parameters: V_c' = R * V_k * R(t).
      setErrorMatrix ( aOldTrack.errorMatrix().similarity( R ) );

   }
   return matrixReturn;
}


KTMoveControl::MoveStatus
KTKinematicMoveable::moveIterate( const KTMoveControl::MoveSpec& aMoveSpec,
				  const MoveDirection aDirection)
{
  // This function moves a KinematicData object to the specified point,
  // line or surface defined by aMoveSpec using an iterative method.
  // The particular specification is obtained by the class inherited
  // from MoveSpec. The iteration process converges quadratically.


  // Save a copy of the track parameters in case move fails. Also clear
  // the error matrix so that it is not updated during the iteration
  // process. This saves a large amount of CPU time if it is possible to
  // do the entire move in one step, e.g. constant B fields in vacuum.
  // Later, restore the matrix and move everything in one step.

  HepSymMatrix* saveErrorPointer = takeOwnershipOfPointer();
  KTKinematicData saveParticle(*this);

  // Create a KTMoveControl object to handle the iterative move
  KTMoveControl moveIterator;

  // Loop until point reached or failure
  MoveStatus moveStatus;
  do {

    // Determine how big the maximum step size can be and pass it
    // to the iterator (which takes the minimum of it and a step size
    // determined from the maximum perpendicular error allowed per step)
    const HepVector3D bFieldVector = bField();
    const double bFieldMagnitude = bFieldVector.mag();
    HepVector3D bFieldDir;
    if(0. == bFieldMagnitude) {
      bFieldDir = HepVector3D(0., 0., 0.);
    }
    else {
      bFieldDir = (1. / bFieldMagnitude) * bFieldVector;
    }

    // Dot and cross products needed by the coefficients
    const double ptot = pmag();  // Total momentum
    const double rho = kBFieldConstant * bFieldMagnitude * charge() / ptot;
    const double sinTheta = pperp() / ptot;

    const double stepSize = getMaxStepSize();
    moveIterator.setPossibleStepSize(rho, sinTheta, stepSize);

    // Calculate the indicial equation parameters using a virtual function
    KTMoveControl::ApproxParameters param = aMoveSpec.calcParameters(*this);

    // Update the iterator
    const double step = moveIterator.chooseArcLength(param, aDirection);
    moveStatus = moveIterator.update(rho, step);

    // If failure, restore initial track
    if(moveStatus == KTMoveControl::kMoveAlgorithmFailed) {
       KTKinematicData::operator=( saveParticle );  // g++ needs this explicit
                                                    // form
       restoreOwnershipOfPointer(saveErrorPointer);
       return moveStatus;
    }

    // Otherwise take the step and keep looping
    moveByArcLength(step);
  }
  while (moveStatus == KTMoveControl::kMoveNotDone);


  // Everything OK. If there is a covariance matrix, restore the initial
  // track and do the entire move here in one step. This can only be done
  // in constant B fields in vacuum.
  if(saveErrorPointer != 0) {
     KTKinematicData::operator=( saveParticle );  // g++ needs this explicit
                                                  // form
     restoreOwnershipOfPointer(saveErrorPointer);
     moveByArcLength( moveIterator.arcLength() );
  }
  return KTMoveControl::kMoveOK;

}


KTMoveControl::MoveStatus
KTKinematicMoveable::calculateDerivativeMatrixBZero(
                                   const KTKinematicData& aOldTrack,
				   HepMatrix& aDerivativeMatrix) const
{
   // Calculate the derivative matrix d(new) / d(old) of the kinematic
   // parameters for the zero B field case.

   // Make an alias for the derivative matrix to keep code readable
   HepMatrix& R = aDerivativeMatrix;

   const HepPoint3D delta = position() - aOldTrack.position();

   const double ptot = pmag();

  // Arc length in 3-D
   const double s3d = momentum().dot(delta) / ptot;
   const double factor1 = s3d / (ptot*ptot*ptot);

   R(kPx,kPx) = 1.;

   R(kPy,kPy) = 1.;

   R(kPz,kPz) = 1.;

   R(kEnergy,kPx) = px() / energy();
   R(kEnergy,kPy) = py() / energy();
   R(kEnergy,kPz) = pz() / energy();
   R(kEnergy,kEnergy) = 1.;

   R(kX,kPx) = (ptot*ptot - px()*px()) * factor1;
   R(kX,kPy) = -px() * py() * factor1;
   R(kX,kPz) = -px() * pz() * factor1;
   R(kX,kX) = 1.;

   R(kY,kPx) = -py() * px() * factor1;
   R(kY,kPy) = (ptot*ptot - py()*py()) * factor1;
   R(kY,kPz) = -py() * pz() * factor1;
   R(kY,kY) = 1.;

   R(kZ,kPx) = -pz() * px() * factor1;
   R(kZ,kPy) = -pz() * py() * factor1;
   R(kZ,kPz) = (ptot*ptot - pz()*pz()) * factor1;

   R(kZ,kZ) = 1.;

   return KTMoveControl::kMoveOK;

}

//
// const member functions
//

//
// static member functions
//
