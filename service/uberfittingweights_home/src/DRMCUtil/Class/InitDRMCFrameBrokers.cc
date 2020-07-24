// -*- C++ -*-
//
// Package:     <DRMCUtil>
// Module:      InitDRMCFrameBrokers
// 
// Description: C++ Function for updating DR MC F77 access routines
//              
// Implementation:
//     1) CD Geometry
//        Extract ADRSenseWireStore, ADRCathodeStore and fill 
//        cdgeomcd common block via fillFortranCDGeometry.
//        Then initialize CDhitsProxy - a seperate C++ geometry interface for cdhits.F
//        It would be good to remove CDhitsProxy and just use 1 package for this.
//
//     2) DR Calibration
//        Extract DriftFunction and initialize DRMCDriftFB - F77 interface
//
//     3) DR Anode MC Constants
//     Extract the different constants from the Frame and call Fortran 
//     functions which serve the constants to the DR MC Fortran routines
//
//     4) Update F77 routines that serve DR MC Anode constants
//        call drmc_beginrun.F
//
// Author:      John J O'Neill
// Created:     Wed Sep 15 18:02:14 EDT 1999
// $Id: InitDRMCFrameBrokers.cc,v 1.12 2003/06/27 20:11:17 huanggs Exp $
//
// Revision history
//
// $Log: InitDRMCFrameBrokers.cc,v $
// Revision 1.12  2003/06/27 20:11:17  huanggs
// check if extracting ZD
//
// Revision 1.11  2003/04/24 14:55:18  huanggs
// call to fillFortranCDGeometry changed due to ZD
//
// Revision 1.10  2000/04/21 17:16:31  pg
// Axtract DRAligned geometry
//
// Revision 1.9  2000/02/15 00:46:41  jjo
// 1) Improve documentation. 2) do not call cathode routines as
// these aren't used now. 3) Call drmc_beginrun_() which
// used to be called by DRMCUpdateBeginRun.cc but no longer exists.
//
// Revision 1.8  1999/11/22 22:35:33  jjo
// Initialize CDHitsProxy.h
//
// Revision 1.7  1999/11/22 01:07:59  jjo
// Change comment, get ready for DRGeantHits lib submission
//
// Revision 1.6  1999/11/02 17:46:16  lkg
// update to include the cathodeStore now needed by fillFortranCDGeometry
//
// Revision 1.5  1999/10/27 14:48:31  jjo
// Comment out CDHitsProxy.h as it isn't in cvs yet
//
// Revision 1.4  1999/10/26 19:06:04  jjo
// CDHitsProxy initialization added but commented out
// as it doesn't work yet
//
// Revision 1.3  1999/10/20 21:38:54  jjo
// Add exit after assert
//
// Revision 1.2  1999/09/30 22:35:58  mclean
// Singleton interface to CDOffCal::DriftFunction (drift2time,time2tdc,charge2adc)
//
// Revision 1.1  1999/09/20 22:45:26  jjo
// Import into cvs
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"

#include "DRMCUtil/InitDRMCFrameBrokers.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FAConstants.h"

// Used to fill cdgeom Fortran common block 
#include "ADRGeom/ADRSenseWireStore.h"
#include "geometryForFortran/geometryForFortran.h"
#include "DetectorConfiguration/DetectorConfiguration.h"
//#include "CDOffCal/DriftFunction.h"

// DRMC FrameBroker objects - provides Fortran access to DRMC constants 
#include "DRMCUtil/DRMCDriftFB.h"
#include "DRMCUtil/DRMCCathnameFB.h"
#include "DRMCUtil/DRMCCoseffFB.h"
#include "DRMCUtil/DRMCEffFB.h"
#include "DRMCUtil/DRMCIonsFB.h"         
#include "DRMCUtil/DRMCLyreffFB.h"
#include "DRMCUtil/DRMCPhlayrFB.h"
#include "DRMCUtil/DRMCPhwireFB.h"
#include "DRMCUtil/DRMCResolutionsFB.h"
#include "DRMCUtil/DRMCSmearFB.h"
#include "DRMCUtil/DRMCSpefudFB.h"
#include "DRMCUtil/DRMCTmlayrFB.h"
#include "DRMCUtil/DRMCWireffFB.h"

#include "DRGeantHits/CDHitsProxy.h"

// STL classes
#include <vector>
#include <map>

extern "C"{
   extern void drmc_beginrun_();  // updates DR MC resolution constants
}

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "DRMCUtil.InitDRMCFrameBrokers" ;


void InitDRMCFrameBrokers( Frame& iFrame )
{
   report( DEBUG, kFacilityString )
      << "Initializing DRMC FrameBrokers - F77 interface to Frame" << endl;

   FAItem< DetectorConfiguration >                   iConfig   ;
   extract( iFrame.record( Stream::kBaseGeometry ) , iConfig ) ;
   
   // ===============================
   // 1) Get CD Geometry for this run
   // ===============================
   // via Adam Lyon's code in Doit/Class/DoitTrackFinder.cc
   // Extract DR anode geometry
   FAItem< ADRSenseWireStore > senseWireStore;
   extract( iFrame.record(Stream::kDRAlignment), senseWireStore);
   if ( ! senseWireStore.valid() )
   {
      report(EMERGENCY, kFacilityString) << "Invalid ADRSenseWireStore" << endl;
      assert(senseWireStore.valid());
      ::exit( 1 );
   }

   // Extract cathode geometry
   FAItem< ADRCathodeStore > cathodeStore;
   extract( iFrame.record(Stream::kBaseGeometry), cathodeStore);
   if ( ! cathodeStore.valid() )
   {
      report(EMERGENCY, kFacilityString) << "Invalid ADRCathodeStore" << endl;
      assert(cathodeStore.valid());
      ::exit(1);
   }

   // Extract ZD geometry
   FAItem< AZDSenseWireStore > zdStore;
   if( iConfig->useZDnotSI() )
   {
   extract( iFrame.record(Stream::kZDAlignment), zdStore);
   if ( ! zdStore.valid() )
   {
      report(EMERGENCY, kFacilityString) << "Invalid AZDSenseWireStore" << endl;
      assert(zdStore.valid());
      ::exit( 1 );
   }
   }

//   DABoolean fill_SI = false;
//   DABoolean fill_ZD = true;
   DABoolean fill_SI = iConfig->useSInotZD();
   DABoolean fill_ZD = iConfig->useZDnotSI();

   // Fill cdgeomcd common block
//   fillFortranCDGeometry(*senseWireStore, *cathodeStore);
   fillFortranCDGeometry(*senseWireStore, *cathodeStore, &(*zdStore),
                         fill_SI, fill_ZD);

   // Initialize CDhitsProxy - C++ geometry interface for cdhits.F
   // Not ideal using the CD geometry from F77 2 different ways 
   CDHitsProxy* pBroker = CDHitsProxy::instance();
   pBroker->setWireStore(senseWireStore);




   // ==================================
   // 2) Get DR Calibration for this run 
   // ==================================
   // The Stream binding will change from kBeginRun to kDRCalibration in the future.
   // The second argument will eventually be irrelevant when 
   // I convert CDOffCal to get its own geometry pointer internally. (Ken McLean)
   FAItem<DriftFunction> DRCalibration;
   extract( iFrame.record(Stream::kBeginRun), DRCalibration);
   if ( ! DRCalibration.valid() ) {
     report(EMERGENCY, kFacilityString) << "No DR Calibration Available " << endl;
     assert( DRCalibration.valid() );
     ::exit( -1 );
   }
   DRMCDriftFB::instance()->setDrift( DRCalibration, senseWireStore ) ;


   
   // ================================
   // 3) Extract DR Anode MC Constants
   // ================================
   // For all of the constants we do the following
   // 1) Extract constants objects from the Frame
   // 2) require valid constants
   // 3) initialize DRMCxxxxFB (FrameBroker) objects with the constants

   // =============== Cos eff constants ===============
   FAConstants< DBDRMCCoseff > cosEff ; 
   extract( iFrame.record( Stream::kDRSimulation), cosEff );
   if ( ! cosEff.valid() ) {
      assert( cosEff.valid() );
      ::exit( -1 );
   }
   DRMCCoseffFB::instance()->setCoseff( cosEff ) ;

   // =============== MC eff constants ===============
   FAConstants< DBDRMCEff > eff ; 
   extract( iFrame.record( Stream::kDRSimulation), eff );
   if ( ! eff.valid() ) {
      assert( eff.valid() );
      ::exit( -1 );
   }
   DRMCEffFB::instance()->setEff( eff ) ;

   // // =============== DBDRMCHitJump ===============
   // HitJump constants used by cdhits.F but this has been removed from the cleo3
   // version initially just to get things going.  
   // // HitJump constants still have problems
   // FAConstants< DBDRMCHitJump > hitJump ; 
   // extract( iFrame.record( Stream::kDRSimulation), hitJump );
   // if ( ! hitJump.valid() ) {
   //    assert( hitJump.valid() );
   //    ::exit( -1 );
   // }
   // Also, Need to write DRMCHitJumpFB object

   // =============== DBDRMCIons ===============
   FAConstants< DBDRMCIons > ions ; 
   extract( iFrame.record( Stream::kDRSimulation), ions );
   if ( ! ions.valid() ) {
      assert( ions.valid() );
      ::exit( -1 );
   }
   DRMCIonsFB::instance()->setIons( ions ) ;

   // =============== DBDRMCLyreff ===============
   FAConstants< DBDRMCLyreff > layerEff ; 
   extract( iFrame.record( Stream::kDRSimulation), layerEff );
   if ( ! layerEff.valid() ) {
      assert( layerEff.valid() );
      ::exit( -1 );
   }
   DRMCLyreffFB::instance()->setLyreff( layerEff ) ;

   // =============== DBDRMCPhlayr ===============
   FAConstants< DBDRMCPhlayr > phLayer ; 
   extract( iFrame.record( Stream::kDRSimulation), phLayer );
   if ( ! phLayer.valid() ) {
      assert( phLayer.valid() );
      ::exit( -1 );
   }
   DRMCPhlayrFB::instance()->setPhlayr( phLayer ) ;

   // =============== DBDRMCPhwire ===============
   FAConstants< DBDRMCPhwire > phWire ; 
   extract( iFrame.record( Stream::kDRSimulation), phWire );
   if ( ! phWire.valid() ) {
      assert( phWire.valid() );
      ::exit( -1 );
   }
   DRMCPhwireFB::instance()->setPhwire( phWire ) ;

   // =============== DBDRMCResolutions ===============
   FAConstants< DBDRMCResolutions > resolutions ; 
   extract( iFrame.record( Stream::kDRSimulation), resolutions );
   if ( ! resolutions.valid() ) {
      assert( resolutions.valid() );
      ::exit( -1 );
   }
   DRMCResolutionsFB::instance()->setResolutions( resolutions ) ;

   // =============== DBDRMCSmear ===============
   FAConstants< DBDRMCSmear > smear ; 
   extract( iFrame.record( Stream::kDRSimulation), smear );
   if ( ! smear.valid() ) {
      assert( smear.valid() );
      ::exit( -1 );
   }
   DRMCSmearFB::instance()->setSmear( smear ) ;

   // =============== DBDRMCSpefud ===============
   FAConstants< DBDRMCSpefud > speciesFudge ; 
   extract( iFrame.record( Stream::kDRSimulation), speciesFudge );
   if ( ! speciesFudge.valid() ) {
      assert( speciesFudge.valid() );
      ::exit( -1 );
   }
   DRMCSpefudFB::instance()->setSpefud( speciesFudge );

   // =============== DBDRMCTmlayr ===============
   FAConstants< DBDRMCTmlayr > tmLayr ; 
   extract( iFrame.record( Stream::kDRSimulation), tmLayr );
   if ( ! tmLayr.valid() ) {
      assert( tmLayr.valid() );
      ::exit( -1 );
   }
   DRMCTmlayrFB::instance()->setTmlayr( tmLayr ) ;

   // =============== DBDRMCWireff ===============
   FAConstants< DBDRMCWireff > wireEff ; 
   extract( iFrame.record( Stream::kDRSimulation), wireEff );
   if ( ! wireEff.valid() ) {
      assert( wireEff.valid() );
      ::exit( -1 );
   }
   DRMCWireffFB::instance()->setWireff( wireEff ) ;


   // Cathodes currently don't use these constants so comment out
   // // =============== Cathode names ===============
   // FAConstants< DBDRMCCathname > cathodeName ; 
   // extract( iFrame.record( Stream::kDRSimulation), cathodeName );
   // if ( ! cathodeName.valid() ) {
   //    assert( cathodeName.valid() );
   //    ::exit( -1 );
   // }
   // DRMCCathnameFB::instance()->setCathname( cathodeName[1] );   


   // =======================================================
   // 4) Update F77 routines that serve DR MC Anode constants
   // =======================================================
   drmc_beginrun_();
}
