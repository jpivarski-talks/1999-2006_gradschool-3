// -*- C++ -*-
//
// Package:     IRGeom
// Module:      CuSupportGeometry
// 
// Description: Function class to create Copper/Carbon fiber/Aluminum 
//              Support and BeO Hybrid Electronics for SV Detector
//
// Implementation:
//     These SiGeom components included in SiGeom to avoid package overlap
//
// Author:      Aren Jansen
// Created:     Fri Jun 25 16:43:56 EDT 1999
// $Id: CuSupportGeometry.cc,v 1.4 2002/05/31 17:19:46 bkh Exp $
//
// Revision history
//
// $Log: CuSupportGeometry.cc,v $
// Revision 1.4  2002/05/31 17:19:46  bkh
// Repair so does not overlap DR or CC endcap and also
// make it one sided only and replicate that side for negative z
//
// Revision 1.3  2000/03/15 21:35:12  pg
// After futile attempts to fix a crash on OSF, it is gone on its own. After print statement was added. I am sure it will come back.
//
// Revision 1.2  1999/07/23 22:23:45  lkg
// Encase various for loops in a pair of {} to deal with screwy loop index
// scope under SunOs/CC
//
// Revision 1.1.1.1  1999/07/22 14:38:43  lkg
// First release
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#include <math.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "IRGeom/CuSupportGeometry.h"
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
#include "DetectorGeometry/DGConstVolumeType.h"
#include "DetectorGeometry/DGConstMaterialPtr.h"
#include "DetectorGeometry/DGPlacedVolume.h"
#include "DetectorGeometry/DGPlacedVolumePtr.h"
#include "DetectorGeometry/DGZrRList.h"
#include "DetectorGeometry/DGVolumeBox.h"
#include "DetectorGeometry/DGVolumeTube.h"
#include "DetectorGeometry/DGVolumeTubePhiSegment.h"
#include "DetectorGeometry/DGVolumePolyCone.h"
#include "DetectorGeometry/DGVolumeCone.h"
#include "DetectorGeometry/DGVolumeTrapezoidSymX.h"
#include "DetectorGeometry/DGVolumeParentPlaced.h"

#include "DetectorGeometry/DGMaterialStore.h"
#include "DetectorGeometry/DGDetectorStore.h"

#include "MaterialDelivery/material_identifiers.h"
#include "ToolBox/Triplet.h"

// STL classes
#include <vector>
#include <map>


//
// constants, enums and typedefs
//
typedef double IRreal;
typedef pair<IRreal,IRreal> IRpair;
typedef Triplet<IRreal, IRreal, IRreal> IRtrip;

static const char* const kFacilityString = "IRGeom.CuSupportGeometry" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: CuSupportGeometry.cc,v 1.4 2002/05/31 17:19:46 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructor and destructor
//
CuSupportGeometry::CuSupportGeometry(const Record& iRecord)
   : m_Record(iRecord)
{
}

CuSupportGeometry::~CuSupportGeometry()
{
}

//
// member functions
//

void
CuSupportGeometry::operator()(DGVolumeParentPlaced* pIRMother)
{
   report(INFO, kFacilityString) << "Constructing copper support." << endl;

   // ------------ get the materials ------------- //
   DGMaterialStore materials ;

   extract( m_Record, materials );

   DGConstMaterialPtr pCopper = materials[ kCopperMatID ];
   DGConstMaterialPtr pCarbon = materials[ kSVCarbonFiberMatID ];
   DGConstMaterialPtr pBeO = materials[ kBerylliumOxideMatID ];
   DGConstMaterialPtr pVacuum = materials[ kVacuumMatID ];
   DGConstMaterialPtr pAluminum = materials[ kAluminum6061MatID ];
   
   // --------- Transformation declarations and/or definitions ---------- //
   HepRotateY3D  flipZ( 180 * k_degrees );
   HepRotateZ3D  rotateAboutZ;
   HepRotateX3D  rotateAboutX;
   HepTransform3D  shiftZ;
   HepTransform3D  shiftXYZ;
   HepTransform3D  flipShiftZ;


   // ------------------------------------------------------------ //
   //   dimensions: Layer3-4 Cone (called Overall) Z-positioning   //
   // ------------------------------------------------------------ //
//   const IRreal kSiLadder3HalfLength = (5.31 * 7.0/2.0) * k_cm;
//   const IRreal kL34ConeOffsetFromSiLadder3 = 5 * k_mm; //FIX!!

   const IRreal kSiGeomCylZHalfLength = 17.09/2.0 * k_in;
   const IRreal kOverallZStart = kSiGeomCylZHalfLength - 
      (0.1472 + 0.8018) * k_in;

   // ----------------------------------------- //
   //   dimensions: Layer 3-4 (main) polycone   //
   // ----------------------------------------- //
   const int MAINVERTEXNUM = 9;
   _vector_DGZrR_ mainRadiiVector;
   
   const IRreal zCoordinates1[] = {
      0.0,
      (7.807-7.612) * k_in,
      (7.807-6.857) * k_in,
      (7.807-6.857) * k_in,
      (7.807-3.710) * k_in,
      (7.807-3.710) * k_in,
      (7.807-0.500) * k_in,
      (7.807-0.500) * k_in,
      7.807 * k_in };
   assert(sizeof(zCoordinates1)/sizeof(IRreal) == MAINVERTEXNUM);
   
   const IRreal outerRadii1[] = {       
      3.119 * k_in, 
      3.199 * k_in,
      3.199 * k_in,
      3.141 * k_in,
      3.984 * k_in,
      (0.161 + 3.984) * k_in,
      (0.161 + 3.984) * k_in,
      4.479 * k_in, 
      4.479 * k_in };
   assert(sizeof(outerRadii1)/sizeof(IRreal) == MAINVERTEXNUM);

   const IRreal innerRadii1[MAINVERTEXNUM] = {
      (2.948 + 3.054)/2.0 * k_in,
      (2.948 + 3.054)/2.0 * k_in,
      (2.948 + 3.054)/2.0 * k_in,
      (2.948 + 3.054)/2.0 * k_in,
      7.790/2.0 * k_in,  
      7.790/2.0 * k_in,  
      7.790/2.0 * k_in,  
      7.790/2.0 * k_in,  
      7.790/2.0 * k_in };
   assert(sizeof(innerRadii1)/sizeof(IRreal) == MAINVERTEXNUM);
    

   { // change scope because SunOS/CC screws up scope of loop variable
      for (int i = 0; i < MAINVERTEXNUM; ++i)
      {
	 DGZrR breakPoint(zCoordinates1[i],
			  innerRadii1[i],
			  outerRadii1[i]);
	 mainRadiiVector.push_back(breakPoint);
      }    
   } // end of scope change

   DGZrRList mainRadiiList( mainRadiiVector );

   // ------------------------------------------------------------------- //
   //   dimensions: Layer 3-4 Cone (Main) copper structure radial boxes   //
   // ------------------------------------------------------------------- //
   const IRreal kCuMainXHalfLength = (((2*3.14*4.120/13)-1.379)/2.0 * k_in);
   const IRreal kCuMainYHalfLength = ((4.355-4.120)/2.0 * k_in);
   const IRreal kCuMainZHalfLength = ((3.710-0.500)/2.0 * k_in );

   const IRtrip kCuMainBoxDims( kCuMainXHalfLength,
				kCuMainYHalfLength,
				kCuMainZHalfLength );

   const IRreal kCuMainRadialBoxZStart = 
      kOverallZStart + kCuMainZHalfLength + ((7.807 - 3.710) * k_in);

   const IRreal kCuMainRadialBoxYStart = 
      -(kCuMainYHalfLength + 4.120 * k_in);

   // ---------------------------------------------------- //
   //   dimensions: BeO Electronics Hybrid Circuit Board   //
   // ---------------------------------------------------- //
   const IRreal kBeOHybridYOffset = 1.50 * k_mm; //For Copper Pad

   const IRreal kBeOHybridXHalfLength1 = 27.00/2.0 * k_mm;
   const IRreal kBeOHybridXHalfLength2 = 33.00/2.0 * k_mm;
   const IRpair kBeOHybridXHalfLengths( kBeOHybridXHalfLength1,
					kBeOHybridXHalfLength2 );
   const IRreal kBeOHybridYHalfLength = 1.02/2.0 * k_mm;
   const IRreal kBeOHybridZHalfLength = 62.00/2.0 * k_mm;


   // ------------------------------------------------- //
   //   dimensions: BeO Hybrid Copper Attachment Pads   //
   // ------------------------------------------------- //
   const IRreal kBeOCuPadXHalfLength1 = 2.34/2.0 * k_mm;
   const IRreal kBeOCuPadXHalfLength2 = 3.70/2.0 * k_mm;
   const IRpair kBeOCuPadXHalfLengths( kBeOCuPadXHalfLength1,
					kBeOCuPadXHalfLength2 );
   const IRreal kBeOCuPadYHalfLength = 1.50/2.0 * k_mm;
   const IRreal kBeOCuPadZHalfLength = 15.07/2.0 * k_mm;

   const IRreal kBeOCuPadOffset = 29.04 * k_mm;

   const IRtrip kBeOBoxDims( kBeOHybridXHalfLength2,
			     kBeOHybridYHalfLength + 2.0*kBeOCuPadYHalfLength,
			     kBeOHybridZHalfLength );


   // ----------------------------------------- //
   //   dimensions: Transition Cone ZOffsets    //
   // ----------------------------------------- //
   const IRreal kAlInnerMainOffset = 0.8018 * k_in;
   const IRreal kAlInnerCFiberOverlap = 0.483 * k_in;
   const IRreal kAlOuterCFiberOverlap = 0.483 * k_in;


   // ------------------------------------ //
   //   dimensions: Inner Aluminum Cone    //
   // ------------------------------------ //
   const IRreal kAlInnerConeZStart = kOverallZStart + kAlInnerMainOffset;
   const IRreal kAlInnerConeRadiusZ2Outer = 5.919/2.0 * k_in;
   const IRreal kAlInnerConeRadiusZ1Outer = 5.480/2.0 * k_in;
   const IRreal kAlInnerConeRadiusZ2Inner = 5.859/2.0 * k_in;
   const IRreal kAlInnerConeRadiusZ1Inner = 5.480/2.0 * k_in;
   const IRreal kAlInnerConeZHalfLength = 0.899/2.0 * k_in;
 
   const IRreal kAlInnerConeInclineAngle = 
      atan((kAlInnerConeRadiusZ2Outer - kAlInnerConeRadiusZ1Outer)
	  /(2.0 * kAlInnerConeZHalfLength)) * k_radians;

   const int ALINNERVERTEXNUM = 4;
   _vector_DGZrR_ alInnerRadiiVector;
   
   const IRreal zCoordinatesAlInner[ALINNERVERTEXNUM] = {
      0.0,
      0.899 * k_in - kAlInnerCFiberOverlap,
      0.899 * k_in - kAlInnerCFiberOverlap,
      0.899 * k_in };
   
   const IRreal outerRadiiAlInner[ALINNERVERTEXNUM] = {
      kAlInnerConeRadiusZ1Outer,
      kAlInnerConeRadiusZ1Outer + tan(kAlInnerConeInclineAngle) * 
      (zCoordinatesAlInner[1]),
      kAlInnerConeRadiusZ1Outer + tan(kAlInnerConeInclineAngle) * 
      (zCoordinatesAlInner[2]),
      kAlInnerConeRadiusZ1Outer + tan(kAlInnerConeInclineAngle) * 
      (zCoordinatesAlInner[3]) };

   const IRreal innerRadiiAlInner[ALINNERVERTEXNUM] = {
      5.375/2.0 * k_in,
      5.375/2.0 * k_in, 
      5.600/2.0 * k_in, 
      5.859/2.0 * k_in };
    

   { // change scope because SunOS/CC screws up scope of loop variable
      for (int i = 0; i < ALINNERVERTEXNUM; ++i) {
	 DGZrR breakPoint(zCoordinatesAlInner[i],
			  innerRadiiAlInner[i],
			  outerRadiiAlInner[i]);
	 alInnerRadiiVector.push_back(breakPoint);
      }    
   } // end of scope change

   DGZrRList alInnerRadiiList( alInnerRadiiVector );
  

   // ------------------------------------------ //
   //   dimensions: Inner Aluminum radial boxes  //
   // ------------------------------------------ //
   const IRreal kAlXHalfLength2 = (2.684 - 2.295)/2.0 * k_in;
   const IRreal kAlYHalfLength2 = (2.684 - 2.295)/2.0 * k_in;
   const IRreal kAlZHalfLength2 = 0.300/2.0 * k_in;

   const IRtrip kAlBoxDims2( kAlXHalfLength2,
			     kAlYHalfLength2,
			     kAlZHalfLength2 );

   const IRreal kAlRadialBoxZStart2 = kAlZHalfLength2 + kAlInnerConeZStart;

   const IRreal kAlRadialBoxYStart2 = 
      -(innerRadiiAlInner[0] - kAlYHalfLength2); 


   // ------------------------------------- //
   //   dimensions: Carbon Fiber Cone       //
   // ------------------------------------- //
   const IRreal kCFiberConeZStart = zCoordinatesAlInner[1] + 
      kAlInnerConeZStart; 

   const IRreal kCFiberConeZHalfLength = 3.72/2.0 * k_in;
   
   const IRreal kCFiberConeRadiusZ1Inner = 2.72 * k_in;
   const IRreal kCFiberConeRadiusZ1Outer = 2.80 * k_in;

   const IRreal kCFiberConeRadiusZ2Inner = 3.72 * k_in;
   const IRreal kCFiberConeRadiusZ2Outer = 3.79 * k_in;     
 
   const IRpair kCFiberConeRadiiZ2( kCFiberConeRadiusZ2Inner,
				    kCFiberConeRadiusZ2Outer );

   const IRpair kCFiberConeRadiiZ1( kCFiberConeRadiusZ1Inner,
				    kCFiberConeRadiusZ1Outer );   

   // ------------------------------------ //
   //   dimensions: Outer Aluminum Cone    //
   // ------------------------------------ //
   const IRreal kAlOuterConeZStart = 
      kCFiberConeZStart + 2.0 * kCFiberConeZHalfLength - kAlOuterCFiberOverlap;

   const IRreal kAlOuterConeZHalfLength = 0.889/2.0 * k_in;
   
   const IRreal kAlOuterConeRadiusZ2Inner = 7.545/2.0 * k_in;
   const IRreal kAlOuterConeRadiusZ2Outer = (7.583/2.0 + 0.018) * k_in;
 
   const IRreal kAlOuterConeRadiusZ1Inner = 7.112/2.0 * k_in;
   const IRreal kAlOuterConeRadiusZ1Outer = 7.172/2.0 * k_in;


   const int ALOUTERVERTEXNUM = 4;
   _vector_DGZrR_ alOuterRadiiVector;
   
   const IRreal zCoordinatesAlOuter[ALOUTERVERTEXNUM] = {
      0.0, 
      kAlOuterCFiberOverlap, 
      kAlOuterCFiberOverlap, 
      0.889 * k_in };
   
   const IRreal outerRadiiAlOuter[ALOUTERVERTEXNUM] = {
      kAlOuterConeRadiusZ1Outer,
      7.431/2.0 * k_in,
      kAlOuterConeRadiusZ2Outer,
      kAlOuterConeRadiusZ2Outer };


   const IRreal innerRadiiAlOuter[ALOUTERVERTEXNUM] = {
      kAlOuterConeRadiusZ1Inner,
      kAlOuterConeRadiusZ1Inner + 
      zCoordinatesAlOuter[1] * tan(15 * k_degrees),
      kAlOuterConeRadiusZ1Inner + 
      zCoordinatesAlOuter[2] * tan(15 * k_degrees),
      kAlOuterConeRadiusZ2Inner};
    

   { // change scope because SunOS/CC screws up scope of loop variable
      for (int i = 0; i < ALOUTERVERTEXNUM; ++i) {
	 DGZrR breakPoint(zCoordinatesAlOuter[i],
			  innerRadiiAlOuter[i],
			  outerRadiiAlOuter[i]);
	 alOuterRadiiVector.push_back(breakPoint);
      }    
   } // end of scope change
   
   DGZrRList alOuterRadiiList( alOuterRadiiVector );
  
   // ----------------------------------------- //
   //  dimensions: Outer Aluminum radial boxes  //
   // ----------------------------------------- //
   const IRreal kAlXHalfLength1 = 0.196/2.0 * k_in;
   const IRreal kAlYHalfLength1 = 0.083/2.0 * k_in;
   const IRreal kAlZHalfLength1 = 0.375/2.0 * k_in;

   const IRtrip kAlBoxDims1( kAlXHalfLength1,
			     kAlYHalfLength1,
			     kAlZHalfLength1 );

   const IRreal kAlRadialBoxZStart1 =  
      kAlOuterConeZStart + 2.0 * kAlOuterConeZHalfLength - kAlZHalfLength1;

   const IRreal kAlRadialBoxYStart1 = 
      outerRadiiAlOuter[3] + kAlYHalfLength1;

 
   // ------------------------------------- //
   //   dimensions: Inner Copper cone       //
   // ------------------------------------- //
   const IRreal kCuInnerConeZHalfLength = 2.900/2.0 * k_in;

   const IRreal kCuInnerConeZStart = 
      kAlInnerConeZStart - (2.0 * kCuInnerConeZHalfLength);

   const IRreal kCuInnerConeRadiusZ2Inner = 
      2.386 * k_in; 

   const IRreal kCuInnerConeRadiusZ2Outer = 2.590 * k_in;

   const IRreal kChangeInRadius = 
      2 * kCuInnerConeZHalfLength * 0.267949;
 
   const IRreal kCuInnerConeRadiusZ1Inner = 
      kCuInnerConeRadiusZ2Inner - kChangeInRadius;

   const IRreal kCuInnerConeRadiusZ1Outer = 
      kCuInnerConeRadiusZ2Outer - kChangeInRadius;

   const IRpair kCuInnerConeRadiiZ2( kCuInnerConeRadiusZ2Inner,
				     kCuInnerConeRadiusZ2Outer );

   const IRpair kCuInnerConeRadiiZ1( kCuInnerConeRadiusZ1Inner,
				     kCuInnerConeRadiusZ1Outer );

   // --------------------------------------------------------------------- //
   //  dimensions: Inner copper structure tube at smaller radius            //
   // --------------------------------------------------------------------- //
   const IRreal kCuInnerLipZHalfLength = 0.098/2.0 * k_in;

   const IRreal kCuInnerLipInnerRadius2 = kCuInnerConeRadiusZ1Inner;
   const IRreal kCuInnerLipInnerRadius1 = 
      kCuInnerLipInnerRadius2 - 
      2.0 * kCuInnerLipZHalfLength * tan(15 * k_degrees);
   const IRreal kCuInnerLipOuterRadius = 1.750 * k_in;

   const IRpair kCuInnerLipZ1Radii( kCuInnerLipInnerRadius1,
				    kCuInnerLipOuterRadius );

   const IRpair kCuInnerLipZ2Radii( kCuInnerLipInnerRadius2,
				    kCuInnerLipOuterRadius );

   const IRreal kCuInnerLipZStart = 
      kCuInnerConeZStart - (kCuInnerLipZHalfLength * 2.0);

   // ------------------------------------------------ //
   //   dimensions: aluminum layer 1-2 nose polycone   //
   // ------------------------------------------------ //
   const IRreal kNose12ZStart = kCuInnerConeZStart - 0.922 * k_in;

   const int NOSE12VERTEXNUM = 9;
   _vector_DGZrR_ nose12RadiiVector;
   
   const IRreal zCoordinates2[NOSE12VERTEXNUM] = {
      0.0,
      0.128 * k_in,
      0.250 * k_in,
      0.379 * k_in,
      0.379 * k_in,
      0.575 * k_in,
      (0.922 - 0.098) * k_in,
      (0.922 - 0.098) * k_in,
      0.922 * k_in };
   
   const IRreal outerRadii2[NOSE12VERTEXNUM] = {
      1.475 * k_in,
      1.600 * k_in,
      1.600 * k_in + 0.268 * (zCoordinates2[2] - zCoordinates2[1]),
      1.600 * k_in + 0.268 * (zCoordinates2[3] - zCoordinates2[1]),
      1.600 * k_in + 0.268 * (zCoordinates2[4] - zCoordinates2[1]),
      1.600 * k_in + 0.268 * (zCoordinates2[5] - zCoordinates2[1]),
      1.600 * k_in + 0.268 * (zCoordinates2[6] - zCoordinates2[1]),
      1.600 * k_in + 0.268 * (zCoordinates2[7] - zCoordinates2[1]),
      1.600 * k_in + 0.268 * (zCoordinates2[8] - zCoordinates2[1]) };

   const IRreal innerRadii2[NOSE12VERTEXNUM] = {
      0.904 * k_in,
      0.904 * k_in,
      0.904 * k_in,
      1.104 * k_in,
      1.333 * k_in,
      1.565 * k_in,
      1.565 * k_in,
      1.750 * k_in,
      1.750 * k_in };
    

   { // change scope because SunOS/CC screws up scope of loop variable
      for (int i = 0; i < NOSE12VERTEXNUM; ++i) {
	 DGZrR breakPoint(zCoordinates2[i],
			  innerRadii2[i],
			  outerRadii2[i]);
	 nose12RadiiVector.push_back(breakPoint);
      }    
   } // end of scope change
   
   DGZrRList nose12RadiiList( nose12RadiiVector );


   // ------------------------------------------------- //
   //   dimensions: BeO Hybrid Mother Volume Placing    //
   // ------------------------------------------------- //
   const IRreal kEdgeOffsetL1 = (1.272 - 1.106) * k_in;
   const IRreal kBeOHybridInclineAngleL1 =
      atan((kCuInnerConeRadiusZ2Inner-kCuInnerConeRadiusZ1Inner)/
	    (2.0 * kCuInnerConeZHalfLength)) * k_radians;   
   const IRreal kBeOHybridYStartL1 = 
      ((kBeOHybridZHalfLength) * sin(kBeOHybridInclineAngleL1))
      + kCuInnerConeRadiusZ1Inner 
      - (kBeOHybridYHalfLength + 2.0 * kBeOCuPadYHalfLength)
      /cos(kBeOHybridInclineAngleL1)
      + kBeOHybridYHalfLength/2.0 * sin(kBeOHybridInclineAngleL1)
      - kEdgeOffsetL1 * sin(kBeOHybridInclineAngleL1);
   const IRreal kBeOHybridZStartL1 = 
      (kBeOHybridZHalfLength) * cos(kBeOHybridInclineAngleL1)
      + kCuInnerConeZStart
      + kBeOHybridYHalfLength/2.0 * cos(kBeOHybridInclineAngleL1)
      - kEdgeOffsetL1 * cos(kBeOHybridInclineAngleL1);


   const IRreal kEdgeOffsetL2 = (1.272 - 0.832) * k_in;
   const IRreal kBeOHybridInclineAngleL2 =
      atan((kCuInnerConeRadiusZ2Outer-kCuInnerConeRadiusZ1Outer)/
	    (2.0 * kCuInnerConeZHalfLength)) * k_radians;
   const IRreal kBeOHybridYStartL2 = 
      (kBeOHybridZHalfLength) * sin(kBeOHybridInclineAngleL2) 
      + kCuInnerConeRadiusZ1Outer 
      +(kBeOHybridYHalfLength + 2.0 * kBeOCuPadYHalfLength)
      /cos(kBeOHybridInclineAngleL2)
      + kBeOHybridYHalfLength/2.0 * sin(kBeOHybridInclineAngleL2)
      - kEdgeOffsetL2 * sin(kBeOHybridInclineAngleL2);
   const IRreal kBeOHybridZStartL2 = 
      (kBeOHybridZHalfLength) * cos(kBeOHybridInclineAngleL2)
      + kCuInnerConeZStart
      + kBeOHybridYHalfLength/2.0 * cos(kBeOHybridInclineAngleL2)
      - kEdgeOffsetL2 * cos(kBeOHybridInclineAngleL2);

   const IRreal kEdgeOffsetL3 =
      (((zCoordinates1[4] + kOverallZStart - kAlInnerConeZStart)
	/cos(15 * k_degrees))
       - 1.272 * k_in) - 1.091 * k_in; 

   const IRreal kBeOHybridInclineAngleL3 =
      atan((innerRadii1[4] - innerRadii1[3])
	   /(zCoordinates1[4] - zCoordinates1[3]));   
   const IRreal kBeOHybridYStartL3 = 
      (kBeOHybridZHalfLength) * sin(kBeOHybridInclineAngleL3) 
      + innerRadii1[3] - 
      (tan(kBeOHybridInclineAngleL3) * 
       (zCoordinates1[3] + kOverallZStart - kAlInnerConeZStart))
       - (kBeOHybridYHalfLength + 2.0 * kBeOCuPadYHalfLength)
      /cos(kBeOHybridInclineAngleL3)
      + kBeOHybridYHalfLength/2.0 * sin(kBeOHybridInclineAngleL3)
      + kEdgeOffsetL3 * sin(kBeOHybridInclineAngleL3);
   const IRreal kBeOHybridZStartL3 = 
      (kBeOHybridZHalfLength) * cos(kBeOHybridInclineAngleL3)
      + kAlInnerConeZStart
      + kBeOHybridYHalfLength/2.0 * cos(kBeOHybridInclineAngleL3)
      + kEdgeOffsetL3 * cos(kBeOHybridInclineAngleL3);

 
   const IRreal kBeOHybridYStartL4S1 = 
      -(outerRadii1[5] + kBeOHybridYHalfLength + 2.0 * kBeOCuPadYHalfLength);

   const IRreal kBeOHybridYStartL4S2 = 
      kBeOHybridYStartL4S1 + -(2.0 * kCuMainYHalfLength);

   const IRreal kBeOHybridZStartL4 =  
      zCoordinates1[4] + kOverallZStart + kBeOHybridZHalfLength;


// Create Volumes

   // Main Copper Support Structure
   DGVolumePtr spMainStruct = 
       new DGVolumePolyCone( IRGeomParameters::kCuMainStructVolName,
			     pCopper,
			     mainRadiiList );

   shiftZ = HepTranslateZ3D( kOverallZStart );

   DGPlacedVolumePtr spEastMainStruct = 
       new DGPlacedVolume( spMainStruct,
			   1,
			   shiftZ,
			   DGTransform::Identity);


   pIRMother->addChild(spEastMainStruct);


   { // change scope because SunOS/CC screws up scope of loop variable
      // Radial Boxes for Main copper
      for (int i = 0; i < IRGeomParameters::kNumBoxes4; i++) {
	 DGVolumePtr spRadialBox = 
	    new DGVolumeBox( IRGeomParameters::kCuRadialBoxVolName[i],
			     pCopper,
			     kCuMainBoxDims );

	 shiftXYZ = HepTranslate3D(0.0, 
				   kCuMainRadialBoxYStart, 
				   kCuMainRadialBoxZStart );

	 double idouble = (double)i;
	 double numdouble = (double)IRGeomParameters::kNumBoxes4;

	 rotateAboutZ = HepRotateZ3D( 
	    ((idouble/numdouble) * 360.0) * k_degrees );

	 flipShiftZ = rotateAboutZ * shiftXYZ;

	 DGPlacedVolumePtr spEastRadialBox = 
	    new DGPlacedVolume( spRadialBox,
				1,
				flipShiftZ,
				DGTransform::Identity);
      
	 pIRMother->addChild(spEastRadialBox);
      }
   } // end of scope change

   // Construct BeO Hybrid components
   DGVolumePtr spBeOBoard = 
      new DGVolumeTrapezoidSymX( IRGeomParameters::kBeOBoardVolName,
				 pBeO,
				 kBeOHybridXHalfLengths,
				 kBeOHybridYHalfLength,
				 kBeOHybridZHalfLength );


   DGPlacedVolumePtr spPlacedBeOBoard = 
      new DGPlacedVolume( spBeOBoard,
			  1,
			  DGTransform::Identity,
			  DGTransform::Identity );
   

   DGVolumePtr spBeOCuPad = 
      new DGVolumeTrapezoidSymX( IRGeomParameters::kBeOCuPadVolName,
				 pCopper,
				 kBeOCuPadXHalfLengths,
				 kBeOCuPadYHalfLength,
				 kBeOCuPadZHalfLength );

   shiftXYZ = HepTranslate3D(
      0.0, 
      kBeOHybridYHalfLength + kBeOCuPadYHalfLength, 
      kBeOCuPadOffset - (kBeOHybridZHalfLength - kBeOCuPadZHalfLength) );
 
   
   DGPlacedVolumePtr spPlacedTopBeOCuPad = 
      new DGPlacedVolume( spBeOCuPad,
			  1,
			  shiftXYZ,
			  DGTransform::Identity );

   shiftXYZ = HepTranslate3D(
      0.0, 
      -(kBeOHybridYHalfLength + kBeOCuPadYHalfLength), 
      kBeOCuPadOffset - (kBeOHybridZHalfLength - kBeOCuPadZHalfLength) );
 
   
   DGPlacedVolumePtr spPlacedBottomBeOCuPad = 
      new DGPlacedVolume( spBeOCuPad,
			  2,
			  shiftXYZ,
			  DGTransform::Identity );

   // Hybrid Bounding Volume
   DGVolumePtr spBeOBox = 
      new DGVolumeBox( "BeO Bounding Box", 
		       pVacuum, 
		       kBeOBoxDims);

//			  kBeOHybridXHalfLength2,
//			  kBeOHybridYHalfLength + 2.0 * kBeOCuPadYHalfLength,
//			  kBeOHybridZHalfLength ) );


   { // change scope because SunOS/CC screws up scope of loop variable
      // Layer 1 BeO Electronics Hybrids, Using components created above
      for (int i = 0; i < IRGeomParameters::kNumBoxes1; i++)
      {

	 DGVolumeParentPlaced* pParentPlacedBeOBox = 
	    new DGVolumeParentPlaced( 
	       IRGeomParameters::kBeOHybridL1VolName[i],
	       spBeOBox );
	 report(DEBUG,kFacilityString) << " HalflengthX : " 
				       << kBeOHybridXHalfLength2 << endl;
//	 cout << " CUSupport : made a BeO box: X = " 
//	      << DGConstVolumeType< DGVolumeBox > (spBeOBox)->halfLengthX() 
//	      << endl;
	 
	 pParentPlacedBeOBox->addChild( spPlacedBeOBoard );
	 pParentPlacedBeOBox->addChild( spPlacedTopBeOCuPad );
	 pParentPlacedBeOBox->addChild( spPlacedBottomBeOCuPad );

	 DGVolumePtr spBeOHybrid ( pParentPlacedBeOBox );

	 rotateAboutX = HepRotateX3D(-kBeOHybridInclineAngleL1);

	 shiftXYZ = HepTranslate3D(0.0, 
				   kBeOHybridYStartL1, 
				   kBeOHybridZStartL1 );

	 double idouble = (double)i;
	 double numdouble = (double)IRGeomParameters::kNumBoxes1;

	 rotateAboutZ = HepRotateZ3D( 
	    (idouble/numdouble * 360.0) * k_degrees );

	 flipShiftZ = rotateAboutZ * shiftXYZ * rotateAboutX;

	 DGPlacedVolumePtr spEastBeOHybrid = 
	    new DGPlacedVolume( spBeOHybrid,
				1,
				flipShiftZ,
				DGTransform::Identity);

	 rotateAboutX = HepRotateX3D(kBeOHybridInclineAngleL1);
      
	 pIRMother->addChild(spEastBeOHybrid);
      }
   } // end of scope change


   { // change scope because SunOS/CC screws up scope of loop variable
      // Layer 2 BeO Electronics Hybrids, Using components created above
      for (int i = 0; i < IRGeomParameters::kNumBoxes2; i++)
      {
	 DGVolumeParentPlaced* pParentPlacedBeOBox = 
	    new DGVolumeParentPlaced( 
	       IRGeomParameters::kBeOHybridL2VolName[i],
	       spBeOBox );
   
	 pParentPlacedBeOBox->addChild( spPlacedBeOBoard );
	 pParentPlacedBeOBox->addChild( spPlacedTopBeOCuPad );
	 pParentPlacedBeOBox->addChild( spPlacedBottomBeOCuPad );

	 DGVolumePtr spBeOHybrid ( pParentPlacedBeOBox );

	 rotateAboutX = HepRotateX3D(-kBeOHybridInclineAngleL1);

	 shiftXYZ = HepTranslate3D(0.0, 
				   kBeOHybridYStartL2, 
				   kBeOHybridZStartL2 );

	 double idouble = (double)i;
	 double numdouble = (double)IRGeomParameters::kNumBoxes2;

	 rotateAboutZ = HepRotateZ3D( 
	    (idouble/numdouble * 360.0) * k_degrees );

	 flipShiftZ = rotateAboutZ * shiftXYZ * rotateAboutX;

	 DGPlacedVolumePtr spEastBeOHybrid = 
	    new DGPlacedVolume( spBeOHybrid,
				1,
				flipShiftZ,
				DGTransform::Identity);
 
	 pIRMother->addChild(spEastBeOHybrid);
      }
   } // end of scope change


   { // change scope because SunOS/CC screws up scope of loop variable
      // Layer 3 BeO Electronics Hybrids, Using components created above
      for (int i = 0; i < IRGeomParameters::kNumBoxes3; i++)
      {
	 DGVolumeParentPlaced* pParentPlacedBeOBox = 
	    new DGVolumeParentPlaced( 
	       IRGeomParameters::kBeOHybridL3VolName[i],
	       spBeOBox );
   
	 pParentPlacedBeOBox->addChild( spPlacedBeOBoard );
	 pParentPlacedBeOBox->addChild( spPlacedTopBeOCuPad );
	 pParentPlacedBeOBox->addChild( spPlacedBottomBeOCuPad );

	 DGVolumePtr spBeOHybrid ( pParentPlacedBeOBox );

	 rotateAboutX = HepRotateX3D(-kBeOHybridInclineAngleL3);

	 shiftXYZ = HepTranslate3D(0.0, 
				   kBeOHybridYStartL3, 
				   kBeOHybridZStartL3 );

	 double idouble = (double)i;
	 double numdouble = (double)IRGeomParameters::kNumBoxes3;

	 rotateAboutZ = HepRotateZ3D( 
	    (idouble/numdouble * 360.0) * k_degrees );

	 flipShiftZ = rotateAboutZ * shiftXYZ * rotateAboutX;

	 DGPlacedVolumePtr spEastBeOHybrid = 
	    new DGPlacedVolume( spBeOHybrid,
				1,
				flipShiftZ,
				DGTransform::Identity);
      
	 pIRMother->addChild(spEastBeOHybrid);
      }
   } // end of scope change


   { // change scope because SunOS/CC screws up scope of loop variable
      // Layer 4 BeO Electronics Hybrids Set 1, Using components created above
      for (int i = 0; i < IRGeomParameters::kNumBoxes4; i++)
      {
	 DGVolumeParentPlaced* pParentPlacedBeOBox = 
	    new DGVolumeParentPlaced( 
	       IRGeomParameters::kBeOHybridL4Set1VolName[i],
	       spBeOBox );
   
	 pParentPlacedBeOBox->addChild( spPlacedBeOBoard );
	 pParentPlacedBeOBox->addChild( spPlacedTopBeOCuPad );
	 pParentPlacedBeOBox->addChild( spPlacedBottomBeOCuPad );
	 
	 DGVolumePtr spBeOHybrid ( pParentPlacedBeOBox );

	 shiftXYZ = HepTranslate3D(0.0, 
				   kBeOHybridYStartL4S1, 
				   kBeOHybridZStartL4 );

	 double idouble = (double)i;
	 double numdouble = (double)IRGeomParameters::kNumBoxes4;

	 rotateAboutZ = HepRotateZ3D( 
	    (((1+ 2.0*idouble)/(2.0 * numdouble)) * 360.0) * k_degrees );

	 flipShiftZ = rotateAboutZ * shiftXYZ;

	 DGPlacedVolumePtr spEastBeOHybrid = 
	    new DGPlacedVolume( spBeOHybrid,
				1,
				flipShiftZ,
				DGTransform::Identity);
      
	 pIRMother->addChild(spEastBeOHybrid);
      }
   } // end of scope change

   { // change scope because SunOS/CC screws up scope of loop variable
      // Layer 4 BeO Electronics Hybrids Set 2, Using components created above
      for (int i = 0; i < IRGeomParameters::kNumBoxes4; i++)
      {
	 DGVolumeParentPlaced* pParentPlacedBeOBox = 
	    new DGVolumeParentPlaced( 
	       IRGeomParameters::kBeOHybridL4Set2VolName[i],
	       spBeOBox );
   
	 pParentPlacedBeOBox->addChild( spPlacedBeOBoard );
	 pParentPlacedBeOBox->addChild( spPlacedTopBeOCuPad );
	 pParentPlacedBeOBox->addChild( spPlacedBottomBeOCuPad );

	 DGVolumePtr spBeOHybrid ( pParentPlacedBeOBox );


	 shiftXYZ = HepTranslate3D(0.0, 
				   kBeOHybridYStartL4S2, 
				   kBeOHybridZStartL4 );

	 double idouble = (double)i;
	 double numdouble = (double)IRGeomParameters::kNumBoxes4;

	 rotateAboutZ = HepRotateZ3D( 
	    ((idouble/numdouble) * 360.0) * k_degrees );

	 flipShiftZ = rotateAboutZ * shiftXYZ;

	 DGPlacedVolumePtr spEastBeOHybrid = 
	    new DGPlacedVolume( spBeOHybrid,
				1,
				flipShiftZ,
				DGTransform::Identity);
      
	 pIRMother->addChild(spEastBeOHybrid);
      }
   } // end of scope change

   // Inner Aluminum Cone
   DGVolumePtr spAlInnerCone = 
       new DGVolumePolyCone( IRGeomParameters::kAlInnerConeVolName,
			     pAluminum,
			     alInnerRadiiList );

   shiftZ = HepTranslateZ3D( kAlInnerConeZStart);

   DGPlacedVolumePtr spEastAlInnerCone = 
       new DGPlacedVolume( spAlInnerCone,
			   1,
			   shiftZ,
			   DGTransform::Identity);

   pIRMother->addChild(spEastAlInnerCone);

   { // change scope because SunOS/CC screws up scope of loop variable
      // Inner Aluminum Radial Boxes
      for (int i = 0, j = 0; i < IRGeomParameters::kAlNumBoxes2; i++, j++)
      {
	 if ((j == 2) || (j == 7))
	 {
	    i--;
	 }
	 else
	 {
	    DGVolumePtr spRadialBox = 
	       new DGVolumeBox( IRGeomParameters::kAlRadialBoxVolName2[i],
				pAluminum,
				kAlBoxDims2 );
	 
	    shiftXYZ = HepTranslate3D(0.0, 
				      kAlRadialBoxYStart2, 
				      kAlRadialBoxZStart2 );
	 
	    double jdouble = (double)j;
	    double numdouble = (double)IRGeomParameters::kAlNumBoxes2;
	    double numstops = numdouble + 2.0;

	    rotateAboutZ = HepRotateZ3D( 
	       ((jdouble/numstops + 1.0/(2.0 * numstops)) * 360.0) * k_degrees );
	 
	    flipShiftZ = rotateAboutZ * shiftXYZ;
	 
	    DGPlacedVolumePtr spEastRadialBox = 
	       new DGPlacedVolume( spRadialBox,
				   1,
				   flipShiftZ,
				   DGTransform::Identity);
	 
	    pIRMother->addChild(spEastRadialBox);
	 }
      }
   } // end of scope change


   // Carbon Fiber Cone
   DGVolumePtr spCFiberCone = 
       new DGVolumeCone( IRGeomParameters::kCFiberConeVolName,
			 pCarbon,
			 kCFiberConeRadiiZ1,
			 kCFiberConeRadiiZ2,
			 kCFiberConeZHalfLength );

   shiftZ = HepTranslateZ3D( kCFiberConeZStart + kCFiberConeZHalfLength);

   DGPlacedVolumePtr spEastCFiberCone = 
       new DGPlacedVolume( spCFiberCone,
			   1,
			   shiftZ,
			   DGTransform::Identity);

   pIRMother->addChild(spEastCFiberCone);

   // Outer Aluminum Cone
   DGVolumePtr spAlOuterCone = 
       new DGVolumePolyCone( IRGeomParameters::kAlOuterConeVolName,
			     pAluminum,
			     alOuterRadiiList );

   shiftZ = HepTranslateZ3D( kAlOuterConeZStart );

   DGPlacedVolumePtr spEastAlOuterCone = 
       new DGPlacedVolume( spAlOuterCone,
			   1,
			   shiftZ,
			   DGTransform::Identity);

   pIRMother->addChild(spEastAlOuterCone);

   { // change scope because SunOS/CC screws up scope of loop variable
      // Outer Aluminum Radial Boxes
      for (int i = 0; i < IRGeomParameters::kAlNumBoxes1; i++)
      {
	 DGVolumePtr spRadialBox = 
	    new DGVolumeBox( IRGeomParameters::kAlRadialBoxVolName1[i],
			     pAluminum,
			     kAlBoxDims1 );
	 
	 shiftXYZ = HepTranslate3D(0.0, 
				   kAlRadialBoxYStart1, 
				   kAlRadialBoxZStart1 );

	 double idouble = (double)i;
	 double numdouble = (double)IRGeomParameters::kAlNumBoxes1;

	 rotateAboutZ = HepRotateZ3D( 
	    ((idouble/numdouble + 1.0/(2.0 * numdouble) ) * 360.0 ) * k_degrees );

	 flipShiftZ = rotateAboutZ * shiftXYZ;

	 DGPlacedVolumePtr spEastRadialBox = 
	    new DGPlacedVolume( spRadialBox,
				1,
				flipShiftZ,
				DGTransform::Identity);
      
	 pIRMother->addChild(spEastRadialBox);
      }
   } // end of scope change


   // Inner Copper Cone
   DGVolumePtr spCuInnerCone = 
       new DGVolumeCone( IRGeomParameters::kCuInnerConeVolName,
			 pCopper,
			 kCuInnerConeRadiiZ1, 
			 kCuInnerConeRadiiZ2,
			 kCuInnerConeZHalfLength );

   shiftZ = HepTranslateZ3D( kCuInnerConeZStart + kCuInnerConeZHalfLength );

   DGPlacedVolumePtr spEastCuInnerCone = 
       new DGPlacedVolume( spCuInnerCone,
			   1,
			   shiftZ,
			   DGTransform::Identity);

   pIRMother->addChild(spEastCuInnerCone);

   // Inner Copper Lip
   DGVolumePtr spCuInnerLip = 
      new DGVolumeCone( 
	 IRGeomParameters::kCuInnerLipVolName,
	 pCopper,
	 kCuInnerLipZ1Radii,
	 kCuInnerLipZ2Radii,
	 kCuInnerLipZHalfLength );
      
   shiftZ = HepTranslateZ3D( 
      kCuInnerLipZStart + kCuInnerLipZHalfLength );
   
   DGPlacedVolumePtr spEastCuInnerLip = 
      new DGPlacedVolume( spCuInnerLip,
			  1,
			  shiftZ,
			  DGTransform::Identity);
   
   pIRMother->addChild(spEastCuInnerLip);

   // Aluminum Nose Cone
   DGVolumePtr spNose12 = 
       new DGVolumePolyCone( IRGeomParameters::kAl12NoseConeVolName,
			     pAluminum,
			     nose12RadiiList );

   shiftZ = HepTranslateZ3D( kNose12ZStart );

   DGPlacedVolumePtr spEastNose12 = 
       new DGPlacedVolume( spNose12,
			   1,
			   shiftZ,
			   DGTransform::Identity);

   pIRMother->addChild(spEastNose12);
}












