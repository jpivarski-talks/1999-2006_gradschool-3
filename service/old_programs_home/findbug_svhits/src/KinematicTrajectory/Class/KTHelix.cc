// -*- C++ -*-
//
// Package:     KinematicTrajectory
// Module:      KTHelix
// 
// Description: Methods which handle helical tracks
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Paul Avery
// Created:     Wed Oct 29 09:59:23 EST 1997
// $Id: KTHelix.cc,v 1.23 2001/02/01 01:34:54 cdj Exp $
//
// Revision history
//
// $Log: KTHelix.cc,v $
// Revision 1.23  2001/02/01 01:34:54  cdj
// properly handle null error matricies
//
// Revision 1.22  2000/06/15 20:16:47  pappas
// Double checked and committed Paul Avery's modifications eliminating
// references to Toolbox classes. Should be completely transparent since
// Toolbox is just typedefs.
//
// Revision 1.21  1999/11/21 22:08:09  wsun
// Sped up position(), moveErrorMatrix(), and setErrorMatrix().
//
// Revision 1.20  1999/10/08 22:18:42  marsh
// Changed TBThreeVector to TBThreePoint.
//
// Revision 1.19  1999/09/10 19:58:48  wsun
// Fixed bug in moveToZPosition().
//
// Revision 1.18  1999/08/18 21:39:28  wsun
// Fixed bug in transport matrix calculation.
//
// Revision 1.17  1999/05/28 00:47:33  wsun
// Fixed bug in moveIterate() -- move helix even if error matrix is null.
//
// Revision 1.16  1999/04/30 21:27:19  wsun
// Fixed transport matrix calculation and ref pt for moveToLine().
//
// Revision 1.15  1999/02/11 20:58:16  avery
// Fix moveByArcLength to get the z position correct.
//
// Revision 1.14  1999/02/10 15:03:16  avery
// Add moveByArcLength method in KTHelix. Modify moveToZPosition to call it.
//
// Revision 1.13  1998/12/09 22:09:01  avery
// Added notion of a "sticky" distance when moving. Fixed some small errors
// in moving KTHelix and KTKinematicData objects over large arcs. Implemented
// some missing set functions.
//
// Revision 1.12  1998/11/09 12:56:31  avery
// Replace abs(...) by fabs(...)
//
// Revision 1.11  1998/10/23 03:17:20  wsun
// Fixed arc length calculation.
//
// Revision 1.9  1998/10/13 17:14:08  avery
// - Added functions to return momentum and position.
// - Added iterate move routines to allow moves to more general surfaces
// - Added arc length argument in move routines
// - Fixed bug in argument list in call to KTKinematicFixedBz constructor
//
// Revision 1.8  1998/07/09 21:32:43  avery
// Bug fixes, finished implementation of move routines, updated documentation.
//
// Revision 1.7  1998/06/01 18:24:05  avery
// Fixed problem with copy constructor
//
// Revision 1.6  1998/05/24 21:30:27  avery
// Add move, PCA routines. Some cleanup
//
// Revision 1.5  1997/12/19 19:42:16  avery
// Changed B field constant to reflect use of KGauss instead of Tesla.
//
// Revision 1.4  1997/12/09 22:14:02  avery
// Added dummy implementation for pointOfClosestApproach and
// xyPointOfClosestApproach so that linking with g++ works.
//
// Revision 1.3  1997/12/03 22:03:12  mkl
// improved documentation; fixed g++ problems (warning about unused parameters etc.)
//
// Revision 1.2  1997/11/19 21:57:26  avery
// Added detailed documentation to header file. Implemented
// moveToZPosition method. Modified moveToRadius to better handle the
// way phi0 was updated. Improve the way null error matrix information
// is returned.
//
// Revision 1.1.1.1  1997/11/04 23:38:45  cdj
// First submission
//
// Revision 1.1.1.1  1997/11/04 22:45:14  avery
// First submission
//
//

#include "Experiment/Experiment.h"

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
#include "Experiment/report.h"
#include "CLHEP/Matrix/Matrix.h"
#include "KinematicTrajectory/KTKinematicData.h"
#include "KinematicTrajectory/KTKinematicFixedBz.h"
#include "KinematicTrajectory/KTHelix.h"

// STL classes

//
// constants, enums and typedefs
//

typedef KTMoveControl::MoveStatus    MoveStatus;

const double kBFieldConstant = -0.0299792458;

static const char* const kFacilityString = "KinematicTrajectory.KTHelix" ;

//
// static data member definitions
//

//
// constructors and destructor
//
KTHelix::KTHelix() :
   m_curvature( 0. ),
   m_phi0( 0. ),
   m_d0( 0. ),
   m_cotTheta( 0. ),
   m_z0( 0. ),
   m_referencePoint( HepPoint3D(0,0,0) ),
   m_errorMatrix( 0 ),
   m_hasNullErrorMatrix( true )
{
}

KTHelix::KTHelix( const ValueType aCurvature,
		  const ValueType aPhi0,
		  const ValueType aD0,
		  const ValueType aCotTheta,
		  const ValueType aZ0,
		  const HepPoint3D& aReferencePoint) :
   m_curvature( aCurvature ),
   m_phi0( aPhi0 ),
   m_d0( aD0 ),
   m_cotTheta( aCotTheta ),
   m_z0( aZ0 ),
   m_referencePoint( aReferencePoint ),
   m_errorMatrix( 0 ),
   m_hasNullErrorMatrix( true )
{
}

KTHelix::KTHelix( const ValueType aCurvature,
		  const ValueType aPhi0,
		  const ValueType aD0,
		  const ValueType aCotTheta,
		  const ValueType aZ0,
		  const HepSymMatrix& aErrorMatrix,
		  const HepPoint3D& aReferencePoint) :
   m_curvature( aCurvature ),
   m_phi0( aPhi0 ),
   m_d0( aD0 ),
   m_cotTheta( aCotTheta ),
   m_z0( aZ0 ),
   m_referencePoint( aReferencePoint ),
   m_errorMatrix( 0 ),
   m_hasNullErrorMatrix( true )
{
   setErrorMatrix( aErrorMatrix );
}


// Copy constructor
KTHelix::KTHelix( const KTHelix& aHelix) :
   m_curvature( aHelix.m_curvature ),
   m_phi0( aHelix.m_phi0 ),
   m_d0( aHelix.m_d0 ),
   m_cotTheta( aHelix.m_cotTheta ),
   m_z0( aHelix.m_z0 ),
   m_referencePoint( aHelix.m_referencePoint ),
   m_errorMatrix( 0 ),
   m_hasNullErrorMatrix( true )
{
   // Copy error matrix if it exists
   if(! aHelix.hasNullErrorMatrix() ) {
      setErrorMatrix( *aHelix.m_errorMatrix );
   }
}


// Similar to copy constructor with optional argument for copying
// over error matrix
KTHelix::KTHelix( const KTHelix& aHelix,
		  const DABoolean aCopyErrorMatrix) :
   m_curvature( aHelix.m_curvature ),
   m_phi0( aHelix.m_phi0 ),
   m_d0( aHelix.m_d0 ),
   m_cotTheta( aHelix.m_cotTheta ),
   m_z0( aHelix.m_z0 ),
   m_referencePoint( aHelix.m_referencePoint ),
   m_errorMatrix( 0 ),
   m_hasNullErrorMatrix( true )
{
   // Copy error matrix if is requested and if it exists
   if(aCopyErrorMatrix && ! aHelix.hasNullErrorMatrix() ) {
      setErrorMatrix( *aHelix.m_errorMatrix );
   }
}


KTHelix::KTHelix( const KTKinematicData& aKinematicData,
		  const ValueType        aBFieldMagnitude,
		  const HepPoint3D&    aReferencePoint) :
   m_referencePoint( aReferencePoint ),
   m_errorMatrix( 0 ),
   m_hasNullErrorMatrix( true )
{
   calculateHelixParameters(aKinematicData,
			    aBFieldMagnitude,
			    aReferencePoint);
}



KTHelix::KTHelix( const KTKinematicData& aKinematicData,
		  const HepVector3D&   aMagneticField,
		  const HepPoint3D&    aReferencePoint) :
   m_referencePoint( aReferencePoint ),
   m_errorMatrix( 0 ),
   m_hasNullErrorMatrix( true )
{
   calculateHelixParameters(aKinematicData,
			    aMagneticField.z(),
			    aReferencePoint);
}



KTHelix::KTHelix( const KTKinematicFixedBz& aKinematicFixedBz,
		  const HepPoint3D&       aReferencePoint) :
   m_referencePoint( aReferencePoint ),
   m_errorMatrix( 0 ),
   m_hasNullErrorMatrix( true )
{
   calculateHelixParameters(aKinematicFixedBz,
			    aKinematicFixedBz.BFieldMagnitude(),
			    aReferencePoint);
}


KTHelix::~KTHelix()
{
   delete m_errorMatrix;
}

//
// assignment operators
//
const KTHelix& KTHelix::operator=( 
   const KTHelix& aParameters )
{
   if( this != &aParameters ){
      m_curvature = aParameters.curvature();
      m_phi0 = aParameters.phi0();
      m_d0 = aParameters.d0();
      m_cotTheta = aParameters.cotTheta();
      m_z0 = aParameters.z0();
      m_referencePoint = aParameters.referencePoint();

      m_hasNullErrorMatrix = aParameters.hasNullErrorMatrix();
      if( ! aParameters.hasNullErrorMatrix() ){
	 setErrorMatrix( aParameters.errorMatrix() );
      }
   }
   return ( *this ) ;
}

// wsun 9-25-98: comparison operators
DABoolean KTHelix::operator==( const KTHelix& aHelix ) const
{
   return ( m_curvature == aHelix.curvature() &&
            m_phi0      == aHelix.phi0() &&
            m_d0        == aHelix.d0() &&
            m_cotTheta  == aHelix.cotTheta() &&
            m_z0        == aHelix.z0() &&
            m_referencePoint == aHelix.referencePoint() ) ;
}
 
DABoolean KTHelix::operator!=( const KTHelix& aHelix ) const
{
   return !( *this == aHelix ) ;
}

//
// member functions
//
void
KTHelix::calculateHelixParameters( const KTKinematicData& aKinematicData,
				   const ValueType        aBFieldMagnitude,
				   const HepPoint3D&    aReferencePoint)
{
   // Calculate helix parameters from kinematic data information and
   // B field.
   //
   // **************************************************************
   // Note: It is assumed that the B field is along the z direction
   // **************************************************************

   double A = kBFieldConstant * aBFieldMagnitude * aKinematicData.charge();

   double px = aKinematicData.px();
   double py = aKinematicData.py();
   double pz = aKinematicData.pz();
   double pt = sqrt(px*px + py*py);

   HepPoint3D delta(aKinematicData.position() - aReferencePoint);
   double cross = (delta.x()*py - delta.y()*px) / pt;
   double dot =   (delta.x()*px + delta.y()*py) / pt;

   double s;     // Arc length to move to get to PCA to reference point
   double d0;    // Distance of closest approach to ref. point in bend plane
   double rho_perp = A / pt;

   if(0. == A) {
      d0  = -cross;
      s = dot;
   }
   else {
      double dx_and_dy_squared = delta.x()*delta.x() + delta.y()*delta.y();
      double disc = sqrt(1. - 2. * rho_perp * cross
			    + rho_perp*rho_perp * dx_and_dy_squared );
      d0   = (-2.*cross + rho_perp * dx_and_dy_squared) / (disc + 1.);
      px = (px + A*delta.y()) / disc;
      py = (py - A*delta.x()) / disc;
      s = atan2(rho_perp*dot, 1.-rho_perp*cross) / rho_perp;
   }

      setCurvature(0.5 * rho_perp);
      setPhi0(atan2(py, px));
      setD0(d0);
      setCotTheta(pz / pt);
      setZ0(delta.z() - cotTheta() * s);

      calculate5x5ErrorMatrixFrom7x7ErrorMatrix(aKinematicData);
}

void
KTHelix::setCurvature( const ValueType aCurvature)
{
   m_curvature = aCurvature;
}

void
KTHelix::setPhi0( const ValueType aPhi0)
{
   m_phi0 = aPhi0;
   if(m_phi0 < 0.) {m_phi0 += (2.*M_PI);}
}

void
KTHelix::setD0( const ValueType aD0)
{
   m_d0 = aD0;
}

void
KTHelix::setCotTheta( const ValueType aCotTheta)
{
   m_cotTheta = aCotTheta;
}

void
KTHelix::setZ0( const ValueType aZ0)
{
   m_z0 = aZ0;
}

void
KTHelix::setReferencePoint(const HepPoint3D& aReferencePoint)
{
   m_referencePoint = aReferencePoint;
}

void
KTHelix::clearErrorMatrix()
{
   delete m_errorMatrix;
   m_errorMatrix = 0;
   m_hasNullErrorMatrix = true;
}

void
KTHelix::setErrorMatrix(const HepSymMatrix& aMatrix)
{
   const unsigned int kMatrixSize = 5;

   assert( kMatrixSize == aMatrix.num_row() );

//    HepSymMatrix* oldError = m_errorMatrix ;
//    m_errorMatrix = new HepSymMatrix( aMatrix ) ;
//    if ( 0 == m_errorMatrix ) {
//       m_errorMatrix = oldError ;

//       // report error if `new' fails
//       report( EMERGENCY ) << "No memory to create KTHelix error matrix"
// 			  << endl ;
//       exit( 1 ) ;
//    }

   //Check to see if new Matrix is null
   m_hasNullErrorMatrix = true;
   for(unsigned int column = 1; column <= kMatrixSize; ++column) {
      for(unsigned int row = column; row <= kMatrixSize; ++row) {
	 if(0 != aMatrix.fast(row,column)) {
	    m_hasNullErrorMatrix = false;
	    break;
	 }
      }
   }
//   delete oldError ;

   if( !m_hasNullErrorMatrix )
   {
      if( m_errorMatrix == 0 )
      {
	 m_errorMatrix = new HepSymMatrix( aMatrix ) ;
      }
      else
      {
	 *m_errorMatrix = aMatrix ;
      }
   }
//    else
//    {
//       delete m_errorMatrix ;
//       m_errorMatrix = 0 ;
//    }
}


KTHelix::MoveStatus
KTHelix::moveByArcLength(ValueType aDistanceMoved)
{
   // Move the reference point of a helix to a point that is a specified
   // arc length along the helix. The reference point is moved to this
   // new position


   KTHelix saveHelix(*this);   // Save current values in order to move
                               // error matrix later


   // Calculate the arc length in the bend plane
   const double sp = aDistanceMoved / sqrt(1. + cotTheta()*cotTheta());

   double rho = 2. * curvature();

   double u0 = cos(phi0());
   double v0 = sin(phi0());

   double x00 = -d0() * v0 + referencePoint().x();
   double y00 =  d0() * u0 + referencePoint().y();
   double z00 =  z0() + referencePoint().z();

   double xint;  // x intersection point
   double yint;  // y intersection point
   double zint;  // z intersection point

   if(0. == rho) {
      xint = x00 + u0*sp;
      yint = y00 + v0*sp;
      zint = z00 + cotTheta()*sp;
   }
   else {
      double cosps = cos(rho*sp);
      double sinps = sin(rho*sp);
      double dcosps = 1. - cosps;

      double uint = u0*cosps - v0*sinps;
      double vint = v0*cosps + u0*sinps;

      xint = x00 + (u0*sinps - v0*dcosps) / rho;
      yint = y00 + (v0*sinps + u0*dcosps) / rho;
      zint = z00 + cotTheta()*sp;

      m_phi0 = atan2(vint, uint);
      if(m_phi0 < 0.) {              // 0 <= phi0 <= 2*Pi
	 m_phi0 += (2.*M_PI);
      }
   }

   // Use the intersection point as the new reference point
   m_d0 = 0;
   m_z0 = 0;
   m_referencePoint = HepPoint3D(xint, yint, zint);
   

   // Move error matrix and return status
   MoveStatus errorStatus = moveErrorMatrix(saveHelix);

   return errorStatus;
}

KTHelix::MoveStatus
KTHelix::moveToReferencePoint(const HepPoint3D& aReferencePoint,
			      ValueType& aDistanceMoved,
			      const MoveDirection aDirection)
{
   // Move the reference point of a helix to a new position. This will
   // change the parameters phi0, d0 and z0 since they are defined at
   // the distance of closest appriach to the reference point.
   //
   // The algorithm is based on the following equations. Let the new
   // parameters be denoted by the suffix "_n". Define the quantities
   //
   //  dx  = xref_n - xref
   //  dy  = yref_n - yref
   //
   //  u0 = cos(phi0)
   //  v0 = sin(phi0)
   //
   //  dx' = u0*dx + v0*dy
   //  dy' =-v0*dx + u0*dy
   //
   // cos(phi0_n) = [u0*(1+rho*d0) - rho*dy] / T
   // sin(phi0_n) = [v0*(1+rho*d0) + rho*dx] / T
   //
   // (1+rho*d0_n)*cos(rho*s) = 1+rho*d0 + rho*[v0*dx - u0*dy]
   //                         = 1+rho*d0-rho*dy'
   // (1+rho*d0_n)*sin(rho*s) = rho*[u0*dx + v0*dy] = rho*dx'
   //
   // Thus we can solve for the arc length, and z0.
   //
   // sp = arctan(sinps/cosps) / rho
   // z0_n = z0 + zref - cot(theta) * sp
   //
   // d0_n = (1./rho)* (T - 1)
   //      = [2*(d0 - dy') + rho * (-d0^2 + dx'^2 + dy'^2)] / (T + 1)
   //
   // T    = sqrt( (1+rho*d0-rho*dy')^2 + rho^2*dx'^2 )


   KTHelix saveHelix(*this);   // Save current values in order to move
                               // error matrix later

   HepPoint3D delta = aReferencePoint - referencePoint();

   const double rho = 2. * curvature();

   const double phi_old = phi0();
   const double d0_old = d0();
   const double z0_old = z0();

   const double u0 = cos(phi_old);
   const double v0 = sin(phi_old);

   const double x_new =  u0 * delta.x() + v0 * delta.y();
   const double y_new = -v0 * delta.x() + u0 * delta.y();

   double phi_new;
   double d0_new;
   double z0_new;

   double sperp;

   aDistanceMoved = 0.;   // Set the distance moved to zero initially

   int sDirection = KTMoveControl::directionInteger(aDirection);

   if(rho == 0.) {
      sperp = x_new;

     // If track is already pinned to the point, there is no other solution
     if(fabs(sperp) < KTMoveControl::stickyDistance()) {
	sperp = 0;
     }
      if(sperp <= 0. && aDirection == KTMoveControl::kDirectionForward  ||
	 sperp >= 0. && aDirection == KTMoveControl::kDirectionBackward) {
	return KTMoveControl::kMoveNoIntersection;
      }
      phi_new = phi_old;
      d0_new = d0_old - y_new;

   } else {
      const double rhod0 = 1. + rho * d0_old;

      // sinphi and cosphi are not normalized to sum to 1.0
      const double cosphi_new = cos(phi_old)*rhod0 - rho * delta.y();
      const double sinphi_new = sin(phi_old)*rhod0 + rho * delta.x();
      phi_new = atan2(sinphi_new, cosphi_new);
      if(phi_new < 0.) {
	 phi_new += 2. * M_PI;
      }

      const double cosps = 1. + rho * (d0_old - y_new);
      const double sinps = rho * x_new;
      const double circum = 2. * M_PI / fabs( rho ) ;

      sperp = (1./rho) * atan2(sinps, cosps);

      // If track is already pinned to the point, add full circle if
      // direction specified
      if(fabs(sperp) < KTMoveControl::stickyDistance()) {
	 sperp = circum * sDirection;
      }
      
      // Add full circle if in wrong direction
      if( sperp < 0. && aDirection == KTMoveControl::kDirectionForward ) {
	 sperp += circum;
      }
      else if( sperp > 0. && aDirection == KTMoveControl::kDirectionBackward) {
	 sperp += -circum;
      }

      const double T = sqrt(cosphi_new*cosphi_new + sinphi_new*sinphi_new);
      d0_new = (2.*rhod0*(d0_old - y_new) + rho * (-d0_old*d0_old
					  + delta.x() * delta.x()
					  + delta.y() * delta.y())) / (T + 1.);
   }

   // z0' = z0 + zref + ct*s - zref' = z0 + ct*s - dz
   z0_new = z0_old + cotTheta()*sperp - delta.z();

   m_d0 = d0_new;
   m_phi0 = phi_new;
   m_z0 = z0_new;

   m_referencePoint = aReferencePoint;

   // Move error matrix and return status
   MoveStatus errorStatus = moveErrorMatrix(saveHelix);

   aDistanceMoved = sperp * sqrt(1. + cotTheta()*cotTheta());
   return errorStatus;
}


KTHelix::MoveStatus
KTHelix::moveToLine(const HepPoint3D&  aPoint,
		    const HepVector3D& aDirCosine,
		    ValueType& aDistanceMoved,
		    const MoveDirection aDirection)
{
  // Check if line is axial. If so, use reference point move
  if(aDirCosine.x() == 0. && aDirCosine.y() == 0.) {
    return moveToReferencePoint(aPoint, aDistanceMoved, aDirection);
  }

  // Otherwise do the move iteratively
  return moveToLineIterate( aPoint, aDirCosine, aDistanceMoved, aDirection );
}


KTHelix::MoveStatus
KTHelix::moveToPlane(const HepPoint3D&  aPoint,
		     const HepVector3D& aNormal,
		     ValueType& aDistanceMoved,
		     const MoveDirection aDirection)
{
  // Move reference point to the point where the helix intersects a
  // plane.

  // Check to see if we can use the Z plane routine
  if(aNormal.x() == 0. && aNormal.y() == 0.) {
    return moveToZPosition(aPoint.z(), aDistanceMoved, aDirection);
  }

  // Check to see if we can use the XY plane routine
  if(aNormal.z() == 0.) {
    return moveToXYPlane(aPoint, aNormal, aDistanceMoved, aDirection);
  }

  // Otherwise do the move iteratively
  return moveToPlaneIterate( aPoint, aNormal, aDistanceMoved, aDirection );
}


KTHelix::MoveStatus
KTHelix::moveToZPosition(const ValueType aZPosition,
			 ValueType& aDistanceMoved,
			 const MoveDirection aDirection)
{
   // Moves the helix reference point to the point of intersection of a
   // helix with a plane at a specified z position.
   //
   //    The algorithm starts with the helix equations of motion
   //
   //      x = x0 + xref + (u0/rho)*sin(rho*sp) - (v0/rho)*(1-cos(rho*sp))
   //      y = y0 + yref + (v0/rho)*sin(rho*sp) + (u0/rho)*(1-cos(rho*sp))
   //      z = z0 + zref + ct*sp
   //
   // where
   //    (xref,yref,zref) is the reference point for the helix
   //    (x0,y0,z0) is the point of closest approach to (xref,yref,zref)
   //    (u0,v0,ct) = (px/pt,py/pt,pz/pt) at that point
   //    rho = 1/radius of curvature
   //    sp is the arc length in the x-y plane from (x0,y0,z0).
   //
   // The arc length is easily computed to be
   //
   //    sp = (z - z0) / ct

   aDistanceMoved = 0.;   // Set the distance moved to zero initially

   double helixZ = position().z() ;

   // Quit if helix has no z component
   if(0. == cotTheta()) {
      // wsun 9-8-99: z0() --> position().z()
      //      if(z0() != aZPosition) {
      if(helixZ != aZPosition) {
	 return KTMoveControl::kMoveNoIntersection;  // Diff. z pos. ... error
      }
      else {
	 return KTMoveControl::kMoveOK;   // Same z position ... OK
      }
   }

   // Calculate arc length to plane (trivial for solenoidal field)

   // wsun 9-8-99: z0() --> position().z()
   //   double sp = (aZPosition - z0()) / cotTheta();
   double sp = (aZPosition - helixZ) / cotTheta();

   int sDirection = KTMoveControl::directionInteger(aDirection);

   // If track is already pinned to the plane, put it there exactly
   if(fabs(sp) < KTMoveControl::stickyDistance()) {
      sp = 0.;  // Reference point moves, even with 0 length move
      // Cannot return here yet.
   }

   // if(arc length corresponds to wrong direction, quit
   if(sp <= 0. && KTMoveControl::kDirectionForward  == aDirection  ||
      sp >= 0. && KTMoveControl::kDirectionBackward == aDirection ) {
      return KTMoveControl::kMoveNoIntersection;
   }

   // Calculate total distance moved
   aDistanceMoved = sp * sqrt(1. + cotTheta()*cotTheta());
   return moveByArcLength(aDistanceMoved);

}


KTHelix::MoveStatus
KTHelix::moveToXYPlane(const HepPoint3D&  aPoint,
		       const HepVector3D& aNormal,
		       ValueType& aDistanceMoved,
		       const MoveDirection aDirection)
{
  // Move reference point to the point where the helix intersects a
  // plane. The plane is assumed to have its normal in the xy plane.
  //
  //  The equation of the plane is (x-xp)*cosb + (y-yp)*sinb = 0
  //
  //  where eta   = (cosb, sinb, 0)
  //
  //    The algorithm starts with the helix equations of motion
  //
  //      x = x0 + xref + (u0/rho)*sin(rho*sp) - (v0/rho)*(1-cos(rho*sp))
  //      y = y0 + yref + (v0/rho)*sin(rho*sp) + (u0/rho)*(1-cos(rho*sp))
  //      z = z0 + zref + ct*sp
  //
  //    where
  //    (xref,yref,zref) is the reference point for the helix
  //    (x0,y0,z0) is the point of closest approach to (xref,yref,zref)
  //    (u0,v0,ct) = (px/pt,py/pt,pz/pt) at that point
  //    rho = 1/radius of curvature
  //    sp is the arc length in the x-y plane from (x0,y0,z0).
  //
  //   Thus the equation of the plane can be written
  //
  //        u0_eta * sin(ps) - v0_eta * (1 - cos(ps)) + rho*del_eta = 0
  //
  //   where the rotated quantities are defined as
  //
  //   u0_eta = u0*cosb + v0*sinb
  //   v0_eta = v0*cosb - u0*sinb
  //   del_eta = (x0+xr-xp)*cosb + (y0+yr-yp)*sinb
  //
  //   To solve, we use the auxiliary quantity w = (2/rho)*sin(ps/2)
  //
  //         sin(ps)     = rho*w*sqrt(1 - rho^2*w^2/4)
  //         1 - cos(ps) = rho^2*w^2 / 2
  //
  //    0.25*rho^2*w^4 - (rho*v0_eta*del_eta + u0_eta^2)*W^2 + del_eta^2 = 0
  //
  //   The solution is
  //
  //   Near solution
  //   1 - cos(ps) = (rho*del_eta)^2 / (B + u0_eta*R)
  //       sin(ps) = -rho*del_eta * (u0_eta + R) / (B + u0_eta*R)
  //
  //   Far solution
  //   1 - cos(ps) = (B + u0_eta*R)
  //       sin(ps) = (u0_eta*(v0_eta - rho*del_eta) + v0_eta*R)
  //
  //       R = sqrt(u0_eta^2 + 2.*rho*del_eta*v0_eta - (rho*del_eta)^2)
  //       B = u0_eta^2 + rho*del_eta*v0_eta

  if(aNormal.z() != 0.) {
    return KTMoveControl::kMoveAlgorithmFailed;
  }

   aDistanceMoved = 0.;   // Set the distance moved to zero initially
   KTHelix saveHelix(*this);   // Save current values in order to move
                               // error matrix later

   HepPoint3D refPoint = referencePoint();

   ValueType rho = 2. * curvature();
   ValueType d0_old = d0();
   ValueType phi_old = phi0();
   ValueType u0 = cos(phi_old);
   ValueType v0 = sin(phi_old);
   ValueType ct = cotTheta();

   const ValueType x0_old = -d0_old * v0;
   const ValueType y0_old =  d0_old * u0;
   const ValueType z0_old = z0();

   const ValueType delx = x0_old + refPoint.x() - aPoint.x();
   const ValueType dely = y0_old + refPoint.y() - aPoint.y();

   const ValueType u0_eta =  u0*aNormal.x() + v0*aNormal.y();
   const ValueType v0_eta = -u0*aNormal.y() + v0*aNormal.x();
   const ValueType del_eta = delx*aNormal.x() + dely*aNormal.y();

   // Up to two intersections are possible. Choose the one
   // first encountered in the direction of motion.

   // Need to treat the straight line case specially
   ValueType s, s1, s2;
   ValueType cosps, sinps;
   ValueType xint, yint, zint, uint, vint;

   int sDirection = KTMoveControl::directionInteger(aDirection);

   if(rho == 0.) {
     if(u0_eta == 0.) {
       return KTMoveControl::kMoveNoIntersection;
     }
     s = -del_eta / u0_eta;

     // If track is already pinned to the plane, put it there exactly
     if(fabs(s) < KTMoveControl::stickyDistance()) {
	s = 0.;
     }
     if(s <= 0. && aDirection == KTMoveControl::kDirectionForward ||
	s >= 0. && aDirection == KTMoveControl::kDirectionBackward) {
       return KTMoveControl::kMoveNoIntersection;
     }

     xint = x0_old + refPoint.x() + u0*s;
     yint = y0_old + refPoint.y() + v0*s;
     zint = z0_old + refPoint.z() + ct*s;

     uint = u0;
     vint = v0;
   }

   // Circular motion
   // Choose smallest positive arc length for forward projection
   // Choose smallest negative arc length for backward projection
   else {
     const ValueType rhoDelEta = rho * del_eta;
     ValueType B = u0_eta*u0_eta + rhoDelEta*v0_eta;
     ValueType disc = u0_eta*u0_eta + 2.*rhoDelEta*v0_eta
                                      - rhoDelEta*rhoDelEta;
     if(disc < 0.) {
       return KTMoveControl::kMoveNoIntersection;
     }

     disc = sqrt(disc);
     ValueType rhoinv = 1. / rho;
     ValueType circum = (2.*M_PI) * fabs(rhoinv);

     ValueType cosps1, sinps1, cosps2, sinps2;
     if(u0_eta >= 0.) {
	cosps1 = 1. - rhoDelEta*rhoDelEta / (B + u0_eta*disc);
	sinps1 =  -rhoDelEta * (u0_eta + disc) / (B + u0_eta*disc);
	cosps2 = 1. - (B + u0_eta*disc);
	sinps2 =  u0_eta*(v0_eta - rhoDelEta) + v0_eta*disc;
     }
     else {
	cosps1 = 1. - rhoDelEta*rhoDelEta / (B - u0_eta*disc);
	sinps1 =  -rhoDelEta * (u0_eta - disc) / (B - u0_eta*disc);
	cosps2 = 1. - (B - u0_eta*disc);
	sinps2 =  u0_eta*(v0_eta - rhoDelEta) - v0_eta*disc;
     }

     s1 = rhoinv * atan2(sinps1, cosps1);
     s2 = rhoinv * atan2(sinps2, cosps2);

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

     // Move forward
     if( aDirection == KTMoveControl::kDirectionForward ) {
       if(s1 < 0) {
	 s1 = s1 + circum;
       }
       if(s2 < 0) {
	 s2 = s2 + circum;
       }
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
     else if( aDirection == KTMoveControl::kDirectionBackward ) {
       if(s1 > 0) {
	 s1 = s1 - circum;
       }
       if(s2 > 0) {
	 s2 = s2 - circum;
       }
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

     // Move to closest intersection
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

     uint = u0*cosps - v0*sinps;
     vint = v0*cosps + u0*sinps;

     ValueType dcosps = 1. - cosps;

     xint = x0_old + refPoint.x() + (u0*sinps - v0*dcosps) * rhoinv;
     yint = y0_old + refPoint.y() + (v0*sinps + u0*dcosps) * rhoinv;
     zint = z0_old + refPoint.z() + ct*s;

     // Adjust phi0
     m_phi0 = atan2(vint, uint);
     if(m_phi0 < 0.) {              // 0 <= phi0 <= 2*Pi
       m_phi0 += (2.*M_PI);
     }
   }

   // Use the intersection point as the new reference point
   m_d0 = 0;
   m_z0 = 0;
   m_referencePoint = HepPoint3D(xint, yint, zint);

   // Move error matrix and return status
   MoveStatus errorStatus = moveErrorMatrix(saveHelix);

   // Calculate total distance moved
   aDistanceMoved = s * sqrt(1. + cotTheta()* cotTheta());
   return errorStatus;
}


KTHelix::MoveStatus
KTHelix::moveToRadius(const ValueType aRadius,
		      ValueType& aDistanceMoved,
		      const MoveDirection aDirection)
{
//   Move the helix reference point to the point of intersection of
//   the helix with a circle centered at the origin.
//

  HepPoint3D center(0., 0., 0.);
  return moveToXYCylinder(center, aRadius, aDistanceMoved, aDirection);
}


KTHelix::MoveStatus
KTHelix::moveToCylinder(const HepPoint3D& aCenter,
			const HepVector3D& aDirCosine,
			const ValueType aRadius,
			ValueType& aDistanceMoved,
			const MoveDirection aDirection)
{
//   Move the helix reference point to the point of intersection of
//   the helix with a cylinder.
//

  // Check to see if axis is aligned along z axis
  if(aDirCosine.x() == 0. && aDirCosine.y() == 0.) {
    return moveToXYCylinder(aCenter, aRadius, aDistanceMoved, aDirection);
  }

  // Otherwise do the move iteratively
  return moveToCylinderIterate( aCenter, aDirCosine, aRadius, aDistanceMoved,
				aDirection );
}


KTHelix::MoveStatus
KTHelix::moveToXYCylinder(const HepPoint3D& aCenter,
			  const ValueType aRadius,
			  ValueType& aDistanceMoved,
			  const MoveDirection aDirection)
{
//   Move the helix reference point to the point of intersection of
//   the helix with a cylinder aligned along the z axis.
//
//    The algorithm starts with the helix equations of motion
//
//      x = x0 + xref + (u0/rho)*sin(rho*sp) - (v0/rho)*(1-cos(rho*sp))
//      y = y0 + yref + (v0/rho)*sin(rho*sp) + (u0/rho)*(1-cos(rho*sp))
//      z = z0 + zref + ct*sp
//
//    where
//    (xref,yref,zref) is the reference point for the helix
//    (x0,y0,z0) is the point of closest approach to (xref,yref,zref)
//    (u0,v0,ct) = (px/pt,py/pt,pz/pt) at that point
//    rho = 1/radius of curvature
//    sp is the arc length in the x-y plane from (x0,y0,z0).
//
//    Squaring and adding the 2-d equations we get
//
//     0 = -delta + (2/rho)*A2*sin(rho*sp) + (2/rho^2)*A1*(1-cos(rho*sp))
//
//    where delta = r^2 - (x0+xr-xcenter)^2 - (yo+yr-ycenter)^2
//          A1 = 1 + rho*(u0*(y0+yr-ycenter) - v0*(x0+xr-xcenter))
//             = 1 + rho*da + rho*u0*(yr-xcenter) - rho*v0*(xr-xcenter)
//          A2 = u0*(x0+xr-xcenter) + v0*(y0+yr-ycenter)
//             = u0*(xr-xcenter) + v0*(yr-ycenter)
//
//    To solve this we introduce the variable w = (2/rho) * sin(rho*sp/2),
//    defined so that it is meaningful even when rho is zero. We have then
//
//              sin(rho*sp) = w * rho * sqrt(1 - (w*rho)^2/4)
//          1 - cos(rho*sp) = (w*rho)^2 / 2
//
//    the equation we must solve is
//
//  0 = (A1^2 + (A2*rho)^2)*w^4 - 2*(A1*delta+2*A2^2)*w^2 + delta^2
//
//    the solution for w^2 is
//
//      w^2 = [A1*delta + 2*A2^2 +- A2*disc] / [A1^2 + (A2*rho)^2]
//
//    where disc = sqrt(4*A1*delta + 4*A2^2 - (rho*delta)^2)
//
//    w^2 is essentially cos(rho*ps). To get a unique solution we need to
//    find sin(rho*sp). This can be found from the second equation above:
//
// sin(rho*sp) = rho*[delta - A1*w^2] / (2*A2)
//             = rho*[delta*A2*rho^2 - 2*A2*A1 -+ A1*disc]/2(A1^2+rho^2*A2^2)
//
//    where the signs are correlated with the solution for w^2. If there is
//    more than one solution, the one closest to the current point along the
//    forward/backward direction should be chosen.
//
//    if rho is 0 (straight line), the solution becomes
//
//          sp = -A2 +- sqrt(delta + A2^2)


   // A2 = u0*xref + v0*yref = r*cos(alpha)
   // A2 > 0 when moving away from center (the usual case)

   aDistanceMoved = 0.;        // Set the distance moved to zero initially
   KTHelix saveHelix(*this);   // Save current values in order to move
                               // error matrix later

   HepPoint3D refPoint = referencePoint();

   ValueType rho = 2. * curvature();
   ValueType d0_old = d0();
   ValueType phi_old = phi0();
   ValueType u0 = cos(phi_old);
   ValueType v0 = sin(phi_old);
   ValueType ct = cotTheta();

   ValueType x0_old = -d0_old * v0;
   ValueType y0_old =  d0_old * u0;
   ValueType z0_old = z0();

   ValueType delx = refPoint.x() - aCenter.x();
   ValueType dely = refPoint.y() - aCenter.y();

   ValueType addx = x0_old + delx;
   ValueType addy = y0_old + dely;

   ValueType A2 = u0*delx + v0*dely;
   ValueType delta = aRadius*aRadius - addx*addx - addy*addy;

   // If moving towards center, two intersections are possible. Choose
   // the one first encountered in the direction of motion.

   // Need to treat the straight line case specially
   ValueType disc, s, s1, s2;
   ValueType cosps, sinps;
   ValueType xint, yint, zint, uint, vint;

   int sDirection = KTMoveControl::directionInteger(aDirection);

   if(rho == 0.) {
      disc = (delta + A2*A2);
      if(disc < 0.) {  // No solution
	 return KTMoveControl::kMoveNoIntersection;
      }

      disc = sqrt(disc);
      s1 = -A2 - disc;
      s2 = -A2 + disc;

      // If track is already pinned to the surface, put it there exactly
      if(fabs(s1) < KTMoveControl::stickyDistance()) {
	 s1 = 0.;
      }
      if(fabs(s2) < KTMoveControl::stickyDistance()) {
	 s2 = 0.;
      }

      // Forward: Choose first root struck in forward direction
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

      // Backward: Choose first root struck in backward direction
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

      // Either direction
      else {
	 if (fabs(s1) <= fabs(s2)) {
	    s = s1;
	 }
	 else {
	    s = s2;
	 }
      }
   

      xint = x0_old + refPoint.x() + u0*s;
      yint = y0_old + refPoint.y() + v0*s;
      zint = z0_old + refPoint.z() + ct*s;

      uint = u0;
      vint = v0;
   }

   // Circular motion
   // Choose smallest positive arc length for forward projection
   // Choose smallest negative arc length for backward projection
   else {
      ValueType A1 = 1. + rho*d0_old + rho * (u0*dely - v0*delx);
      disc = 4. * (A1*delta + A2*A2) - (rho*delta)*(rho*delta);
      if(disc < 0.) {
	 return KTMoveControl::kMoveNoIntersection;
      }

      disc = sqrt(disc);
      ValueType den = 1. / (A1*A1 + (rho*A2)*(rho*A2));
      ValueType rhoinv = 1. / rho;
      ValueType circum = (2.*M_PI) * fabs(rhoinv);

      ValueType ww1 = (A1*delta + 2.*A2*A2 - A2*disc) * den;
      ValueType cosps1 = 1. - 0.5 * rho*rho * ww1;
      ValueType sinps1 =  0.5*rho * (rho*rho*A2*delta
                                      - 2.*A1*A2 + A1*disc) * den;
      s1 = rhoinv * atan2(sinps1, cosps1);

      ValueType ww2 = ww1 + 2.*A2*disc*den;
      ValueType cosps2 = 1. - 0.5 * rho*rho * ww2;
      ValueType sinps2 =  sinps1 - rho*A1*disc*den;
      s2 = rhoinv * atan2(sinps2, cosps2);

      // If track is already pinned to the surface, put it there exactly
      if(fabs(s1) < KTMoveControl::stickyDistance()) {
	 s1 = 0.;
      }
      if(fabs(s2) < KTMoveControl::stickyDistance()) {
	 s2 = 0.;
      }

     // Move forward
      if(aDirection == KTMoveControl::kDirectionForward) {
	 if(s1 < 0.) {
	    s1 = s1 + circum;
	 }
	 if(s2 < 0.) {
	    s2 = s2 + circum;
	 }
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
	 if(s1 > 0.) {
	    s1 = s1 - circum;
	 }
	 if(s2 > 0.) {
	    s2 = s2 - circum;
	 }
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

      uint = u0*cosps - v0*sinps;
      vint = v0*cosps + u0*sinps;

      ValueType dcosps = 1. - cosps;

      xint = x0_old + refPoint.x() + (u0*sinps - v0*dcosps) / rho;
      yint = y0_old + refPoint.y() + (v0*sinps + u0*dcosps) / rho;
      zint = z0_old + refPoint.z() + ct*s;

      // Adjust phi0
      m_phi0 = atan2(vint, uint);
      if(m_phi0 < 0.) {              // 0 <= phi0 <= 2*Pi
	 m_phi0 += (2.*M_PI);
      }
   }

   // Use the intersection point as the new reference point
   m_d0 = 0;
   m_z0 = 0;
   m_referencePoint = HepPoint3D(xint, yint, zint);

   // Move error matrix and return status
   MoveStatus errorStatus = moveErrorMatrix(saveHelix);

   // Calculate total distance moved
   aDistanceMoved = s * sqrt(1. + cotTheta()* cotTheta());
   return errorStatus;
}


KTHelix::MoveStatus
KTHelix::moveErrorMatrix( const KTHelix& aOldHelix)
{
   // Move the error matrix from one reference point to another. The old
   // helix parameters before the move are in aOldHelix.

   // Quit if no error matrix or if track has not moved.
   if( hasNullErrorMatrix() ) {return KTMoveControl::kMoveOK;}
   if(referencePoint() == aOldHelix.referencePoint() ) {
      return KTMoveControl::kMoveOK;
   }

   // Calculate derivative matrix and apply similarity transformation
   HepMatrix R(5,5,0);
   MoveStatus matrixReturn = calculate5x5DerivativeMatrix(aOldHelix, R);
   if( KTMoveControl::kMoveCovarianceMoveFailed != matrixReturn )
   {
//       // Calculate covariance matrix of new parameters: V_c' = R*V_c * R(t).
//       setErrorMatrix ( aOldHelix.errorMatrix().similarity( R ) );

      // HepSymMatrix::similarity() takes too long.  Do the similarity
      // transformation by hand.  Also, use our knowledge of which transport
      // matrix elements are zero to speed up the calculation.

      HepSymMatrix V( aOldHelix.errorMatrix() ) ;

      double phiK   = R( kPhi0, kCurvature ) ;
      double phiphi = R( kPhi0, kPhi0 ) ;
      double phid   = R( kPhi0, kD0 ) ;

      double dK   = R( kD0, kCurvature ) ;
      double dphi = R( kD0, kPhi0 ) ;
      double dd   = R( kD0, kD0 ) ;

      double zK   = R( kZ0, kCurvature ) ;
      double zphi = R( kZ0, kPhi0 ) ;
      double zd   = R( kZ0, kD0 ) ;
      double zt   = R( kZ0, kCotTheta ) ;

      // A = V_c * R(t)
      double Aphi[ 5 ], Ad[ 5 ], Az[ 6 ] ;
      for( int m = kCurvature ; m <= kCotTheta; ++m )
      {
	 Aphi[ m ] = 0. ;
	 Ad[ m ]   = 0. ;
	 Az[ m ]   = 0. ;
      }
      Az[ 5 ] = 0. ;

      for( int i = kCurvature ; i <= kCotTheta ; ++i )
      {
	 for( int j = kCurvature ; j <= kD0 ; ++j )
	 {
	    double Vij = V( i, j ) ;

	    Aphi[ i ] += Vij * R( kPhi0, j ) ;
	    Ad[ i ]   += Vij * R( kD0, j ) ;
	    Az[ i ]   += Vij * R( kZ0, j ) ;
	 }
	 Az[ i ] += V( i, kCotTheta ) * R( kZ0, kCotTheta ) + V( i, kZ0 ) ;
      }
      Az[ kZ0 ] =
	 V.fast( kZ0, kCurvature ) * R( kZ0, kCurvature ) +
	 V.fast( kZ0, kPhi0 )      * R( kZ0, kPhi0 ) +
	 V.fast( kZ0, kD0 )        * R( kZ0, kD0 ) +
	 V.fast( kZ0, kCotTheta )  * R( kZ0, kCotTheta ) +
	 V.fast( kZ0, kZ0 ) ;


      V.fast( kPhi0, kCurvature ) = Aphi[ kCurvature ] ;
      V.fast( kPhi0, kPhi0 ) =
	 Aphi[ kCurvature ] * R( kPhi0, kCurvature ) +
	 Aphi[ kPhi0 ]      * R( kPhi0, kPhi0 ) +
	 Aphi[ kD0 ]        * R( kPhi0, kD0 ) ;

      V.fast( kD0, kCurvature )   = Ad[ kCurvature ] ;
      V.fast( kD0, kPhi0 ) =
	 Aphi[ kCurvature ] * R( kD0, kCurvature ) +
	 Aphi[ kPhi0 ]      * R( kD0, kPhi0 ) +
	 Aphi[ kD0 ]        * R( kD0, kD0 ) ;
      V.fast( kD0, kD0 ) =
	 Ad[ kCurvature ] * R( kD0, kCurvature ) +
	 Ad[ kPhi0 ]      * R( kD0, kPhi0 ) +
	 Ad[ kD0 ]        * R( kD0, kD0 ) ;

      V.fast( kCotTheta, kPhi0 ) = Aphi[ kCotTheta ] ;
      V.fast( kCotTheta, kD0 )   = Ad[ kCotTheta ] ;

      V.fast( kZ0, kCurvature )   = Az[ kCurvature ] ;
      V.fast( kZ0, kPhi0 ) =
	 Az[ kCurvature ] * R( kPhi0, kCurvature ) +
	 Az[ kPhi0 ]      * R( kPhi0, kPhi0 ) +
	 Az[ kD0 ]        * R( kPhi0, kD0 ) ;
      V.fast( kZ0, kD0 ) =
	 Az[ kCurvature ] * R( kD0, kCurvature ) +
	 Az[ kPhi0 ]      * R( kD0, kPhi0 ) +
	 Az[ kD0 ]        * R( kD0, kD0 ) ;
      V.fast( kZ0, kCotTheta )    = Az[ kCotTheta ] ;
      V.fast( kZ0, kZ0 ) =
	 Az[ kCurvature ] * R( kZ0, kCurvature ) +
	 Az[ kPhi0 ]      * R( kZ0, kPhi0 ) +
	 Az[ kD0 ]        * R( kZ0, kD0 ) +
	 Az[ kCotTheta ]  * R( kZ0, kCotTheta ) +
	 Az[ kZ0 ]        * R( kZ0, kZ0 ) ;

      setErrorMatrix( V ) ;
   }
   return matrixReturn;
}


KTMoveControl::MoveStatus
KTHelix::moveToLineIterate( const HepPoint3D&  aPoint,
			    const HepVector3D& aDirCosine,
			    ValueType& aDistanceMoved,
			    const MoveDirection aDirection)
{
   // This function moves a KTHelix object to the point of closest approach
   // to the specified line using an iterative method.

   KTMoveControl::Line line(aPoint, aDirCosine);
   KTMoveControl::MoveStatus status =
      moveIterate(line, aDistanceMoved, aDirection);

   return status ;
}


KTMoveControl::MoveStatus
KTHelix::moveToPlaneIterate( const HepPoint3D&  aPoint,
			     const HepVector3D& aNormal,
			     ValueType& aDistanceMoved,
			     const MoveDirection aDirection)
{
  // This function moves a KTHelix object to the specified plane using
  // an iterative method.

   KTMoveControl::Plane plane(aPoint, aNormal);
   return moveIterate(plane, aDistanceMoved, aDirection);
}


KTMoveControl::MoveStatus
KTHelix::moveToCylinderIterate( const HepPoint3D&  aCenter,
				const HepVector3D& aDirCosine,
				const ValueType aRadius,
				ValueType& aDistanceMoved,
				const MoveDirection aDirection)
{
  // This function moves a KTHelix object to the specified cylinder using
  // an iterative method.

   KTMoveControl::Cylinder cylinder(aCenter, aDirCosine, aRadius);
   return moveIterate(cylinder, aDistanceMoved, aDirection);
}


KTMoveControl::MoveStatus
KTHelix::moveIterate( const KTMoveControl::MoveSpec& aMoveSpec,
		      ValueType& aDistanceMoved,
		      const MoveDirection aDirection)
{
  // This function moves a KTHelix object to the specified point,
  // line or surface defined by aMoveSpec using an iterative method.
  // The particular specification is obtained by the class inherited
  // from MoveSpec. The iteration process converges quadratically.


  // Do the transport using the KinematizFixedBz method on the
  // equivalent particle.
  // To build the particle, we need the B field. Since the momentum
  // is not known, we assume Pt = 1.0, charge = 1.0, mass = 1.0
  // and compute the B field necessary to give the correct curvature.

  aDistanceMoved = 0.;   // Set the distance moved to zero initially

  // Calculate the magnetic field assuming pt = 1. We don't care what
  // it really is since all we are trying to do is make a particle
  // that has the right curvature.
  double bField;
  if(curvature() == 0.) {
     bField = 0.;
  }
  else {
     bField = 2.*curvature() / kBFieldConstant;
  }

  // Build the particle. The covariance matrix is not copied over.
  KTKinematicFixedBz particle(*this, 1.0, bField, false);

  // Move the particle
  MoveStatus status = particle.moveIterate(aMoveSpec,
					   aDirection);
  if(status != KTMoveControl::kMoveOK) {
     return status;
  }

  // Everything OK. If there is a covariance matrix, set the reference
  // point to where the particle is now located and move the helix to it.
  // This can only be done in constant B fields in vacuum.

  //  if(!hasNullErrorMatrix()) {
     status = moveToReferencePoint(particle.position(),
				   aDistanceMoved,
				   aDirection);
     //  }
  return status;

}


//
// const member functions
//

const HepSymMatrix&
KTHelix::errorMatrix() const
{
   // Return the 5x5 error matrix. If the error matrix is null, return
   // a 5x5 matrix filled with zeros stored as a static variable.

   if(! hasNullErrorMatrix() ) {
      assert(0!= m_errorMatrix);
      return *m_errorMatrix;
   }
   else {
      return nullErrorMatrix();
   }
}

void
KTHelix::calculate5x5ErrorMatrixFrom7x7ErrorMatrix(
	                         const KTKinematicData& aKinematicData)
{
   if(aKinematicData.hasNullErrorMatrix()) {return;}

   const double pt = aKinematicData.pperp();
   const double A = 2. * curvature() * pt;
   const double denominator = 1. / (pt*pt);
   const double px = aKinematicData.px();
   const double py = aKinematicData.py();
   const double pz = aKinematicData.pz();

   HepMatrix R(5,7,0);

   R(kCurvature,KTKinematicData::kPx) = -px * (curvature() * denominator);
   R(kCurvature,KTKinematicData::kPy) = -py * (curvature() * denominator);

   R(kPhi0,KTKinematicData::kPx) = -py * denominator;
   R(kPhi0,KTKinematicData::kPy) =  px * denominator;
   R(kPhi0,KTKinematicData::kX)  = -px * (A * denominator);
   R(kPhi0,KTKinematicData::kY)  = -py * (A * denominator);

   R(kD0,KTKinematicData::kX) = -py / pt;
   R(kD0,KTKinematicData::kY) =  px / pt;

   R(kCotTheta,KTKinematicData::kPx) = -px * (cotTheta() * denominator);
   R(kCotTheta,KTKinematicData::kPy) = -py * (cotTheta() * denominator);
   R(kCotTheta,KTKinematicData::kPz) =  1. / pt;

   R(kZ0,KTKinematicData::kX) = -px * (pz * denominator);
   R(kZ0,KTKinematicData::kY) = -py * (pz * denominator);
   R(kZ0,KTKinematicData::kZ) = 1.;

   // Calculate covariance matrix of new parameters: V_c = R * V_K * R(t).
   setErrorMatrix ( aKinematicData.errorMatrix().similarity( R ) );

   return;
}


KTHelix::MoveStatus
KTHelix::calculate5x5DerivativeMatrix(const KTHelix& aOldHelix,
				      HepMatrix& aDerivativeMatrix) const
{
   // Calculate the derivative matrix d(new) / d(old) of the helix
   // parameters taken wrt a new and old reference point.

   // Make an alias for the derivative matrix to keep code readable
   HepMatrix& R = aDerivativeMatrix;

   const double dx = referencePoint().x() - aOldHelix.referencePoint().x();
   const double dy = referencePoint().y() - aOldHelix.referencePoint().y();

   const double rho = 2. * aOldHelix.curvature();
   const double rhoda = 1. + rho*aOldHelix.d0();
   const double rhoda_pr = 1. + rho*d0();

   const double den = 1. / rhoda_pr;

   const double u0 = cos(aOldHelix.phi0() );
   const double v0 = sin(aOldHelix.phi0() );

   const double xpr = u0*dx + v0*dy;
   const double ypr = u0*dy - v0*dx;

   R(kCurvature,kCurvature) = 1.;
   R(kPhi0,kCurvature) = 2.* xpr * (den*den);
   R(kPhi0,kPhi0) = rhoda * (rhoda - rho*ypr) * (den*den);
   R(kPhi0,kD0) = -(rho*rho) * xpr * (den*den);

   if(0. == rho) {
      R(kD0,kCurvature) = xpr*xpr;
   }
   else {
      const double den2 = rhoda_pr + rhoda - rho*ypr;
      if(0. == den2) {return KTMoveControl::kMoveCovarianceMoveFailed;}
      R(kD0,kCurvature) = 2. * xpr*xpr * den / den2;
   }

   R(kD0,kPhi0) = rhoda * xpr * den;
   R(kD0,kD0) = (rhoda - rho*ypr) * den;
   R(kCotTheta,kCotTheta) = 1.;

   double s; // arc length
   if(0. == rho) {
      s = xpr;
      R(kZ0,kCurvature) = -2.* aOldHelix.cotTheta() * xpr
	 * (aOldHelix.d0() - ypr);
   }
   else {
      // wsun 4-30-99: Use alternative formula for s -- mathematically the
      //               same but is probably faster and more accurate.
      //
      // wsun 8-18-97: Switch back to PRA's original formula because phi0
      //               in [0, 2pi] screws up cases where phi0 crosses zero.

      s = atan2(rho*xpr, rhoda - rho*ypr) / rho;
//      s = ( phi0() - aOldHelix.phi0() ) / rho ;
      R(kZ0,kCurvature) = 2.* aOldHelix.cotTheta() * (-s + xpr*den*den)/rho;
   }

   // wsun 4-30-99: Fixed typo dy-->dx.
   R(kZ0,kPhi0) = aOldHelix.cotTheta()
                        * (ypr*rhoda - rho*(dx*dx+dy*dy) ) * den*den;
//                        * (ypr*rhoda - rho*(dx*dy+dy*dy) ) * den*den;

   R(kZ0,kD0) = -aOldHelix.cotTheta() * rho * xpr * den*den;
   R(kZ0,kCotTheta) = s;
   R(kZ0,kZ0) = 1.;

   return KTMoveControl::kMoveOK;
}


HepPoint3D
KTHelix::position() const{

   // Return the point of closest approach in the bend plane to the
   // current reference point.

   if( fabs( d0() ) < 1.e-12 )
   {
      return HepPoint3D(referencePoint().x(),
			referencePoint().y(),
			referencePoint().z() + z0() );
   }
   else
   {
      return HepPoint3D(referencePoint().x() - d0()*sin(phi0()),
			referencePoint().y() + d0()*cos(phi0()),
			referencePoint().z() + z0() );
   }
}


HepVector3D
KTHelix::momentum(const HepVector3D& aMagneticField) const{

   // Return the 3-momentum at the point of closest approach in the
   // bend plane to the current reference point. It is assumed that
   // the charge is +-1.

   if(curvature() == 0.) {
      return HepVector3D(0.,0.,0.);

   } else {
      const double pperp = fabs(0.5 * kBFieldConstant * aMagneticField.z()
				 / curvature());
      return HepVector3D(pperp*cos(phi0()),
			 pperp*sin(phi0()),
			 pperp*cotTheta());
   }
}


HepPoint3D
KTHelix::pointOfClosestApproach(const HepPoint3D& ) const{

   // Return the point of closest approach (in 3-D) of the helix
   // to the specified point.

   assert(false); // Not implemented yet
   return 0.;
}


HepPoint3D
KTHelix::pointOfClosestApproachBend(const HepPoint3D& aPoint) const{

   cout << "pointOfClosestApproachBend( " << aPoint << " ) = ";

   // Return the point of closest approach (in bend plane) of the helix
   // to the specified xy point.
   //
   // Let the point be denoted (xp,yp,zp). Define the quantities
   //
   //  (xref,yref,zref) is the reference point for the helix
   //  (x0,y0,z0) is the point of closest approach to (xref,yref,zref)
   //  (u0,v0,ct) = (px/pt,py/pt,pz/pt) at that point
   //  rho = 1/radius of curvature
   //  sp is the arc length in the x-y plane from (x0,y0,z0).
   //
   //  dx  = xp - xref
   //  dy  = yp - yref
   //
   //  u0 = cos(phi0)
   //  v0 = sin(phi0)
   //
   //  dx' = u0*dx + v0*dy
   //  dy' =-v0*dx + u0*dy
   //
   // T*cos(rho*s) = 1+rho*d0 + rho*[v0*dx - u0*dy]
   //              = 1+rho*d0-rho*dy'
   // T*sin(rho*s) = rho*[u0*dx + v0*dy] = rho*dx'
   //
   // sp = arctan(sinps/cosps) / rho
   //
   // T    = sqrt( (1+rho*d0-rho*dy')^2 + rho^2*dx'^2 )
   //
   // The PCA can then be written
   //
   // x = x0 + (u0/rho)*sin(rho*sp) - (v0/rho) * (1 - cos(rho*sp))
   // y = y0 + (v0/rho)*sin(rho*sp) + (u0/rho) * (1 - cos(rho*sp))
   // z = z0 + cot(theta) * sp
   //
   // where x0 = -d0*v0 + xref
   //       y0 =  d0*u0 + yref
   //
   // When the helix is a straight line, the equations simplify to
   //
   // x = x0 + u0*sp
   // y = y0 + v0*sp
   // z = z0 + cot(theta)*sp

   const double dx = aPoint.x() - referencePoint().x();
   const double dy = aPoint.y() - referencePoint().y();

   const double rho = 2. * curvature();

   const double da = d0();

   const double u0 = cos(phi0());
   const double v0 = sin(phi0());

   const double dxPrime =  u0 * dx + v0 * dy;
   const double dyPrime = -v0 * dx + u0 * dy;

   double sperp;

   if(rho == 0.) {
      sperp = dxPrime;

      cout << HepPoint3D( -da*v0 + referencePoint().x() + u0*sperp,
			 +da*u0 + referencePoint().y() + v0*sperp,
			 +z0()  + referencePoint().z() + cotTheta()*sperp )
	   << endl;

      return HepPoint3D( -da*v0 + referencePoint().x() + u0*sperp,
			 +da*u0 + referencePoint().y() + v0*sperp,
			 +z0()  + referencePoint().z() + cotTheta()*sperp );


   } else {
      double cosps = 1. + rho * (da - dyPrime);
      double sinps = rho * dxPrime;
      const double T = sqrt(cosps*cosps + sinps*sinps);

      sinps = sinps / T;
      cosps = cosps / T;
      sperp = (1./rho) * atan2(sinps, cosps);

      double x, y, z;
      const double dcosps = 1. - cosps;

      x = -da*v0 + referencePoint().x() + (u0/rho)*sinps - (v0/rho)*dcosps;
      y =  da*u0 + referencePoint().y() + (v0/rho)*sinps + (u0/rho)*dcosps;
      z =  z0()  + referencePoint().z() + cotTheta() * sperp;

      cout << HepPoint3D(x,y,z) << endl;

      return HepPoint3D(x,y,z);
   }
}


KTHelix::ValueType
KTHelix::distanceOfClosestApproach(const HepPoint3D& ) const{

   // Return the distance of closest approach (in 3-D) of the helix
   // to the specified point.

   assert(false); // Not implemented yet
   return 0.;
}


KTHelix::ValueType
KTHelix::distanceOfClosestApproachBend(const HepPoint3D& aPoint) const{

   cout << "distanceOfClosestApproachBend( " << aPoint << " ) = ";

   // Return the distance of closest approach (in bend plane) of the helix
   // to the specified xy point.
   //
   //  The calculation is based on
   //
   //     dca = (1./rho)* (T - 1)
   //         = [ 2*delta / (sqrt(1+2*delta) + 1) ]
   //
   //      delta = da - dy' + (rho/2) * [(da-y')^2 + dx'^2]
   //
   //      dx = xp - xref
   //      dy = yp - yref
   //
   //     dx' = dx*cos(phi0) + dy*sin(phi0)
   //     dy' =-dx*sin(phi0) + dy*cos(phi0)

   const double u0 = cos(phi0());
   const double v0 = sin(phi0());

   const double rho = 2. * curvature();
   const double da = d0();

   const double dx = aPoint.x() - referencePoint().x();
   const double dy = aPoint.y() - referencePoint().y();

   const double dxPrime =  u0*dx + v0*dy;
   const double dyPrime = -v0*dx + u0*dy;

   const double daMinusYprime = da - dyPrime;

   // Straight line
   if(rho == 0.) {
      cout << daMinusYprime << endl;

      return ( daMinusYprime );
   }

   // Helix
   else {
      const double delta = daMinusYprime
	     + 0.5*rho * (daMinusYprime*daMinusYprime + dxPrime*dxPrime);
      const double T = sqrt(1. + 2.*delta);

      cout << ( 2.*delta / (T + 1.) ) << endl;

      return ( 2.*delta / (T + 1.) );
   }

}


HepPoint3D
KTHelix::pointOfClosestApproach(const HepPoint3D&,
				const HepVector3D& ) const{

   // Return the point of closest approach (in 3-D) of the helix
   // to the specified line.

   assert(false); // Not implemented yet
   return 0.;
}


KTHelix::ValueType
KTHelix::distanceOfClosestApproach(const HepPoint3D&,
				   const HepVector3D& ) const{

   // Return the distance of closest approach (in 3-D) of the helix
   // to the specified line.

   assert(false); // Not implemented yet
   return 0.;
}


//
// static member functions
//
const HepSymMatrix&
KTHelix::nullErrorMatrix()
{
   static HepSymMatrix null(5,0);
   return null;
}
