// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIZDLayerCylinder
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Jul 15 14:57:20 EDT 2003
// $Id: HIZDLayerCylinder.cc,v 1.1 2003/07/26 03:27:48 mccann Exp $
//
// Revision history
//
// $Log: HIZDLayerCylinder.cc,v $
// Revision 1.1  2003/07/26 03:27:48  mccann
// adding file for ZD surfaces
//

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

// user include files
//#include "Experiment/report.h"
#include "HelixIntersection/HIZDLayerCylinder.h"


//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HelixIntersection.HIZDLayerCylinder" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIZDLayerCylinder.cc,v 1.1 2003/07/26 03:27:48 mccann Exp $";
static const char* const kTagString = "$Name:  $";

const string HIZDLayerCylinder::kSurfaceType = "HIZDLayerCylinder" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HIZDLayerCylinder::HIZDLayerCylinder()
{
}

HIZDLayerCylinder::HIZDLayerCylinder(
   const DGConstMaterialPtr& aInnerMaterial,
   const DGConstMaterialPtr& aOuterMaterial,
   const AEWireLayerCylindrical& aWireLayer,
   int aLayerNumber,
   const DGTransform& aTransform,
   HICondition* aCondition,
   HIHelixOperation* aOperation )
   : HIDRLayerCylinder( aInnerMaterial,
			aOuterMaterial,
			aWireLayer,
			aLayerNumber,
			aTransform,
			aCondition,
			aOperation )
{
}

// HIZDLayerCylinder::HIZDLayerCylinder( const HIZDLayerCylinder& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIZDLayerCylinder::~HIZDLayerCylinder()
{
}

//
// assignment operators
//
// const HIZDLayerCylinder& HIZDLayerCylinder::operator=( const HIZDLayerCylinder& rhs )
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
