// -*- C++ -*-
//
// Package:     <Doit>
// Module:      DoitTrackFinder
// 
// Description: Wrapping of Doit Track Finder
//
// Implementation:
//     dufind_findtracks can be found in the Fortran section
//
// Author:      Adam Lyon
// Created:     Thu Apr 16 16:34:24 EDT 1998
// $Id:#
//
// Revision history
//
// $Log: DoitTrackFinder.cc,v $
// Revision 1.63  2001/11/29 22:15:00  bkh
// Get tdc value out of newly modified hit object not table of raw hits
//
// Revision 1.62  2001/06/07 17:05:28  cdj
// Fixed minor memory leak
//
// Revision 1.61  2001/04/05 02:01:39  lyon
// Pass SV Hybrid efficiencies to Fortran
//
// Revision 1.60  2001/02/26 19:00:56  cdj
// changed second extract of MagneticField to StartRun
//
// Revision 1.59  2001/02/26 00:59:49  cdj
// now gets MagneticField from StartRun
//
// Revision 1.58  2001/01/25 14:53:26  cdj
// last change now compiles under OSF
//
// Revision 1.57  2001/01/24 21:32:10  cdj
// fixed memory leak
//
// Revision 1.56  2001/01/05 17:08:58  lyon
// Make empty lattices, even if user says useSilicon=false
//
// Revision 1.55  2001/01/05 05:37:16  lyon
// Added Doit status words
//
// Revision 1.54  2000/12/11 08:07:10  wsun
// Bug fix in #layers hit and added Si to #layers hit and #hits expected.
//
// Revision 1.53  2000/11/03 20:55:27  lyon
// Fix misordered call to DoitSeedTrackQuality constructor
//
// Revision 1.52  2000/11/01 01:23:25  lyon
// Add Fittable flag and new members to quality object
//
// Revision 1.51  2000/10/29 02:35:24  wsun
// SeedDRHitLink::kFITABLE --> kFITTABLE
//
// Revision 1.50  2000/10/25 15:37:15  lyon
// Handle translation of BUCKCD from doit to fitFlag in SeedDRHitLink link data
//
// Revision 1.49  2000/10/11 19:08:52  wsun
// Fixed calculation of # hits expected on track.
//
// Revision 1.48  2000/09/28 21:48:53  lyon
// Set doit mode number during begin run
//
// Revision 1.47  2000/08/23 13:46:54  lyon
// use aligned geometry instead of base geometry
//
// Revision 1.46  2000/07/24 19:45:52  pg
// Fixed protection against TOO MANY SI HITS case
//
// Revision 1.45  2000/06/27 21:15:13  pg
// Added protection against TOO MANY SI HITS case
//
// Revision 1.44  2000/06/20 22:12:50  lyon
// Remove rogue cout put in by Peter!
//
// Revision 1.43  2000/06/15 17:14:29  lyon
// Move debug messages to come out in debug mode only
//
// Revision 1.42  2000/06/09 00:35:15  lkg
// New flag "m_useSilicon" added as a member datum.  No silicon information,
// constants or hits, are extracted and passed into Fortran-land when
// this flag is false.
//
// Revision 1.41  2000/06/04 21:04:20  pg
//  Added doitSVZLattice to zeroCache(), removed spurious report() calls
//
// Revision 1.40  2000/06/04 18:48:09  lyon
// Allow multiple connections in cathode lattices
//
// Revision 1.39  2000/05/30 20:28:16  pg
// Aded code creating Si lattices to seed tracks
//
// Revision 1.38  2000/05/27 05:22:26  lyon
// Handle cathode clusters
//
// Revision 1.37  2000/05/19 20:08:49  cdj
// reset Lattice topology to allow only one Track per hit
//
// Revision 1.36  2000/05/19 19:58:54  cdj
// C++ now knows what device the doit hit comes from
//
// Revision 1.35  2000/05/19 15:55:24  lyon
// Change lattice topology contraints to allow multiple hits on track
//
// Revision 1.34  2000/05/17 14:46:57  pg
//   Ficed assertion problem while loading Si hits into Doit.
//
// Revision 1.33  2000/05/15 18:30:31  pg
//    Changes to accomodate modifications to svscratch.inc .
//
// Revision 1.32  2000/05/10 20:21:12  lyon
// Does cathodes again
//
// Revision 1.30  2000/04/05 19:27:32  lyon
// Removed cathodes so Online will work
//
// Revision 1.29  2000/03/29 21:07:24  pg
// Reflected changes in geometryForFortran.
//
// Revision 1.28  2000/03/28 15:59:25  pg
// Now filling Si common blocks
//
// Revision 1.27  2000/03/03 15:19:28  lyon
// Get the TDC right and add quality and window information
//
// Revision 1.26  2000/02/08 18:33:43  lyon
// Allow for no bunch finder running and DoitModeNumber
//
// Revision 1.25  2000/01/07 19:38:51  lyon
// Make a lattice, even if nothing goes in it
//
// Revision 1.24  1999/12/06 22:31:44  lyon
// Removed DEBUG reports
//
// Revision 1.23  1999/12/01 22:14:54  lyon
// allow for gaps in identifier numbers
//
// Revision 1.22  1999/11/29 21:48:04  lyon
// Add protection if no TDC in raw hit
//
// Revision 1.21  1999/11/18 22:55:46  lyon
// Oops - do not declare tdc in if block

//
// Revision 1.20  1999/11/18 22:32:57  lyon
// Pass TDC into Doit
//
// Revision 1.19  1999/11/04 17:32:06  lyon
// Oops - for processors do not delete memory -- added zeroCache
//
// Revision 1.18  1999/10/31 15:13:25  lyon
// Added clearCache to fix memory leaks
//
// Revision 1.17  1999/10/11 21:48:28  lyon
// Give Doit access to raw cathode hits FOR NOW
//
// Revision 1.16  1999/10/07 17:40:55  lyon
// Make empty pointer tables if no tracks
//
// Revision 1.15  1999/08/31 21:17:32  lyon
// Internalizes seeds and lattices
//
// Revision 1.14  1999/07/26 15:27:01  lyon
// Modified for Lattice interface change
//
// Revision 1.13  1999/06/21 23:02:26  pg
//  Modifications for new Si hits
//
// Revision 1.12  1999/05/28 03:52:21  lyon
// Oops - reomved cd/svgmbj
//
// Revision 1.11  1999/05/28 03:23:12  lyon
// Oops - fixup
//
// Revision 1.10  1999/05/28 03:17:25  lyon
// No longer loads geometry from zfiles
//
// Revision 1.9  1999/05/26 01:52:20  lyon
// New Lattice Interface
//
// Revision 1.8  1999/05/20 13:07:15  lyon
// Do Fortran communcation via functions instead of direct common block stuffing
//
// Revision 1.7  1999/05/17 20:10:54  lyon
// Remove annoying prints and do cddupl, cdcurl and cdkill
//
// Revision 1.6  1999/05/11 18:52:12  lyon
// Remove my geom dump
//
// Revision 1.5  1999/05/11 18:48:52  lyon
// Use new Lattice interface
//
// Revision 1.4  1999/04/21 19:55:46  lyon
// Add #includes for other hit types sent to findTracks
//
// Revision 1.3  1999/04/08 07:39:38  wsun
// Added extraction and filtering of Si and Cathode hits in TrackFitter.
//
// Revision 1.2  1998/11/23 19:32:53  lyon
// Now calls standard dufind
//
// Revision 1.1.1.1  1998/11/19 03:48:35  lyon
// Doit code moved from TrackFinder
//
//
// -- Old revision history when Doit was part of TrackFinder
// Revision 1.9  1998/11/09 16:56:41  lyon
// Updated for new Lattice specification
//
// Revision 1.8  1998/10/17 20:42:25  lyon
// Add link data
//
// Revision 1.7  1998/10/08 14:06:30  lyon
// Using Lattice::connect
//
// Revision 1.6  1998/10/06 18:18:24  lyon
// Uses new Lattice type specification
//
// Revision 1.5  1998/09/18 14:01:51  lyon
// Fixed ugly printout
//
// Revision 1.3  1998/08/10 18:54:42  lyon
// Uses new TRSeedTrack in TrackRoot
//
// Revision 1.2  1998/07/26 19:20:00  lyon
// Working version
//

#include "Experiment/Experiment.h"
#include "Experiment/types.h"

#include <stdlib.h> // for ::exit
#include <assert.h>
#include <memory> //for auto_ptr

#include "Experiment/cleo_math.h"

#include "TrackRoot/TRSeedTrack.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ADRGeom/ADRSenseWireStore.h"
#include "ActiveElements/AEWireInfo.h"
#include "ActiveElements/AEWireLayerCylindrical.h"
#include "ASiStorePro/ASiStore.h"
#include "Experiment/fortran_types.h"
#include "Experiment/report.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAPtrTable.h"
#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"
#include "Lattice/Lattice.h"

#include "CalibratedData/CalibratedDRHit.h"
#include "CalibratedData/CalibratedCathodeHit.h"
#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"
#include "Doit/DoitTrackFinder.h"
#include "geometryForFortran/geometryForFortran.h"
#include "TrackFinder/SeedDRHitLink.h"
#include "CleoDB/DBEventHeader.h"
#include "MagField/MagneticField.h"
#include "CathodeFortranAccess/CathodeFortranAccess.h"
#include "RawData/CollatedRawDRHit.h"

#include "Doit/device_constants.inc"
#include "DAException/DANoDataException.h"

// Constants
#include "DBSVHybridEfficiency.hh"


#include <string>
#include <vector>
#include <map>
// Fortran header files!


// Declare fortran function with extern "C" to avoid name mangling
extern "C" 
{
   void tfgmi0_();                    // TF initialization
   void doitnl_();                    // DOIT Namelist setup
   void tfhini_();                    // TF Histogram initialization
   void tfinit_();                    // More TF initialization
   void tfgmi2_();                    // Even more
   void tfxfin_();                    // Yet even more

   void sfinit_(const char*, const char*, int, int);  // Initialize SF
   void sfcuts_(const char*, const char*, int, int);  // Set up SF parameters
   void sfind_();                                     // Run SF
   
   void dufind_();

   void doitclearhits_();

   void doitgetnumtracks_(FInteger*);

   void doitloaddrhits_(FInteger*, FInteger*, FInteger*, FReal*, 
			FReal*, FReal*, FReal*,
			FReal*, FReal*, FInteger*, FInteger*, 
			FReal*, FReal*, FInteger*, 
			FInteger*);

   void doitloadsvhits_(FInteger*, FInteger*, FInteger*, FInteger*, FInteger*, 
			FInteger*, FReal*, FReal*, FReal*,
			FReal*, FReal*, FInteger*);

   void doitgettrackinfo_(FInteger*, FReal*, FReal*, FReal*, FReal*, 
			  FReal*, FInteger*, FReal*, FReal*, FReal*, 
			  FInteger*, FReal*, FInteger*, FInteger*,
			  FInteger*, FInteger*, FInteger*, FInteger*);

   void doitgethitontrackinfo_(FInteger*, FInteger*, FInteger*,
			       FInteger*, FInteger*, FInteger*, FReal*, 
			       FReal*, FInteger*, FInteger*);

   void doitfillparams_(FInteger*, FInteger*, FReal*, FInteger*);

   void doitbeginrun_(FInteger*, FInteger*);

   void doitnewsveff_(FInteger*, FReal*, FReal*);

   void doitpostdufind_();

   // Diagnostics
   //void diag_dufind_(int& key, int& intin, int& logicalout);

   void doit_begin_run_fortran_();                    // Whoohoo
}


// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Doit.DoitTrackFinder" ;

//
// static data member definitions
//

//
// constructors and destructor
//

// Default
DoitTrackFinder::DoitTrackFinder() :
   TrackFinder(),
   m_modeNumber(1),    // Mode number for regular Doit
   m_useSilicon( true ),
   m_doitSeedTracks(0),
   m_doitLattice(0),  // Lattice for DR anode wire hits
   m_doitSVRLattice(0),  // Lattice for SVR hits
   m_doitSVZLattice(0),  // Lattice for SVR hits
   m_doitCathodeLattice(0), // Lattice for DR cathode hits
   m_doitSeedQualities(0),
   m_hitIdentifiers(new STL_VECTOR(int))
{}


// DoitTrackFinder::DoitTrackFinder( const DoitTrackFinder& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DoitTrackFinder::~DoitTrackFinder()
{
  // Clear the cache
  clearCache();

  delete m_hitIdentifiers;
}

//
// assignment operators
//
// const DoitTrackFinder& DoitTrackFinder::operator=( 
//                                             const DoitTrackFinder& rhs )
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


//////////////////////////////////////////////////////////////////////
// initialize -- Initialization code for DOIT
//
void DoitTrackFinder::initialize() const
{
   // Initialize CD Geometry (eventually replace with real CLEO3 C++ geometry)
   //    This assumes that zfiles are already loaded - 
   //    use special suez executable for this!

   // report(NOTICE, kFacilityString) 
   //    << "In DoitTrackFinder::initialize" << endl;

   // report(NOTICE, kFacilityString) << "In DoitTrackFinder::passed cdgmbj" 
   // 				   << endl;

}


//////////////////////////////////////////////////////////////////////
// beginRun -- Code to run at the begin of a new run for DOIT
void DoitTrackFinder::beginRun(Frame& theFrame) const
{
   static DABoolean first = true;
   
   // Setup CD geometry for this run (replace with real CLEO3 C++ geometry)
   //    assume run 75001, event 0
   // report(NOTICE, kFacilityString) << "Before doit_begin_run" << endl;

   // Setup geometry
   // Get geometry information for the DR
   FAItem< ADRSenseWireStore > senseWireStore;
   extract( theFrame.record(Stream::kDRAlignment), senseWireStore);

   if ( ! senseWireStore.valid() )
   {
      report(EMERGENCY, kFacilityString) << "Invalid ADRSenseWireStore" 
					   << endl;
      assert(senseWireStore.valid());
      ::exit(1);
   }

   // Extract cathode geometry
   FAItem< ADRCathodeStore > cathodeStore;
   extract( theFrame.record(Stream::kDRAlignment), cathodeStore);

   if ( ! cathodeStore.valid() )
   {
      report(EMERGENCY, kFacilityString) << "Invalid ADRCathodeStore" 
					   << endl;
      assert(cathodeStore.valid());
      ::exit(1);
   }

   // Extract Si geometry
   FAItem< ASiStore > siStore;
   if ( m_useSilicon )
   {
      extract( theFrame.record(Stream::kSVAlignment), siStore);

      if ( ! siStore.valid() )
      {
	 report(EMERGENCY, kFacilityString) << "Invalid ASiStore" 
					 << endl;
	 assert(siStore.valid());
	 ::exit(1);
      }
   }

   // Set the run and mode number
   FInteger run = theFrame.record(Stream::kBeginRun).syncValue().runNumber();
   FInteger imode = modeNumber();
   doitbeginrun_(&run, &imode);
   

   // Fill cdgeomcd common block
   fillFortranCDGeometry(*senseWireStore, *cathodeStore);
   // Fill svgeometry common block
   if ( m_useSilicon )
   {
      fillFortranSVGeometry(*siStore);
   }

   // Setup TF
   tfgmi0_();
   
   if ( first ) 
   {
      first = false;
      doitnl_();        // initialize name list

      tfhini_();        // Histograms for TF

   }

   // More TF setup
   tfinit_();
      
   tfgmi2_();

   tfxfin_();

   // Initialize SF - the Short/Seed/Stub Track finder
   string caller("DoitTrackFinder.initialize");
   string initSet("SHORT");
   sfinit_( caller.c_str(), initSet.c_str(), caller.length(), 
	    initSet.length() );

}

void DoitTrackFinder::fillSVEfficiencyConstants(const Record& theRecord) const
{

  // Get the SV Efficiency constants
  FAConstants<DBSVHybridEfficiency> effs;

  DABoolean gotConstants = true;

  // Protect against absent constants for now
  try
  {
    extract( theRecord, effs );
  }
  catch ( NO_CONSTANTS_EXCEPTION(DBSVHybridEfficiency)& iException )
  {
    gotConstants = false;
    report(WARNING, kFacilityString) 
      << "Could not extract SVHybridEfficiency constants\n"
      << "Using perfect efficiencies for Doit -- be sure this is what you want"
      << endl;
  }
    
  // How many of them are there
  unsigned int nHybrids;
  
  if ( gotConstants )
  {
    nHybrids = effs.size();
  }
  else
  {
    nHybrids = 122;
  }

  // Now we make a temporary array to pass these to Fortran
  // (Would be much nicer if Doit were in C++ so it could use these numbers
  //  directly)

  FReal* rPhiEff = new FReal[nHybrids];
  FReal* rZEff   = new FReal[nHybrids];

  // Copy the numbers -- note I assume that the constants are ordered
  //                     by Hybrid!!! -- Start at 1!
  for ( unsigned int i = 0; i < nHybrids; ++i )
  {
    // Get this constant
    if ( gotConstants )
    {
      const DBSVHybridEfficiency& theConstant = effs[i+1];
      rPhiEff[i] = theConstant.get_PhiEfficiency();
      rZEff[i]   = theConstant.get_ZEfficiency();
    }
    else
    {
      rPhiEff[i] = 1.0;
      rZEff[i] = 1.0;
    }
  }

  // Pass these arrays to Fortran
  FInteger nHybridsF = nHybrids;
  doitnewsveff_(&nHybridsF, rPhiEff, rZEff);
  delete [] rPhiEff;
  delete [] rZEff;
}

//////////////////////////////////////////////////////////////////////
// Find the tracks for this event for the given hits with DOIT
DABoolean
DoitTrackFinder::findTracks(
   STL_VECTOR(const CalibratedDRHit*)* theDRHits, 
   STL_VECTOR(const CalibratedSVRphiHit*)* theSVRphiHits, 
   STL_VECTOR(const CalibratedSVZHit*)* theSVZHits, 
   STL_VECTOR(const CalibratedCathodeHit*)* theCathodeHits, 
   const Record& theRecord,
   int diagLevel) const
{

   // Fill the Common block with the hits
   if ( diagLevel > 0 )
     report(DEBUG, kFacilityString) 
	<< "FILLING FORTRAN COMMONS!!!!!!!!!!!" << endl;
   fillFortranCommons( theDRHits, theRecord.frame() );

   if ( m_useSilicon ) {
      if ( diagLevel > 0 )
	 report(DEBUG, kFacilityString) 
	    << "FILLING FORTRAN COMMONS with SVRphiHits !!!!!!!!!!!" << endl;
      fillFortranCommons( theSVRphiHits, theRecord.frame() );
      if ( diagLevel > 0 )
	 report(DEBUG, kFacilityString) 
	    << "FILLING FORTRAN COMMONS with SVZHits !!!!!!!!!!!" << endl;
      fillFortranCommons( theSVZHits, theRecord.frame() );
   }

   if ( diagLevel > 0 )
      report(DEBUG, kFacilityString) 
	<< "FILLING FORTRAN COMMONS DONE!!!!!!!!" << endl;

   // Eventually, seedsIn will be a pointer to list of seed tracks
   DABoolean seedsIn = false;

   // Number of seeds
   int numberOfSeeds = 0; // Set below

   // Do wee need to stuff the seeds?
   if ( seedsIn )
   {
      // Fill common blocks with seed tracks
      // and set numberOfSeeds variable
      // TBA
   }


   // Call DUFIND
   dufind_();

   // Note that CDDUPL, CDCURL and CDKILL are no longer called

   return true;
      
}


//////////////////////////////////////////////////////////////////////
// Fill Common blocks for DOIT and SF with DR Hits information
void DoitTrackFinder::fillFortranCommons( 
   STL_VECTOR(const CalibratedDRHit*)* theDRHits,
   const Frame& theFrame ) const
{

   // zero out the common blocks
   doitclearhits_();

   // Get this run and event number
   FInteger run = theFrame.record(Stream::kEvent).syncValue().runNumber();
   FInteger event = theFrame.record(Stream::kEvent).syncValue().eventNumber();

   // Magnetic field
   FReal bfield;
   FAItem< MagneticField > magneticField;
   extract( theFrame.record( Stream::kStartRun ) , magneticField );
   if ( magneticField.valid() )
   {
     bfield = (magneticField->BField()).z();
   }
   else
   {
         report( EMERGENCY, kFacilityString )
	   << "Failed to extract Magnetic Field";

	 assert(magneticField.valid());
	 ::exit(1);
   }

   // Get the mode number
   FInteger imode = modeNumber();

   // Set these parameters in Doit
   doitfillparams_(&run, &event, &bfield, &imode);

   // Fill the ***RAW*** Cathode hits
   CathodeFortranAccess::fillFortranWithRawPads(theFrame);

   // Now fill the cathode clusters (do clustering if not done already)
   CathodeFortranAccess::fillFortranWithClusters(theFrame);

   // Get geometry information for the DR
   FAItem< ADRSenseWireStore > senseWireStore;
   extract(theFrame.record(Stream::kDRAlignment), senseWireStore);

   // Are there any DR hits?
   if ( theDRHits != 0 )
   {

     for ( unsigned int l = 0; l < theDRHits->size(); ++l )
     {

       const CalibratedDRHit* aDRHit = (*theDRHits)[l];

       int layer      = aDRHit->layer();
       int wire       = aDRHit->wireInLyr();
       int   tdc      = aDRHit->rawChargeTime().second ;
       float time     = aDRHit->time();
       float charge   = aDRHit->charge();
       float distance = aDRHit->distance();
       int   qual     = aDRHit->qualHit();
       int   ntdc     = aDRHit->nTDC();
       float earlyGap = aDRHit->earlyGap();
       float lateGap  = aDRHit->lateGap();


       // Calculate the phi of this hit
       //   Get layer information
       const AEWireLayerCylindrical& theLayer = senseWireStore->layer(layer);
       float phi = theLayer.phi0() + wire*theLayer.cellPhiWidth();

       if ( phi < 0.0 )
	 phi += 2.0*M_PI; // M_PI = PI, defined in math.h
       
       if ( phi >= 2.0*M_PI )
	 phi -= 2.0*M_PI;
       
       float xpos = theLayer.radius() * cos(phi);
       float ypos = theLayer.radius() * sin(phi);

       
       // Determine the "logical address" of this hit (this the cellid)
       int cdla = senseWireStore->cellID(layer, wire);

       int idx;
       // Fill this hit in the common blocks
       doitloaddrhits_(&layer, &wire, &tdc, &time, &charge, &phi, &distance,
		       &xpos, &ypos, &cdla, &qual, &earlyGap,
		       &lateGap, &ntdc, &idx);

       // Keep track of this hit's ID
       int hitId = aDRHit->identifier();
       
       // Check that we know what we are doing - idx should be l+1
       if ( idx != l+1 )
       {
	 report(EMERGENCY,kFacilityString) << "Problem with idx" << endl;
	 assert(idx == l+1);
       }

       // Do we have to add to the vector?
       if ( m_hitIdentifiers->size() <= l )
       {
	 m_hitIdentifiers->push_back(hitId);
       }
       else
       {
	 (*m_hitIdentifiers)[l] = hitId;
       }

     } // for over DR hits

   }  // if there are DR hits

}

//////////////////////////////////////////////////////////////////////
// Fill Common blocks for DOIT and SF with SV Hits information
void DoitTrackFinder::fillFortranCommons( 
   STL_VECTOR(const CalibratedSVRphiHit*)* theSVRphiHits,
   const Frame& theFrame ) const
{

   // zero out the common blocks
//   doitclearhits_();

   // Get this run and event number
   FInteger run = theFrame.record(Stream::kEvent).syncValue().runNumber();
   FInteger event = theFrame.record(Stream::kEvent).syncValue().eventNumber();


   // Get geometry information for the Silicon
   FAItem< ASiStore > siStore;
   if ( m_useSilicon )
   {
      extract(theFrame.record(Stream::kSVAlignment), siStore);
   }

   // Fill the hits
   int l=0;
   int idx = 1;
   // Are there any SV RPhi hits?
   while ( (l < theSVRphiHits->size()) && (idx > 0) )
   {
       const CalibratedSVRphiHit* aHit = (*theSVRphiHits)[l];

       int hittype = 0;
       int layer = aHit->layer();
       int hybrid  = aHit->hybrid();
       int parent = aHit->parent();
       int quality = aHit->quality();

       float pulse    = aHit->charge();
       HepPoint3D point = aHit->worldcoord( *siStore);
       float x = point.x();
       float y = point.y();
// Have to change this to be a center of the halfladder (or may be a ladder)
       float z = 0.;
       float phi = point.phi();

       // Keep track of this hit's ID
       int hitId = aHit->identifier();
       // Fill this hit in the common blocks
       doitloadsvhits_(&hittype, &hitId, &parent, &layer, &hybrid, &quality,
		       &x, &y, &z, &phi, &pulse, &idx);       
       l++; // advance the index
       
   }  // if there are SVRphi hits

   if(idx == 0) report(WARNING, kFacilityString) << " I have " 
						 << theSVRphiHits->size() 
						 << " CalibratedSVRphi hits, "
						 << " only " << l-1  
						 << " Si hits are allowed"
						 << endl;
   
}

//////////////////////////////////////////////////////////////////////
// Fill Common blocks for DOIT and SF with SV Hits information
void DoitTrackFinder::fillFortranCommons( 
   STL_VECTOR(const CalibratedSVZHit*)* theSVZHits,
   const Frame& theFrame ) const
{

   // zero out the common blocks
//   doitclearhits_();

   // Get this run and event number
//   FInteger run = theFrame.record(Stream::kEvent).syncValue().runNumber();
//   FInteger event = theFrame.record(Stream::kEvent).syncValue().eventNumber();


   // Get geometry information for the Si
   FAItem< ASiStore > siStore;
   if ( m_useSilicon )
   {
      extract(theFrame.record(Stream::kSVAlignment), siStore);
   }

   // Fill the hits
   int l=0;
   int idx = 1;
//idx = 0 indicates buffer overflow
   while (l < theSVZHits->size() && idx > 0) 
   {
      
      const CalibratedSVZHit* aHit = (*theSVZHits)[l];

      int hittype = 1;
      int layer = aHit->layer();
      int hybrid  = aHit->hybrid();
      int parent = aHit->parent();
      int quality = aHit->quality();
      
      float pulse    = aHit->charge();
      HepPoint3D point = aHit->worldcoord( *siStore);
      float x = point.x();
      float y = point.y();
      float z = point.z();
      float phi = point.phi();
      
      // Keep track of this hit's ID
      int hitId = aHit->identifier();
      // Fill this hit in the common blocks
      doitloadsvhits_(&hittype, &hitId, &parent, &layer, &hybrid, &quality,
		      &x, &y, &z,&phi, &pulse, &idx);
      
      l++;
   }  // end of the loop
   if(idx == 0) report(WARNING, kFacilityString) << " I have " 
						 << theSVZHits->size() 
						 << " CalibratedSVZHits, space exists only for "
						 << l-1 << endl;

}

//////////////////////////////////////////////////////////////////////
// void insertTracksIntoFrame
//
//   - No longer needed (producers/processors do the insert)
//
void DoitTrackFinder::insertTracksIntoFrame(Frame& theframe, 
					    int diaglevel) const
{}

//////////////////////////////////////////////////////////////////////
// void getTracksFromFortran
//
//    - Extract Tracks from Fortran
//      (Used to put tracks in the frame, but this is now left up
//       to the producers and processors)
// 
void DoitTrackFinder::getTracksFromFortran(Frame& theFrame, int diagLevel,
					   const char* const drHitUsageTag) 
{
   if ( diagLevel > 0 )
      report(DEBUG, kFacilityString) << "In INSERT \n" << endl;

   // Get the number of tracks
   int nTracks;
   doitgetnumtracks_(&nTracks);

   // Let's check if there are old tracks
   if ( m_doitSeedTracks )
   {
     report(WARNING, kFacilityString) << "Doit tracks, lattice and qualities"
				      << " are about to be overwritten. This"
				      << " will probably cause a MEMORY LEAK!"
				      << " You should call "
				      << "DoitTrackFinder::clearCache between"
				      << " doit calls.  -- Continuing at your "
				      << " own peril!" << endl;
   }


   
   // Make the pointer table for the Seed Tracks
   // If there are no tracks, then the pointer table will be empty
   auto_ptr<FAPtrTable<TRSeedTrack> > pSeedTracks( 
      new FAPtrTable<TRSeedTrack> );
   pSeedTracks->reserve( nTracks );

   // Make the pointer table for the seed qualities
   // If there are no tracks, then the pointer table will be empty
   auto_ptr<FAPtrTable<DoitSeedQuality> > pSeedQualities( 
      new FAPtrTable<DoitSeedQuality> );
   pSeedQualities->reserve( nTracks );

   // Let's extract the DR hits
   const Record& theRecord = theFrame.record(Stream::kEvent);
   FATable<CalibratedDRHit> drHits;

   if ( drHitUsageTag )
   {
     extract( theRecord, drHits, drHitUsageTag );
   }
   else
   {
     extract( theRecord, drHits );
   }
// Extract Si hits
   FATable<CalibratedSVRphiHit> svrHits;
   FATable<CalibratedSVZHit> svzHits;
   if ( m_useSilicon ) {
     extract(theRecord, svrHits);
     extract(theRecord, svzHits);
   }

   // Let's extract the Cathode clusters
   FATable<CalibratedCathodeHit> cathodeClusters;
   extract(theRecord, cathodeClusters);

   // Extract the ADRSenseWireStore -- needed to calculate expected number
   // of hit DR layers.
   FAItem< ADRSenseWireStore > wireStore ;
   extract( theFrame.record( Stream::kDRAlignment ), wireStore ) ;

   // Make a lattice for the seedTrack, CalibratedDRHit connection
   auto_ptr<SeedTrackDRHitLattice> pLattice( 
      new SeedTrackDRHitLattice(pSeedTracks.get(), 
				drHits, 
				SeedTrackDRHitLattice::LNodeMulti) );

   // Make a lattice for the seedTrack, CalibratedSVRphiHit connection
   auto_ptr<SeedTrackSVRHitLattice> pSVRLattice( 
      new SeedTrackSVRHitLattice(pSeedTracks.get(),
				 svrHits, 
				 SeedTrackSVRHitLattice::LNodeMulti) );
							 
   // Make a lattice for the seedTrack, CalibratedSVZHit connection
   auto_ptr<SeedTrackSVZHitLattice> pSVZLattice( 
      new SeedTrackSVZHitLattice(pSeedTracks.get(),
				 svzHits, 
				 SeedTrackSVZHitLattice::LNodeMulti) );

   // Make a lattice for the seedTrack, CalibratedCathodeHit connection
   auto_ptr<SeedTrackCathodeHitLattice> pCathodeLattice( 
      new SeedTrackCathodeHitLattice(pSeedTracks.get(),
				     cathodeClusters, 
				     SeedTrackCathodeHitLattice::LNodeMulti,
				     SeedTrackCathodeHitLattice::LLinkMulti,
				     SeedTrackCathodeHitLattice::RLinkMulti,
				     SeedTrackCathodeHitLattice::RNodeMulti) );
   

   // Check if there are no tracks -- don't bother continuing if none
   if ( nTracks == 0 ) 
   {
      fillCache( pSeedTracks.release(),
		 pSeedQualities.release(),
		 pLattice.release(),
		 pSVRLattice.release(),
		 pSVZLattice.release(),
		 pCathodeLattice.release() );
      return;
   }

   // Loop over tracks
   for ( int i = 1; i <= nTracks; ++ i )
   {


      // Keep track of how many hits are on each layer
      int nHitsOnDRLayer[48];
      for( int dummy = 0 ; dummy < 48 ; ++dummy )
      {
	 nHitsOnDRLayer[dummy] = 0 ;
      }

      int nHitsOnSVRLayer[5];
      for( int dummy2 = 0 ; dummy2 < 5 ; ++dummy2 )
      {
	 nHitsOnSVRLayer[dummy2] = 0 ;
      }

      int nHitsOnSVZLayer[5];
      for( int dummy3 = 0 ; dummy3 < 5 ; ++dummy3 )
      {
	 nHitsOnSVZLayer[dummy3] = 0 ;
      }

      int  nHitsOnCathLayer = 0;
     
      int trknum = i;
      int nhits, ndof, kincd, ok;
      float curcd, phi0cd, d0cd, tancd, z0cd, chi2, chiz, chi3, resicd;
      FInteger stat1, stat2, stat3, stat4;

      // Get the track information
      doitgettrackinfo_(&trknum, &curcd, &phi0cd, &d0cd, &tancd,
			&z0cd, &nhits, &chi2, &chiz, &chi3, &ndof, 
			&resicd, &kincd, &stat1, &stat2, &stat3,
			&stat4, &ok);

      if (ok != 0 ) 
      {
	 report(EMERGENCY, kFacilityString) << "Retrieval of Track Failed! " 
					    << "Track = " << i
					    << endl;
	 assert(ok == 0);
	 ::exit(1);
      }

      if ( diagLevel > 0 )
	 report(DEBUG, kFacilityString) << "Track " << i << " " 
					 << curcd << " " 
					 << phi0cd << " "
					 << d0cd << " " 
					 << tancd << " " 
					 << z0cd << endl;
      
      

      // Make a seed track
      TRSeedTrack* seed = new TRSeedTrack(i, curcd, phi0cd, d0cd, tancd, z0cd);
      pSeedTracks->insert( seed );

      // Make an iterator for this seed track
      FAPtrTable<TRSeedTrack>::const_iterator seedIter = 
	pSeedTracks->find(i);

      // Find the largest and smallest layer that was hit
      // to make the number of expected hits
      int smallestLayer = 60;
      int largestLayer = 0;

      
      // Let's figure out what hits are on this track
      for ( int j = 1; j <= nhits; ++ j)
      {

	int trknum = i;
	int hitnum = j;
	int layer, wire, iadr;
	int device, buckcd;
	float pull, meas;
	
	// Get hit information
	doitgethitontrackinfo_(&trknum, &hitnum, &iadr, 
			       &device, &layer, &wire,
			       &pull, &meas, &buckcd, &ok);

	// Do stuff depending on device
	if( device == kDoitDeviceDR ) 
	{

	  if ( ok == 1 )
	  {
	    report(EMERGENCY, kFacilityString) << "Problem getting hit."
					       << " Track = " << i 
					       << " Hit = " << j << endl;
	    assert(ok != 1);
	    ::exit(1);
	   }

	 
	  if ( diagLevel > 0 )
	    report ( INFO, kFacilityString ) << "Hit on track " << i << " " 
					     << "Layer = " << layer << " " 
					     << "wire = " << wire << " "
					     << "pull = " << pull << " " 
					     << " measurement = " << meas 
					     << " buckcd = " << buckcd
					     <<endl;
	  
	  // The iadr (internal doit hit address) happens to be the
	  // index of the hit in our vector (almost)
	  //  -- Let's check that this is so
	  
	  if ( m_hitIdentifiers->size() <= (iadr-1) )
	  {
	    report(EMERGENCY, kFacilityString) << "m_hitIdentifier is " 
					       << "too small" << endl;
	    assert(false);
	  }

	  int hitId = (*m_hitIdentifiers)[iadr-1];
	  
	  
	  if ( drHits[hitId].layer() != layer ||
	       drHits[hitId].wireInLyr() != wire )
	  {
	    report(EMERGENCY, kFacilityString) 
	      << "Internal hit inconsistency! \n" 
	      << "  Have hitId, layer, wire " << hitId 
	      << " " << layer << " " << wire
	      << "\n  Got instead layer, wire "
	      << drHits[hitId].layer()
	      << " " << drHits[hitId].wireInLyr()  << endl;
	    
	    assert(drHits[hitId].layer() == layer &&
		   drHits[hitId].wireInLyr() == wire );
	    ::exit(1);
	  }

	  
	  // Make an iterator pointing to this hit
	  FATable<CalibratedDRHit>::const_iterator drIter = 
	    drHits.find(hitId);
	  
	  if ( diagLevel > 0 )
	    report ( INFO, kFacilityString ) 
	      << "Connecting hit (" << layer << ", " << wire << ")"
	      << "[hit id = " << hitId << ", drIter points to " 
	      << drIter->identifier() << " ] to seed track " << i << endl;
	  
	  // Connect this hit to the seed track with the lattice

	  // Setup the fit flag
	  SeedDRHitLink::fitFlagEnum fitFlag;
	  switch ( buckcd )
	  {
	    case 0:
	      fitFlag = SeedDRHitLink::kFITTABLE;
	      break;

	    default:
	      fitFlag = SeedDRHitLink::kDO_NOT_FIT;
	  };
	  
	  // Make the link data (use default fitting weight for now)
	  SeedDRHitLink latlink(meas, pull, fitFlag);
	  
	  // Connect - a copy of latlink is made
	  if ( pLattice->connect(seedIter, drIter, latlink) == 0 )
	  {
	    report(EMERGENCY, kFacilityString) 
	      << "DR wire Lattice connect failed" << endl;
	  }
	  
	   // Update the layer watchers
	  if ( layer < smallestLayer ) smallestLayer = layer;
	  if ( layer > largestLayer  ) largestLayer  = layer;

	  // Keep track of how many hits are on this layer
	  nHitsOnDRLayer[layer]++;

	} // if ( DR wire hit )

	else if ( device == kDoitDeviceCathode )
	{
	  
	  // Get the cathode in the list
	  int cathId = wire;  // Yep - we stick it in the wire
	  
	  // Is this cluster really there?
	  FATableItr<CalibratedCathodeHit> theClusterItr = 
	    cathodeClusters.find( cathId );
	  
	  // Turns out FATableItr inherits from FAItem -- so check validity
	  if ( ! theClusterItr.valid() )
	  {
	    report(EMERGENCY, kFacilityString) << "Trying to add invalid "
					       << "cluster " << cathId
					       << endl;
	     
	    // Skip the hit
	    continue;
	  }
	  
	  // Cluster is good! Add it
	  if ( diagLevel > 0 )
	  {
	    report ( INFO, kFacilityString ) << "Cathode Hit on track " 
					     << i << " cluster# " 
					     << cathId << endl;

	    report(INFO, kFacilityString) << "Putting cathode on " 
					  << seedIter->identifier() << endl;
	  }
	  
	  if ( pCathodeLattice->connect(seedIter->identifier(), 
					     theClusterItr->identifier()) 
	       == 0 )
	  {
	    report(EMERGENCY, kFacilityString) 
	      << "DR cathode Lattice connect failed" << endl;
	  }

	  nHitsOnCathLayer++;

	} // else if ( DR Cathode hit )

	else if ( device == kDoitDeviceSVRphi )
	{
	  // Get the SVRphiHit id in the list
	  int svrId = wire;  // Yep - we stick it in the wire too
	  
	  // Is this cluster really there?
	  FATableItr<CalibratedSVRphiHit> theSVRItr = svrHits.find( svrId );
	  
	  // Turns out FATableItr inherits from FAItem -- so check validity
	  if ( ! theSVRItr.valid() )
	  {
	    report(EMERGENCY, kFacilityString) << "Trying to add invalid "
					       << "SVRphi hit " << svrId
					       << endl;
	     
	    // Skip the hit
	    continue;
	  }
	  
	  // Cluster is good! Add it
	  if ( diagLevel > 0 )
	    report ( INFO, kFacilityString ) << " SVR Hit on track " 
					     << i << " hit # " 
					     << svrId << endl;
	  
	  if ( pSVRLattice->connect(seedIter->identifier(), 
					 theSVRItr->identifier()) == 0 )
	  {
	    report(EMERGENCY, kFacilityString) 
	      << "SVR Lattice connect failed" << endl;
	  }

	  // Keep track of how many hits are on this layer
	  ++( nHitsOnSVRLayer[ theSVRItr->layer() ] ) ;

	} // else if ( SV r-phi hit )

	else if ( device == kDoitDeviceSVZ )
	{
	  // Get the CalibratedSVZHit id in the list
	  int svzId = wire;  // Yep - we stick it in the wire too
	  
	  // Is this cluster really there?
	  FATableItr<CalibratedSVZHit> theSVZItr = svzHits.find( svzId );
	  
	  // Turns out FATableItr inherits from FAItem -- so check validity
	  if ( ! theSVZItr.valid() )
	  {
	    report(EMERGENCY, kFacilityString) << "Trying to add invalid "
					       << "SVZ hit " << svzId
					       << endl;
	     
	    // Skip the hit
	    continue;
	  }
	  
	  // Hit is valid! Add it
	  if ( diagLevel > 0 )
	    report ( INFO, kFacilityString ) << " SVZ Hit on track " 
					     << i << " hit # " 
					     << svzId << endl;

	  
	  if ( pSVZLattice->connect(seedIter->identifier(), 
					 theSVZItr->identifier()) == 0 )
	  {
	    report(EMERGENCY, kFacilityString) 
	      << "SVZ Lattice connect failed" << endl;
	  }

	  // Keep track of how many hits are on this layer
	  ++( nHitsOnSVZLayer[ theSVZItr->layer() ] ) ;

	} // else if ( SV r-z hit )

	else
	{
	  report(EMERGENCY, kFacilityString) << "Hit from unknown device! "
					     << "# " << device << endl;
	  continue;
	} // if ( device == ... )

      }  // for ( hits in track )
      
      // Now fill the seed quality object
      
      FAItem< MagneticField > magneticField;
      extract( theFrame.record( Stream::kStartRun ) , magneticField );

      int hitsexpected = 0 ;
      KTHelix testHelix( seedIter->curvature(),
			 seedIter->phi0(),
			 seedIter->d0(),
			 seedIter->cotTheta(),
			 seedIter->z0(),
			 seedIter->referencePoint() ) ;

      // Swim the test helix to each layer and check if it's still in the
      // chamber.  First do the Si.
      for( int siLayer = SiGeomDescription::kNFirstLayer ;
	   siLayer <

	      SiGeomDescription::kNFirstLayer + SiGeomDescription::kNLayers ;
	   ++siLayer )
      {
	 // Transport the helix to the layer radius.
	 double arc = 0. ;
	 if( testHelix.moveToRadius(
	    SiGeomDescription::PointLayer[ siLayer -
				      SiGeomDescription::kNFirstLayer ].perp(),
	    arc,
	    KTMoveControl::kDirectionForward ) !=
	     KTMoveControl::kMoveOK )
	 {
	    break ;
	 }

	 // Check z position of helix.
	 Meters zpos = testHelix.position().z() ;
	 Meters zlim = 0.5 * SiGeomDescription::kZStep *
	    SiGeomDescription::kNZStep[ siLayer -
				      SiGeomDescription::kNFirstLayer ] ;
	 if( fabs( zpos ) > zlim )
	 {
	    break ;
	 }

	 // Increment expected counter for rphi and z hit.
	 hitsexpected += 2 ;
      }
 


      // Now do the DR.
      DABoolean addCathodeToExpected = true ;
      for( int iLayer = ADRSenseWireStore::kFirstLayer ;
	   iLayer < ADRSenseWireStore::kFirstLayer +
	      ADRSenseWireStore::kNumberOfLayers ;
	   ++iLayer )
      {
	 // Transport the helix to the layer radius.
	 double arc = 0. ;
	 if( testHelix.moveToRadius( wireStore->radiusNominal( iLayer ),
				     arc,
				     KTMoveControl::kDirectionForward ) !=
	     KTMoveControl::kMoveOK )
	 {
	    addCathodeToExpected = false ;
	    break ;
	 }

	 // Check z position of helix.
	 Meters zpos = testHelix.position().z() ;
	 if( zpos > wireStore->zMax( iLayer ) ||
	     zpos < wireStore->zMin( iLayer ) )
	 {
	    addCathodeToExpected = false ;
	    break ;
	 }

	 ++hitsexpected ;
      }

      if( addCathodeToExpected )
      {
	 ++hitsexpected ;
      }


      // Make the Doit status object
      DoitStatusInfo doitStatus( (UInt32)stat1, (UInt32)stat2, (UInt32)stat3,
				 (UInt32)stat4 );

      DABoolean goodZ = (kincd != -1 && kincd != -3 );
      DABoolean inwardDoitCurler = (kincd == -2 || kincd == -3);
      DABoolean zEndpointUsed = false;  // No more z-escapes??
      DABoolean originUsed = doitStatus.d0Constraint2();

      // Count DR layers
      int nLayersWithHit = 0;
      int nLayersWithMoreThanOneHit = 0;
      for ( unsigned int layerctr = 1; layerctr <= 47; ++layerctr)
      {
	if ( nHitsOnDRLayer[layerctr] > 0 ) nLayersWithHit++;
	if ( nHitsOnDRLayer[layerctr] > 1 ) nLayersWithMoreThanOneHit++;
      }

      if ( nHitsOnCathLayer > 0 ) nLayersWithHit++;
      if ( nHitsOnCathLayer > 1 ) nLayersWithMoreThanOneHit++;

      for ( unsigned int layerctrsvr = 1; layerctrsvr <= 4; ++layerctrsvr)
      {
	if ( nHitsOnSVRLayer[layerctrsvr] > 0 ) nLayersWithHit++;
	if ( nHitsOnSVRLayer[layerctrsvr] > 1 ) nLayersWithMoreThanOneHit++;
      }

      for ( unsigned int layerctrsvz = 1; layerctrsvz <= 4; ++layerctrsvz)
      {
	if ( nHitsOnSVZLayer[layerctrsvz] > 0 ) nLayersWithHit++;
	if ( nHitsOnSVZLayer[layerctrsvz] > 1 ) nLayersWithMoreThanOneHit++;
      }

      // Assume this is true for now (??)
      DABoolean useForExtrapolation = true;
	
      // Make the seed quality
      DoitSeedQuality* qual = new DoitSeedQuality(i, chi2, ndof, nhits, 
						  hitsexpected, 
						  nLayersWithHit,
						  nLayersWithMoreThanOneHit,
						  zEndpointUsed, originUsed,
						  useForExtrapolation, goodZ,
						  inwardDoitCurler, chiz, 
						  chi3, resicd, doitStatus);
					
      // Insert into the list
      pSeedQualities->insert(qual);

   } // for ( tracks )

   if ( diagLevel > 0 )
   {

      // SeedTable (LeftTable) is a vector of pointers to seeds (left nodes)
      typedef SeedTrackDRHitLattice::VectorLeftID SeedTable;

      //SeedTableIterator (LeftItr) is an iterator for the above vector
      //(to get the value of the seed, dereference twice [**SeedTableIterator])
      typedef SeedTrackDRHitLattice::LeftItr   SeedTableIterator;

      // Hit map (LtoRmap) connects the seeds to the DR hits
      typedef SeedTrackDRHitLattice::VectorRightID   HitTable;

      // HitMapIterator (LtoRItr) is an iterator for the above
      typedef SeedTrackDRHitLattice::RightItr   HitTableIterator;

      // Get a list of Seed track ID's
      SeedTable seedIDs = pLattice->vLeftID();

      // Loop over the seeds in the lattice
      for ( SeedTableIterator seedIter = seedIDs.begin();
	    seedIter != seedIDs.end();
	    ++seedIter )
      {

	 // Get a list of hits that belong to this track
	 // (Left = seed, far means go accross lattice)
	 const HitTable* hitTable = pLattice->vRightGivenLeft(seedIter);
	 
	 report( INFO, kFacilityString ) 
	    << "Seed Track # " << (*seedIter)
	    << " has these hits:" << endl;

	 // Hit Table (vector) has the hits that belong to this seed
	 // Loop over them
	 for ( HitTableIterator hitIter = hitTable->begin();
	       hitIter != hitTable->end();
	       ++hitIter )
	 {
	    report( INFO, kFacilityString ) 
	       << "( " << (*hitIter) << ";  " 
	       <<  drHits[(*hitIter)].layer() << ", "
	       <<  drHits[(*hitIter)].wireInLyr() 
	       << " )" << endl;
	 }
      }
   }  //  if (diagLevel > 0)

   //save all the data
   fillCache( pSeedTracks.release(),
	      pSeedQualities.release(),
	      pLattice.release(),
	      pSVRLattice.release(),
	      pSVZLattice.release(),
	      pCathodeLattice.release() );
   
}


//////////////////////////////////////////////////////////////////////
// getSeedTracks (must be non-const so can be deleted by proxy)
//
//   Get a FAPtrTable of seed tracks from the last Doit call
FAPtrTable<TRSeedTrack>* DoitTrackFinder::getSeedTracks() const
{ 
  return m_doitSeedTracks;
}


//////////////////////////////////////////////////////////////////////
// getLattice 
//
//  Get the lattice connecting seed tracks and hits from the last Doit call
SeedTrackDRHitLattice* DoitTrackFinder::getLattice() const
{ 
  return m_doitLattice;
}

//////////////////////////////////////////////////////////////////////
// getSVRLattice 
//
//  Get the lattice connecting seed tracks and SVR hits from the
//  last Doit call
SeedTrackSVRHitLattice* 
DoitTrackFinder::getSVRLattice() const
{ 
  return m_doitSVRLattice;
}

//////////////////////////////////////////////////////////////////////
// getSVZLattice 
//
//  Get the lattice connecting seed tracks and SVZ hits from the
//  last Doit call
SeedTrackSVZHitLattice* 
DoitTrackFinder::getSVZLattice() const
{ 
  return m_doitSVZLattice;
}

//////////////////////////////////////////////////////////////////////
// getCathodeLattice 
//
//  Get the lattice connecting seed tracks and cathode hits from the
//  last Doit call
SeedTrackCathodeHitLattice* DoitTrackFinder::getCathodeLattice() const
{ 
  return m_doitCathodeLattice;
}


//////////////////////////////////////////////////////////////////////
// getSeedQualities
//
// Get a FAPtrTable of seed track qualities from the last Doit call
FAPtrTable<DoitSeedQuality>* DoitTrackFinder::getSeedQualities() const
{
  return m_doitSeedQualities;
}

//////////////////////////////////////////////////////////////////////
// clearCache
// 
void DoitTrackFinder::clearCache() 
{

  // Clear the cache variables if not already
  if ( m_doitLattice != 0 )
  {
    delete m_doitLattice;
    m_doitLattice = 0;
  }

  if ( m_doitSVRLattice != 0 )
  {
    delete m_doitSVRLattice;
    m_doitSVRLattice = 0;
  }

  if ( m_doitSVZLattice != 0 )
  {
    delete m_doitSVZLattice;
    m_doitSVZLattice = 0;
  }

  if ( m_doitCathodeLattice != 0 )
  {
    delete m_doitCathodeLattice;
    m_doitCathodeLattice = 0;
  }

  if ( m_doitSeedTracks != 0 )
  {
    // This will call eraseAll
    delete m_doitSeedTracks;
    m_doitSeedTracks = 0;
  }

  if ( m_doitSeedQualities != 0 )
  {
    // This will call eraseAll
    delete m_doitSeedQualities;
    m_doitSeedQualities = 0;
  }

}  

//////////////////////////////////////////////////////////////////////
// zeroCache
// 
void DoitTrackFinder::zeroCache() 
{
  // zero the cache variables
  m_doitLattice = 0;
  m_doitSVRLattice = 0;
  m_doitSVZLattice = 0;
  m_doitCathodeLattice = 0;
  m_doitSeedTracks = 0;
  m_doitSeedQualities = 0;
}  

//////////////////////////////////////////////////////////////////////
// Set the mode number
//
void DoitTrackFinder::setModeNumber(int modeNumber)
{
  m_modeNumber = modeNumber;
}

//////////////////////////////////////////////////////////////////////
// Set whether we use silicon
//
void DoitTrackFinder::setSiliconUsage(DABoolean useSilicon)
{
  m_useSilicon = useSilicon;
}
