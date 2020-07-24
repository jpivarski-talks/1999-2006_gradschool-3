// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIWireLayerCylinder
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Apr 20 14:34:20 EDT 1999
// $Id: HIWireLayerCylinder.cc,v 1.11 2003/09/01 19:43:35 wsun Exp $
//
// Revision history
//
// $Log: HIWireLayerCylinder.cc,v $
// Revision 1.11  2003/09/01 19:43:35  wsun
// Renamed HIDRLayerCylinder --> HIWireLayerCylinder.
//
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
#include "HelixIntersection/HIWireLayerCylinder.h"

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

static const char* const kFacilityString = "HIWireLayerCylinder" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIWireLayerCylinder.cc,v 1.11 2003/09/01 19:43:35 wsun Exp $";
static const char* const kTagString = "$Name:  $";

const string HIWireLayerCylinder::kSurfaceType = "HIWireLayerCylinder" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HIWireLayerCylinder::HIWireLayerCylinder()
{
}

HIWireLayerCylinder::HIWireLayerCylinder(
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

// HIWireLayerCylinder::HIWireLayerCylinder( const HIWireLayerCylinder& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIWireLayerCylinder::~HIWireLayerCylinder()
{
}

//
// assignment operators
//
// const HIWireLayerCylinder& HIWireLayerCylinder::operator=( const HIWireLayerCylinder& rhs )
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
