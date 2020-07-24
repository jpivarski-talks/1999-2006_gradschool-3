#if !defined(KINEMATICTRAJECTORY_KTHELIX_H)
#define KINEMATICTRAJECTORY_KTHELIX_H
// -*- C++ -*-
//
// Package:     KinematicTrajectory
// Module:      KTHelix
// 
/**\class KTHelix KTHelix.h KinematicTrajectory/KTHelix.h
  
  Description: Describes a helical path traveled by a charged particle

  Usage:

  \par Motion along the helix
  This class describes the helical path traversed by a charged particle
  in a solenoidal B field (B along z direction). You can think of a
  helix as the path traced by a particle moving in a circle in the XY
  plane while the circle moves at constant velocity along the z axis.
  The helix is described by five parameters:
\code
   curvature  = 1 / (2*radius of curvature)
   phi0       = phi angle of momentum at PCAxy = atan(py/px)
   d0         = signed 2-D distance of closest approach of PCAxy
   cot(theta) = cotangent of angle wrt +z axis = Pz / Ptot
   z0         = z of PCAxy
\endcode
  \c PCAxy is defined as the point on the trajectory nearest the reference
  point in the XY plane. This point is not the absolute point of
  closest approach since the latter depends on the z motion of the
  particle. Likewise, \c d0 is measured only in the XY plane.
  <br><br>
  The equations of motion can be written using these parameters as follows:
\code
   x = xr + x0 + (u0/rho) * sin(rho*sp) - (v0/rho) * [1-cos(rho*sp)]
   y = yr + y0 + (v0/rho) * sin(rho*sp) + (u0/rho) * [1-cos(rho*sp)]
   x = zr + z0 + cot(theta) * sp

   px = px0*cos(rho*sp) - py0*sin(rho*sp)
   py = px0*sin(rho*sp) + py0*cos(rho*sp)
   pz = pz0
\endcode
  where
\code
       sp = arc length in the xy plane
       rho = 2 * curvature
       u0 = cos(phi0)
       v0 = sin(phi0)
       x0 = -d0*sin(phi0)
       y0 = +d0*cos(phi0)
       (xr,yr,zr) = reference point
       px0 = Pperp * cos(phi0)
       py0 = Pperp * sin(phi0)
       pz0 = Pperp * cot(theta)
       Pperp = Ptot / sqrt(1 + cot(theta)**2)
\endcode
  The point \c (xr+x0,yr+y0,zr+z0) is the point of closest
  approach (PCAxy) of the helix to the reference point in the x-y plane.
  Kinematic quantities are normally evaluated at this point. Note that
  \c Pperp and \c Ptot are constant on the helix.


  \par Building a simple KTHelix object
  In addition to the usual copy constructor, KTHelix objects can be built
  from basic information, e.g.,
\code
   double curvature = 0.300;
   double phi0 = 0.5;
   double d0 = 0.001;
   double cotTheta = 0.9;
   double z0 = 0.02;
   HepPoint3D refPoint(0.002, 0.003, 0.);
   KTHelix helix(curvature, phi0, d0, cotTheta, z0, refPoint);
\endcode
  The reference point defaults to \c (0,0,0) if not provided.


  \par Building a KTHelix object with an error matrix
  To build a KTHelix object with a 5x5 error matrix, you can construct
  it as follows:
\code
   double curvature = 0.300;
   double phi0 = 0.5;
   double d0 = 0.001;
   double cotTheta = 0.9;
   double z0 = 0.02;
   HepPoint3D refPoint(0.002, 0.003, 0.);
   HepSymMatrix errMatrix(5,1);  // Create a 5x5 unit matrix for now
   KTHelix aHelix(curvature, phi0, d0, cotTheta, z0, errMatrix, refPoint)
\endcode
  Note: The error information is stored internally as a pointer to a
  symmetric matrix. If \c errMatrix is not provided in the constructor call,
  a null pointer is stored. If error information is requested and the
  pointer is null, a matrix filled with zeros is returned.
  <br><br>
  Users should be aware that an error matrix will always slow operations
  such as helix movement and creation of KTKinematicData objects because
  of the CPU time needed to transport or convert the error matrix.

  \par Building a KTHelix object from a KTKinematicData object
  Helix objects can also be constructed from KTKinematicData objects, which
  contain the four momentum and position. You must supply the
  KTKinematicData object and the B field. For example:
\code
  HepVector3D momentum(0.5, 0.6, 1.6);
  HepPoint3D  position(0.002, -0.004, 0.03);
  double mass = 0.1396;
  double charge = 1.;
  KTKinematicData kineTrack(momentum, position, mass, charge);
  HepVector3D bField(0., 0., -15.);
  KTHelix helix(kineTrack, bField);
\endcode
  Note that the B field is specified in \e kilogauss.

  \par Setting and retrieving helix parameters
  All 5 helix parameters, the reference point and the error matrix
  can be set or retrieved individually. For example,
\code
   // Define a null helix object
   KTHelix helix;
   HepPoint3D refPoint(0.002, 0.003, 0.03);
   HepSymMatrix errMatrix;


   // Set the parameters one at a time
   helix.setCurvature(0.30);
   helix.setPhi0(2.5);
   helix.setD0(0.002);
   helix.setCotTheta(-0.5);
   helix.setZ0(0.03);
   helix.setReferencePoint(refPoint);
   helix.setErrorMatrix(errMatrix);


   // Retrieve the parameters
   double curvature = helix.curvature();
   double phi0 = helix.phi0();
   double d0 = helix.d0();
   double cotTheta = helix.cotTheta();
   double z0 = helix.z0();
   HepPoint3D refPoint = helix.referencePoint();
   HepSymMatrix errMatrix = helix.errorMatrix();


   // Get the point of closest approach to a specified point in the XY plane
   HepPoint3D point(0.3, 0.4, 0.0);
   double pca = helix.pcaBend(point);


   // Check if the error information is present
   if(helix.hasNullErrorMatrix()) {
      cout << "Helix has no error matrix" << endl;
   }
\endcode

  \par How to move helices
  Member functions are provided which change the reference point, in
  effect "moving" the helix because the parameters \c phi, \c d0 and
  \c z0 are defined at the point of closest approach to the reference
  point. If the helix has an error matrix it is also moved. The following
  move routines are implemented:
\code
   moveToReferencePoint    Move to specified reference point
   moveToLine              Move to specified line (axial only)
   moveToZPosition         Move to specified z coordinate
   moveToPlane             Move to specified plane (barrel, Z plane only)
   moveToRadius            Move to specified radius
   moveToCylinder          Move to specified cylinder (axial only)
\endcode
  The following code explains how these functions are called. Suppose we
  want to move a helix to a radius of 1 meter from the origin.
\code
   double radius = 1.0;
   KTHelix helix1, helix2;
   KTHelix::MoveStatus status1, status2;
   KTMoveControl::MoveDirection direction = KTMoveControl::kDirectionForward;

   status1 = helix1.moveToRadius(radius, direction);
   if(status1 != KTMoveControl::kMoveOK) {
      (code to deal with bad move goes here)
   }

   status2 = helix2.moveToRadius(radius);
   if(status2 != KTMoveControl::kMoveOK) {
      (code to deal with bad move goes here)
   }
\endcode
  There are several important features about the move routines:

  \arg All the move routines take an argument
  that specifies the direction to move along the helix. The directions
  correspond to forward, backward or either. If not specified, the
  direction is assumed to be "either", i.e., move by the minimum path
  necessary. This default is useful when you may not know if a
  given reference point is just before or just behind a silicon plane
  and you do not want to move the helix by a full turn.

  \arg All the move routines have a return value that tells if the move
  was successful.

  \arg The direction and status words used by the move routines are part
  of another class, KTMoveControl (it is also used by the
  KTKinematicMoveable classes). This class handles the machinery needed
  for moves which require multiple iterations to converge.

  \arg For the moveToZPosition, moveToRadius and MoveToPlane, the point of
  intersection of the helix with the surface is found and it is used as
  the new reference point. Thus \c d0 = \c z0 = \c 0 for these functions.
  <br><br>
  The second call to moveToRadius is equivalent to
\code
   status2 = helix2.moveToRadius(radius, KTMoveControl::kDirectionEither);
\endcode
*/
//
// Author:      Paul Avery
// Created:     Thu Oct 23 14:59:26 EDT 1997
// $Id: KTHelix.h,v 1.11 2002/03/12 22:55:23 bkh Exp $
//
// Revision history
//
// $Log: KTHelix.h,v $
// Revision 1.11  2002/03/12 22:55:23  bkh
// Store matrix not pointer to avoid "new"
//
// Revision 1.10  2000/06/15 20:16:52  pappas
// Double checked and committed Paul Avery's modifications eliminating
// references to Toolbox classes. Should be completely transparent since
// Toolbox is just typedefs.
//
// Revision 1.9  2000/04/03 20:49:39  marsh
// Doxygenated comments.
//
// Revision 1.8  1999/02/10 15:03:28  avery
// Add moveByArcLength method in KTHelix. Modify moveToZPosition to call it.
//
// Revision 1.7  1998/10/13 22:40:38  wsun
// Added comparison operators == and !=.
//
// Revision 1.6  1998/10/13 17:13:57  avery
// - Added functions to return momentum and position.
// - Added iterate move routines to allow moves to more general surfaces
// - Added arc length argument in move routines
// - Fixed bug in argument list in call to KTKinematicFixedBz constructor
//
// Revision 1.5  1998/07/09 21:33:34  avery
// Bug fixes, finished implementation of move routines, updated documentation.
//
// Revision 1.4  1998/06/01 18:24:25  avery
// Fixed problem with copy constructor
//
// Revision 1.3  1998/05/24 21:30:43  avery
// Add move, PCA routines. Some cleanup
//
// Revision 1.2  1997/11/19 21:57:28  avery
// Added detailed documentation to header file. Implemented
// moveToZPosition method. Modified moveToRadius to better handle the
// way phi0 was updated. Improve the way null error matrix information
// is returned.
//
// Revision 1.1.1.1  1997/11/04 23:38:46  cdj
// First submission
//
// Revision 1.1.1.1  1997/11/04 22:45:14  avery
// First submission
//
//

// system include files

// user include files
#include "CLHEP/Geometry/Vector3D.h"   // For 3-Vector declarations
#include "CLHEP/Geometry/Point3D.h"    // For 3-Point declarations
#include "CLHEP/Matrix/SymMatrix.h"    // Symmetric matrices

#include "KinematicTrajectory/KTMoveControl.h"

// forward declarations
class KTKinematicData;
class KTKinematicFixedBz;

class KTHelix
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef double ValueType;
      typedef KTMoveControl::MoveStatus    MoveStatus;
      typedef KTMoveControl::MoveDirection MoveDirection;

      ///This is needed to associate the elements correctly in the error matrix
      enum ParameterOrder {kCurvature = 1,
			   kPhi0,
			   kD0,
			   kCotTheta,
			   kZ0};

      // Constructors and destructor
      KTHelix();

      KTHelix( const KTHelix& aHelix );

      KTHelix( const KTHelix& aHelix,
	       const DABoolean aCopyErrorMatrix );

      KTHelix( const ValueType aCurvature,
	       const ValueType aPhi0,
	       const ValueType aD0,
	       const ValueType aCotTheta,
	       const ValueType aZ0,
	       const HepPoint3D& aReferencePoint = HepPoint3D(0,0,0) ) ;

      KTHelix( const ValueType aCurvature,
	       const ValueType aPhi0,
	       const ValueType aD0,
	       const ValueType aCotTheta,
	       const ValueType aZ0,
	       const HepSymMatrix& aErrorMatrix,
	       const HepPoint3D& aReferencePoint = HepPoint3D(0,0,0) ) ;

   // **************************************************************
   // It is assumed that the B field is along the z direction
   // **************************************************************
      KTHelix( const KTKinematicData& aKinematicData ,
	       const HepVector3D&   aMagneticField,
	       const HepPoint3D&    aReferencePoint = HepPoint3D(0,0,0) ) ;

      KTHelix( const KTKinematicData& aKinematicData ,
	       const ValueType        aBFieldMagnitude,
	       const HepPoint3D&    aReferencePoint = HepPoint3D(0,0,0) ) ;

      KTHelix( const KTKinematicFixedBz& aKinematicFixedBz,
	       const HepPoint3D&    aReferencePoint = HepPoint3D(0,0,0) ) ;

      virtual ~KTHelix();

      // assignment operator(s)
      const KTHelix& operator=( const KTHelix& aHelix);

      // wsun 9-25-98: comparison operators
      DABoolean operator==( const KTHelix& aHelix ) const ;
      DABoolean operator!=( const KTHelix& aHelix ) const ;

      // member functions

      void setCurvature( const ValueType aCurvature );
      void setPhi0(      const ValueType aPhi0 );
      void setD0(        const ValueType aD0 );
      void setCotTheta(  const ValueType aCotTheta );
      void setZ0(        const ValueType aZ0 );

      /// This only changes the reference point! No move implied!!
      void setReferencePoint(const HepPoint3D& aReferencePoint);

      void clearErrorMatrix(void);
      void setErrorMatrix(const HepSymMatrix& aMatrix);

      MoveStatus moveToReferencePoint( const HepPoint3D& aPoint,
				       ValueType& aDistanceMoved,
				       const MoveDirection aDirection =
				             KTMoveControl::kDirectionEither);

      MoveStatus moveByArcLength( ValueType aArcLength);

      MoveStatus moveToLine( const HepPoint3D&  aPoint,
			     const HepVector3D& aDirCosine,
			     ValueType& aDistanceMoved,
			     const MoveDirection aDirection =
			                     KTMoveControl::kDirectionEither);

      MoveStatus moveToZPosition( const ValueType aZPosition,
				  ValueType& aDistanceMoved,
				  const MoveDirection aDirection = 
				             KTMoveControl::kDirectionEither);

      MoveStatus moveToPlane( const HepPoint3D&  aPoint,
			      const HepVector3D& aNormal,
			      ValueType& aDistanceMoved,
			      const MoveDirection aDirection =
			                     KTMoveControl::kDirectionEither);

      MoveStatus moveToRadius( const ValueType aRadius,
			       ValueType& aDistanceMoved,
			       const MoveDirection aDirection =
			                     KTMoveControl::kDirectionEither);

      MoveStatus moveToCylinder( const HepPoint3D& aCenter,
				 const HepVector3D& aDirCosine,
				 const ValueType aRadius,
				 ValueType& aDistanceMoved,
				 const MoveDirection aDirection =
				             KTMoveControl::kDirectionEither);

      MoveStatus moveErrorMatrix(const KTHelix& aOldHelix);


      // const member functions
      ValueType curvature() const { return m_curvature;} ;
      ValueType phi0() const { return m_phi0;};
      ValueType d0() const { return m_d0;};
      ValueType cotTheta() const { return m_cotTheta;};
      ValueType z0() const { return m_z0;};
      const HepPoint3D& referencePoint() const { return m_referencePoint;};

      /// \return TRUE if errors are all zero
      DABoolean hasNullErrorMatrix() const {return m_hasNullErrorMatrix;};
      const HepSymMatrix& errorMatrix() const;

      /// Current position at pca to reference point
      HepPoint3D  position() const;
      /// Current momentum at pca to reference point
      HepVector3D momentum(const HepVector3D& aMagneticField) const;

      /// PCA to point
      HepPoint3D pointOfClosestApproach(const HepPoint3D& aPoint) const;
      HepPoint3D pca(const HepPoint3D& aPoint) const {
	 return pointOfClosestApproach(aPoint); };

      /// PCA to 2-D point
      HepPoint3D pointOfClosestApproachBend(const HepPoint3D& aPoint) const;
      HepPoint3D pcaBend(const HepPoint3D& aPoint) const {
	 return pointOfClosestApproachBend(aPoint); };

      /// DCA to point
      ValueType distanceOfClosestApproach( const HepPoint3D& aPoint) const;
      ValueType dca(const HepPoint3D& aPoint) const {
	 return distanceOfClosestApproach(aPoint); };

      /// DCA to 2-D point
      ValueType distanceOfClosestApproachBend( const HepPoint3D& aPoint) const;
      ValueType dcaBend(const HepPoint3D& aPoint) const {
	 return distanceOfClosestApproachBend(aPoint); };

      /// PCA to line
      HepPoint3D pointOfClosestApproach(const HepPoint3D& aPoint,
					  const HepVector3D& aEta) const;
      HepPoint3D pca(const HepPoint3D& aPoint,
		       const HepVector3D& aEta) const {
	 return pointOfClosestApproach(aPoint, aEta); };

      /// DCA to line
      ValueType distanceOfClosestApproach( const HepPoint3D& aPoint,
	                                   const HepVector3D& aEta) const;
      ValueType dca( const HepPoint3D& aPoint,
		     const HepVector3D& aEta) const {
	 return distanceOfClosestApproach(aPoint, aEta); };


      DABoolean transportMatrix(const KTHelix& aOldHelix,
				HepMatrix& aDerivativeMatrix) const {
	 return (KTMoveControl::kMoveOK == 
		 calculate5x5DerivativeMatrix(aOldHelix,
					      aDerivativeMatrix) ); }

      // static member functions

   protected:
      // protected member functions

      MoveStatus moveToLineIterate( const HepPoint3D&  aPoint,
				    const HepVector3D& aDirCosine,
				    ValueType& aDistanceMoved,
				    const MoveDirection aDirection
				         = KTMoveControl::kDirectionEither);

      MoveStatus moveToPlaneIterate( const HepPoint3D&  aPoint,
				     const HepVector3D& aNormal,
				     ValueType& aDistanceMoved,
				     const MoveDirection aDirection
				          = KTMoveControl::kDirectionEither);

      MoveStatus moveToCylinderIterate( const HepPoint3D&  aCenter,
					const HepVector3D& aDirCosine,
					const ValueType aRadius,
					ValueType& aDistanceMoved,
					const MoveDirection aDirection
				           = KTMoveControl::kDirectionEither);

      MoveStatus moveIterate( const KTMoveControl::MoveSpec& aMoveSpec,
			      ValueType& aDistanceMoved,
			      const MoveDirection aDirection
			           = KTMoveControl::kDirectionEither);


      // protected const member functions

   private:
      // Constructors and destructor

      // private member functions
      void calculateHelixParameters( const KTKinematicData& aKinematicData ,
				     const ValueType        aBFieldMagnitude,
				     const HepPoint3D&    aReferencePoint
				                  = HepPoint3D(0,0,0) ) ;

      MoveStatus moveToXYPlane( const HepPoint3D&  aPoint,
				const HepVector3D& aNormal,
				ValueType& aDistanceMoved,
				const MoveDirection aDirection
				          =  KTMoveControl::kDirectionEither);

      MoveStatus moveToXYCylinder( const HepPoint3D& aCenter,
				   const ValueType aRadius,
				   ValueType& aDistanceMoved,
				   const MoveDirection aDirection
				           = KTMoveControl::kDirectionEither);

      void calculate5x5ErrorMatrixFrom7x7ErrorMatrix(
	                         const KTKinematicData& aKinematicData);

      // private const member functions
      MoveStatus calculate5x5DerivativeMatrix(
					  const KTHelix& aOldHelix,
				          HepMatrix& aDerivativeMatrix) const;

      // private static member functions
      static const HepSymMatrix& nullErrorMatrix();

      // data members
      ValueType m_curvature;    // 1 / (2 * radius of curvature)
      ValueType m_phi0;         // phi at point of closest approach to
                                //    reference point
      ValueType m_d0;           // Distance of closest approach to ref. point
      ValueType m_cotTheta;     // cot(theta)
      ValueType m_z0;           // z position at point of closest approach
                                //    to reference point
      HepPoint3D m_referencePoint;  // Reference point for helix
      HepSymMatrix m_errorMatrix;  // 5x5 covariance matrix for helix
                                    // Order is (curv, phi0, d0, cotTheta, Z0)

      DABoolean m_hasNullErrorMatrix; // TRUE if track has no error matrix

      // ********* Future Optimization ********
      // To avoid unnecessarily moving the covariance matrix when a track
      // is projected (which involves a large amount of computation), we keep
      // a copy of the old track information. Only when the error information
      // is requested will the covariance matrix be updated.
      //
      // KTHelix* m_oldHelix;
      //
      enum TransportMethods { kNone,
			      kArcLength,
                              kPoint,
			      kXYPoint,
			      kZPosition,
			      kRadius,
			      kPlane,
			      kLine } m_transportMethod;
      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "KinematicTrajectory/Template/KTHelix.cc"
//#endif

#endif /* KINEMATICTRAJECTORY_KTHelix_H */
