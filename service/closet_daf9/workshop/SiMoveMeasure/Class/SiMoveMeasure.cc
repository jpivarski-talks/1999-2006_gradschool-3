// -*- C++ -*-
//
// Package:     SiMoveMeasure
// Module:      SiMoveMeasure
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Tue Sep 11 10:21:06 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "SiMoveMeasure/SiMoveMeasure.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "Navigation/NavTrack.h"
#include "/home/mccann/workshop/DualTrackHelices/DualTrackHelices/DualTrackHelices.h"
#include "CleoDB/DBEventHeader.h"

#include "ASiStorePro/ASiStore.h"
#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"
#include "TrackFitter/TrackFitSVRHitLink.h"
#include "TrackFitter/TrackFitSVZHitLink.h"

#include "HelixIntersection/HIHelixIntersector.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HISiSurfaceFactory.h"
#include "HelixIntersection/HICompositeSurface.h"
#include "KinematicTrajectory/KTHelix.h"
#include "KinematicTrajectory/KTKinematicData.h"

//RICH example 
//Dedx example
//Event Shape example


// STL classes
// You may have to uncomment some of these or other stl headers 
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "Processor.SiMoveMeasure" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.21 2001/09/01 22:44:37 llh14 Exp $";
static const char* const kTagString = "$Name: v06_00_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
SiMoveMeasure::SiMoveMeasure( void )               // anal1
   : Processor( "SiMoveMeasure" ),
     m_directory( "directory", this, "/cdat/dafe/mccann/SiMoveMeasure/" ),
     m_placement( "placement", this, "unmoved" ),
     m_suppressGlobal( "suppressGlobal", this, false ),
     m_suppressLayer( "suppressLayer", this, false ),
     m_suppressLadder( "suppressLadder", this, false )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &SiMoveMeasure::event,    Stream::kEvent );
   //bind( &SiMoveMeasure::beginRun, Stream::kBeginRun );
   //bind( &SiMoveMeasure::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

SiMoveMeasure::~SiMoveMeasure()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
SiMoveMeasure::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
SiMoveMeasure::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
SiMoveMeasure::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here
   int ladder, layer;
   char number[3];

   if ( ! m_suppressLadder.value() )
      for ( ladder = 1;  ladder <= 61;  ladder++ )
      {
	 sprintf( number, "%02d", ladder );
	 m_ladderRphiFile[ ladder-1 ].open(
	    ( m_directory.value() + string( "/" ) + m_placement.value() +
	      string( "_ladder" ) + string( number ) + string( "-r.ntuple" ) ).c_str(), ios::out );
	 m_ladderRphiCount[ ladder-1 ] = 0;
      }

   if ( ! m_suppressLayer.value() )
      for ( layer = 1;  layer <= 4;  layer++ )
      {
	 sprintf( number, "%1d", layer );
	 m_layerRphiFile[ layer-1 ].open(
	    ( m_directory.value() + string( "/" ) + m_placement.value() +
	   string( "_layer" ) + string( number ) + string( "-r.ntuple" ) ).c_str(), ios::out );
	 m_layerRphiCount[ layer-1 ] = 0;
      }

   if ( ! m_suppressGlobal.value() )
   {
      m_globalRphiFile.open(
	 ( m_directory.value() + string( "/" ) + m_placement.value() +
	   string( "_global-r.ntuple" ) ).c_str(), ios::out );
      m_globalRphiCount = 0;
   }
   
   if ( ! m_suppressLadder.value() )
      for ( ladder = 1;  ladder <= 61;  ladder++ )
      {
	 sprintf( number, "%02d", ladder );
	 m_ladderZFile[ ladder-1 ].open(
	    ( m_directory.value() + string( "/" ) + m_placement.value() +
	      string( "_ladder" ) + string( number ) + string( "-z.ntuple" ) ).c_str(), ios::out );
	 m_ladderZCount[ ladder-1 ] = 0;
      }

   if ( ! m_suppressLayer.value() )
      for ( layer = 1;  layer <= 4;  layer++ )
      {
	 sprintf( number, "%1d", layer );
	 m_layerZFile[ layer-1 ].open(
	    ( m_directory.value() + string( "/" ) + m_placement.value() +
	      string( "_layer" ) + string( number ) + string( "-z.ntuple" ) ).c_str(), ios::out );
	 m_layerZCount[ layer-1 ] = 0;
      }

   if ( ! m_suppressGlobal.value() )
   {
      m_globalZFile.open(
	 ( m_directory.value() + string( "/" ) + m_placement.value() +
	   string( "_global-z.ntuple" ) ).c_str(), ios::out );
      m_globalZCount = 0;
   }
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
SiMoveMeasure::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   report( DEBUG, kFacilityString ) << "starting event" << endl;

//     FAItem< DBEventHeader > header;
//     extract( iFrame.record( Stream::kEvent ), header );
//     report( DEBUG, kFacilityString ) << "using header" << endl;
//     unsigned int run = header->run();
//     unsigned int event = header->number();

//     if ( run == 113347  ||
//  	run == 113455  ||
//  	run == 113560  ||
//  	run == 113668     )
//        return ActionBase::kFailed;

   FAItem< ASiStore > store;
   extract( iFrame.record( Stream::kSVAlignment ), store );
   if ( ! store.valid() )
   {
      report( EMERGENCY, kFacilityString )
	 << "ASiStore is not valid!!!" << endl;
      return ActionBase::kFailed;
   }

   report( DEBUG, kFacilityString ) << "starting loop over tracks" << endl;

   // loop over tracks
   FATable< NavTrack > navtracks;
   extract( iFrame.record( Stream::kEvent ), navtracks );
   FATable< NavTrack >::const_iterator nav_iter;
   FATable< NavTrack >::const_iterator nav_begin = navtracks.begin();
   FATable< NavTrack >::const_iterator nav_end = navtracks.end();
   FATable< DualTrackHelices > dualtracks;
   extract( iFrame.record( Stream::kEvent ), dualtracks );
   FATable< DualTrackHelices >::const_iterator dualtracks_iter;

   for ( nav_iter = nav_begin;  nav_iter != nav_end;  nav_iter++ )
   {
      dualtracks_iter = dualtracks.find( nav_iter->identifier() );

      HIHelix trackHelix( (* nav_iter->muonHelix() ),
			  DBCandidate::mass( DBCandidate::Hypo( DBCandidate::kMuon ) ) );
//        HIHelix trackHelix( dualtracks_iter->helix() );

      const double cotTheta = trackHelix.cotTheta();
      const double phi0 = trackHelix.phi0();
      const double z0 = trackHelix.z0();

      report( DEBUG, kFacilityString ) << "track (cotTheta = " << cotTheta << ", phi0 = "
				       << phi0 << ", z0 = " << z0 << ")" << endl;

      cout << "push( @tracks, [ " << cotTheta << ", " << phi0 << ", " << z0;
      
      const STL_VECTOR( const CalibratedSVRphiHit* )* rphi_hits = nav_iter->seedSVRHits();
      const STL_VECTOR( const CalibratedSVZHit* )* z_hits = nav_iter->seedSVZHits();

      if ( rphi_hits->size() == 0  &&  z_hits->size() == 0 ) continue;

//  //   for ( SmallCount sensor = 1;  sensor <= 447;  sensor++ )
//     // This looks horrible, but you'll only ever have to go through ~16 possibilities
//     SmallCount sensor;
//     for ( rphi_iter = rphi_begin;  rphi_iter != rphi_end;  rphi_iter++ )
//        for ( z_iter = z_begin;  z_iter != z_end;  z_iter++ )
//  	 if ( ( sensor = (* rphi_iter)->sensor() ) == (* z_iter)->sensor() )
//  	 {
//  	    rphi_matched.push_back( (* rphi_iter) );
//  	    z_matched.push_back( (* z_iter) );
//  	 }
//     if ( rphi_matched.size() == 0 ) continue;

      STL_VECTOR( HIIntersectionSurface* ) surfaces;
      STL_VECTOR( HIIntersectionSurface* ) composite_surface;
      HISiSurfaceFactory siSurfaceFactory( iFrame, 0, 0, false );

      report( DEBUG, kFacilityString ) << "created surface factory" << endl;
      
      siSurfaceFactory.generateCenterSurfacesWithHits( (* rphi_hits ), (* z_hits ), 0 );
      report( DEBUG, kFacilityString ) << "generated surfaces" << endl;
      siSurfaceFactory.transferSurfacesTo( surfaces, false );
      report( DEBUG, kFacilityString ) << "transferred surfaces" << endl;

      // make one composite surface from all the layers
      STL_VECTOR( HIIntersectionSurface* )::const_iterator surfaces_iter;
      STL_VECTOR( HIIntersectionSurface* )::const_iterator surfaces_begin = surfaces.begin();
      STL_VECTOR( HIIntersectionSurface* )::const_iterator surfaces_end = surfaces.end();
      composite_surface.push_back( (* surfaces_begin ) );
      report( DEBUG, kFacilityString ) << "pushed first surface onto composite" << endl;
      STL_VECTOR( HIIntersectionSurface* )::const_iterator composite_surface_begin = composite_surface.begin();
      for ( surfaces_iter = surfaces_begin, surfaces_iter++;
	    surfaces_iter != surfaces_end;  surfaces_iter++ )
      {
	 HICompositeSurface* tmp_surface = ( (HICompositeSurface*) (* surfaces_iter) );
	 ( (HICompositeSurface*) (* composite_surface.begin()) )->addChildren( *tmp_surface );
      }
      report( DEBUG, kFacilityString ) << "made composite surface" << endl;

      HIHelixIntersector intersector( composite_surface,
				      HIHelixIntersector::kIncreasingRadius, &trackHelix );
      report( DEBUG, kFacilityString ) << "created intersector" << endl;
      
      for ( HIHelixIntersector::IntersectionStatus status =
	       intersector.swimToCurrentSurface( KTMoveControl::kDirectionForward );
	    status == HIHelixIntersector::kIntersectionOK;
	    status = intersector.swimToNextIntersection( KTMoveControl::kDirectionForward ) )
      {
	 HIIntersectionSurface* surface = intersector.currentSurface();
	 report( DEBUG, kFacilityString ) << "  sub-surface in the composite surface" << endl;
	 
	 do { 
	    report( DEBUG, kFacilityString ) << "    hit in the sub-surface" << endl;

	    double phi = trackHelix.phi0();
	    double z = trackHelix.position().z();
	    
	    const CalibratedHit* hit = surface->calibratedHit();
	    int sensor = ( (CalibratedSVHit*) (hit) )->sensor();
	    int ladder = store->ladderForSensor( sensor );
	    int truelayer = store->layerForSensor( sensor );
	    int layer = truelayer;
	    if ( truelayer == 1  ||  truelayer == 2 )
	       layer = store->clamshellForHybrid( ( (CalibratedSVHit*) (hit) )->hybrid() );

	    HepPoint3D location = ( (CalibratedSVHit*) (hit) )->worldcoord( (* store) );
	    double radius = location.perp();

	    report( DEBUG, kFacilityString ) << "    layer = " << layer << ", ladder = " << ladder
					     << ", sensor = " << sensor << ", radius = " << radius << endl;
	    
	    double residual = surface->dca( trackHelix );

	    report( DEBUG, kFacilityString ) << "    residual = " << residual << endl;
	    
	    if ( hit->deviceType() == CalibratedHit::SVR )
	    {
	       cout << ", [ " << truelayer << ", " << phi << ", " << z << ", " << ladder
		    << ", " << sensor << ", \"r\", " << residual << " ]";

	       if ( ! m_suppressLadder.value() )
	       {
		  report( DEBUG, kFacilityString )
		     << "    filling Rladder" << ladder << " " << m_ladderRphiCount[ ladder-1 ] << endl;
		  m_ladderRphiFile[ ladder-1 ] << sensor << " " << residual << " " << phi << " " << z << endl;
		  m_ladderRphiCount[ ladder-1 ]++;
	       }

	       if ( ! m_suppressLayer.value() )
	       {
		  report( DEBUG, kFacilityString )
		     << "    filling Rlayer" << layer << " " << m_layerRphiCount[ layer-1 ] << endl;
		  m_layerRphiFile[ layer-1 ] << ladder << " " << residual << " " << phi << " " << z << endl;
		  m_layerRphiCount[ layer-1 ]++;
	       }
	       
	       if ( ! m_suppressGlobal.value() )
	       {
		  report( DEBUG, kFacilityString )
		     << "    filling Rglobal" << " " << m_globalRphiCount << endl;
		  m_globalRphiFile << radius << " " << residual << " " << phi << " " << z << endl;
		  m_globalRphiCount++;
	       }
	    }
	    else if ( hit->deviceType() == CalibratedHit::SVZ )
	    {
	       cout << ", [ " << truelayer << ", " << phi << ", " << z << ", " << ladder
		    << ", " << sensor << ", \"z\", " << residual << " ]";

	       if ( ! m_suppressLadder.value() )
	       {
		  report( DEBUG, kFacilityString )
		     << "    filling Zladder" << ladder << " " << m_ladderZCount[ ladder-1 ] << endl;
		  m_ladderZFile[ ladder-1 ] << sensor << " " << residual << " " << phi << " " << z << endl;
		  m_ladderZCount[ ladder-1 ]++;
	       }

	       if ( ! m_suppressLayer.value() )
	       {
		  report( DEBUG, kFacilityString )
		     << "    filling Zlayer" << layer << " " << m_layerZCount[ layer-1 ] << endl;
		  m_layerZFile[ layer-1 ] << ladder << " " << residual << " " << phi << " " << z << endl;
		  m_layerZCount[ layer-1 ]++;
	       }

	       if ( ! m_suppressGlobal.value() )
	       {
		  report( DEBUG, kFacilityString )
		     << "    filling Zglobal" << " " << m_globalZCount << endl;
		  m_globalZFile << radius << " " << residual << " " << phi << " " << z << endl;
		  m_globalZCount++;
	       }
	    }
	    
	 } while ( surface->advanceToNextCalibratedHit( trackHelix ) );

	 report( DEBUG, kFacilityString ) << "  after while" << endl;
      } // end loop over surfaces

      cout << " ] );" << endl;
      
      report( DEBUG, kFacilityString ) << "deleting surfaces" << endl;
      for ( surfaces_iter = surfaces_begin;  surfaces_iter != surfaces_end;  surfaces_iter++ )
      {
	 (* surfaces_iter)->deleteCondition();
	 delete (* surfaces_iter );
      }
      report( DEBUG, kFacilityString ) << "done with track!" << endl;

   } // end track loop  

   report( DEBUG, kFacilityString ) << "done with event!" << endl;

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
SiMoveMeasure::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
SiMoveMeasure::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   return ActionBase::kPassed;
}
*/

//
// const member functions
//

//
// static member functions
//
