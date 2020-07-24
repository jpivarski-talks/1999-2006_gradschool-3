// -*- C++ -*-
//
// Package:     IRGeom
// Module:      MaskInnerGeometry
// 
// Description: Function class to create IRGeom Inner Tungsten Mask
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 12:36:57 EDT 1999
// $Id: MaskInnerGeometry.cc,v 1.2 2002/05/31 17:19:47 bkh Exp $
//
// Revision history
//
// $Log: MaskInnerGeometry.cc,v $
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
#include "IRGeom/MaskInnerGeometry.h"
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
#include "DetectorGeometry/DGVolumeCone.h"
#include "DetectorGeometry/DGVolumePolyCone.h"
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

static const char* const kFacilityString = "IRGeom.MaskInnerGeometry" ;

typedef double IRreal;
typedef pair<IRreal,IRreal> IRpair;


// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: MaskInnerGeometry.cc,v 1.2 2002/05/31 17:19:47 bkh Exp $";
static const char* const kTagString = "$Name: v01_01_02 $";

//
// static data member definitions
//

//
// constructors and destructor
//
MaskInnerGeometry::MaskInnerGeometry(const Record& iRecord)
    : m_Record(iRecord)
{
}

// MaskInnerGeometry::MaskInnerGeometry( const MaskInnerGeometry& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

MaskInnerGeometry::~MaskInnerGeometry()
{
}

//
// assignment operators
//
// const MaskInnerGeometry& MaskInnerGeometry::operator=( const MaskInnerGeometry& rhs )
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
MaskInnerGeometry::operator()(DGVolumeParentPlaced* pIRMother)
{
   report(INFO, kFacilityString) << "Constructing inner mask." << endl;

   // ------------ get the materials ------------- //
   DGMaterialStore materials ;

   extract( m_Record, materials );

   DGConstMaterialPtr pTungsten = materials[ kTungstenMatID ];

   // ------------ Define some useful transformations ------------ //
   HepRotateX3D  flipZ( 180 * k_degrees );
   HepTransform3D  shiftZ;
   HepTransform3D  flipShiftZ;

   // Needed dimension from copper pipe;
   const IRreal kCuPipeOuterRadius1 = 2.196 * k_cm;
   const IRreal kCuPipeOuterRadius2 = 2.196 * k_cm;
   
   //   dimensions: Inner Tungsten Mask
   const IRreal kInnerWMaskZ1 = 16.9 * k_cm;
   const IRreal kInnerWMaskZ2 = 18.7 * k_cm;
   const IRreal kInnerWMaskOuterRadius1 = 3.46 * k_cm;
   const IRreal kInnerWMaskOuterRadius2 = 5.12 * k_cm;
   
   const IRreal kInnerWMaskZHalfLength = 
       (kInnerWMaskZ2 - kInnerWMaskZ1)/2.0;

   const IRpair kInnerWMaskZ1Radii(kCuPipeOuterRadius1,
				   kInnerWMaskOuterRadius1);

   const IRpair kInnerWMaskZ2Radii(kCuPipeOuterRadius2,
				   kInnerWMaskOuterRadius2);
   
   // Create Inner Mask volumes
   DGVolumePtr spInnerMask = 
       new DGVolumeCone(IRGeomParameters::kInnerTungstenMaskVolName,
			pTungsten,
			kInnerWMaskZ1Radii,
			kInnerWMaskZ2Radii,
			kInnerWMaskZHalfLength);

   shiftZ = HepTranslateZ3D(kInnerWMaskZ1 + kInnerWMaskZHalfLength);
   DGPlacedVolumePtr spEastInnerMask = 
       new DGPlacedVolume( spInnerMask,
			   1,
			   shiftZ,
			   DGTransform::Identity);

   pIRMother->addChild(spEastInnerMask);
}

//
// const member functions
//

//
// static member functions
//




