// -*- C++ -*-
//
// Package:     IRGeom
// Module:      MaskHybridGeometry
// 
// Description: Function class to create IRGeom Hybrid (Phase 2-3) 
//              Tungsten Mask
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 12:36:45 EDT 1999
// $Id: MaskHybridGeometry.cc,v 1.2 2002/05/31 17:19:46 bkh Exp $
//
// Revision history
//
// $Log: MaskHybridGeometry.cc,v $
// Revision 1.2  2002/05/31 17:19:46  bkh
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
#include "IRGeom/MaskHybridGeometry.h"
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

static const char* const kFacilityString = "IRGeom.MaskHybridGeometry" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: MaskHybridGeometry.cc,v 1.2 2002/05/31 17:19:46 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
MaskHybridGeometry::MaskHybridGeometry(const Record& iRecord)
    : m_Record(iRecord)
{
}

// MaskHybridGeometry::MaskHybridGeometry( const MaskHybridGeometry& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

MaskHybridGeometry::~MaskHybridGeometry()
{
}

//
// assignment operators
//
// const MaskHybridGeometry& MaskHybridGeometry::operator=( const MaskHybridGeometry& rhs )
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
MaskHybridGeometry::operator()(DGVolumeParentPlaced* pIRMother)
{
   report(INFO, kFacilityString) << "Constructing Hybrid Mask." << endl;

   // ------------ get the materials ------------- //
   DGMaterialStore materials ;

   extract( m_Record, materials );

   DGConstMaterialPtr pTungsten = materials[ kTungstenMatID ];

   // ------------ Define some useful transformations ------------ //
   HepRotateX3D  flipZ( 180 * k_degrees );
   HepTransform3D  shiftZ;
   HepTransform3D  flipShiftZ;

   //   dimensions: Hybrid Tungsten Mask Part 1
   const IRreal kHybridWMask1Z1 = 33.1 * k_cm;
   const IRreal kHybridWMask1Z2 = 60.0 * k_cm;

   const IRreal kHybridWMask1InnerRadius1 = 3.05 * k_cm;
   const IRreal kHybridWMask1InnerRadius2 = 3.5 * k_cm;

   const IRreal kHybridWMask1OuterRadius1 = 3.68 * k_cm;
   const IRreal kHybridWMask1OuterRadius2 = 3.68 * k_cm;
   
   const IRreal kHybridWMask1ZHalfLength = 
       (kHybridWMask1Z2 - kHybridWMask1Z1)/2.0;

   const IRpair kHybridWMask1Z1Radii(kHybridWMask1InnerRadius1,
				     kHybridWMask1OuterRadius1);

   const IRpair kHybridWMask1Z2Radii(kHybridWMask1InnerRadius2,
				     kHybridWMask1OuterRadius2);
   
   // Create Hybrid Mask Part 1 volumes
   DGVolumePtr spHybridMask1 = 
       new DGVolumeCone(IRGeomParameters::kHybridTungstenMaskVol1Name,
			pTungsten,
			kHybridWMask1Z1Radii,
			kHybridWMask1Z2Radii,
			kHybridWMask1ZHalfLength);

   shiftZ = HepTranslateZ3D(kHybridWMask1Z1 + kHybridWMask1ZHalfLength);
   DGPlacedVolumePtr spEastHybridMask1 = 
       new DGPlacedVolume( spHybridMask1,
			   1,
			   shiftZ,
			   DGTransform::Identity);


   //   dimensions: Hybrid Tungsten Mask Part 2
   const IRreal kHybridWMask2Z1 = 33.1 * k_cm;
   const IRreal kHybridWMask2Z2 = 60.0 * k_cm;

   const IRreal kHybridWMask2InnerRadius = 3.94 * k_cm;
   const IRreal kHybridWMask2OuterRadius = 6.60 * k_cm;

   const IRreal kHybridWMask2ZHalfLength = 
       (kHybridWMask2Z2 - kHybridWMask2Z1)/2.0;

   const IRpair kHybridWMask2Radii(kHybridWMask2InnerRadius,
				   kHybridWMask2OuterRadius);

   
   // Create Hybrid Mask Part 2 volumes
   DGVolumePtr spHybridMask2 = 
       new DGVolumeTube(IRGeomParameters::kHybridTungstenMaskVol2Name,
			pTungsten,
			kHybridWMask2Radii,
			kHybridWMask2ZHalfLength);

   shiftZ = HepTranslateZ3D(kHybridWMask2Z1 + kHybridWMask2ZHalfLength);
   DGPlacedVolumePtr spEastHybridMask2 = 
       new DGPlacedVolume( spHybridMask2,
			   1,
			   shiftZ,
			   DGTransform::Identity);

   pIRMother->addChild(spEastHybridMask1);
   pIRMother->addChild(spEastHybridMask2);
}

//
// const member functions
//

//
// static member functions
//
