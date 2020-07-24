// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIIntersectionCylinder
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Sep 25 16:47:08 EDT 1998
// $Id: HIIntersectionCylinder.cc,v 1.11 2000/12/03 07:24:43 wsun Exp $
//
// Revision history
//
// $Log: HIIntersectionCylinder.cc,v $
// Revision 1.11  2000/12/03 07:24:43  wsun
// Use moveToRadius if possible.
//
// Revision 1.10  2000/01/11 20:30:42  wsun
// Removed helix copy from propagateHelix().
//
// Revision 1.9  1999/12/15 20:58:39  wsun
// Updated to new HIIntersectionSurface interface.
//
// Revision 1.8  1999/11/21 22:31:38  wsun
// Sped up calculations.
//
// Revision 1.7  1999/08/03 21:58:08  wsun
// Removed z-component of normal.
//
// Revision 1.6  1999/07/12 06:51:11  wsun
// Cleaned up code.
//
// Revision 1.5  1999/06/30 22:23:09  wsun
// Cylinders now keep a copy of the transform instead of a pointer.
//
// Revision 1.4  1999/06/24 22:24:59  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.3  1999/04/28 22:57:22  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:06:27  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:26  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HIIntersectionCylinder.h"

#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Vector3D.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HelixIntersection.HIIntersectionCylinder" ;

const string HIIntersectionCylinder::kSurfaceType = "HIIntersectionCylinder" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HIIntersectionCylinder::HIIntersectionCylinder()
{
}

HIIntersectionCylinder::HIIntersectionCylinder(
   const DGConstMaterialPtr& aInnerMaterial,
   const DGConstMaterialPtr& aOuterMaterial,
   Meters aRadius,
   const DGTransform& aTransform,
   HICondition* aCondition,
   HIHelixOperation* anOperation ) :
   HIIntersectionSurface( aInnerMaterial,
			  aOuterMaterial,
			  aRadius,
			  aCondition,
			  anOperation ),
   m_transformIsIdentity( aTransform == DGTransform::Identity ),
   m_transform( aTransform )
{
}

// HIIntersectionCylinder::HIIntersectionCylinder( const HIIntersectionCylinder& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIIntersectionCylinder::~HIIntersectionCylinder()
{
}

//
// assignment operators
//
// const HIIntersectionCylinder& HIIntersectionCylinder::operator=( const HIIntersectionCylinder& rhs )
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

DABoolean
HIIntersectionCylinder::propagateHelix(
   HIHelix& helixToPropagate,
   KTMoveControl::MoveDirection aDirection )
{
//   HIHelix savedHelix = helixToPropagate ;

   KTMoveControl::MoveStatus status ;

   if( printDiagnostics() )
   {
      cout << "init pos/mom: " << helixToPropagate.position() << " / "
	   << helixToPropagate.momentum() 
	   << " r " << helixToPropagate.position().perp() << endl ;
   }

   Hep3Vector dirCosine = m_transform * HepVector3D( 0., 0., 1. ) ;
   HepPoint3D center    = m_transform * HepPoint3D( 0., 0., 0. ) ;

   if( m_transformIsIdentity ||
       ( dirCosine == HepVector3D( 0., 0., 1. ) &&
	  center == HepPoint3D( 0., 0., 0. ) ) )
   {
      status = helixToPropagate.moveToRadius( radius(), aDirection ) ;
   }
   else
   {
      status = helixToPropagate.moveToCylinder( center,
						dirCosine,
						radius(),
						aDirection ) ;
   }

   if( printDiagnostics() )
   {
      cout << "move pos/mom: " << helixToPropagate.position() << " / "
	   << helixToPropagate.momentum()
	   << " r " << helixToPropagate.position().perp() << endl ;
   }

   DABoolean isOK = ( status == KTMoveControl::kMoveOK ) ;
//    if( !isOK )
//    {
//       helixToPropagate = savedHelix ;
//    }
   return isOK ;
}


//
// const member functions
//


void
HIIntersectionCylinder::normal( const HepPoint3D& aPoint,
				HepNormal3D& outputNormal ) const
{
   double norm = aPoint.perp() ;
   outputNormal.setX( aPoint.x() ) ;
   outputNormal.setY( aPoint.y() ) ;
   outputNormal.setZ( 0. ) ;

   if( !m_transformIsIdentity )
   {
      outputNormal = m_transform * outputNormal ;
   }

   outputNormal = outputNormal.unit() ;
}


void
HIIntersectionCylinder::normal( const HIHelix& aHelix,
				HepNormal3D& outputNormal ) const
{
   normal( aHelix.position(), outputNormal ) ;
}

//
// static member functions
//
