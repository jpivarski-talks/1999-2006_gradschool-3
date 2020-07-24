// -*- C++ -*-
//
// Package:     <package>
// Module:      SiGeometryProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Alexander Undrus
// Created:     Fri May 22 22:36:27 EDT 1998
// $Id: SiGeometryProxy.cc,v 1.28 2002/05/31 17:22:39 bkh Exp $
//
// Revision history
//
// $Log: SiGeometryProxy.cc,v $
// Revision 1.28  2002/05/31 17:22:39  bkh
// Fix so does not erase IRGeom components
//
// Revision 1.27  2002/04/25 16:13:22  bkh
// Complete fix of Si Geometry for putting diamond inside ladders
//
// Revision 1.26  2002/03/04 21:18:01  bkh
// Make parent volumes enclose children completely; revamp of ladder object
//
// Revision 1.25  2001/05/01 20:07:28  ajm36
// put the vees on the ladders right (I think)
//
// Revision 1.24  2000/03/28 15:38:08  pg
// Fixed SiAlignedGeometry proxy
//
// Revision 1.23  2000/02/03 14:45:30  pg
//   Assigned unique names for all ladders and their contents.
// So, now we have 61 separate volumes to hold ladders, with sensors
// inside getting unique names as well (61 of them). This is forced
// by GEANT requirements.
//
// Revision 1.22  2000/01/26 20:44:32  undrus
//  copy numbers of sensors are now increasing from West to East
//
// Revision 1.21  2000/01/20 02:04:44  undrus
// Use Air for the material of bounding boxes etc (was vacuum)
//
// Revision 1.20  2000/01/14 02:53:42  undrus
// fixed bug in calculation of east sensors; names in EVS description reduced
// to 4 symbols
//
// Revision 1.19  2000/01/06 21:41:22  undrus
// Improved programming style, the material for empty spaces is Vacuum (was Air occasionally)
//
// Revision 1.18  2000/01/06 02:55:01  undrus
// Vees become parts of ladders; Names of Vees get the layer number
//
// Revision 1.17  2000/01/06 01:38:51  undrus
// Added Layer level in volume hierarchy
//
// Revision 1.16  1999/12/20 01:18:27  undrus
//  changed number of sensors in hybrids for layer 3 (outer radius ladders - 4 to the West, 3 to the East) and for layer 1 (ladder 1-3 - 2 sensors to the West)
//
// Revision 1.15  1999/11/16 20:25:40  pg
// Modifications for use in Cleog3
//
// Revision 1.14  1999/10/05 17:31:16  undrus
//  upgrade cout commands to report(DEBUG ...
//
// Revision 1.13  1999/07/27 22:32:58  lkg
// Al -> CarbonFiber for tube material
//
// Revision 1.12  1999/06/30 22:34:53  pg
// Fixed a bug for Extra Volume Spec proxy. Couple of bugs fixed in SiGeometryProxy: copy numbers for Vees and bounding volumes for the lab frame caused problems for the Geant3Interface.
//
// Revision 1.11  1999/05/13 03:31:44  cleo3
//  begin numbering with 1 to accomodate requirements of  DetectorGeometry
//
// Revision 1.10  1999/03/26 17:57:10  cleo3
//  minor changes to satisfy CC compiler
//
// Revision 1.9  1999/03/25 20:33:32  cleo3
//  added passive elements : Vees and Cylinder
//
// Revision 1.8  1999/01/29 16:44:13  pg
// Corrected Si sensor length
//
// Revision 1.7  1999/01/21 18:35:39  cleo3
//  kAirMatID is used for the gas inside bounding volume, Silicon is used as sensor's material
//
// Revision 1.6  1999/01/19 22:37:06  bkh
// Material reworked
//
// Revision 1.5  1998/10/20 18:44:14  bkh
// make compatible with new DetectorGeometry
//
// Revision 1.4  1998/09/07 19:57:01  bkh
// update to new DetectorGeometry
//
// Revision 1.3  1998/08/27 03:17:25  cleo3
//  West sensors were flipped about Y axis, copy numbers of ladders begin with 0 (was 1)
//
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Experiment/units.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"
 
#include "SiGeom/SiGeometryProxy.h"
#include "SiGeom/SiGeomDescription.h"

#include "DetectorGeometry/DGVolume.h"
#include "DetectorGeometry/DGVolumePtr.h"
#include "DetectorGeometry/DGMaterial.h"
//#include "DetectorGeometry/DGMaterialPtr.h"
//#include "DetectorGeometry/DGMaterialStore.h"
#include "DetectorGeometry/DGConstMaterialPtr.h"
#include "DetectorGeometry/DGPlacedVolume.h"
#include "DetectorGeometry/DGPlacedVolumePtr.h"
#include "DetectorGeometry/DGVolumeTube.h"
#include "DetectorGeometry/DGVolumeBox.h"
#include "DetectorGeometry/DGVolumeTrapezoidSym.h"
#include "DetectorGeometry/DGVolumeParentPlaced.h"

#include "DetectorGeometry/DGMaterialStore.h"
#include "DetectorGeometry/DGDetectorStore.h"
#include "DetectorGeometry/DGVolumePolyCone.h"
#include "DetectorGeometry/DGZrRList.h"

#include "ToolBox/Triplet.h"

#include "CLHEP/Geometry/Point3D.h"

#include "MaterialDelivery/material_identifiers.h"
//#include "MaterialDelivery/MaterialDelivery.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "SiGeom.SiGeometryProxy" ;

//
// static data member definitions
//

//
// constructors and destructor
//
SiGeometryProxy::SiGeometryProxy()
   : m_SiGeometry( 0 )
{
}

// SiGeometryProxy::SiGeometryProxy( const SiGeometryProxy& iproxy )
// {
//   *this = iproxy;
// }

SiGeometryProxy::~SiGeometryProxy()
{
   delete m_SiGeometry ;
}

//
// assignment operators
//
// const SiGeometryProxy& SiGeometryProxy::operator=( const SiGeometryProxy& iproxy )
// {
//   if( this != &iproxy ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iproxy );
//   }
//
//   return *this;
// }

//
// member functions
//
void
SiGeometryProxy::invalidateCache()
{
//   cout << "calling  SiGeometryProxy::invalidateCache" << endl;
   delete m_SiGeometry ;
   m_SiGeometry = 0 ;
}

const SiGeometryProxy::value_type*
SiGeometryProxy::faultHandler( const Record& iRecord,
			      const IfdKey& iKey )
{

#if defined(CLEO_DEBUG)   /* just to be paranoid */
   if( 0 != m_SiGeometry )
   {
      report( EMERGENCY, kFacilityString )
	 << "proxy objectP should be 0; this is a programming error!"
	 << endl;
   }
#endif
   
   // first clean up
   delete m_SiGeometry ;

   // ---------- get the materials --------------------
   report(DEBUG, kFacilityString) 
      << "====== Get the materials for Si detector"  << endl;
   DGMaterialStore m_materials;

   extract( iRecord.frame().record( Stream::kBaseGeometry),
            m_materials);

   DGConstMaterialPtr pSilicon  = m_materials[ kSiliconMatID ];
   DGConstMaterialPtr pCarbon   = m_materials[ kSVCarbonFiberMatID ];
   DGConstMaterialPtr pGas      = m_materials[ kAirMatID ];
   DGConstMaterialPtr pVacuum   = m_materials[ kVacuumMatID ];
   DGConstMaterialPtr pDiamond  = m_materials[ kDiamondMatID ];


    // ---------- create the Geometry --------------------

   //Create the necessary Volumes
   report(DEBUG, kFacilityString) 
      << "====== Create the necessary Volumes for Si detector"  << endl;
   
   const float kMotherRadius = SiGeomDescription::kMotherRadius;
   const float kMotherSemiHalfLength = SiGeomDescription::kMotherSemiHalfLength;
   const float kCylRadius            = SiGeomDescription::kCylRadius;
   const float kCylThickness         = SiGeomDescription::kCylThickness;
   const float kCylSemiHalfLength    = SiGeomDescription::kCylSemiHalfLength;
   const float kVeeSide              = SiGeomDescription::kVeeSide;
   const float kVeeThickness         = SiGeomDescription::kVeeThickness;
   const float kSensorThickness      = SiGeomDescription::kSensorThickness;
   const float kSensorWidth          = SiGeomDescription::kSensorWidth;
   const float kZStep                = SiGeomDescription::kZStep;
   const int tempStep                = SiGeomDescription::kMaxPhiStep;
   const double kVeeBBHalfHeight     = SiGeomDescription::kVeeBBHalfHeight ;
   const double kLadderBBHalfHeight  = SiGeomDescription::kLadderBBHalfHeight ;
   const double kyLadderOffset       = SiGeomDescription::kyLadderOffset ;
   const SmallCount MaxPhiStep = 26;


   const double kTrapHalfWidth ( kSensorWidth/2 + 2.5*kSensorThickness ) ;

   const HepTranslateY3D moveY  (  kyLadderOffset ) ;
   const HepTranslateY3D moveYb ( -kyLadderOffset ) ;

   int kNZStep [SiGeomDescription::kNLayers]; 
   int kNZStepEast [SiGeomDescription::kNLayers]; 
   HepVector3D PLayer[SiGeomDescription::kNLayers]; 
   int NPhiStep[SiGeomDescription::kNLayers]; 
   HepVector3D RStep[SiGeomDescription::kNLayers]; 
   float LayerRadMin[SiGeomDescription::kNLayers]; 
   float LayerRadMax[SiGeomDescription::kNLayers]; 

   for(int j7 = 0; j7 < 4; j7++) { 
      kNZStep[j7] = SiGeomDescription::kNZStep[j7];
      kNZStepEast[j7] = SiGeomDescription::kNZStepEast[j7];
      PLayer[j7] = SiGeomDescription::PointLayer[j7];
      NPhiStep[j7] = SiGeomDescription::NPhiStep[j7];
      RStep[j7] = SiGeomDescription::RStep[j7];
      LayerRadMin[j7] = SiGeomDescription::LayerRadMin[j7];
      LayerRadMax[j7] = SiGeomDescription::LayerRadMax[j7];
   }
   
   HepVector3D PLayerShift[SiGeomDescription::kNLayers];
   
   HepVector3D ex1 = RStep[2];
   
//    cout << "  x     " << ex1.x() << endl ;
//    cout << "  y     " << ex1.y() << endl ;
//    cout << "  z     " << ex1.z() << endl ;
//    cout << "  Player[1].z     " <<  PLayer[1].z() << endl ;

  
   for(int j = 0; j < 4; j++) { 
      PLayerShift[j] = PLayer[j] + RStep[j] ;
   } 
   
   ex1 = PLayerShift[3];
   
//--------------CREATE various basic volumes-----------------//  
   //The bounding volume for the Si and lab volumes
   //      cout << "=======bounding volume  "  << endl;

   const UInt32 nSteps ( 6 ) ;
   _vector_DGZrR_ motherVector;

   const double zMother[] = {
      kMotherSemiHalfLength,//0
      kCylSemiHalfLength,//1
      17.09/2.0 * k_in - (0.1472 + 0.8018) * k_in-.001,//2 from IRGeom
      17.09/2.0 * k_in - (0.1472 + 0.8018) * k_in-.001,//3 from IRGeom
      0.116225 * k_m -.002,//4
      0.116225 * k_m -.002};//5

   const double rInnerMother[] = {
      9.45  * k_cm,//0
      3.199 * k_in,//1
      3.199 * k_in,//2
      2.757 * k_in,//3
      0.0459 *k_m ,//4
      2.315 *k_cm   };//5

   for( UInt32 ijk ( 0 ) ; ijk < 2*nSteps ; ++ijk )
   {
      if( nSteps > ijk )
      {
	 DGZrR zrr ( -zMother[ijk], rInnerMother[ijk], kMotherRadius ) ;
	 motherVector.push_back( zrr ) ;
      }
      else
      {
	 const UInt32 j ( 2*nSteps - ijk - 1 ) ;
	 DGZrR zrr (  zMother[j], rInnerMother[j], kMotherRadius ) ;
	 motherVector.push_back( zrr ) ;
      }
   }
      
   DGVolumePtr pBoundingVolume = 
      new DGVolumePolyCone( SiGeomDescription::kSiBBName, 
			    pVacuum,
			    motherVector ) ;

//			   pair<double,double>( 0. ,kMotherRadius ) ,
//			   kMotherSemiHalfLength );
   
   DGVolumePtr pLabBoundingVolume = 
      new DGVolumeTube(SiGeomDescription::kNameLabFrame, 
                       pGas,
                       pair<double,double>( 0. ,kMotherRadius ) ,
                       kMotherSemiHalfLength );
   //The logical volume for the lab frame
   DGVolumeParentPlaced* pLab =
      new DGVolumeParentPlaced( SiGeomDescription::kNameLabFrame,
                                pLabBoundingVolume);
   DGVolumePtr spLab( pLab );

   //The logical volume for the Si
   DGVolumeParentPlaced* pSi = 
      new DGVolumeParentPlaced( SiGeomDescription::kVolumeNameSi,
                                pBoundingVolume );
   DGVolumePtr spSi( pSi );
   
   // The Carbon Fiber Cylinder
   
   DGVolumePtr pCylinder = 
      new DGVolumeTube(SiGeomDescription::kCylName, 
                       pCarbon,
                       pair<double,double>( kCylRadius ,
					    kCylRadius+kCylThickness ) ,
                       kCylSemiHalfLength );

   // Create Vee
   //   cout << "=======Vees"  << endl;

   DGVolumePtr spVeeSide;
   DGVolumeParentPlaced* pVeeBox;
   DGVolumePtr spVeeBox[SiGeomDescription::kNLayers];
   const HepRotateZ3D RotateCW  ( -M_PI/3 );
   const HepRotateZ3D RotateCCW (  M_PI/3 );

   const HepRotateZ3D rotate180Z ( M_PI );

   const HepTranslateX3D TransCA ( ( -kVeeSide-kVeeThickness)/4 );
   const HepTranslateX3D TransCB ( (  kVeeSide+kVeeThickness)/4 );
   DGPlacedVolumePtr spBoxSide;

   for(int mLayer = 0;  mLayer < SiGeomDescription::kNLayers; mLayer++)
   {  
      spVeeSide = new DGVolumeBox 
	 (SiGeomDescription::kVeeSideName[mLayer], pDiamond, 
	  Triplet<double,double,double>((kVeeSide-kVeeThickness)/2,
					kVeeThickness/2,
					kZStep*kNZStep[mLayer]/2.));


      pVeeBox = new DGVolumeParentPlaced(
	 SiGeomDescription::kVeeName[mLayer], 
         new DGVolumeTrapezoidSym(SiGeomDescription::kVeeBBName[mLayer], 
				  pGas, 
				  kVeeSide*0.6,
				  pair<double,double>( kVeeSide*0.6,
						       kVeeThickness ) ,
				  pair<double,double>( kVeeBBHalfHeight,
						       kVeeBBHalfHeight ) ,
				  kZStep*kNZStep[mLayer]/2               ) );
      
      spVeeBox[mLayer] = pVeeBox;

// First Vees side      
      spBoxSide = new DGPlacedVolume(spVeeSide, 1, TransCB*RotateCW, 
                                     DGTransform::Identity); 
      pVeeBox->addChild(spBoxSide); 
// Second Vee side      
      spBoxSide = new DGPlacedVolume(spVeeSide, 2, TransCA*RotateCCW, 
                                     DGTransform::Identity); 
      pVeeBox->addChild(spBoxSide); 
   }   // loop over layers (counter mLayer)

// Create layers, ladders, sensors objects

   DGVolumeParentPlaced* pLayer[SiGeomDescription::kNLayers];
   
   DGVolumePtr spLadder[SiGeomDescription::kNLayers][MaxPhiStep];
   DGPlacedVolumePtr spBoxSensor[10];
   DGVolumeParentPlaced* pLadder;
   DGPlacedVolumePtr spVeePlacedBox;
// Reset sensor and ladder counters 
   int iSenCoun = 0;
   int iLadderCount = 0;

   const double kZLayer[] = {
      0.11, 0.11, .1884, .2685 } ;

   for(int nLayer = 0;  nLayer < SiGeomDescription::kNLayers; nLayer++)
   {  
// Layers
      pLayer[nLayer] = new DGVolumeParentPlaced(
	 SiGeomDescription::kLayerName[nLayer], 
	 new DGVolumeTube(SiGeomDescription::kLayerBBName[nLayer],pVacuum, 
			  pair<double,double>
			  (LayerRadMin[nLayer], LayerRadMax[nLayer]), 
			  kZLayer[nLayer] ));
      
      DGPlacedVolumePtr spLayerPlaced =  new DGPlacedVolume(
	 pLayer[nLayer], 1, DGTransform::Identity, DGTransform::Identity );
      pSi->addChild( spLayerPlaced );
      

// Ladders
      const int NPh = NPhiStep[ nLayer ];

      for(int kLadder = 0; kLadder < NPh; kLadder++)
      {
	 iLadderCount++;
// Every ladder has different name - they are all unique, damn it!
// Bounding boxes don't have to be, though.
	 pLadder = new DGVolumeParentPlaced(
	    SiGeomDescription::kLadderName[iLadderCount-1], 
	    new DGVolumeTrapezoidSym( 
	       SiGeomDescription::kLadderBBName[nLayer],
 	       pGas,
	       kTrapHalfWidth ,
	       pair<double,double>( kTrapHalfWidth ,
				    5.5*kVeeThickness ) ,
	       pair<double,double>( kLadderBBHalfHeight ,
				    kLadderBBHalfHeight  ) ,
	       kZStep*kNZStep[nLayer]/2. + 4*kSensorThickness ));

	 spLadder[nLayer][kLadder] = pLadder;
	 DGVolumePtr spSensor = 
	    new DGVolumeBox(SiGeomDescription::kSensorName[iLadderCount-1], 
			    pSilicon, 
			    Triplet<double,double,double>( kSensorWidth/2.,
							   kSensorThickness/2.,
							   kZStep/2. ) );

//  PUT SENSORS in LADDERS

	 const HepTransform3D Flip ( 0 == nLayer%2 ? DGTransform::Identity :
				     HepReflectY3D()*HepReflectX3D() ) ;
	 
	 const HepTransform3D flipZ ( 3 == nLayer ? 
				      (HepTransform3D) rotate180Z :
				      HepTransform3D::Identity       ) ;

	 int EastSensors;
	 if ( (nLayer == 0 && kLadder >= 0 && kLadder <=2 ) ||
	      (nLayer == 2 && kLadder - (kLadder/2)*2 == 1))
	 { 
	    EastSensors = kNZStepEast[nLayer] - 1 ;
	 }
	 else 
	 {
	    EastSensors = kNZStepEast[nLayer] ;
	 }
         for(int iSensor = kNZStep[nLayer] - 1; iSensor >= 0; iSensor--)    
	 {
	    iSenCoun++;

	    const HepTransform3D FlipWest ( ( iSensor < EastSensors ) ? 
					    DGTransform::Identity     :
					    HepReflectZ3D()*HepReflectX3D() ) ;

	    const HepTranslateZ3D TransZ ( ( 2*iSensor + 1 - kNZStep[nLayer] )*
					   0.5*kZStep );
	    spBoxSensor[iSensor] =
	       new DGPlacedVolume( spSensor, iSenCoun, 
				   moveY*TransZ*FlipWest*Flip*flipZ, 
				   DGTransform::Identity); 
	    pLadder->addChild(spBoxSensor[iSensor]); 
	 } // loop over sensors (counter iSensor)

	 // PUT VEES in LADDERS

	 const HepTranslateY3D tran ( kSensorThickness );

         spVeePlacedBox =
	    new DGPlacedVolume( spVeeBox[nLayer], 1, 
				tran, DGTransform::Identity ) ;       
	 
	 pLadder->addChild( spVeePlacedBox );
      }  // loop over ladders (counter kLadder)
   }  // loop over layer (counter nLayer)
   
   // put Ladders in Layers
   
   for(int iLayer = 0; iLayer < 4; iLayer++)
   { 
      const int NPhi ( NPhiStep[ iLayer ] ) ;
      const double div ( NPhiStep[ iLayer ] ) ;
      const HepVector3D PLayerCurrent=PLayer[iLayer];
      const HepVector3D PLayerShiftCurrent=PLayerShift[iLayer];
      const HepTranslate3D translatexy2 ( PLayerCurrent ); //translate 

      const HepTransform3D flipZ ( 3 == iLayer ? moveY*rotate180Z :
				   (HepTransform3D) moveYb            ) ;
      
      for(int i = 0; i < NPhi; i++)
      {
	 const HepRotateZ3D rotateZZ ( M_PI/div*2.*i );
	 
	 const HepTranslate3D vzz  ( rotateZZ*PLayerCurrent      ) ;
	 const HepTranslate3D vpzz ( rotateZZ*PLayerShiftCurrent ) ;
      
	 const HepTransform3D tran ( ( i%2 != 0 ? vpzz :
				       ( 0 == i ? translatexy2 : vzz ) )*
				     rotateZZ ) ;

// All ladders are unique -> copy number = 1
	 pLayer[ iLayer ]->addChild(
	    new DGPlacedVolume( spLadder[iLayer][i], 1, tran*flipZ,
				DGTransform::Identity ) ) ;
      }
   }   
   
   // Put Cylinder Into Si
             

   DGPlacedVolumePtr spCyl = 
      new DGPlacedVolume( pCylinder, 1, DGTransform::Identity,
                          DGTransform::Identity );
   
   pSi->addChild( spCyl );
   
   //Put the Si into the lab frame
   
   DGPlacedVolumePtr spPlacedSi =
      new DGPlacedVolume( spSi,
                          1,
                          DGTransform::Identity,
                          DGTransform::Identity );
   pLab->addChild( spPlacedSi );
   
   
   //Now set up the storage
   
   m_SiGeometry = new DGDetectorStore ;
   m_SiGeometry->setRoot( spLab );

//    cout <<  m_SiGeometry << endl;

   report(DEBUG, kFacilityString) 
      << "====== Finish of creation of Si detector" << endl; 
   
   return ( m_SiGeometry ) ;

}

//
// const member functions
//

//
// static member functions
//






