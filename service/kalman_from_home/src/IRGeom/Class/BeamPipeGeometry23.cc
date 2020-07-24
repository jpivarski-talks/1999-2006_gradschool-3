// -*- C++ -*-
//
// Package:     <package>
// Module:      BeamPipeGeometry23
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Aren Jansen
// Created:     Tue Jun 22 11:49:15 EDT 1999
// $Id: BeamPipeGeometry23.cc,v 1.5 2002/10/25 20:40:52 bkh Exp $
//
// Revision history
//
// $Log: BeamPipeGeometry23.cc,v $
// Revision 1.5  2002/10/25 20:40:52  bkh
// Correct beam pipe thickness (was .038" too thick)
//
// Revision 1.4  2002/05/31 17:19:45  bkh
// Repair so does not overlap DR or CC endcap and also
// make it one sided only and replicate that side for negative z
//
// Revision 1.3  2000/03/06 18:38:52  lkg
// Break Cu pipe into two sections to avoid GEANT limit on volume parameters
//
// Revision 1.2  1999/07/23 22:23:43  lkg
// Encase various for loops in a pair of {} to deal with screwy loop index
// scope under SunOs/CC
//
// Revision 1.1.1.1  1999/07/22 14:38:43  lkg
// First release
//

#include "Experiment/Experiment.h"

// system include files
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

#include "IRGeom/BeamPipeGeometry23.h"
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

static const char* const kFacilityString = "IRGeom.BeamPipeGeometry23" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: BeamPipeGeometry23.cc,v 1.5 2002/10/25 20:40:52 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
BeamPipeGeometry23::BeamPipeGeometry23(const Record& iRecord)
    : m_Record(iRecord)
{
}

// BeamPipeGeometry23::BeamPipeGeometry23( const BeamPipeGeometry23& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

BeamPipeGeometry23::~BeamPipeGeometry23()
{
}

//
// assignment operators
//
// const BeamPipeGeometry23& BeamPipeGeometry23::operator=( const BeamPipeGeometry23& rhs )
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
BeamPipeGeometry23::operator()(DGVolumeParentPlaced* pIRMother)
{
   report(INFO, kFacilityString) 
       << "Constructing the CLEO2-3 beam pipe." << endl;
    
   // ------------ get the materials ------------- //
   DGMaterialStore materials ;

   extract( m_Record, materials );
   

   DGConstMaterialPtr pAir       = materials[ kAirMatID ];
   DGConstMaterialPtr pBeryllium = materials[ kBerylliumMatID ];
   DGConstMaterialPtr pCopper    = materials[ kCopperMatID ];
   DGConstMaterialPtr pGold      = materials[ kGoldMatID ];
   DGConstMaterialPtr pWater     = materials[ kWaterMatID ];
   DGConstMaterialPtr pAluminum  = materials[ kAluminumMatID ];
   
   // ------------ Define some useful transformations ------------ //
   HepRotateX3D  flipZ( 180 * k_degrees );
   

   // ----- The central most section of the Interaction Region

   //   dimensions: Beryllium tubes
   const IRreal kInnerBePipeOuterRadius = (1.678/2.0) * k_in;
   const IRreal kInnerBePipeThickness = 0.013 * k_in;
   const IRreal kInnerBePipeInnerRadius = 
      kInnerBePipeOuterRadius - kInnerBePipeThickness;
   
   const IRreal kOuterBePipeInnerRadius = (1.710/2.0) * k_in;
   const IRreal kOuterBePipeOuterRadius = (1.728/2.0) * k_in;
   const IRreal kOuterBePipeThickness = 
      kOuterBePipeOuterRadius - kOuterBePipeInnerRadius;

   const IRreal kBeHalfLength =  18.0 * k_cm;

   //   dimesions: Gold coating
   const IRreal kAuThickness = 5.0 * k_um;


   //   dimensions:  Aluminum Cap
   const IRreal kAlCapZStart = ((14.426/2.0) - 1.257 - (0.858 - 0.713))*k_in;
   const IRreal kAlCapZEnd = kAlCapZStart + 0.858 * k_in;
   const IRreal kAlCapZ[6] = { kAlCapZStart, 
			       kAlCapZEnd - (0.713*k_in), 
			       kAlCapZEnd - (0.713*k_in), 
			       kAlCapZEnd - 0.134*k_in, 
			       kAlCapZEnd - 0.030*k_in, 
			       kAlCapZEnd};
   const IRreal kAlCapInnerRadius = (1.728/2.0) * k_in;
   const IRreal kAlCapOuterRadius[6] = { (1.776/2.0)*k_in, 
					 (1.776/2.0)*k_in, 
					 (2.007/2.0)*k_in, 
					 (2.337/2.0)*k_in, 
					 ((1.776/2.0)+0.020)*k_in, 
					 ((1.776/2.0)+0.020)*k_in };

   // turn the Al Cap dimentions into the list of Radii needed for a polycone
   _vector_DGZrR_ AlRadiiVector;
   for ( int i = 0; i < 5; i++ ) {
      DGZrR tmpRadii(kAlCapZ[i], kAlCapInnerRadius, kAlCapOuterRadius[i]);
      AlRadiiVector.push_back( tmpRadii );
   }
   DGZrRList AlRadiiList( AlRadiiVector );

/* Obsolete non-tapered cu pipe code
   //   dimensions: Copper pipe Z locations
   const IRreal kCuPipeZStart = kBeHalfLength;
   const IRreal kCuPipeZEnd = (23.26/2.0)*k_in;
   const IRreal kCuPipeInnerRadius = (1.300/2.0)*k_in;
   const IRreal kCuPipeOuterRadius = ((1.776/2.0)+0.020)*k_in;


   //      Cu Pipe
   const IRreal kCuThinPipeZ[2] = {
      kCuPipeZStart, kCuPipeZEnd};
   const IRreal kCuThinPipeRadius[] = {
      kCuPipeInnerRadiusStart, kCuTipTaperR,  2.7*k_cm};


   // Turn the Cu pipe dimensions into the list of Radii needed for a polycone
   _vector_DGZrR_  cuRadiiVector;
   for ( int i = 0; i < 2; i++ ) {
      DGZrR tmpRadii(kCuThinPipeZ[i], kCuPipeInnerRadius, kCuPipeOuterRadius);
      cuRadiiVector.push_back( tmpRadii );
   }
*/

/*
   //   dimensions:  final radius for taper in the Cu pipe tip
   //                needed for pipe and masking
   const IRreal kCuTipTaperZ = (25.0 * k_cm) + (kAlCapZEnd - kBeHalfLength);
   const IRreal kCuTipTaperR =  1.5 * k_cm;

   //   dimensions: Copper pipe thicknesses and length
   const IRreal kCuPipeThicknessA = 0.4 * k_cm;
   const IRreal kCuPipeThicknessB = 0.6 * k_cm;
   const IRreal kCuPipeLength = 243.0 * k_cm;

   //   dimensions: Copper pipe Z locations
   const IRreal kCuPipeZStart = kAlCapZEnd;
   const IRreal kCuPipeZEnd = kAlCapZEnd + kCuPipeLength;
   const IRreal kCuPipeInnerRadiusStart = 2.1 * k_cm;


   //      Thin Pipe
   const IRreal kCuThinPipeZ[3] = {
      kCuPipeZStart,           kCuTipTaperZ, 61.3*k_cm};

   const IRreal kCuThinPipeRadius[3] = {
      kCuPipeInnerRadiusStart, kCuTipTaperR,  2.7*k_cm};

   //      Thick Pipe
   const IRreal kCuThickPipeZ[3] = {
      74.0*k_cm, 195.0*k_cm, kCuPipeZEnd};

   const IRreal kCuThickPipeRadius[3] = {
       2.7*k_cm,   6.0*k_cm, 6.0*k_cm};
*/

   // dimensions: polycone break points

   const int kCuLoop2Start = 11;
   const int BREAKNUM = 19;

   IRreal cuPipeZ[BREAKNUM] = {
       18.0, 18.7, 18.7, 21.1, 21.1, 25.0, 
       25.6, 25.6, 26.8, 26.8, 30.7, 30.7, 
       34.6, 34.6, 42.5, 42.5, 58.1, 100.0, 
       250.0};
   
   IRreal cuPipeInnerRadii[BREAKNUM] = {
       2.098,                                         // Cu Pipe 1
       2.098 - ((2.098-1.5)/(25.0-18.0))*(18.7-18.0),
       2.098 - ((2.098-1.5)/(25.0-18.0))*(18.7-18.0),
       2.098 - ((2.098-1.5)/(25.0-18.0))*(21.1-18.0),
       2.098 - ((2.098-1.5)/(25.0-18.0))*(21.1-18.0),
       1.5,                                           
       1.5 + ((4.5-1.5)/(100.0-25.0))*(25.6-25.0),    // Cu Pipe 2
       1.5 + ((4.5-1.5)/(100.0-25.0))*(25.6-25.0),
       1.5 + ((4.5-1.5)/(100.0-25.0))*(26.8-25.0),
       1.5 + ((4.5-1.5)/(100.0-25.0))*(26.8-25.0),
       1.5 + ((4.5-1.5)/(100.0-25.0))*(30.7-25.0),
       1.5 + ((4.5-1.5)/(100.0-25.0))*(30.7-25.0),  // Cu Pipe 3
       1.5 + ((4.5-1.5)/(100.0-25.0))*(34.6-25.0),
       1.5 + ((4.5-1.5)/(100.0-25.0))*(34.6-25.0),
       1.5 + ((4.5-1.5)/(100.0-25.0))*(42.5-25.0),
       1.5 + ((4.5-1.5)/(100.0-25.0))*(42.5-25.0),
       1.5 + ((4.5-1.5)/(100.0-25.0))*(58.1-25.0),
       4.5, 4.5};

   IRreal cuPipeOuterRadii[BREAKNUM] = {
       2.196, 2.196, 5.12, 5.72, 2.77, 2.77, 
       2.77,  5.12,  5.12, 3.34, 3.34, 2.92,
       2.92,  2.41,  2.41, 3.01, 
       0.5 + (1.5 + ((4.5-1.5)/(100.0-25.0))*(58.1-25.0)),
       5.0, 5.0};
       
   { // change scope because SunOS/CC screws up scope of loop variable
      for (int i = 0; i < BREAKNUM; ++i) {
	 cuPipeZ[i] *= k_cm;
	 cuPipeInnerRadii[i] *= k_cm;
	 cuPipeOuterRadii[i] *= k_cm;
      }
   } // end of scope change

   // we must break the pipe into two pieces because of a limit in
   // turn the Cu pipe dimensions into the list of Radii needed for a polycone
   _vector_DGZrR_  cuRadiiVector1;
   _vector_DGZrR_  cuRadiiVector2;

   // GEANT on the number of parameters (50)
   { // change scope because SunOS/CC screws up scope of loop variable
      for ( int i = 0; i < kCuLoop2Start; i++ ) {
	 DGZrR tmpRadii(cuPipeZ[i],
			cuPipeInnerRadii[i],
			cuPipeOuterRadii[i]);
	 cuRadiiVector1.push_back( tmpRadii );
      }
   } // end of scope change
   { // change scope because SunOS/CC screws up scope of loop variable
      for ( int i = kCuLoop2Start; i < BREAKNUM; i++ ) {
	 DGZrR tmpRadii(cuPipeZ[i],
			cuPipeInnerRadii[i],
			cuPipeOuterRadii[i]);
	 cuRadiiVector2.push_back( tmpRadii );
      }
   } // end of scope change
   DGZrRList cuRadiiList1( cuRadiiVector1 );
   DGZrRList cuRadiiList2( cuRadiiVector2 );


/*   
   // --------------- Assemble the inner beam pipe section --------------
   
   // --- create a mother volume that will house the Be and Cu pipes. --- //
   //     Fill this volume with vacuum                                    //

   IRreal kVacuumLength = kCuPipeZEnd;
   IRreal kVacuumInnerRad = 0.0;


   // Find the maximum outer radius of the various components             //
   //    Its outer radius must not conflict with the silicon detector,    //
   //    a check that we should add when we have the Silicon geometry     //

   IRreal kVacuumOuterRad = (2.337/2.0) * k_in;
   _vector_DGZrR_::iterator itEnd( cuRadiiVector.end() );
   for( _vector_DGZrR_::iterator radius( cuRadiiVector.begin() ) ;
	radius != itEnd;
	++radius )
   {
      IRreal checkRadius = (*radius).outerRadius();
      if ( checkRadius > kVacuumOuterRad ) {
	 kVacuumOuterRad = checkRadius;
      }
   }
   report( INFO, kFacilityString )
      << "Maximum radius of Cu Tube: " << kVacuumOuterRad << endl;
   
    
    // define the shape of the containing volume, a tube                  //
    IRpair kVacuumRadii( kVacuumInnerRad, kVacuumOuterRad );
    DGVolumePtr pBPMotherTube = 
	new DGVolumeTube( "",
			  pVacuum,
			  kVacuumRadii,			  
			  kVacuumLength );

    // now the logical volume and smart pointer                           //
    DGVolumeParentPlaced* pBPMother = 
	new DGVolumeParentPlaced( IRGeomParameters::kBPMotherVolName,
				  pBPMotherTube );
    DGVolumePtr spBPMother( pBPMother );
    
 */
 

   // --- Now create all of the volumes contained in this mother volume --- //
   //   the outer Beryllium beam pipe section, a tube                       //
   
   IRpair kOuterBePipeRadii( kOuterBePipeInnerRadius, 
			     kOuterBePipeOuterRadius );
   DGVolumePtr spOuterBePipe = 
      new DGVolumeTube( IRGeomParameters::kOuterBePipeVolName,
					    pBeryllium,
					    kOuterBePipeRadii,
					    kBeHalfLength/2 );
   
   //   the inner Beryllium beam pipe section, a tube
   IRpair kInnerBePipeRadii( kInnerBePipeInnerRadius, 
			     kInnerBePipeOuterRadius );
   DGVolumePtr spInnerBePipe = 
      new DGVolumeTube( IRGeomParameters::kInnerBePipeVolName,
					    pBeryllium,
					    kInnerBePipeRadii,
					    kBeHalfLength/2 );

   //   line the Inner beam pipe with Gold                                 //
   IRpair kAuLiningRadii(kInnerBePipeInnerRadius - kAuThickness, 
			 kInnerBePipeInnerRadius );
   DGVolumePtr spAuLining = new DGVolumeTube(
                                        IRGeomParameters::kAuLiningVolName,
					pGold,
					kAuLiningRadii,
					kBeHalfLength/2 );

   //   the coolant pipe section with Water                               //
   IRpair kCoolantPipeRadii(kInnerBePipeOuterRadius, 
			    kOuterBePipeInnerRadius );
   DGVolumePtr spCoolantPipe = new DGVolumeTube(
                                        IRGeomParameters::kCoolantPipeVolName,
					pWater,
					kCoolantPipeRadii,
					kBeHalfLength/2 );

   //   create the Aluminum Cap                                           //
   DGVolumePtr spAlCap = new DGVolumePolyCone(
                                        IRGeomParameters::kAlCapVolName, 
					pAluminum,
					AlRadiiList );

   //   create the Copper Pipe                                            //
   DGVolumePtr spCuPipe1 = new DGVolumePolyCone(
                                        IRGeomParameters::kCuPipeVolName1,
					pCopper,
					cuRadiiList1 );
   DGVolumePtr spCuPipe2 = new DGVolumePolyCone(
                                        IRGeomParameters::kCuPipeVolName2,
					pCopper,
					cuRadiiList2 );

   const HepTranslateZ3D moveZ ( kBeHalfLength/2 ) ;

   // --- Place the pipes into the IR mother volume --- //
   DGPlacedVolumePtr spPlacedOuterBePipe = 
      new DGPlacedVolume( spOuterBePipe, 
			  1, 
			  moveZ, 
			  DGTransform::Identity );
   pIRMother->addChild( spPlacedOuterBePipe );

   DGPlacedVolumePtr spPlacedInnerBePipe =
      new DGPlacedVolume( spInnerBePipe,
			  1,
			  moveZ,
			  DGTransform::Identity );
   pIRMother->addChild( spPlacedInnerBePipe );

   DGPlacedVolumePtr spPlacedAuLining = 
      new DGPlacedVolume( spAuLining, 
			  1, 
			  moveZ, 
			  DGTransform::Identity ); 
   pIRMother->addChild( spPlacedAuLining );

   DGPlacedVolumePtr spPlacedCoolantPipe =
      new DGPlacedVolume( spCoolantPipe,
			  1,
			  moveZ,
			  DGTransform::Identity );
   pIRMother->addChild( spPlacedCoolantPipe );

//----------------------------------------------

   DGPlacedVolumePtr spWestAlCap =
      new DGPlacedVolume( spAlCap,
			  1,
			  DGTransform::Identity,
			  DGTransform::Identity );
   pIRMother->addChild( spWestAlCap );

   DGPlacedVolumePtr spWestCuPipe1 = 
      new DGPlacedVolume( spCuPipe1,
			  1,
			  DGTransform::Identity,
			  DGTransform::Identity );
   pIRMother->addChild( spWestCuPipe1 );
   DGPlacedVolumePtr spWestCuPipe2 = 
      new DGPlacedVolume( spCuPipe2,
			  1,
			  DGTransform::Identity,
			  DGTransform::Identity );
   pIRMother->addChild( spWestCuPipe2 );

}

//
// const member functions
//

//
// static member functions
//
