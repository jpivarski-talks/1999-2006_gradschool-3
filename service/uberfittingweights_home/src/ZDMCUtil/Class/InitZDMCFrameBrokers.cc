// -*- C++ -*-
//
// Package:     <ZDMCUtil>
// Module:      InitZDMCFrameBrokers
// 
// Description: C++ Function for updating ZD MC F77 access routines
//              
// Implementation:
//     1) CD Geometry
//        Extract ADRSenseWireStore, ADRCathodeStore and
//        AZDSenseWireStore, and fill cdgeomcd common block
//        via fillFortranCDGeometry. Then initialize ZDhitsProxy -
//        a seperate C++ geometry interface for zdhits.F. It would
//        be good to remove ZDhitsProxy and just use 1 package for this.
//
//     2) ZD Calibration
//        Extract DriftFunction and initialize ZDMCDriftFB - F77 interface
//
//     3) ZD Anode MC Constants
//     Extract the different constants from the Frame and call Fortran 
//     functions which serve the constants to the ZD MC Fortran routines
//
//     4) Update F77 routines that serve ZD MC Anode constants
//        call zdmc_beginrun.F
//
// Author:      Guangshun Huang
// Created:     Tue Dec 17, 2002
//
// Revision history
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"

#include "ZDMCUtil/InitZDMCFrameBrokers.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FAConstants.h"

// Used to fill cdgeom Fortran common block 
#include "ADRGeom/ADRSenseWireStore.h"
#include "AZDGeom/AZDSenseWireStore.h"
#include "geometryForFortran/geometryForFortran.h"
#include "DetectorConfiguration/DetectorConfiguration.h"

// ZDMC FrameBroker objects - provides Fortran access to ZDMC constants 
#include "ZDMCUtil/ZDMCDriftFB.h"
#include "ZDMCUtil/ZDMCPhlayrFB.h"
#include "ZDMCUtil/ZDMCPhwireFB.h"
#include "ZDMCUtil/ZDMCResolutionsFB.h"
#include "ZDMCUtil/ZDMCWireffFB.h"

#include "ZDGeantHits/ZDHitsProxy.h"

// STL classes
#include <vector>
#include <map>

extern "C"{
   extern void zdmc_beginrun_();  // updates ZD MC resolution constants
}

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "ZDMCUtil.InitZDMCFrameBrokers" ;


void InitZDMCFrameBrokers( Frame& iFrame )
{
   report( DEBUG, kFacilityString )
      << "Initializing ZDMC FrameBrokers - F77 interface to Frame" << endl;

   FAItem< DetectorConfiguration >                   iConfig   ;
   extract( iFrame.record( Stream::kBaseGeometry ) , iConfig ) ;

   if( iConfig->useSInotZD() ) {
      return;
   }

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

   // Fill cdgeomcd common block, for new structure
// comment out, better structure later
//   fillFortranCDGeometry(*senseWireStore, *cathodeStore, &(*zdStore),
//                         fill_SI, fill_ZD);

   // Initialize ZDhitsProxy - C++ geometry interface for zdhits.F
   // Not ideal using the CD geometry from F77 2 different ways 

   ZDHitsProxy* pBroker = ZDHitsProxy::instance();
   pBroker->setWireStore(zdStore);


   // ==================================
   // 2) Get ZD Calibration for this run 
   // ==================================
   // The Stream binding will change from kBeginRun to kZDCalibration in the
   // future. The second argument will eventually be irrelevant when I convert
   // ZDDriftFunctionProd to get its own geometry pointer internally. (Ken McLean)
   FAItem<ZDDriftFunction> ZDCalibration;
   extract( iFrame.record(Stream::kBeginRun), ZDCalibration);
   if ( ! ZDCalibration.valid() ) {
     report(EMERGENCY, kFacilityString) << "No ZD Calibration Available" << endl;
     assert( ZDCalibration.valid() );
     ::exit( -1 );
   }
   ZDMCDriftFB::instance()->setDrift( ZDCalibration, zdStore ) ;

   
   // ================================
   // 3) Extract ZD Anode MC Constants
   // ================================
   // For all of the constants we do the following
   // 1) Extract constants objects from the Frame
   // 2) require valid constants
   // 3) initialize ZDMCxxxxFB (FrameBroker) objects with the constants

   // =============== DBZDMCPhlayr ===============
   FAConstants< DBZDMCPhlayr > phLayer ; 
   extract( iFrame.record( Stream::kZDSimulation), phLayer );
   if ( ! phLayer.valid() ) {
      assert( phLayer.valid() );
      ::exit( -1 );
   }
   ZDMCPhlayrFB::instance()->setPhlayr( phLayer ) ;

   // =============== DBZDMCPhwire ===============
   FAConstants< DBZDMCPhwire > phWire ; 
   extract( iFrame.record( Stream::kZDSimulation), phWire );
   if ( ! phWire.valid() ) {
      assert( phWire.valid() );
      ::exit( -1 );
   }
   ZDMCPhwireFB::instance()->setPhwire( phWire ) ;

   // =============== DBZDMCResolutions ===============
   FAConstants< DBZDMCResolutions > resolutions ; 
   extract( iFrame.record( Stream::kZDSimulation), resolutions );
   if ( ! resolutions.valid() ) {
      assert( resolutions.valid() );
      ::exit( -1 );
   }
   ZDMCResolutionsFB::instance()->setResolutions( resolutions ) ;

   // =============== DBZDMCWireff ===============
   FAConstants< DBZDMCWireff > wireEff ; 
   extract( iFrame.record( Stream::kZDSimulation), wireEff );
   if ( ! wireEff.valid() ) {
      assert( wireEff.valid() );
      ::exit( -1 );
   }
   ZDMCWireffFB::instance()->setWireff( wireEff ) ;

   // =======================================================
   // 4) Update F77 routines that serve ZD MC Anode constants
   // =======================================================
   zdmc_beginrun_();
}
