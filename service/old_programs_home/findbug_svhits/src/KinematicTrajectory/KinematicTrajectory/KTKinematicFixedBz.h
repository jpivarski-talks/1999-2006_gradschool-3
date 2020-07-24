#if !defined(KINEMATICTRAJECTORY_KTKINEMATICFIXEDBZ_H)
#define KINEMATICTRAJECTORY_KTKINEMATICFIXEDBZ_H
// -*- C++ -*-
//
// Package:     KinematicTrajectory
// Module:      KTKinematicFixedBz
// 
// Description: Methods for moving KTKinematicData objects in a fixed
//              solenoidal B field. Inherits from KTKinematicMoveable.
//
// Usage:
//
// Given the momentum and position of a particle on a helix, it is
// relatively straightforward to calculate the momentum and position as a
// function of the arc length s, where s = 0 at the original point. Let
// (x0,y0,z0) be an initial point on the helix and (px0,py0,pz0) be its
// momentum there. Then the position and momentum of any other point on the
// helix can be written
//
//   x  = x0 + (px0/a) * sin(rho*s) - (py0/a) * [1-cos(rho*s)]
//   y  = y0 + (py0/a) * sin(rho*s) + (px0/a) * [1-cos(rho*s)]
//   x  = z0 + (pz0/p) * s
//   px = px0*cos(rho*s) - py0*sin(rho*s)
//   py = px0*sin(rho*s) + py0*cos(rho*s)
//   pz = pz0</PRE></DIR>
//
// where
//
//   s = arc length from (x0,y0,z0)
//   a = -0.0299792458 * Bfield * charge (Bfield in Kgauss)
//   rho = a / p</PRE></DIR>
//
// For a straight line, these equations simplify to
//
//   x = x0 + (px0/p) * s
//   y = y0 + (py0/p) * s
//   x = z0 + (pz0/p) * s
//   px = px0
//   py = px0
//   pz = pz0
//
//
// ************************************************************************
// Building a KTKinematicFixedBz object from a helix
// ************************************************************************
//
// This example takes a helix and builds KTKinematicFixedBz objects pion1
// and pion2, with and without an error matrix, respectively.
//
//   KTHelix helix(...);
//   double mass = 0.1396;
//   double bMag = -15.;
//   DABoolean noErrorMatrix = false;
//
//   KTKinematicFixedBz pion1(helix, mass, bMag);
//   KTKinematicFixedBz pion2(helix, mass, bMag, noErrorMatrix);
//
//
// ************************************************************************
// Building a KTKinematicFixedBz object from kinematic information
// ************************************************************************
//
// All that needs to be added is the B field magnitude. Again we build
// KTKinematicFixedBz objects pion1 and pion2, without and with an error
// matrix, respectively.
//
//   KTKinematicData particle(...);
//   double bMag = -15.;
//   DABoolean noErrorMatrix = false;
//
//   KTKinematicFixedBz pion1(particle, bMag);
//   KTKinematicFixedBz pion2(particle, bMag, noErrorMatrix);
//
// ************************************************************************
// Building a KTKinematicFixedBz object from scratch
// ************************************************************************
//
// You can also build KTKinematicFixedBzobjects from momentum and position
// information, similar to the method used for KTKinematicData objects.
// Once again we build two objects, one with and one without the error
// matrix.
//
//   HepVector3D momentum(1.2, -0.5, 0.6);
//   HepPoint3D  position(0.002, 0.003, 0.);
//   double mass = 0.1396;
//   double charge = -1.0;
//   double bMag = -15.;
//   HepSymMatrix errMatrix(7,1);  Create a 7x7 unit matrix for now
//
//   KTKinematicFixedBz pion1(momentum, position, mass, charge, bMag,
//                            errMatrix);
//   KTKinematicFixedBz pion2(momentum, position, mass, charge, bMag);
//
//
// ************************************************************************
// Examples of how to use KTKinematicFixedBz objects
// ************************************************************************
//
// Assume that we have already built a particle of type KTKinematicFixedBz.
// Let's try to move the object. Remember that when you move any
// KTKinematicMoveable object you can specify the direction of the move
// as being forward, backard or either (the specification of "either"
// means that the object will take the shortest path to the desired point,
// either forward or backward). The definitions of these directions can be
// found in the KTMoveControl class. For coding clarity it is better to
// typedef them as shown below:
//
//   typedef KTMoveControl::kDirectionForward  kForward;
//   typedef KTMoveControl::kDirectionBackward kBackward;
//   typedef KTMoveControl::kDirectionEither   kEither;
//
// You can also typedef the return argument in a similar way.
//
//   typedef KTMoveControl::MoveStatus MoveStatus;</PRE>
//
//
// Move forward to a plane
//   HepPoint3D  aPlanePoint (0.2, 0.5, 1.2);
//   HepVector3D aPlaneNormal(0.8, 0.6, 0.);
//   MoveStatus status = pion.moveToPlane(aPlanePoint, aPlaneNormal, kForward);
//
// Move to the specified radius (pick closest path)
//   double radius = 1.2;
//   MoveStatus status = pion.moveToRadius(radius);
//
// Move to the point of closest approach to a point (pick closest path)
//   HepPoint3D aPoint(0.22, 0.55, 1.22);
//   MoveStatus status2 = pion.moveToPoint(aPoint);
//
// Calculate the DCA to origin (in bend plane)
//   MoveStatus status2 = pion.dcaBend( HepPoint3D(0.,0.,0.) );
//
//
// Author:      Paul Avery
// Created:     Wed Oct  8 16:31:49 EDT 1997
// $Id: KTKinematicFixedBz.h,v 1.6 2000/06/15 20:16:53 pappas Exp $
//
// Revision history
//
// $Log: KTKinematicFixedBz.h,v $
// Revision 1.6  2000/06/15 20:16:53  pappas
// Double checked and committed Paul Avery's modifications eliminating
// references to Toolbox classes. Should be completely transparent since
// Toolbox is just typedefs.
//
// Revision 1.5  1998/07/09 21:33:39  avery
// Bug fixes, finished implementation of move routines, updated documentation.
//
// Revision 1.4  1998/06/01 18:24:29  avery
// Fixed problem with copy constructor
//
// Revision 1.3  1998/05/27 17:12:54  avery
// Improved documentation
//
// Revision 1.2  1998/05/24 21:30:47  avery
// Add move, PCA routines. Some cleanup
//
// Revision 1.1.1.1  1997/11/04 23:38:47  cdj
// First submission
//
// Revision 1.1.1.1  1997/11/04 22:45:14  avery
// First submission
//

// system include files

// user include files
#include "KinematicTrajectory/KTKinematicMoveable.h"

// forward declarations

class KTKinematicFixedBz : public KTKinematicMoveable
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef double ValueType;

      // Constructors and destructor
      KTKinematicFixedBz();
  
      KTKinematicFixedBz( const KTKinematicFixedBz& aKinematicFixedBz);

      KTKinematicFixedBz( const KTKinematicFixedBz& aKinematicFixedBz,
			  const DABoolean aCopyErrorMatrix);

      KTKinematicFixedBz( const KTKinematicData& aKinematicData,
			  const ValueType aBFieldMagnitude,
			  const DABoolean aCopyErrorMatrix = true);

      KTKinematicFixedBz( const HepVector3D& aMomentum,
			  const HepPoint3D& aPosition,
			  const ValueType aMass,
			  const ValueType aCharge,
			  const ValueType aBFieldMagnitude );

      KTKinematicFixedBz( const HepVector3D& aMomentum,
			  const HepPoint3D& aPosition,
			  const ValueType aMass,
			  const ValueType aCharge,
			  const ValueType aBFieldMagnitude,
			  const HepSymMatrix& aErrorMatrix ) ;

      KTKinematicFixedBz( const KTHelix& aHelix,
			  const ValueType aMass,
			  const ValueType aBFieldMagnitude,
			  const DABoolean aCopyErrorMatrix = true) ;

      virtual ~KTKinematicFixedBz();

      // member functions
      virtual MoveStatus moveByArcLength( const ValueType aArcLength);

      virtual MoveStatus moveToPoint( const HepPoint3D& aPoint,
				      const MoveDirection aDirection =
				             KTMoveControl::kDirectionEither);

      virtual MoveStatus moveToPointBend( const HepPoint3D& aPoint,
					  const MoveDirection aDirection =
					     KTMoveControl::kDirectionEither);

      virtual MoveStatus moveToLine( const HepPoint3D&  aPoint,
				     const HepVector3D& aDirCosine,
				     const MoveDirection aDirection =
				             KTMoveControl::kDirectionEither);

      virtual MoveStatus moveToZPosition( const ValueType aZPosition,
					  const MoveDirection aDirection =
					     KTMoveControl::kDirectionEither);

      virtual MoveStatus moveToPlane( const HepPoint3D&  aPoint,
				      const HepVector3D& aNormal,
				      const MoveDirection aDirection =
				             KTMoveControl::kDirectionEither);

      virtual MoveStatus moveToRadius( const ValueType aRadius,
				       const MoveDirection aDirection =
				             KTMoveControl::kDirectionEither);

      virtual MoveStatus moveToCylinder( const HepPoint3D&  aCenter,
					 const HepVector3D& aDirCosine,
					 const ValueType aRadius,
					 const MoveDirection aDirection =
					     KTMoveControl::kDirectionEither);

      // const member functions

      ValueType BFieldMagnitude() const {return m_BFieldMagnitude;}

      // B field at a given point
      virtual HepVector3D bField( ) const
                       { return HepVector3D( 0.,0.,BFieldMagnitude() ); };

      // Maximum step size allowed
      virtual ValueType getMaxStepSize( ) const { return 1.e20; };

      virtual HepPoint3D pointOfClosestApproach(
	                          const HepPoint3D& aPoint) const;

      virtual HepPoint3D pointOfClosestApproachBend(
	                          const HepPoint3D& aPoint) const;


      virtual ValueType distanceOfClosestApproach(
	                          const HepPoint3D& aPoint) const;

      virtual ValueType distanceOfClosestApproachBend(
	                          const HepPoint3D& aPoint) const;


      //PCA, DCA to line
      virtual HepPoint3D pointOfClosestApproach(
	                          const HepPoint3D&  aPoint,
				  const HepVector3D& aDirCosine) const;

      virtual ValueType distanceOfClosestApproach(
	                          const HepPoint3D&  aPoint,
				  const HepVector3D& aDirCosine) const;


      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor

      // assignment operator(s)
      const KTKinematicFixedBz& operator=( const KTKinematicFixedBz& );

      // private member functions
      virtual MoveStatus moveToXYPlane( const HepPoint3D&  aPoint,
					const HepVector3D& aNormal,
					const MoveDirection aDirection =
					     KTMoveControl::kDirectionEither);

      MoveStatus moveToXYCylinder( const HepPoint3D& aCenter,
				   const ValueType aRadius,
				   const MoveDirection aDirection =
				             KTMoveControl::kDirectionEither);

      MoveStatus moveErrorMatrix( const KTKinematicData& aOldTrack);

      // private const member functions
      MoveStatus calculateDerivativeMatrix(
                                          const KTKinematicData& aOldTrack,
					  HepMatrix& aDerivativeMatrix) const;

      // data members
      ValueType m_BFieldMagnitude;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "KinematicTrajectory/Template/KTKinematicFixedBz.cc"
//#endif

#endif /* KINEMATICTRAJECTORY_KTKINEMATICFIXEDBZ_H */
