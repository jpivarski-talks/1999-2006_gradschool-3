// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIDRLayerCylinder
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Apr 20 14:34:20 EDT 1999
// $Id: HIDRLayerCylinder.cc,v 1.3 1999/06/30 22:23:08 wsun Exp $
//
// Revision history
//
// $Log: HIDRLayerCylinder.cc,v $
// Revision 1.3  1999/06/30 22:23:08  wsun
// Cylinders now keep a copy of the transform instead of a pointer.
//
// Revision 1.2  1999/04/28 22:57:18  wsun
// Fixes for Solaris.
//
// Revision 1.1  1999/04/27 22:06:22  wsun
// Major reorganization and structural changes.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "ActiveElements/AEWireLayerCylindrical.h"
#include "HelixIntersection/HIDRLayerCylinder.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HIDRLayerCylinder" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIDRLayerCylinder.cc,v 1.3 1999/06/30 22:23:08 wsun Exp $";
static const char* const kTagString = "$Name:  $";

const string HIDRLayerCylinder::kSurfaceType = "HIDRLayerCylinder" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HIDRLayerCylinder::HIDRLayerCylinder()
{
}

HIDRLayerCylinder::HIDRLayerCylinder(
   const DGConstMaterialPtr& aInnerMaterial,
   const DGConstMaterialPtr& aOuterMaterial,
   const AEWireLayerCylindrical& aWireLayer,
   int aLayerNumber,
   const DGTransform& aTransform,
   HICondition* aCondition,
   HIHelixOperation* aOperation )
   : HIIntersectionCylinder( aInnerMaterial,
			     aOuterMaterial,
			     aWireLayer.radius(),
			     aTransform,
			     aCondition,
			     aOperation ),
     m_wireLayer( &aWireLayer ),
     m_layerNumber( aLayerNumber )
{
}

// HIDRLayerCylinder::HIDRLayerCylinder( const HIDRLayerCylinder& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIDRLayerCylinder::~HIDRLayerCylinder()
{
}

//
// assignment operators
//
// const HIDRLayerCylinder& HIDRLayerCylinder::operator=( const HIDRLayerCylinder& rhs )
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

//
// static member functions
//
