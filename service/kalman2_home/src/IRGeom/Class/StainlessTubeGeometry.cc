// -*- C++ -*-
//
// Package:     IRGeom
// Module:      StainlessTubeGeometry
// 
// Description: Function class to create IRGeom Stainless Steel Support tube 
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 14:31:41 EDT 1999
// $Id: StainlessTubeGeometry.cc,v 1.2 2002/05/31 17:19:48 bkh Exp $
//
// Revision history
//
// $Log: StainlessTubeGeometry.cc,v $
// Revision 1.2  2002/05/31 17:19:48  bkh
// Repair so does not overlap DR or CC endcap and also
// make it one sided only and replicate that side for negative z
//
// Revision 1.1.1.1  1999/07/22 14:38:42  lkg
// First release
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "IRGeom/StainlessTubeGeometry.h"
#include "Experiment/report.h"
#include "Experiment/units.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FAItem.h"           

#include "IRGeom/IRGeometryProxy.h"
#include "IRGeom/IRGeomParameters.h"

#include "DetectorGeometry/DGVolume.h"
#include "DetectorGeometry/DGVolumePtr.h"
#include "DetectorGeometry/DGMaterial.h"
#include "DetectorGeometry/DGConstMaterialPtr.h"
#include "DetectorGeometry/DGPlacedVolume.h"
#include "DetectorGeometry/DGPlacedVolumePtr.h"
#include "DetectorGeometry/DGZrRList.h"
#include "DetectorGeometry/DGVolumeTube.h"
#include "DetectorGeometry/DGVolumeParentPlaced.h"

#include "DetectorGeometry/DGMaterialStore.h"
#include "DetectorGeometry/DGDetectorStore.h"

#include "MaterialDelivery/material_identifiers.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
typedef double IRreal;
typedef pair<IRreal, IRreal> IRpair;
static const char* const kFacilityString = "IRGeom.StainlessTubeGeometry" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: StainlessTubeGeometry.cc,v 1.2 2002/05/31 17:19:48 bkh Exp $";
static const char* const kTagString = "$Name: v01_01_02 $";

//
// static data member definitions
//

//
// constructors and destructor
//
StainlessTubeGeometry::StainlessTubeGeometry(const Record& iRecord)
    : m_Record(iRecord)
{
}

// StainlessTubeGeometry::StainlessTubeGeometry( const StainlessTubeGeometry& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

StainlessTubeGeometry::~StainlessTubeGeometry()
{
}

//
// assignment operators
//
// const StainlessTubeGeometry& StainlessTubeGeometry::operator=( const StainlessTubeGeometry& rhs )
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

void
StainlessTubeGeometry::operator()(DGVolumeParentPlaced* pIRMother)
{
   report(INFO, kFacilityString) << "Constructing stainless support tubes." 
				 << endl;

   // ------------ get the materials ------------- //
   DGMaterialStore materials ;

   extract( m_Record, materials );

   DGConstMaterialPtr pStainless = materials[kStainless316MatID];

   // ------------ Define some useful transformations ------------ //
   HepRotateX3D  flipZ( 180 * k_degrees );
   HepTransform3D  shiftZ;
   HepTransform3D  flipShiftZ;

   // Position coordinates
   const IRreal kStainlessZStart = 64.01 * k_cm;
   const IRreal kStainlessZEnd = 216.49 * k_cm;

   // Stainless Tube dimensions
   const IRreal kStainlessInnerRadius = 4.375 * k_in;
   const IRreal kStainlessOuterRadius = 5.375 * k_in;
   const IRreal kStainlessZHalfLength 
       =  ((kStainlessZEnd - kStainlessZStart)/2.0);

   // Create stainless tube volumes
   IRpair kStainlessRadii( kStainlessInnerRadius,
			   kStainlessOuterRadius );

   DGVolumePtr spStainlessTube =
       new DGVolumeTube( IRGeomParameters::kStainlessTubeVolName,
			 pStainless,
			 kStainlessRadii,
			 kStainlessZHalfLength);

   shiftZ = HepTranslateZ3D(kStainlessZStart + kStainlessZHalfLength);
   DGPlacedVolumePtr spEastStainlessTube = 
       new DGPlacedVolume( spStainlessTube,
			   1,
			   shiftZ,
			   DGTransform::Identity);

   pIRMother->addChild(spEastStainlessTube);
}

//
// const member functions
//

//
// static member functions
//
