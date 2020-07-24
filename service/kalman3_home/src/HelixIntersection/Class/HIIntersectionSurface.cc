// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIIntersectionSurface
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Sep 22 18:06:19 EDT 1998
// $Id: HIIntersectionSurface.cc,v 1.17 2001/03/27 06:28:13 wsun Exp $
//
// Revision history
//
// $Log: HIIntersectionSurface.cc,v $
// Revision 1.17  2001/03/27 06:28:13  wsun
// * Added checks for floating exceptions.
// * Calculate d0 and z0 arc length derivatives only if surface has different
//   material on each side.
//
// Revision 1.16  2001/02/22 01:59:23  wsun
// Added flags to control deletion of condition and operation.
//
// Revision 1.15  2000/09/05 20:34:51  wsun
// Mods for forcing fitting weight to user-specified value.
//
// Revision 1.14  2000/02/22 00:19:01  wsun
// Added debug printout.
//
// Revision 1.13  2000/02/03 07:28:36  wsun
// Added use of HICalibratedHits.
//
// Revision 1.12  2000/01/11 20:28:08  wsun
// Check if helix is at surface in arcLengthDerivatives().
//
// Revision 1.11  1999/12/15 20:25:54  wsun
// * Sped up arcLengthDerivatives().
// * Modified interface for arcLengthDerivatives, derivatives() and normal()
//   to avoid instantiating temporary HepVectors and HepNormal3Ds.
//
// Revision 1.10  1999/11/21 22:31:39  wsun
// Sped up calculations.
//
// Revision 1.9  1999/09/02 19:13:55  wsun
// Check for divide-by-zero in arcLengthDerivatives().
//
// Revision 1.8  1999/09/01 23:57:34  wsun
// Check aLastSurface != this in HIIntersectionSurface::arcLengthDerivatives().
//
// Revision 1.7  1999/08/27 20:17:46  wsun
// Added arcLengthDerivatives() and changed signature for performOperation().
//
// Revision 1.6  1999/07/12 06:47:48  wsun
// New convention -- normal() points towards outer material.
//
// Revision 1.5  1999/06/24 22:25:00  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.4  1999/06/02 23:05:25  wsun
// Added features for low-momentum fitting.
//
// Revision 1.3  1999/04/28 22:57:23  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:06:29  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:22  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HICondition.h"
#include "HelixIntersection/HIHelixOperation.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "CLHEP/Geometry/Normal3D.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HelixIntersection.HIIntersectionSurface" ;

const string HIIntersectionSurface::kSurfaceType = "HIIntersectionSurface" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HIIntersectionSurface::HIIntersectionSurface()
   : m_printDiagnostics( false ),
     m_forceFittingWeight( false ),
     m_fittingWeight( 0. ),
     m_dtorDeletesCondition( true ),
     m_dtorDeletesOperation( false ),
     m_condition( 0 ),
     m_operation( 0 ),
     m_useReverseHitOrder( false )
{
}

HIIntersectionSurface::HIIntersectionSurface(
   const DGConstMaterialPtr& aInnerMaterial,
   const DGConstMaterialPtr& aOuterMaterial,
   Meters aRadius,
   HICondition* aCondition,
   HIHelixOperation* anOperation,
   DABoolean aDtorDeletesCondition,
   DABoolean aDtorDeletesOperation )
   : m_innerMaterial( aInnerMaterial ),
     m_outerMaterial( aOuterMaterial ),
     m_radius( aRadius ),
     m_condition( aCondition ),
     m_operation( anOperation ),
     m_printDiagnostics( false ),
     m_forceFittingWeight( false ),
     m_fittingWeight( 0. ),
     m_dtorDeletesCondition( true ),
     m_dtorDeletesOperation( false ),
     m_useReverseHitOrder( false )
{
}

// HIIntersectionSurface::HIIntersectionSurface( const HIIntersectionSurface& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIIntersectionSurface::~HIIntersectionSurface()
{
   if( m_dtorDeletesCondition )
   {
      deleteCondition() ;
   }

   if( m_dtorDeletesOperation )
   {
      deleteOperation() ; 
   }
}

//
// assignment operators
//
// const HIIntersectionSurface& HIIntersectionSurface::operator=( const HIIntersectionSurface& rhs )
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

const DGConstMaterialPtr&
HIIntersectionSurface::materialInLastTransport( const HIHelix& aHelix ) const
{
   // Normal points toward outer material.
   HepNormal3D norm ;
   normal( aHelix, norm ) ;
   return ( norm.dot( aHelix.momentumUnit() ) * aHelix.lastArcLength() > 0. ) ?
      m_innerMaterial : m_outerMaterial ;
}

void
HIIntersectionSurface::deleteCondition()
{
   delete m_condition ;
   m_condition = 0 ;
}


void HIIntersectionSurface::deleteOperation()
{
   delete m_operation ;
   m_operation = 0 ;
}

DABoolean
HIIntersectionSurface::conditionSatisfied( const HIHelix& aHelix )
{
   if( m_condition == 0 )
   {
      return true ;
   }

   return m_condition->satisfied( aHelix, *this ) ;
}


DABoolean
HIIntersectionSurface::performOperation(
   HIHelix& aHelix,
   const HIHelix& aLastHelix,
   const HIIntersectionSurface& aLastSurface )
{
   if( m_operation == 0 )
   {
      return true ;
   }

   // Make a local copy of aHelix because perform(...) takes references
   // to helices.  We don't want two helices in perform(...) to be assigned
   // to the same address.

   HIHelix savedHelix( aHelix ) ;
   DABoolean success = m_operation->perform( aHelix, savedHelix, aLastHelix,
					     *this, aLastSurface ) ;
   return success ;
}


void HIIntersectionSurface::arcLengthDerivatives(
   const HIHelix& aCurrentHelix,
   const HIHelix& aLastHelix,
   const HIIntersectionSurface& aLastSurface,
   HepVector& outputVector ) const
{
   if( outputVector.num_row() != HIHelix::kNTrackParameters )
   {
      report( WARNING, kFacilityString )
	 << "Vector to be filled by arcLengthDerivatives()"
	 << " has the wrong size." << endl ;
      return ;
   }

   Meters lastArc = aCurrentHelix.lastArcLength() ;
   double t = aCurrentHelix.cotTheta() ;
   double cscTh = sqrt( 1. + sqr( t ) ) ;

   // Radial derivatives:
   //
   //      r = d0 + sin( phi - phi0 ) / K
   // =>  sr ~ ( phi - phi0 ) / K          (sr = radial arc length = s/cscTh)
   //
   // =>  dK = -dPhi0 / sr = dD0 / sr^2
   //
   HepNormal3D normal ;
   this->normal( aCurrentHelix, normal ) ;

   Hep3Vector momUnit = aCurrentHelix.momentumUnit() ;

   double normX = normal.x() ;
   double normY = normal.y() ;
   double momX = momUnit.x() ;
   double momY = momUnit.y() ;

   double cotNormalRPt = 0. ;
   double normalDotPt = normX * momX + normY * momY ;
   if( normalDotPt != 0. )
   {
      cotNormalRPt = ( normX * momY - normY * momX ) / normalDotPt ;
   }

   double cotNormalRPtOld = 0. ;
   double dSdZ0Old = 0. ;

   // Do not calculate d0 and z0 derivatives for the current and last surfaces
   // if they have the same materials on either side.  In this case, the
   // arc length of this transport and the next (or previous) transport are
   // 100% correlated.  Hence, the contribution of arc length uncertainty to
   // the scattering angle will be double-counted.  If the materials differ,
   // there is still some correlation, but the mistake made is much smaller
   // since the densities of the two materials usually differ significantly.

   if( aLastHelix.atSurface() && &aLastSurface != this &&
       aLastSurface.innerMaterial() != aLastSurface.outerMaterial() )
   {
      HepNormal3D normalOld ;
      aLastSurface.normal( aLastHelix, normalOld ) ;

      Hep3Vector momUnitOld = aLastHelix.momentumUnit() ;

      double normX = normalOld.x() ;
      double normY = normalOld.y() ;
      double momX = momUnitOld.x() ;
      double momY = momUnitOld.y() ;

      // normalDotPtOld = rphi components of normalOld.dot( momUnitOld ) ;
      double normalDotPtOld = normX * momX + normY * momY ;

      if( normalDotPtOld != 0. )
      {
	 // cotNormalRPtOld =
	 // normalROld.cross( ptUnitOld ).z() / normalDotPtOld
	 cotNormalRPtOld = ( normX * momY - normY * momX ) / normalDotPtOld ;
      }

      double momUnitDotNormalOld = momUnitOld.dot( normalOld ) ;
      if( momUnitDotNormalOld != 0. )
      {
	 dSdZ0Old = normalOld.z() / momUnitDotNormalOld ;
      }

      if( printDiagnostics() )
      {
	 cout << aLastSurface.surfaceType() << " normalOld " << normalOld
	      << " cotNormRPt " << cotNormalRPtOld
	      << endl ;
      }
   }


   // dS/dPhi0
   outputVector( HIHelix::kPhi0 ) = lastArc * cotNormalRPt ;

   // dS/dK
   outputVector( HIHelix::kCurvature ) =
      -lastArc / cscTh * outputVector( HIHelix::kPhi0 );

   // dS/dt
   outputVector( HIHelix::kCotTheta ) = t / sqr( cscTh ) * lastArc ;

   // dS/dD0
   outputVector( HIHelix::kD0 ) = - cscTh * cotNormalRPtOld ;

   // dS/dZ0
   outputVector( HIHelix::kZ0 ) = dSdZ0Old ;

   if( m_innerMaterial != m_outerMaterial )
   {
      outputVector( HIHelix::kD0 ) += cscTh * cotNormalRPt ;

      double normalDotP = momUnit.dot( normal ) ;
      if( normalDotP != 0. )
      {
	 outputVector( HIHelix::kZ0 ) -= normal.z() / normalDotP ;
      }
   }

   if( printDiagnostics() )
   {
      cout << this->surfaceType() << " normal " << normal
	   << " cotNormRPt " << cotNormalRPt
      << endl ;
   }
}

//
// static member functions
//

