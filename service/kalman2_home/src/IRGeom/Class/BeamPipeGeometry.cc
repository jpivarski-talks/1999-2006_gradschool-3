// -*- C++ -*-
//
// Package:     IRGeom
// Module:      BeamPipeGeometry
// 
// Description: function class to create IRGeom Beam Pipe
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Aren Jansen
// Created:     Tue Jun 15 16:06:57 EDT 1999
// $Id: BeamPipeGeometry.cc,v 1.3 2002/05/31 17:19:45 bkh Exp $
//
// Revision history
//
// $Log: BeamPipeGeometry.cc,v $
// Revision 1.3  2002/05/31 17:19:45  bkh
// Repair so does not overlap DR or CC endcap and also
// make it one sided only and replicate that side for negative z
//
// Revision 1.2  1999/07/23 22:23:42  lkg
// Encase various for loops in a pair of {} to deal with screwy loop index
// scope under SunOs/CC
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

#include "IRGeom/BeamPipeGeometry.h"
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
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
typedef double IRreal;
typedef pair<IRreal,IRreal> IRpair;

static const char* const kFacilityString = "IRGeom.BeamPipeGeometry" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: BeamPipeGeometry.cc,v 1.3 2002/05/31 17:19:45 bkh Exp $";
static const char* const kTagString = "$Name: v01_01_02 $";

//
// static data member definitions
//

//
// constructors and destructor
// 
BeamPipeGeometry::BeamPipeGeometry(const Record& iRecord): m_Record(iRecord)
{
}

// BeamPipeGeometry::BeamPipeGeometry( const BeamPipeGeometry& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

BeamPipeGeometry::~BeamPipeGeometry()
{
}

//
// assignment operators
//
// const BeamPipeGeometry& BeamPipeGeometry::operator=( const BeamPipeGeometry& rhs )
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
BeamPipeGeometry::operator()(DGVolumeParentPlaced* pIRMother)
{
   report(INFO, kFacilityString) << "Constructing the CLEO3 beam pipe." << endl;
    
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
   const IRreal kInnerBePipeOuterRadius = (1.720/2.0) * k_in;
   const IRreal kInnerBePipeThickness = 0.029 * k_in;
   const IRreal kInnerBePipeInnerRadius = 
      kInnerBePipeOuterRadius - kInnerBePipeThickness;
   
   const IRreal kOuterBePipeInnerRadius = (1.748/2.0) * k_in;
   const IRreal kOuterBePipeOuterRadius = (1.814/2.0) * k_in;
   const IRreal kOuterBePipeThickness = 
      kOuterBePipeOuterRadius - kOuterBePipeInnerRadius;

   const IRreal kBeHalfLength =  (11.617 * k_in)/2.0;

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
   { // change scope because SunOS/CC screws up scope of loop variable
      for ( int i = 0; i < 5; i++ ) {
	 DGZrR tmpRadii(kAlCapZ[i], kAlCapInnerRadius, kAlCapOuterRadius[i]);
	 AlRadiiVector.push_back( tmpRadii );
      }
   } // end of scope change
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

   // turn the Cu pipe dimensions into the list of Radii needed for a polycone
   _vector_DGZrR_  cuRadiiVector;

   { // change scope because SunOS/CC screws up scope of loop variable
      for ( int i = 0; i < 3; i++ ) {
	 IRreal outerRadius = kCuThinPipeRadius[i] + kCuPipeThicknessA;
	 DGZrR tmpRadii(kCuThinPipeZ[i],
			kCuThinPipeRadius[i],
			outerRadius);
	 cuRadiiVector.push_back( tmpRadii );
      }
   } // end of scope change

   { // change scope because SunOS/CC screws up scope of loop variable
      for ( int i = 0; i < 3; i++ ) {
	 IRreal outerRadius = kCuThickPipeRadius[i] + kCuPipeThicknessB;
	 DGZrR tmpRadii(kCuThickPipeZ[i],
			kCuThickPipeRadius[i],
			outerRadius);
	 cuRadiiVector.push_back( tmpRadii );
      }
   } // end of scope change
   DGZrRList cuRadiiList( cuRadiiVector );


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
   DGVolumePtr spCuPipe = new DGVolumePolyCone(
                                        IRGeomParameters::kCuPipeVolName,
					pCopper,
					cuRadiiList );


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

   DGPlacedVolumePtr spWestAlCap =
      new DGPlacedVolume( spAlCap,
			  1,
			  DGTransform::Identity,
			  DGTransform::Identity );
   pIRMother->addChild( spWestAlCap );

   DGPlacedVolumePtr spWestCuPipe = 
      new DGPlacedVolume( spCuPipe,
			  1,
			  DGTransform::Identity,
			  DGTransform::Identity );
   pIRMother->addChild( spWestCuPipe );

}

//
// const member functions
//

//
// static member functions
//















