#if !defined(KINEMATICTRAJECTORY_KTMOVECONTROL_H)
#define KINEMATICTRAJECTORY_KTMOVECONTROL_H
// -*- C++ -*-
//
// Package:     KinematicTrajectory
// Module:      KTMoveControl
// 
// Description: Controls how moves requiring iteration are handled for
//              KTHelix and KTKinematicMoveable objects
//
// Usage:
//    <usage>
//
// Author:      Paul Avery
// Created:     Sat May 16 13:51:15 EDT 1998
// $Id: KTMoveControl.h,v 1.4 2000/06/15 20:16:54 pappas Exp $
//
// Revision history
//
// $Log: KTMoveControl.h,v $
// Revision 1.4  2000/06/15 20:16:54  pappas
// Double checked and committed Paul Avery's modifications eliminating
// references to Toolbox classes. Should be completely transparent since
// Toolbox is just typedefs.
//
// Revision 1.3  1998/12/09 22:09:35  avery
// Added notion of a "sticky" distance when moving. Fixed some small errors
// in moving KTHelix and KTKinematicData objects over large arcs. Implemented
// some missing set functions.
//
// Revision 1.2  1998/07/09 21:33:45  avery
// Bug fixes, finished implementation of move routines, updated documentation.
//
// Revision 1.1  1998/05/23 17:20:34  avery
// Add KTMoveControl.h
//

// system include files

// user include files
#include "CLHEP/Geometry/Vector3D.h" // For 3-Vector declarations
#include "CLHEP/Geometry/Point3D.h"  // For 3-Point declarations

// forward declarations
class KTHelix;
class KTKinematicMoveable;

class KTMoveControl
{
      // friend classes and functions

  public:
      // constants, enums and typedefs

      typedef double ValueType;

      enum MoveStatus {kMoveOK,
		       kMoveNotDone,
		       kMoveNoIntersection, 
		       kMoveAlgorithmFailed,
                       kMoveCovarianceMoveFailed};

      enum MoveDirection {kDirectionForward, 
			  kDirectionBackward,
			  kDirectionEither};

      // Special classes
      // This class encapsulates the information on how the helix is
      // approximated.
      class ApproxParameters
      {
	 // Approximate the helix by a parabola, where the distance "s"
	 // to the desired point satisfies the equation
	 //
	 //       quadratic*s^2 + 2*linear*s + constant = 0

	 public:
	    ApproxParameters(const ValueType aQuadratic,
			     const ValueType aLinear,
			     const ValueType aConstant) :
	       m_quadratic(aQuadratic),
	       m_linear(aLinear),
	       m_constant(aConstant) { };
//	    ~ApproxParameters();

	    ValueType quadratic() const { return m_quadratic; };
	    ValueType linear()    const { return m_linear; };
	    ValueType constant()  const { return m_constant; };

	 private:
	    double m_quadratic;
	    double m_linear;
	    double m_constant;
      };



    // New classes for specifying the kind of move being done. These
    // classes are used to define virtual functions in KTKinematicMoveable
    // and KTHelix.
      class MoveSpec // Base class from which all the others inherit
      {
	 public:
	    MoveSpec() { }

	    virtual ~MoveSpec();

	    virtual ApproxParameters calcParameters(
	                   const KTKinematicMoveable& aParticle) const = 0;

      };

      // These classes specify the move type. They are used to automatically
      // select the correct move function through inheritance and virtual
      // functions.
      class Point3D : public MoveSpec
      {
	 public:
	    Point3D( const HepPoint3D& aPoint) :
	       m_point(aPoint) { } 

	    const HepPoint3D point() const { return m_point; } 

	    virtual ApproxParameters calcParameters(
	                        const KTKinematicMoveable& aParticle) const;

	 private:
	    HepPoint3D m_point;
      };

      class Point2D : public MoveSpec
      {
	 public:
	    Point2D( const HepPoint3D& aPoint) :
	       m_point(aPoint) { } ;

	    const HepPoint3D point() const { return m_point; } ;

	    virtual ApproxParameters calcParameters(
	                        const KTKinematicMoveable& aParticle) const;

	 private:
	HepPoint3D m_point;  //2-D point
      };

      class Line : public MoveSpec
      {
	 public:
	    Line( const HepPoint3D&  aPoint,
		  const HepVector3D& aDirCosine) :
	       m_point(aPoint),
	       m_dirCosine(aDirCosine) { } ;

	    const HepPoint3D point()      const { return m_point; } ;
	    const HepVector3D dirCosine() const { return m_dirCosine; } ;

	    virtual ApproxParameters calcParameters(
                                const KTKinematicMoveable& aParticle) const;

	 private:
	    HepPoint3D  m_point;
	    HepVector3D m_dirCosine;
      };

      class Plane : public MoveSpec
      {
	 public:
	    Plane( const HepPoint3D&  aPoint,
		   const HepVector3D& aNormal) :
	       m_point(aPoint),
	       m_normal(aNormal) { } ;

	    const HepPoint3D  point()  const { return m_point; } ;
	    const HepVector3D normal() const { return m_normal; } ;

	    virtual ApproxParameters calcParameters(
	                         const KTKinematicMoveable& aParticle) const;

	 private:
	    HepPoint3D  m_point;
	    HepVector3D m_normal;
      };

      class Cylinder : public MoveSpec
      {
	 public:
	    Cylinder( const HepPoint3D&  aCenter,
		      const HepVector3D& aDirCosine,
		      const ValueType      aRadius) :
	       m_center(aCenter),
	       m_dirCosine(aDirCosine),
               m_radius(aRadius) { } ;

	    const HepPoint3D  center()    const { return m_center; } ;
	    const HepVector3D dirCosine() const { return m_dirCosine; } ;
	          ValueType     radius()    const { return m_radius; } ;

	    virtual ApproxParameters calcParameters(
	                        const KTKinematicMoveable& aParticle) const;

	 private:
	    HepPoint3D  m_center;
	    HepVector3D m_dirCosine;
	    double        m_radius;
      };


      // Constructors and destructor

      KTMoveControl();

      KTMoveControl( const unsigned int aMaxIterations,
		     const ValueType    aMaxTurns,
		     const ValueType    aMaxArcLength);

      KTMoveControl( const KTMoveControl& aMoveControl );

      virtual ~KTMoveControl();

      // member functions

      void setMinStepSize        ( const ValueType aMinStep );
      void setMaxStepSize        ( const ValueType aMaxStep );
      void setMaxStepSizeFromRho ( const ValueType aRho,
				   const ValueType aSinTheta);
      void setPossibleStepSize   ( const ValueType aRho,
				   const ValueType aSinTheta,
				   const ValueType aStep);

      MoveStatus update ( const ValueType aRho,
			  const ValueType aStep);

      // const member functions
      unsigned int maxIterations( ) const { return m_maxIterations;} ;
      ValueType    maxTurns( )      const { return m_maxTurns;} ;
      ValueType    maxArcLength( )  const { return m_maxArcLength;} ;

      unsigned int iterations( ) const { return m_iterations;} ;
      ValueType    turns( )      const { return m_turns;} ;
      ValueType    arcLength( )  const { return m_arcLength;} ;

      ValueType minStepSize( ) const  { return m_minStepSize;};
      ValueType maxStepSize( ) const  { return m_maxStepSize;};
      ValueType safeFraction( ) const { return m_safeFraction;};

      ValueType chooseArcLength( const ApproxParameters& aParameters,
				 const MoveDirection aDirection) const;

      // static member functions
      static ValueType maxErrorPerStep( ) { return m_maxErrorPerStep;} ;
      static ValueType calculateMaxStepSize( const ValueType aRho,
					     const ValueType aSinTheta);
      static void setMaxErrorPerStep( const ValueType aMaxErrorPerStep) ;

      static void setStoredMaxIterations( unsigned int aMaxIterations );
      static void setStoredMaxTurns( const ValueType aMaxTurns );
      static void setStoredMaxArcLength( const ValueType aMaxArcLength );
      static void setStoredMinStepSize( const ValueType aMinStepSize );

      static void setStickyDistance( const ValueType aStickyDistance );

      static ValueType stickyDistance( ) { return m_stickyDistance;};

      static DABoolean solveQuadratic( const ValueType aQuadratic,
				       const ValueType aLinear,
				       const ValueType aConstant,
				       ValueType& aS1,
				       ValueType& aS2);

      static int directionInteger( const MoveDirection aDirection);
      static ValueType fuzzyDistance( );

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor

      // assignment operator(s)
      const KTMoveControl& operator=( const KTMoveControl& );

      // private member functions

      // private const member functions

      // data members
      unsigned int m_maxIterations;   // Maximum number of iterations allowed
      ValueType    m_maxTurns;        // Maximum number of turns allowed
      ValueType    m_maxArcLength;    // Maximum total arc length allowed

      unsigned int m_iterations;      // Number of iterations
      ValueType    m_turns;           // Number of turns
      ValueType    m_arcLength;       // Total arc length traversed

      ValueType    m_minStepSize;     // Min single step along arc
      ValueType    m_maxStepSize;     // Max single step along arc allowed
      ValueType    m_safeFraction;    // Fraction of step to accept roots

      // static data members
      static ValueType m_maxErrorPerStep;  // Maximum perp displacement
                                           // error per step
      static unsigned int m_storedMaxIterations;
      static double       m_storedMaxTurns;
      static double       m_storedMaxArcLength;

      static double       m_storedMinStepSize;
      static double       m_storedSafeFraction;

      static double       m_stickyDistance;
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "KinematicTrajectory/Template/KTMoveControl.cc"
//#endif

#endif /* KINEMATICTRAJECTORY_KTMOVECONTROL_H */
