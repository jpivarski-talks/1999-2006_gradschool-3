// -*- C++ -*-
//
// Package:     IRGeom
// Module:      MaskMainGeometry
// 
// Description: Function class to create IRGeom Main Tungsten Mask
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 12:37:05 EDT 1999
// $Id: MaskMainGeometry.cc,v 1.2 2002/05/31 17:19:47 bkh Exp $
//
// Revision history
//
// $Log: MaskMainGeometry.cc,v $
// Revision 1.2  2002/05/31 17:19:47  bkh
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
#include "IRGeom/MaskMainGeometry.h"
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
#include "DetectorGeometry/DGVolumePolyCone.h"
#include "DetectorGeometry/DGVolumeCone.h"
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
typedef pair<IRreal,IRreal> IRpair;

static const char* const kFacilityString = "IRGeom.MaskMainGeometry" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: MaskMainGeometry.cc,v 1.2 2002/05/31 17:19:47 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
MaskMainGeometry::MaskMainGeometry(const Record& iRecord)
    : m_Record(iRecord)
{
}

// MaskMainGeometry::MaskMainGeometry( const MaskMainGeometry& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

MaskMainGeometry::~MaskMainGeometry()
{
}

//
// assignment operators
//
// const MaskMainGeometry& MaskMainGeometry::operator=( const MaskMainGeometry& rhs )
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
MaskMainGeometry::operator()(DGVolumeParentPlaced* pIRMother)
{
   report(INFO, kFacilityString) << "Constructing main mask." << endl;

   // ------------ get the materials ------------- //
   DGMaterialStore materials ;

   extract( m_Record, materials );

   DGConstMaterialPtr pTungsten = materials[ kTungstenMatID ];
   DGConstMaterialPtr pVacuum    = materials[ kVacuumMatID ];
   
   // ------------ Define some useful transformations ------------ //
   HepRotateX3D  flipZ( 180 * k_degrees );
   HepTransform3D  shiftZ;
   HepTransform3D  flipShiftZ;

   // Needed dimension from copper pipe;
   const IRreal kCuPipeOuterRadius1 = 2.77 * k_cm;
   const IRreal kCuPipeOuterRadius2 = 2.77 * k_cm;
   
   //   dimensions: Main Tungsten Mask
   const IRreal kMainWMaskZ1 = 21.1 * k_cm;
   const IRreal kMainWMaskZ2 = 25.6 * k_cm;
   const IRreal kMainWMaskOuterRadius1 = 5.72 * k_cm;
   const IRreal kMainWMaskOuterRadius2 = 6.62 * k_cm;
   
   const IRreal kMainWMaskZHalfLength = 
       (kMainWMaskZ2 - kMainWMaskZ1)/2.0;

   const IRpair kMainWMaskZ1Radii(kCuPipeOuterRadius1,
				   kMainWMaskOuterRadius1);

   const IRpair kMainWMaskZ2Radii(kCuPipeOuterRadius2,
				  kMainWMaskOuterRadius2);
   
   // Create Main Mask volumes

   DGVolumePtr spMainMask = 
       new DGVolumeCone(IRGeomParameters::kMainTungstenMaskVolName,
			pTungsten,
			kMainWMaskZ1Radii,
			kMainWMaskZ2Radii,
			kMainWMaskZHalfLength);

   shiftZ = HepTranslateZ3D(kMainWMaskZ1 + kMainWMaskZHalfLength);
   DGPlacedVolumePtr spEastMainMask = 
       new DGPlacedVolume( spMainMask,
			   1,
			   shiftZ,
			   DGTransform::Identity);


   pIRMother->addChild(spEastMainMask);
}

//
// const member functions
//

//
// static member functions
//
