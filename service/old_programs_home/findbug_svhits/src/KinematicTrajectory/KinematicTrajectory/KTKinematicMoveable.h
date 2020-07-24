#if !defined(KINEMATICTRAJECTORY_KTKINEMATICMOVEABLE_H)
#define KINEMATICTRAJECTORY_KTKINEMATICMOVEABLE_H
// -*- C++ -*-
//
// Package:     KinematicTrajectory
// Module:      KTKinematicMoveable
// 
// Description: Abstract base class which allows transport of KTKinematicData
//              objects.
//
// Usage:
//
// KinematicMoveable inherits from KTKinematicData and allows KTKinematicData
// objects to be moved through the detector. Specific subclasses allow
// different kinds of magnetic fields and/or energy loss mechanisms to be
// used.
//
// Since there is no specification of a magnetic field or material geometry
// in this class, all public move routines defined here are pure virtual
// functions, which means that KTKinematicMoveable is an abstract class and
// no objects of its type can be instantiated. Classes which inherit from
// class MUST provide implementations of these methods.
//
// ************************************************************************
// Specification of move direction and status
// ************************************************************************
//
// When you move any KTKinematicMoveable object (or objects from its
// descendents) you can specify the direction of the move as being forward,
// backard or either (the specification of "either" means that the object
// will take the shortest path to the desired point, either forward or
// backward). Forward and backward mean exactly what they say; the object
// will move only in the specified direction even if it has to make almost
// a complete turn.
//
// The definitions of these directions can be found in the KTMoveControl
// class. For reasons of coding clarity it is better to typedef them as
// shown below:
//
//   typedef KTMoveControl::kDirectionForward  kForward;
//   typedef KTMoveControl::kDirectionBackward kBackward;
//   typedef KTMoveControl::kDirectionEither   kEither;
//
// You can also typedef the return argument in a similar way to make your
// code simpler to read.
//
//   typedef KTMoveControl::MoveStatus MoveStatus;
//
// ************************************************************************
// Move routines
// ************************************************************************
//
// The following move routines are defined as pure virtual functions. There
// are implementations of these routines (with different) names for the
// case where the helix is a straight line. These latter functions are called
// automatically by the descendents of this class when they detect that the
// magnetic field is zero or the particle has no charge.
//
// moveByArcLength     Move by a particular arc length
// moveToPoint         Move to PCA to a point
// moveToPointbend     Move to PCA to a point (in bend plane)
// moveToLine          Move to PCA to an arbitrary line
// moveToZPosition     Move to plane with normal along z axis
// moveToPlane         Move to an arbitrary plane
// moveToRadius        Move to a radius (cylinder centered at origin)
// moveToCylinder      Move to an arbitrary cylinder
//
//
// Author:      Paul Avery
// Created:     Mon Oct  6 14:32:10 EDT 1997
// $Id: KTKinematicMoveable.h,v 1.6 2000/06/15 20:16:53 pappas Exp $
//
// Revision history
//
// $Log: KTKinematicMoveable.h,v $
// Revision 1.6  2000/06/15 20:16:53  pappas
// Double checked and committed Paul Avery's modifications eliminating
// references to Toolbox classes. Should be completely transparent since
// Toolbox is just typedefs.
//
// Revision 1.5  1998/07/09 21:33:41  avery
// Bug fixes, finished implementation of move routines, updated documentation.
//
// Revision 1.4  1998/06/01 18:24:32  avery
// Fixed problem with copy constructor
//
// Revision 1.3  1998/05/27 17:12:56  avery
// Improved documentation
//
// Revision 1.2  1998/05/24 21:30:48  avery
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
#include "KinematicTrajectory/KTMoveControl.h"
#include "KinematicTrajectory/KTKinematicData.h"

// forward declarations
class KTHelix;

class KTKinematicMoveable : public KTKinematicData
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef KTMoveControl::MoveStatus    MoveStatus;
      typedef KTMoveControl::MoveDirection MoveDirection;

      // Constructors and destructor
      KTKinematicMoveable();

      KTKinematicMoveable( const KTKinematicData& aKinematicData );

      KTKinematicMoveable( const KTKinematicData& aKinematicData,
			   const DABoolean aCopyErrorMatrix );

      KTKinematicMoveable( const HepVector3D& aMomentum ,
			   const HepPoint3D& aPosition,
			   const ValueType aMass,
			   const ValueType aCharge) ;

      KTKinematicMoveable( const HepVector3D& aMomentum ,
			   const HepPoint3D& aPosition,
			   const ValueType aMass,
			   const ValueType aCharge,
			   const HepSymMatrix& aErrorMatrix ) ;

      KTKinematicMoveable( const KTHelix& aHelix ,
			   const ValueType aMass,
			   const HepVector3D& aMagneticField ,
			   const DABoolean aCopyErrorMatrix = true);

      KTKinematicMoveable( const KTHelix& aHelix ,
			   const ValueType aMass,
			   const ValueType aBFieldMagnitude,
			   const DABoolean aCopyErrorMatrix = true);

      // Let the compiler create this
//      KTKinematicMoveable( const KTKinematicMoveable& );

      // Let the compiler create this
      // assignment operator(s)
//      const KTKinematicMoveable& operator=(
//                                        const KTKinematicMoveable& );

      // Let the compiler create this
//      virtual ~KTKinematicMoveable();

      // member functions
      virtual MoveStatus moveByArcLength( const ValueType aArcLength) = 0;

      virtual MoveStatus moveToPoint( const HepPoint3D& aPoint,
				      const MoveDirection aDirection =
				         KTMoveControl::kDirectionEither) = 0;

      virtual MoveStatus moveToPointBend( const HepPoint3D& aPoint,
					  const MoveDirection aDirection =
					  KTMoveControl::kDirectionEither) = 0;

      virtual MoveStatus moveToLine( const HepPoint3D& aPoint,
				     const HepVector3D& aDirCosine,
	                             const MoveDirection aDirection =
				         KTMoveControl::kDirectionEither) = 0;

      virtual MoveStatus moveToZPosition( const ValueType aZPosition,
					  const MoveDirection aDirection =
					  KTMoveControl::kDirectionEither) = 0;

      virtual MoveStatus moveToPlane( const HepPoint3D&  aPoint,
				      const HepVector3D& aNormal,
				      const MoveDirection aDirection =
				         KTMoveControl::kDirectionEither) = 0;

      virtual MoveStatus moveToRadius( const ValueType aRadius,
				       const MoveDirection aDirection =
				         KTMoveControl::kDirectionEither) = 0;

      virtual MoveStatus moveToCylinder( const HepPoint3D&  aCenter,
					 const HepVector3D& aDirCosine,
					 const ValueType aRadius,
					 const MoveDirection aDirection =
					  KTMoveControl::kDirectionEither) = 0;

      MoveStatus moveIterate( const KTMoveControl::MoveSpec& aMoveSpec,
			      const MoveDirection aDirection
			         = KTMoveControl::kDirectionEither);


      // const member functions

      // B field at a given point
      virtual HepVector3D bField( ) const = 0;

      // Maximum step size allowed (depends on magnetic field
      // inhomogeneity, multiple scattering, dE/dx, etc.)
      virtual ValueType getMaxStepSize( ) const = 0;

      //PCA, DCA to point or point in bend plane
      virtual HepPoint3D pointOfClosestApproach(
	                          const HepPoint3D& aPoint) const = 0;
      HepPoint3D pca( const HepPoint3D& aPoint) const {
	 return pointOfClosestApproach(aPoint); }

      virtual HepPoint3D pointOfClosestApproachBend(
	                          const HepPoint3D& aPoint) const = 0;
      HepPoint3D pcaBend( const HepPoint3D& aPoint) const {
	 return pointOfClosestApproachBend(aPoint); }


      virtual ValueType distanceOfClosestApproach(
	                          const HepPoint3D& aPoint) const = 0;
      ValueType dca( const HepPoint3D& aPoint) const {
	 return distanceOfClosestApproach(aPoint); }


      virtual ValueType distanceOfClosestApproachBend(
	                          const HepPoint3D& aPoint) const = 0;
      ValueType dcaBend( const HepPoint3D& aPoint) const {
	 return distanceOfClosestApproachBend(aPoint); }


      //PCA, DCA to line
      virtual HepPoint3D pointOfClosestApproach(
	                          const HepPoint3D&  aPoint,
				  const HepVector3D& aDirCosine) const = 0;
      HepPoint3D pca( const HepPoint3D& aPoint,
				const HepVector3D& aDirCosine) const {
	 return pointOfClosestApproach(aPoint, aDirCosine); }

      virtual ValueType distanceOfClosestApproach(
	                          const HepPoint3D&  aPoint,
				  const HepVector3D& aDirCosine) const = 0;
      ValueType dca( const HepPoint3D& aPoint,
		     const HepVector3D& aDirCosine) const {
	 return distanceOfClosestApproach(aPoint, aDirCosine); }



      // static member functions

   protected:
      // protected member functions

      // These functions do moves in zero magnetic field, no material
      MoveStatus moveByArcLengthBZero( const ValueType aArcLength);

      MoveStatus moveToPointBZero( const HepPoint3D& aPoint,
	                           const MoveDirection aDirection
				         = KTMoveControl::kDirectionEither);

      MoveStatus moveToPointBendBZero( const HepPoint3D& aPoint,
				       const MoveDirection aDirection
				           = KTMoveControl::kDirectionEither);

      MoveStatus moveToLineBZero( const HepPoint3D&  aPoint,
				  const HepVector3D& aDirCosine,
				  const MoveDirection aDirection
				        = KTMoveControl::kDirectionEither);

      MoveStatus moveToPlaneBZero( const HepPoint3D&  aPoint,
				   const HepVector3D& aNormal,
				   const MoveDirection aDirection
				         = KTMoveControl::kDirectionEither);

      MoveStatus moveToCylinderBZero( const HepPoint3D&  aCenter,
				      const HepVector3D& aDirCosine,
				      const ValueType aRadius,
				      const MoveDirection aDirection
				            = KTMoveControl::kDirectionEither);


      MoveStatus moveErrorMatrixBZero( const KTKinematicData& aOldTrack);


      MoveStatus moveToPointIterate( const HepPoint3D& aPoint,
				     const MoveDirection aDirection
				        = KTMoveControl::kDirectionEither) {
	                      KTMoveControl::Point3D point(aPoint);
                              return moveIterate(point, aDirection);  };

      MoveStatus moveToPointBendIterate( const HepPoint3D& aPoint,
					 const MoveDirection aDirection
				           = KTMoveControl::kDirectionEither) {
	                      KTMoveControl::Point2D point(aPoint);
                              return moveIterate(point, aDirection);  };

      MoveStatus moveToLineIterate( const HepPoint3D&  aPoint,
				    const HepVector3D& aDirCosine,
				    const MoveDirection aDirection
				       = KTMoveControl::kDirectionEither) {
	                      KTMoveControl::Line line(aPoint, aDirCosine);
                              return moveIterate(line, aDirection);  };

      MoveStatus moveToPlaneIterate( const HepPoint3D&  aPoint,
				     const HepVector3D& aNormal,
				     const MoveDirection aDirection
				        = KTMoveControl::kDirectionEither) {
	                      KTMoveControl::Plane plane(aPoint, aNormal);
                              return moveIterate(plane, aDirection);  };

      MoveStatus moveToCylinderIterate( const HepPoint3D&  aCenter,
					const HepVector3D& aDirCosine,
					const ValueType aRadius,
					const MoveDirection aDirection
				           = KTMoveControl::kDirectionEither) {
	                      KTMoveControl::Cylinder cylinder(aCenter,
							       aDirCosine,
							       aRadius);
                              return moveIterate(cylinder, aDirection);  };


      // protected const member functions

      MoveStatus calculateDerivativeMatrixBZero(
	                           const KTKinematicData& aOldTrack,
				   HepMatrix& aDerivativeMatrix) const;

   private:
      // Constructors and destructor

      // private member functions

      // private const member functions

      // data members

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "KinematicTrajectory/Template/KTKinematicMoveable.cc"
//#endif

#endif /* KINEMATICTRAJECTORY_KTKINEMATICMOVEABLE_H */
