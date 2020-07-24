// -*- C++ -*-
//
// Package:     BeamWallStudyProc
// Module:      BeamWallStudyProc
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Oct 29 13:58:19 EST 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "BeamWallStudyProc/BeamWallStudyProc.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "Navigation/NavTrack.h"
#include "TrackRoot/TRHelixFit.h"
#include "TrackRoot/TRTrackFitQuality.h"
#include "TrackRoot/TRSeedTrackQuality.h"
#include "TrackDelivery/TDKinematicFit.h"
#include "CleoDB/DBEventHeader.h"
#include "EventProperties/EventProperties.h"
// #include "BeamSpot/BeamSpot.h"

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
static const char* const kFacilityString = "Processor.BeamWallStudyProc" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.31 2002/06/12 20:06:57 cleo3 Exp $";
static const char* const kTagString = "$Name: v06_06_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
BeamWallStudyProc::BeamWallStudyProc( void )               // anal1
   : Processor( "BeamWallStudyProc" )
   , m_beam_x(0.)
   , m_beam_y(0.)
   , m_beam_z(0.)
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &BeamWallStudyProc::event,    Stream::kEvent );
   //bind( &BeamWallStudyProc::beginRun, Stream::kBeginRun );
   //bind( &BeamWallStudyProc::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

BeamWallStudyProc::~BeamWallStudyProc()                    // anal5
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
BeamWallStudyProc::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
BeamWallStudyProc::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
BeamWallStudyProc::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   m_ci_close = iHistoManager.histogram(
      100, "Closest Intersection", 100, 0., 0.003 );
   m_ci_far = iHistoManager.histogram(
      200, "Closest Intersection", 100, 0., 0.12 );
   m_d0_close_pos = iHistoManager.histogram(
      300, "D0 for Positive Tracks", 100, -0.005, 0.005 );
   m_d0_far_pos = iHistoManager.histogram(
      400, "D0 for Positive Tracks", 100, -0.5, 0.5 );
   m_d0_close_neg = iHistoManager.histogram(
      350, "D0 for Negative Tracks", 100, -0.005, 0.005 );
   m_d0_far_neg = iHistoManager.histogram(
      450, "D0 for Negative Tracks", 100, -0.5, 0.5 );
   m_zdiff_close_pos = iHistoManager.histogram(
      500, "Z Vertex Resolution for Positive Tracks", 100, -0.005, 0.005 );
   m_zdiff_closer_pos = iHistoManager.histogram(
      501, "Z Vertex Resolution for Positive Tracks", 100, -0.0002, 0.0002 );
   m_zdiff_far_pos = iHistoManager.histogram(
      600, "Z Vertex Resolution for Positive Tracks", 100, -0.1, 0.1 );
   m_zdiff_close_neg = iHistoManager.histogram(
      550, "Z Vertex Resolution for Negative Tracks", 100, -0.005, 0.005 );
   m_zdiff_closer_neg = iHistoManager.histogram(
      551, "Z Vertex Resolution for Negative Tracks", 100, -0.0002, 0.0002 );
   m_zdiff_far_neg = iHistoManager.histogram(
      650, "Z Vertex Resolution for Negative Tracks", 100, -0.1, 0.1 );
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
BeamWallStudyProc::event( Frame& iFrame )          // anal3 equiv.
{
   report( NOTICE, kFacilityString ) << "here in event()" << endl;

   double ci( get_closest_intersection( iFrame ) );
   double wciz( get_weighted_ciz( iFrame ) );

   m_ci_close->fill( ci );
   m_ci_far->fill( ci );

   FAItem< DBEventHeader > header;
   extract( iFrame.record( Stream::kEvent ), header );

   FATable< NavTrack > tracks;
   extract( iFrame.record( Stream::kEvent ), tracks );
   FATable< NavTrack >::const_iterator track_iter;
   FATable< NavTrack >::const_iterator tracks_begin = tracks.begin();
   FATable< NavTrack >::const_iterator tracks_end = tracks.end();

   for ( track_iter = tracks_begin;  track_iter != tracks_end;  track_iter++ ) {
      KTHelix helix( *(track_iter->pionHelix()) );
      double distanceMoved(0.);
      helix.moveToReferencePoint( HepPoint3D(m_beam_x,m_beam_y,m_beam_z), distanceMoved );
      if ( helix.curvature() > 0. ) {
	 m_d0_close_pos->fill( helix.d0() );
	 m_d0_far_pos->fill( helix.d0() );
      }
      else {
	 m_d0_close_neg->fill( helix.d0() );
	 m_d0_far_neg->fill( helix.d0() );
      } // end if positive/negative
   } // end loop over tracks

   for ( track_iter = tracks_begin;  track_iter != tracks_end;  track_iter++ ) {
      double best_z(1000.);
      double best_z_uncert2(1000.);

      for ( FATable< NavTrack >::const_iterator other_iter = track_iter;
	    other_iter != tracks_end;
	    other_iter++ )
	 if ( other_iter != track_iter ) {
	    double tx(-1000.), ty(-1000.), tz(-1000.), tsig2z(-1000.), tzsep(-1000.);
	    KTHelix track( *track_iter->pionHelix() );
	    KTHelix other( *other_iter->pionHelix() );
	    if ( calc_intersection( tx, ty, tz, tsig2z, tzsep, track, other ) ) {
	       double z_uncert2( tsig2z + sqr(tzsep) + sqr(tx - m_beam_x) + sqr(ty - m_beam_y) );
	       if ( z_uncert2 < best_z_uncert2 ) {
		  best_z = tz;
		  best_z_uncert2 = z_uncert2;
	       } // end if we have a new winner
	    } // end if intersection was successful
	 } // end if not on diagonal, end secondary loop over tracks
      
      if ( track_iter->pionHelix()->curvature() > 0. ) {
	 m_zdiff_close_pos->fill( best_z - wciz );
	 m_zdiff_closer_pos->fill( best_z - wciz );
	 m_zdiff_far_pos->fill( best_z - wciz );
      }
      else {
	 m_zdiff_close_neg->fill( best_z - wciz );
	 m_zdiff_closer_neg->fill( best_z - wciz );
	 m_zdiff_far_neg->fill( best_z - wciz );
      } // end if positive/negative
   } // end loop over tracks

   cout << "ABUL " << header->run() << " " << header->number() << " " << tracks.size() << endl;

   cout.precision(10);
   for ( track_iter = tracks_begin;  track_iter != tracks_end;  track_iter++ ) {
      FAItem< TRHelixFit > helixp( track_iter->pionHelix() );
      FAItem< TDKinematicFit > fitp( track_iter->pionFit() );

      cout << "KABUL " << helixp->phi0()                                                        //  0
	   << " " << helixp->cotTheta()								//  1
	   << " " << helixp->errorMatrix()( KTHelix::kCurvature, KTHelix::kCurvature )		//  2
	   << " " << helixp->errorMatrix()( KTHelix::kCurvature, KTHelix::kPhi0 )		//  3
	   << " " << helixp->errorMatrix()( KTHelix::kCurvature, KTHelix::kD0 )			//  4
	   << " " << helixp->errorMatrix()( KTHelix::kCurvature, KTHelix::kCotTheta )		//  5
	   << " " << helixp->errorMatrix()( KTHelix::kCurvature, KTHelix::kZ0 )			//  6
	   << " " << helixp->errorMatrix()( KTHelix::kPhi0, KTHelix::kPhi0 )			//  7
	   << " " << helixp->errorMatrix()( KTHelix::kPhi0, KTHelix::kD0 )			//  8
	   << " " << helixp->errorMatrix()( KTHelix::kPhi0, KTHelix::kCotTheta )		//  9
	   << " " << helixp->errorMatrix()( KTHelix::kPhi0, KTHelix::kZ0 )			// 10
	   << " " << helixp->errorMatrix()( KTHelix::kD0, KTHelix::kD0 )			// 11
	   << " " << helixp->errorMatrix()( KTHelix::kD0, KTHelix::kCotTheta )			// 12
	   << " " << helixp->errorMatrix()( KTHelix::kD0, KTHelix::kZ0 )			// 13
	   << " " << helixp->errorMatrix()( KTHelix::kCotTheta, KTHelix::kCotTheta )		// 14
	   << " " << helixp->errorMatrix()( KTHelix::kCotTheta, KTHelix::kZ0 )			// 15
	   << " " << helixp->errorMatrix()( KTHelix::kZ0, KTHelix::kZ0 )			// 16
	   << " " << fitp->px()									// 17
	   << " " << fitp->py()									// 18
	   << " " << fitp->pz()									// 19
	   << " " << fitp->energy()								// 20
	   << " " << fitp->x()									// 21
	   << " " << fitp->y()									// 22
	   << " " << fitp->z()									// 23
	   << " " << fitp->errorMatrix()( KTKinematicData::kPx, KTKinematicData::kPx )		// 24
	   << " " << fitp->errorMatrix()( KTKinematicData::kPx, KTKinematicData::kPy )		// 25
	   << " " << fitp->errorMatrix()( KTKinematicData::kPx, KTKinematicData::kPz )		// 26
	   << " " << fitp->errorMatrix()( KTKinematicData::kPx, KTKinematicData::kEnergy )	// 27
	   << " " << fitp->errorMatrix()( KTKinematicData::kPx, KTKinematicData::kX )		// 28
	   << " " << fitp->errorMatrix()( KTKinematicData::kPx, KTKinematicData::kY )		// 29
	   << " " << fitp->errorMatrix()( KTKinematicData::kPx, KTKinematicData::kZ )		// 30
	   << " " << fitp->errorMatrix()( KTKinematicData::kPy, KTKinematicData::kPy )		// 31
	   << " " << fitp->errorMatrix()( KTKinematicData::kPy, KTKinematicData::kPz )		// 32
	   << " " << fitp->errorMatrix()( KTKinematicData::kPy, KTKinematicData::kEnergy )	// 33
	   << " " << fitp->errorMatrix()( KTKinematicData::kPy, KTKinematicData::kX )		// 34
	   << " " << fitp->errorMatrix()( KTKinematicData::kPy, KTKinematicData::kY )		// 35
	   << " " << fitp->errorMatrix()( KTKinematicData::kPy, KTKinematicData::kZ )		// 36
	   << " " << fitp->errorMatrix()( KTKinematicData::kPz, KTKinematicData::kPz )		// 37
	   << " " << fitp->errorMatrix()( KTKinematicData::kPz, KTKinematicData::kEnergy )	// 38
	   << " " << fitp->errorMatrix()( KTKinematicData::kPz, KTKinematicData::kX )		// 39
	   << " " << fitp->errorMatrix()( KTKinematicData::kPz, KTKinematicData::kY )		// 40
	   << " " << fitp->errorMatrix()( KTKinematicData::kPz, KTKinematicData::kZ )		// 41
	   << " " << fitp->errorMatrix()( KTKinematicData::kEnergy, KTKinematicData::kEnergy )	// 42
	   << " " << fitp->errorMatrix()( KTKinematicData::kEnergy, KTKinematicData::kX )	// 43
	   << " " << fitp->errorMatrix()( KTKinematicData::kEnergy, KTKinematicData::kY )	// 44
	   << " " << fitp->errorMatrix()( KTKinematicData::kEnergy, KTKinematicData::kZ )	// 45
	   << " " << fitp->errorMatrix()( KTKinematicData::kX, KTKinematicData::kX )		// 46
	   << " " << fitp->errorMatrix()( KTKinematicData::kX, KTKinematicData::kY )		// 47
	   << " " << fitp->errorMatrix()( KTKinematicData::kX, KTKinematicData::kZ )		// 48
	   << " " << fitp->errorMatrix()( KTKinematicData::kY, KTKinematicData::kY )		// 49
	   << " " << fitp->errorMatrix()( KTKinematicData::kY, KTKinematicData::kZ )		// 50
	   << " " << fitp->errorMatrix()( KTKinematicData::kZ, KTKinematicData::kZ )		// 51
	   << " " << helixp->curvature()							// 52
	   << " " << helixp->d0()								// 53
	   << " " << helixp->z0()								// 54
	   << " " << track_iter->pionQuality()->fitAbort()					// 55
	   << " " << track_iter->seedTrack()->phi0()						// 56
	   << " " << track_iter->seedTrack()->cotTheta()					// 57
	   << " " << track_iter->seedTrack()->curvature()					// 58
	   << " " << track_iter->seedTrack()->d0()						// 59
	   << " " << track_iter->seedTrack()->z0()						// 60
	   << " " << track_iter->identifier()                                                   // 61
	   << endl;
   } // end loop over tracks

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
BeamWallStudyProc::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
BeamWallStudyProc::endRun( Frame& iFrame )         // anal4 equiv.
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

double BeamWallStudyProc::get_weighted_z0( Frame& iFrame )
{
   FATable< NavTrack > tracks;
   extract( iFrame.record( Stream::kEvent ), tracks );
   FATable< NavTrack >::const_iterator tracks_end = tracks.end();

   double weighted_z0(1000.);
   double znumer(0.);
   double zdenom(0.);

   vector< KTHelix* > intersecting_helices;
   FATable< NavTrack >::const_iterator track_iter;
   for ( track_iter = tracks.begin();  track_iter != tracks_end;  track_iter++ ) {
      // Quality cuts for weighted z0 determination
      if ( ! track_iter->seedQuality()->originUsed()            &&  // - I want real z information!
	   ! track_iter->pionQuality()->fitAbort()              &&  // - Don't let the fit fail
	   1./abs(track_iter->pionHelix()->curvature()) > 0.42  &&  // - Diameter must be greater than 42 cm
                                                                    //   to guarantee that it pokes through
                                                                    //   the first stereo superlayer
	   ! track_iter->pionHelix()->hasNullErrorMatrix()          // - I want an error matrix for
                                                                    //   weighted means
	 ) {
	 double z0err2( track_iter->pionHelix()->errorMatrix()( KTHelix::kZ0, KTHelix::kZ0 ) );

	 znumer += track_iter->pionHelix()->z0() / z0err2;
	 zdenom += 1. / z0err2;
      } // end passes weighted z0 track cuts
   } // end loop over tracks

   if ( zdenom != 0. )
      weighted_z0 = znumer / zdenom;

   return weighted_z0;
}

double BeamWallStudyProc::get_closest_intersection( Frame& iFrame )
{
   FATable< NavTrack > tracks;
   extract( iFrame.record( Stream::kEvent ), tracks );
   FATable< NavTrack >::const_iterator tracks_end = tracks.end();

   double closest_inter(1000.);

   FATable< NavTrack >::const_iterator track_iter;
   FATable< NavTrack >::const_iterator other_iter;
   for ( track_iter = tracks.begin();  track_iter != tracks_end;  track_iter++ )
      for ( other_iter = track_iter;  other_iter != tracks_end;  other_iter++ )
	 if ( other_iter != track_iter  &&
	      (! track_iter->pionQuality()->fitAbort())  &&
	      (! other_iter->pionQuality()->fitAbort()) ) {
	    double tx(-1000.), ty(-1000.), tz(-1000.), tsig2z(-1000.), tzsep(-1000.);
	    KTHelix track( *track_iter->pionHelix() );
	    KTHelix other( *other_iter->pionHelix() );
	    if ( calc_intersection( tx, ty, tz, tsig2z, tzsep, track, other ) ) {
	       
	       double perp( sqrt(sqr(tx - m_beam_x) + sqr(ty - m_beam_y)) );
	       if ( perp < closest_inter ) closest_inter = perp;

	    } // end if successful intersection
	 } // end cuts on track_iter and other_iter

   return closest_inter;
}

double BeamWallStudyProc::get_weighted_ciz( Frame& iFrame )
{
   FATable< NavTrack > tracks;
   extract( iFrame.record( Stream::kEvent ), tracks );
   FATable< NavTrack >::const_iterator tracks_end = tracks.end();

   double weighted_ciz(1000.);
   double ciznumer(0.);
   double cizdenom(0.);

   FATable< NavTrack >::const_iterator track_iter;
   FATable< NavTrack >::const_iterator other_iter;
   for ( track_iter = tracks.begin();  track_iter != tracks_end;  track_iter++ ) {
      if ( ! track_iter->seedQuality()->originUsed()            &&
	   ! track_iter->pionQuality()->fitAbort()              &&
	   1./abs(track_iter->pionHelix()->curvature()) > 0.42  &&
	   ! track_iter->pionHelix()->hasNullErrorMatrix()        
	 ) {
	 // Secondary track loop to calculate pairwise intersections
	 for ( other_iter = track_iter;  other_iter != tracks_end;  other_iter++ ) {
	    if ( other_iter != track_iter  &&
		 ! other_iter->seedQuality()->originUsed()            &&
		 ! other_iter->pionQuality()->fitAbort()              &&
		 1./abs(other_iter->pionHelix()->curvature()) > 0.42  &&
		 ! other_iter->pionHelix()->hasNullErrorMatrix()        
	       ) { 
	       double tx(-1000.), ty(-1000.), tz(-1000.), tsig2z(-1000.), tzsep(-1000.);
	       KTHelix track( *track_iter->pionHelix() );
	       KTHelix other( *other_iter->pionHelix() );
	       if ( calc_intersection( tx, ty, tz, tsig2z, tzsep, track, other ) ) {

		  // uncertainty in the z intersection + mismatch in z + mismatch in perp
		  double z_uncert2( tsig2z + sqr(tzsep) + sqr(tx - m_beam_x) + sqr(ty - m_beam_y) );
		  ciznumer += tz / z_uncert2;
		  cizdenom += 1. / z_uncert2;

	       } // end if successful intersection
	    } // end if secondary track passes quality cuts
	 } // end loop over secondary track
      } // end if primary track passes quality cuts
   } // end loop over tracks

   if ( cizdenom != 0. ) weighted_ciz = ciznumer / cizdenom;

   return weighted_ciz;
}

DABoolean BeamWallStudyProc::calc_intersection(
   double& x, double& y, double& z, double& sig2z, double& zsep,
   KTHelix& a, KTHelix& b )
{
   x = y = z = sig2z = zsep = 1000.;  // To more easily find unchecked failures

   // Express the two helices in terms of (x - xa)^2 + (y - ya)^2 == ra^2
   double ra( 1./2./a.curvature() );
   double xa( -(a.d0() + ra) * sin(a.phi0()) );
   double ya( (a.d0() + ra) * cos(a.phi0()) );

   double rb( 1./2./b.curvature() );
   double xb( -(b.d0() + rb) * sin(b.phi0()) );
   double yb( (b.d0() + rb) * cos(b.phi0()) );

   // Zero or infinitely many solutions
   // I don't want to deal with either case
   if ( xa == xb  &&  ya == yb ) return false;

   double disc( (pow(4.*sqr(ra)*(xa - xb) - 
		     4.*(sqr(rb)*(xa - xb) + 
			(xa + xb)*(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb)))
		     ,2.) - 16.*(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb))*
		 (pow(xa,4.) - 2.*sqr(xa)*sqr(xb) + pow(xb,4.) + 
		  2.*sqr(xb)*(sqr(ra) - sqr(rb) + sqr(ya - yb)) + 
		  2.*sqr(xa)*(-sqr(ra) + sqr(rb) + sqr(ya - yb)) + 
		  pow(-sqr(ra) + sqr(rb) + sqr(ya - yb),2.) - 
		  4.*sqr(rb)*sqr(ya - yb)))/
		(64.*pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),2.)) );
   // no real solutions
   if ( disc < 0 ) return false;

   double pre( (sqr(rb)*(xa - xb) + sqr(ra)*(-xa + xb) + 
		(xa + xb)*(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb)))/
	       (2.*(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb))) );

   // The two x solutions
   double xsol1( pre + sqrt( disc ) );
   double xsol2( pre - sqrt( disc ) );

   // Two possibilities for the y solution to each x solution
   double ysol1a( yb + sqrt( sqr(rb) - sqr(xsol1 - xb) ) );
   double ysol1b( yb - sqrt( sqr(rb) - sqr(xsol1 - xb) ) );
   double ysol1;
   if ( abs( sqr(xsol1 - xa) + sqr(ysol1a - ya) - sqr(ra) ) <
	abs( sqr(xsol1 - xa) + sqr(ysol1b - ya) - sqr(ra) )   )
      ysol1 = ysol1a;
   else
      ysol1 = ysol1b;

   double ysol2a( yb + sqrt( sqr(rb) - sqr(xsol2 - xb) ) );
   double ysol2b( yb - sqrt( sqr(rb) - sqr(xsol2 - xb) ) );
   double ysol2;
   if ( abs( sqr(xsol2 - xa) + sqr(ysol2a - ya) - sqr(ra) ) <
	abs( sqr(xsol2 - xa) + sqr(ysol2b - ya) - sqr(ra) )   )
      ysol2 = ysol2a;
   else
      ysol2 = ysol2b;

   // Calculate the arclength of each solution along the two tracks.
   // Throw out a solution if the particle needs to travel (too far)
   // backward to get to it. If both solutions are okay, take the one
   // closest to the origin.
   DABoolean arclength1_ok( arclength_test( xsol1, ysol1, a, b ) );
   DABoolean arclength2_ok( arclength_test( xsol2, ysol2, a, b ) );
   
   if ( !arclength1_ok  &&  !arclength2_ok ) return false;

   else if ( arclength1_ok  &&  !arclength2_ok ) {
      x = xsol1;  y = ysol1;
   }

   else if ( !arclength1_ok  &&  arclength2_ok ) {
      x = xsol2;  y = ysol2;
   }

   else if ( sqr(xsol1) + sqr(ysol1) < sqr(xsol2) + sqr(ysol2) ) {
      x = xsol1;  y = ysol1;
   }

   else {
      x = xsol2;  y = ysol2;
   }      

   double asinphi1( 2. * a.curvature() * (x * cos(a.phi0()) + y * sin(a.phi0())) );
   double acosphi1( 2. * a.curvature() * (x * sin(a.phi0()) - y * cos(a.phi0())
					  + a.d0()) + 1. );
   double aarclength( atan2(asinphi1, acosphi1) / 2. / a.curvature() );

   double bsinphi1( 2. * b.curvature() * (x * cos(b.phi0()) + y * sin(b.phi0())) );
   double bcosphi1( 2. * b.curvature() * (x * sin(b.phi0()) - y * cos(b.phi0())
					  + b.d0()) + 1. );
   double barclength( atan2(bsinphi1, bcosphi1) / 2. / b.curvature() );

   double az( a.cotTheta() * aarclength + a.z0() );
   double bz( b.cotTheta() * barclength + b.z0() );
   double sig2_az( a.errorMatrix()(KTHelix::kCotTheta, KTHelix::kCotTheta) * sqr(aarclength)
		   + a.errorMatrix()(KTHelix::kZ0, KTHelix::kZ0)
		   + 2. * aarclength * a.errorMatrix()(KTHelix::kCotTheta, KTHelix::kZ0) );
   double sig2_bz( b.errorMatrix()(KTHelix::kCotTheta, KTHelix::kCotTheta) * sqr(barclength)
		   + b.errorMatrix()(KTHelix::kZ0, KTHelix::kZ0)
		   + 2. * barclength * b.errorMatrix()(KTHelix::kCotTheta, KTHelix::kZ0) );

   z = (az/sig2_az + bz/sig2_bz) / (1./sig2_az + 1./sig2_bz);
   sig2z = 1. / (1./sig2_az + 1./sig2_bz);
   zsep = abs(az - bz) + sqrt(sig2_az) + sqrt(sig2_bz);  // so that uncertain intersections may be ruled out

   // Because it is not useful for the `closest_intersection'
   // algorithm, I had to throw out a great deal of lovely code that
   // calculates uncertanties in x and y.  If you want this, ask me
   // about it.
   return true;
}

DABoolean BeamWallStudyProc::arclength_test( double x, double y, KTHelix& a, KTHelix& b )
{
   const double tailCut = 0.005;

   double asinphi1( 2. * a.curvature() * (x * cos(a.phi0()) + y * sin(a.phi0())) );
   double acosphi1( 2. * a.curvature() * (x * sin(a.phi0()) - y * cos(a.phi0())
					  + a.d0()) + 1. );
   double aarclength( atan2(asinphi1, acosphi1) / 2. / a.curvature() );

   double bsinphi1( 2. * b.curvature() * (x * cos(b.phi0()) + y * sin(b.phi0())) );
   double bcosphi1( 2. * b.curvature() * (x * sin(b.phi0()) - y * cos(b.phi0())
					  + b.d0()) + 1. );
   double barclength( atan2(bsinphi1, bcosphi1) / 2. / b.curvature() );

   return ( aarclength > -tailCut  &&  aarclength < M_PI/2./abs(a.curvature()) )  &&
      ( barclength > -tailCut  &&  barclength < M_PI/2./abs(b.curvature()) );
}
