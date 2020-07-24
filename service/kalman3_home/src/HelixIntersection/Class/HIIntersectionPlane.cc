// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIIntersectionPlane
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Sep 25 16:46:54 EDT 1998
// $Id: HIIntersectionPlane.cc,v 1.7 1999/12/15 20:44:29 wsun Exp $
//
// Revision history
//
// $Log: HIIntersectionPlane.cc,v $
// Revision 1.7  1999/12/15 20:44:29  wsun
// Added data member for rphiHalfWidth.
//
// Revision 1.6  1999/06/24 22:25:00  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.5  1999/06/02 23:05:25  wsun
// Added features for low-momentum fitting.
//
// Revision 1.4  1999/05/03 19:30:51  wsun
// Force normal vector to be unit.
//
// Revision 1.3  1999/04/28 22:57:23  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:06:28  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:25  wsun
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
#include "HelixIntersection/HIIntersectionPlane.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HIIntersectionPlane" ;

const string HIIntersectionPlane::kSurfaceType = "HIIntersectionPlane" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HIIntersectionPlane::HIIntersectionPlane()
{
}

HIIntersectionPlane::HIIntersectionPlane(
   const DGConstMaterialPtr& aInnerMaterial,
   const DGConstMaterialPtr& aOuterMaterial,
   const HepNormal3D& aNormalToPlane,
   const HepPoint3D& aPointOnPlane,
   Meters aRphiHalfWidth,
   HICondition* aCondition,
   HIHelixOperation* anOperation ) :
   HIIntersectionSurface( aInnerMaterial,
			  aOuterMaterial,
			  aPointOnPlane.perp(),
			  aCondition,
			  anOperation ),
   m_normal( aNormalToPlane.unit() ),
   m_point( aPointOnPlane ),
   m_rphiHalfWidth( aRphiHalfWidth )
{
}

// HIIntersectionPlane::HIIntersectionPlane( const HIIntersectionPlane& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIIntersectionPlane::~HIIntersectionPlane()
{
}

//
// assignment operators
//
// const HIIntersectionPlane& HIIntersectionPlane::operator=( const HIIntersectionPlane& rhs )
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
HIIntersectionPlane::propagateHelix( HIHelix& helixToPropagate,
				     KTMoveControl::MoveDirection aDirection )
{
//    HIHelix savedHelix = helixToPropagate ;

//    if( printDiagnostics() )
//    {
//       cout
// 	 << "init pos/mom "
// 	 << helixToPropagate.position() << " / "
// 	 << helixToPropagate.momentum() << endl
// 	 << "SV pt/norm " << m_point << " / " << m_normal << endl ;
//    }


   // First do a move to reference point (= wire position) and then
   // move iteratively to a plane.
   KTMoveControl::MoveStatus status =
      helixToPropagate.moveToPlane( m_point, m_normal, aDirection ) ;

//    if( printDiagnostics() )
//    {
//       cout
// 	 << "move pos/mom " << helixToPropagate.position() << " / "
// 	 << helixToPropagate.momentum() << endl
// 	 << "direction " << aDirection << " arc length "
// 	 << helixToPropagate.lastArcLength() << endl ;
//    }

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

//
// static member functions
//
