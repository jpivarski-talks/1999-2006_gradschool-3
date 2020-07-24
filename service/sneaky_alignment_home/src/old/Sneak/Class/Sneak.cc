// -*- C++ -*-
//
// Package:     Sneak
// Module:      Sneak
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Mar 12 11:25:36 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "Sneak/Sneak.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "BeamSpot/BeamSpot.h"

#include "Navigation/NavTrack.h"
#include "TrackRoot/TRHelixFit.h"
#include "TrackRoot/TRTrackFitQuality.h"
#include "TrackRoot/TRSeedTrackQuality.h"
#include "TrackDelivery/TDKinematicFit.h"

#include "Navigation/NavShower.h"
#include "Navigation/NavTkShMatch.h"
#include "C3cc/CcShowerAttributes.h"
#include "DedxInfo/DedxInfo.h"
#include "Navigation/NavRich.h"

#include "MinuitInterface/MinuitInterface.h"
#include "Sneak/SneakFcn.h"

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
static const char* const kFacilityString = "Processor.Sneak" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

static const double M_KMASS = 0.497672;
static const double M_PIMASS = 0.13957018;

//
// static data member definitions
//



//
// constructors and destructor
//
Sneak::Sneak( void )               // anal1
   : Processor( "Sneak" )
   , m_explicitBeamSpot( "explicitBeamSpot", this, false )
   , m_beamX( "beamX", this, 0. )
   , m_beamY( "beamY", this, 0. )
   , m_beamZ( "beamZ", this, 0. )
   , m_beam_x(0.)
   , m_beam_y(0.)
   , m_beam_z(0.)
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &Sneak::event,    Stream::kEvent );
   bind( &Sneak::beginRun, Stream::kBeginRun );
   //bind( &Sneak::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

Sneak::~Sneak()                    // anal5
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
Sneak::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
Sneak::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
Sneak::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   const char* label[kNumEntries] = {
      "peop", "meop",
      "pdedx", "mdedx",
      "prich", "mrich",
      "zsep",
      "ppx", "ppy", "ppz",
      "mpx", "mpy", "mpz",
      "kmass",
      "x0", "y0", "z0",
      "x", "y", "z", "decay",
      "kdircos"
   };

   m_ntuple = iHistoManager.ntuple(
      1, "sneak", kNumEntries, 262144, label );
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
Sneak::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;
   int i;

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks);
   FATable<NavTrack>::const_iterator tracks_end = tracks.end();

   FATable<NavTkShMatch> matches;
   extract(iFrame.record(Stream::kEvent), matches);
   FATable<NavTkShMatch>::const_iterator matches_end = matches.end();
   
   FATable<DedxInfo> dedx;
   extract(iFrame.record(Stream::kEvent), dedx);
   FATable<DedxInfo>::const_iterator dedx_end = dedx.end();

   FATable<NavRich> rich;
   extract(iFrame.record(Stream::kEvent), rich);
   FATable<NavRich>::const_iterator rich_end = rich.end();

   for ( FATable<NavTrack>::const_iterator track_this = tracks.begin();
	 track_this != tracks_end;
	 ++track_this )
      for ( FATable<NavTrack>::const_iterator track_that = track_this;
	    track_that != tracks_end;
	    ++track_that )
	 if ( track_this != track_that  &&
	      track_this->pionHelix()->curvature() * track_that->pionHelix()->curvature() < 0. ) {

	    FATable<NavTkShMatch>::const_iterator
	       match_this(matches.find(track_this->identifier()));
	    FATable<NavTkShMatch>::const_iterator
	       match_that(matches.find(track_that->identifier()));
	    double eoverp_this(match_this != matches_end ? match_this->eOverP() : 1000.);
	    double eoverp_that(match_that != matches_end ? match_that->eOverP() : 1000.);

	    FATable<DedxInfo>::const_iterator
	       dedx_this(dedx.find(track_this->identifier()));
	    FATable<DedxInfo>::const_iterator
	       dedx_that(dedx.find(track_that->identifier()));
	    double piSigma_this(dedx_this != dedx_end ? dedx_this->piSigma() : 1000.);
	    double piSigma_that(dedx_that != dedx_end ? dedx_that->piSigma() : 1000.);

	    FATable<NavRich>::const_iterator
	       rich_this(rich.find(track_this->identifier()));
	    FATable<NavRich>::const_iterator
	       rich_that(rich.find(track_that->identifier()));
	    double piLogLike_this(rich_this != rich_end ? rich_this->pionLogLikelihood() : 1000.);
	    double piLogLike_that(rich_that != rich_end ? rich_that->pionLogLikelihood() : 1000.);

	    double tx(1000.), ty(1000.), tsigx(1000.), tsigy(1000.);
	    double tz(1000.), tzsep(1000.), tsigz(1000.);
	    KTHelix helix_this(*track_this->pionHelix());
	    KTHelix helix_that(*track_that->pionHelix());
	    if ( calc_intersection( tx, ty, tsigx, tsigy, tz, tzsep, tsigz,
				    helix_this, helix_that )                &&
		 abs(tzsep) < 0.005 ) {
	       int which(track_this->pionHelix()->curvature() > 0. ? 1 : 2);

	       Hep3Vector p_this(track_this->pionFit()->momentum());
	       Hep3Vector p_that(track_that->pionFit()->momentum());
	       Hep3Vector p_kaon(p_this + p_that);

	       double e_this(sqrt(p_this.mag2() + sqr(M_PIMASS)));
	       double e_that(sqrt(p_that.mag2() + sqr(M_PIMASS)));

	       double m_kaon(sqrt(sqr(e_this + e_that) - p_kaon.mag2()));

	       if ( m_kaon > 0.3  &&  m_kaon < 0.7 ) {

		  vector<KTHelix> origin_tracks;
		  for ( FATable< NavTrack >::const_iterator track_theotherthing = tracks.begin();
			track_theotherthing != tracks_end;
			++track_theotherthing )
		     if ( track_theotherthing != track_this                              &&
			  track_theotherthing != track_that                              &&
			  ! track_theotherthing->seedQuality()->originUsed()             &&
			  ! track_theotherthing->pionQuality()->fitAbort()               &&
			  1./fabs(track_theotherthing->pionHelix()->curvature()) > 0.42  &&
			  ! track_theotherthing->pionHelix()->hasNullErrorMatrix()          )
			origin_tracks.push_back(KTHelix(*track_theotherthing->pionHelix()));
		  HepPoint3D origin(event_vertex(origin_tracks));
		  double x(tx - origin.x());
		  double y(ty - origin.y());
		  double z(tz - origin.z());
		  Hep3Vector flight_kaon(x, y, z);
		  double direction_cosine(p_kaon.dot(flight_kaon)/p_kaon.mag()/flight_kaon.mag());

		  if ( direction_cosine > 0.75 ) {

		     float v[kNumEntries];  for ( i = 0;  i < kNumEntries;  i++ ) v[i] = 1000.;
		     v[kPlusEOverP] =  (which == 1 ? eoverp_this : eoverp_that);
		     v[kMinusEOverP] = (which == 2 ? eoverp_this : eoverp_that);
		     v[kPlusDedx] =  (which == 1 ? piSigma_this : piSigma_that);
		     v[kMinusDedx] = (which == 2 ? piSigma_this : piSigma_that);
		     v[kPlusRich] =  (which == 1 ? piLogLike_this : piLogLike_that);
		     v[kMinusRich] = (which == 2 ? piLogLike_this : piLogLike_that);
		     v[kZSep] = tzsep;
		     v[kPlusPx] = (which == 1 ? p_this.x() : p_that.x());
		     v[kPlusPy] = (which == 1 ? p_this.y() : p_that.y());
		     v[kPlusPz] = (which == 1 ? p_this.z() : p_that.z());
		     v[kMinusPx] = (which == 2 ? p_this.x() : p_that.x());
		     v[kMinusPy] = (which == 2 ? p_this.y() : p_that.y());
		     v[kMinusPz] = (which == 2 ? p_this.z() : p_that.z());
		     v[kKMass] = m_kaon;
		     v[kX0] = origin.x();
		     v[kY0] = origin.y();
		     v[kZ0] = origin.z();
		     v[kX] = x;
		     v[kY] = y;
		     v[kZ] = z;
		     v[kDecay] = sqrt(sqr(x) + sqr(y) + sqr(z));
		     v[kDirectionCosine] = direction_cosine;

		     report(INFO, kFacilityString)
			<< "this = " << helix_this.curvature()
			<< " " << helix_this.phi0()
			<< " " << helix_this.d0()
			<< " " << helix_this.cotTheta()
			<< " " << helix_this.z0()
			<< " that = " << helix_that.curvature()
			<< " " << helix_that.phi0()
			<< " " << helix_that.d0()
			<< " " << helix_that.cotTheta()
			<< " " << helix_that.z0() << endl;

		     Hep3Vector momentum_plus(which == 1 ?
					      track_this->pionFit()->momentum() :
					      track_that->pionFit()->momentum()   );
		     Hep3Vector momentum_minus(which == 2 ?
					       track_this->pionFit()->momentum() :
					       track_that->pionFit()->momentum()   );

		     MinuitInterface* mi = MinuitInterface::instance();
		     SneakFcn wiggle_plus(origin,
					  (which == 2 ? helix_this : helix_that),
					  (which == 1 ? helix_this : helix_that),
					  momentum_minus, momentum_plus);
		     SneakFcn wiggle_minus(origin,
					   (which == 1 ? helix_this : helix_that),
					   (which == 2 ? helix_this : helix_that),
					   momentum_plus, momentum_minus);

		  double par[2] = {0., 0.};
		  wiggle_plus.iterate(par);
		  wiggle_minus.iterate(par);

// 		  for ( par[SneakFcn::kDeltaPhi0] = -1.;
// 			par[SneakFcn::kDeltaPhi0] < 1.;
// 			par[SneakFcn::kDeltaPhi0] += 0.001 )
// 		     cout << "PHI0 " << par[SneakFcn::kDeltaPhi0] << " " << wiggle_plus.iterate(par) << endl;
// 		  par[SneakFcn::kDeltaPhi0] = 0.;

// 		  for ( par[SneakFcn::kDeltaD0] = -1.;
// 			par[SneakFcn::kDeltaD0] < 1.;
// 			par[SneakFcn::kDeltaD0] += 0.001 )
// 		     cout << "D0   " << par[SneakFcn::kDeltaD0] << " " << wiggle_plus.iterate(par) << endl;
// 		  par[SneakFcn::kDeltaD0] = 0.;

// 		  assert(false);



//  		  mi->loadFcn(wiggle_plus);
// //  		  mi->runSimplex(10000, 1e-9);
// 		  mi->runMigrad();
//  		  report(INFO, kFacilityString)
//  		     << "wiggle_plus = " << mi->parameter(SneakFcn::kDeltaPhi0).value()
//  		     << " " << mi->parameter(SneakFcn::kDeltaD0).value()
//  		     << " " << mi->parameter(SneakFcn::kDeltaCotTheta).value()
//  		     << " with a height of " << mi->minuitStats().functionMinimum() << endl;
//  		  mi->loadFcn(wiggle_minus);
// //  		  mi->runSimplex(10000, 1e-9);
// 		  mi->runMigrad();
//  		  report(INFO, kFacilityString)
//  		     << "wiggle_minus = " << mi->parameter(SneakFcn::kDeltaPhi0).value()
//  		     << " " << mi->parameter(SneakFcn::kDeltaD0).value()
//  		     << " " << mi->parameter(SneakFcn::kDeltaCotTheta).value()
//  		     << " with a height of " << mi->minuitStats().functionMinimum() << endl;
		  
		     m_ntuple->fill(v);

		  } // flight and momentum point the same way (cos(alpha) > 0.75)
	       } // within kaon mass window (0.3 to 0.7 GeV)
	    } // track intersection is (5mm) good in z
	 } // pair of tracks with opposite charges

   return ActionBase::kPassed;
}

ActionBase::ActionResult
Sneak::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   m_beam_x = m_beamX.value();
   m_beam_y = m_beamY.value();
   m_beam_z = m_beamZ.value();

   if ( ! m_explicitBeamSpot.value() ) {
      FAItem< BeamSpot > spot;
      extract( iFrame.record( Stream::kBeginRun ), spot );
      m_beam_x = spot->center().x();
      m_beam_y = spot->center().y();
      m_beam_z = spot->center().z();
   }

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
Sneak::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   return ActionBase::kPassed;
}
*/

HepPoint3D Sneak::event_vertex( vector<KTHelix> good_tracks )
{
   double weighted_z(1000.);
   double znumer(0.);
   double zdenom(0.);

   int i, j, num_tracks(good_tracks.size());
   for ( i = 0;  i < num_tracks;  i++ )
      for ( j = i+1;  j < num_tracks;  j++ ) {
	 double tx(-1000.), ty(-1000.), tsigx(-1000.), tsigy(-1000.);
	 double tz(-1000.), tzsep(-1000.), tsigz(-1000.);

	 if ( calc_intersection(tx, ty, tsigx, tsigy, tz, tzsep, tsigz,
				good_tracks[i], good_tracks[j]) ) {

	    // z-quality of intersection is uncertainty in the z intersection +
	    // mismatch in z + distance from the beamspot (added in quadrature)
	    double zqual( sqr(tsigz) + sqr(tzsep) + sqr(tx - m_beam_x) + sqr(ty - m_beam_y) );

	    znumer += tz / zqual;
	    zdenom += 1. / zqual;

	 } // end if successful intersection
      } // end loop over tracks

   if ( zdenom != 0. ) weighted_z = znumer / zdenom;

   double weighted_x(1000.);
   double xnumer(0.);
   double xdenom(0.);

   for ( i = 0;  i < num_tracks;  i++ )
      for ( j = i+1;  j < num_tracks;  j++ ) {
	 double tx(-1000.), ty(-1000.), tsigx(-1000.), tsigy(-1000.);
	 double tz(-1000.), tzsep(-1000.), tsigz(-1000.);
	 
	 if ( calc_intersection(tx, ty, tsigx, tsigy, tz, tzsep, tsigz,
				good_tracks[i], good_tracks[j]) ) {
	    
	    // x-quality of intersection is uncertainty in the x intersection +
	    // mismatch in z + distance from already-determined weighted_z
	    // (added in quadrature with weights to normalize the distribution)
	    double xqual( sqr(tzsep)/sqr(0.002) +
			  sqr(tz - weighted_z)/sqr(0.025) );
	    // The sigx criteria didn't do much anyway... I get to speed up code
// 	    double xqual( sqr(tsigx)/sqr(0.2) + sqr(tzsep)/sqr(0.002) +
// 			  sqr(tz - weighted_z)/sqr(0.025) );
	    
	    xnumer += tx / xqual;
	    xdenom += 1. / xqual;
	    
	 } // end if successful intersection
      } // end loop over tracks

   if ( xdenom != 0. ) weighted_x = xnumer / xdenom;

   // the beam is a ribbon: no need for event-by-event determinations of y
   return HepPoint3D(weighted_x, m_beam_y, weighted_z);
}

DABoolean Sneak::calc_intersection( double& x, double& y, double& sigx, double& sigy,
				     double& z, double& zsep, double& sigz, 
				     KTHelix& a, KTHelix& b )
{
   x = y = sigx = sigy = z = zsep = sigz = 1000.;  // To more easily find unchecked failures

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
   int which_num;
   char which_let;
   
   if ( !arclength1_ok  &&  !arclength2_ok ) return false;

   else if ( arclength1_ok  &&  !arclength2_ok ) {
      x = xsol1;  y = ysol1;  which_num = 1;
      if ( abs(ysol1 - ysol1a) < abs(ysol1 - ysol1b) )
	 which_let = 'a';  else  which_let = 'b';
   }

   else if ( !arclength1_ok  &&  arclength2_ok ) {
      x = xsol2;  y = ysol2;  which_num = 2;
      if ( abs(ysol2 - ysol2a) < abs(ysol2 - ysol2b) )
	 which_let = 'a';  else  which_let = 'b';
   }      

   else if ( sqr(xsol1) + sqr(ysol1) < sqr(xsol2) + sqr(ysol2) ) {
      x = xsol1;  y = ysol1;  which_num = 1;
      if ( abs(ysol1 - ysol1a) < abs(ysol1 - ysol1b) )
	 which_let = 'a';  else  which_let = 'b';
   }

   else {
      x = xsol2;  y = ysol2;  which_num = 2;
      if ( abs(ysol2 - ysol2a) < abs(ysol2 - ysol2b) )
	 which_let = 'a';  else  which_let = 'b';
   }      

   // now for the derivatives
   double dpre_dra( (ra*(-xa + xb))/(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb)) );
   double dpre_dxa( (sqr(ra)*(sqr(xa) - 2.*xa*xb + sqr(xb) - sqr(ya - yb)) + 
		     sqr(rb)*(-sqr(xa) + 2.*xa*xb - sqr(xb) + sqr(ya - yb)) + 
		     pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),2.))/
		    (2.*pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),2.)) );
   double dpre_dya( ((sqr(ra) - sqr(rb))*(xa - xb)*(ya - yb))/
		    pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),2.) );
   double dpre_drb( (rb*(xa - xb))/(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb)) );
   double dpre_dxb( (sqr(rb)*(sqr(xa) - 2.*xa*xb + sqr(xb) - sqr(ya - yb)) + 
		     sqr(ra)*(-sqr(xa) + 2.*xa*xb - sqr(xb) + sqr(ya - yb)) + 
		     pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),2.))/
		    (2.*pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),2.)) );
   double dpre_dyb( -(((sqr(ra) - sqr(rb))*(xa - xb)*(ya - yb))/
		      pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),2.)) );

   double ddisc_dra( (ra*sqr(ya - yb)*(-sqr(ra) + sqr(rb) + sqr(xa) - 2.*xa*xb + 
					 sqr(xb) + sqr(ya) - 2.*ya*yb + sqr(yb)))/
		     pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),2.) );
   double ddisc_dxa( -(((xa - xb)*sqr(ya - yb)*
			(-pow(ra,4.) + sqr(rb)*
			 (-sqr(rb) + sqr(xa) - 2.*xa*xb + sqr(xb) + 
			  sqr(ya) - 2.*ya*yb + sqr(yb)) + 
			 sqr(ra)*(2.*sqr(rb) + sqr(xa) - 2.*xa*xb + sqr(xb) + 
				    sqr(ya) - 2.*ya*yb + sqr(yb))))/
		       pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),3.)) );
   double ddisc_dya( ((-pow(4.*sqr(ra)*(xa - xb) - 
			    4.*(sqr(rb)*(xa - xb) + 
			       (xa + xb)*(sqr(xa) - 2.*xa*xb + sqr(xb) + 
					  sqr(ya - yb))),2.) + 
		       4.*(-((xa + xb)*(4.*sqr(ra)*(xa - xb) - 
				       4.*(sqr(rb)*(xa - xb) + 
					  (xa + xb)*(sqr(xa) - 2.*xa*xb + sqr(xb) + 
						     sqr(ya - yb))))) - 
			  4.*(-sqr(ra) - sqr(rb) + sqr(xa) + sqr(xb) + 
			     sqr(ya - yb))*
			  (sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb)) - 
			  2.*(pow(xa,4.) - 2.*sqr(xa)*sqr(xb) + pow(xb,4.) + 
			     2.*sqr(xb)*(sqr(ra) - sqr(rb) + sqr(ya - yb)) + 
			     2.*sqr(xa)*(-sqr(ra) + sqr(rb) + sqr(ya - yb)) + 
			     pow(-sqr(ra) + sqr(rb) + sqr(ya - yb),2.) - 
			     4.*sqr(rb)*sqr(ya - yb)))*
		       (sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb)) + 
		       16.*(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb))*
		       (pow(xa,4.) - 2.*sqr(xa)*sqr(xb) + pow(xb,4.) + 
			2.*sqr(xb)*(sqr(ra) - sqr(rb) + sqr(ya - yb)) + 
			2.*sqr(xa)*(-sqr(ra) + sqr(rb) + sqr(ya - yb)) + 
			pow(-sqr(ra) + sqr(rb) + sqr(ya - yb),2.) - 
			4.*sqr(rb)*sqr(ya - yb)))*(ya - yb))/
		     (1.6*pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),3.)) );
   double ddisc_drb( (rb*sqr(ya - yb)*(sqr(ra) - sqr(rb) + sqr(xa) - 2.*xa*xb + 
					 sqr(xb) + sqr(ya) - 2.*ya*yb + sqr(yb)))/
		     pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),2.) );
   double ddisc_dxb( ((xa - xb)*sqr(ya - yb)*(-pow(ra,4.) + 
						sqr(rb)*(-sqr(rb) + sqr(xa)
							   - 2.*xa*xb + sqr(xb) + 
							   sqr(ya) - 2.*ya*yb + sqr(yb)) + 
						sqr(ra)*(2.*sqr(rb) + sqr(xa)
							   - 2.*xa*xb + sqr(xb) + 
							   sqr(ya) - 2.*ya*yb + sqr(yb))))/
		     pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),3.) );
   double ddisc_dyb( ((pow(4.*sqr(ra)*(xa - xb) - 
			   4.*(sqr(rb)*(xa - xb) + 
			      (xa + xb)*(sqr(xa) - 2.*xa*xb + sqr(xb) + 
					 sqr(ya - yb))),2.) + 
		       4.*((xa + xb)*(4.*sqr(ra)*(xa - xb) - 
				     4.*(sqr(rb)*(xa - xb) + 
					(xa + xb)*(sqr(xa) - 2.*xa*xb + sqr(xb) + 
						   sqr(ya - yb)))) - 
			  4.*(sqr(ra) + sqr(rb) - sqr(xa) - sqr(xb) - 
			     sqr(ya - yb))*
			  (sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb)) + 
			  2.*(pow(xa,4.) - 2.*sqr(xa)*sqr(xb) + pow(xb,4.) + 
			     2.*sqr(xb)*(sqr(ra) - sqr(rb) + sqr(ya - yb)) + 
			     2.*sqr(xa)*(-sqr(ra) + sqr(rb) + sqr(ya - yb)) + 
			     pow(-sqr(ra) + sqr(rb) + sqr(ya - yb),2.) - 
			     4.*sqr(rb)*sqr(ya - yb)))*
		       (sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb)) - 
		       16.*(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb))*
		       (pow(xa,4.) - 2.*sqr(xa)*sqr(xb) + pow(xb,4.) + 
			2.*sqr(xb)*(sqr(ra) - sqr(rb) + sqr(ya - yb)) + 
			2.*sqr(xa)*(-sqr(ra) + sqr(rb) + sqr(ya - yb)) + 
			pow(-sqr(ra) + sqr(rb) + sqr(ya - yb),2.) - 
			4.*sqr(rb)*sqr(ya - yb)))*(ya - yb))/
		     (1.6*pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),3.)) );

   // Very time consuming and not very useful.  (Returns -1000. for sigx and sigy

//    double dx_dcirc[kNumCirc];
//    double dy_dcirc[kNumCirc];

//    if ( which_num == 1 ) {
//       dx_dcirc[kRa] = dpre_dra + ddisc_dra / 2. / sqrt( disc );
//       dx_dcirc[kXa] = dpre_dxa + ddisc_dxa / 2. / sqrt( disc );
//       dx_dcirc[kYa] = dpre_dya + ddisc_dya / 2. / sqrt( disc );
//       dx_dcirc[kRb] = dpre_drb + ddisc_drb / 2. / sqrt( disc );
//       dx_dcirc[kXb] = dpre_dxb + ddisc_dxb / 2. / sqrt( disc );
//       dx_dcirc[kYb] = dpre_dyb + ddisc_dyb / 2. / sqrt( disc );
//    }
//    else {
//       dx_dcirc[kRa] = dpre_dra - ddisc_dra / 2. / sqrt( disc );
//       dx_dcirc[kXa] = dpre_dxa - ddisc_dxa / 2. / sqrt( disc );
//       dx_dcirc[kYa] = dpre_dya - ddisc_dya / 2. / sqrt( disc );
//       dx_dcirc[kRb] = dpre_drb - ddisc_drb / 2. / sqrt( disc );
//       dx_dcirc[kXb] = dpre_dxb - ddisc_dxb / 2. / sqrt( disc );
//       dx_dcirc[kYb] = dpre_dyb - ddisc_dyb / 2. / sqrt( disc );
//    }

//    if ( which_let == 'a' ) {
//       dy_dcirc[kRa] = -(x - xb) * dx_dcirc[kRa] / sqrt( sqr(rb) - sqr(x - xb) );
//       dy_dcirc[kXa] = -(x - xb) * dx_dcirc[kXa] / sqrt( sqr(rb) - sqr(x - xb) );
//       dy_dcirc[kYa] = -(x - xb) * dx_dcirc[kYa] / sqrt( sqr(rb) - sqr(x - xb) );
//       dy_dcirc[kRb] = (rb - (x - xb)*dx_dcirc[kRb]) / sqrt( sqr(rb) - sqr(x - xb) );
//       dy_dcirc[kXb] = -(x - xb) * (dx_dcirc[kXb] - 1.) / sqrt( sqr(rb) - sqr(x - xb) );
//       dy_dcirc[kYb] = 1. - (x - xb)*dx_dcirc[kYb]/sqrt( sqr(rb) - sqr(x - xb) );
//    }
//    else {
//       dy_dcirc[kRa] = (x - xb) * dx_dcirc[kRa] / sqrt( sqr(rb) - sqr(x - xb) );
//       dy_dcirc[kXa] = (x - xb) * dx_dcirc[kXa] / sqrt( sqr(rb) - sqr(x - xb) );
//       dy_dcirc[kYa] = (x - xb) * dx_dcirc[kYa] / sqrt( sqr(rb) - sqr(x - xb) );
//       dy_dcirc[kRb] = (-rb + (x - xb)*dx_dcirc[kRb]) / sqrt( sqr(rb) - sqr(x - xb) );
//       dy_dcirc[kXb] = (x - xb) * (dx_dcirc[kXb] - 1.) / sqrt( sqr(rb) - sqr(x - xb) );
//       dy_dcirc[kYb] = 1. + (x - xb)*dx_dcirc[kYb]/sqrt( sqr(rb) - sqr(x - xb) );
//    }

//    // This is a Jacobian for transforming between circle variables and
//    // tracking variables
//    double dcirc_dtrack[kNumCirc][kNumTrack];
//    int i, j;
//    for ( i = 0;  i < kNumCirc;  i++ )
//       for ( j = 0;  j < kNumTrack;  j++ )
// 	 dcirc_dtrack[i][j] = 0;
   
//    dcirc_dtrack[kRa][kCurva] = -1. / 2. / sqr( a.curvature() );

//    dcirc_dtrack[kXa][kCurva] = -dcirc_dtrack[kRa][kCurva] * sin( a.phi0() );
//    dcirc_dtrack[kXa][kD0a]   = -sin( a.phi0() );
//    dcirc_dtrack[kXa][kPhi0a] = -(ra + a.d0()) * cos( a.phi0() );

//    dcirc_dtrack[kYa][kCurva] = dcirc_dtrack[kRa][kCurva] * cos( a.phi0() );
//    dcirc_dtrack[kYa][kD0a]   = cos( a.phi0() );
//    dcirc_dtrack[kYa][kPhi0a] = -(ra + a.d0()) * sin( a.phi0() );

//    dcirc_dtrack[kRb][kCurvb] = -1. / 2. / sqr( b.curvature() );

//    dcirc_dtrack[kXb][kCurvb] = -dcirc_dtrack[kRb][kCurvb] * sin( b.phi0() );
//    dcirc_dtrack[kXb][kD0b]   = -sin( b.phi0() );
//    dcirc_dtrack[kXb][kPhi0b] = -(rb + b.d0()) * cos( b.phi0() );

//    dcirc_dtrack[kYb][kCurvb] = dcirc_dtrack[kRb][kCurvb] * cos( b.phi0() );
//    dcirc_dtrack[kYb][kD0b]   = cos( b.phi0() );
//    dcirc_dtrack[kYb][kPhi0b] = -(rb + b.d0()) * sin( b.phi0() );

//    // Finally--- derivatives of x and y in terms of tracking variables!
//    double dx_dtrack[kNumTrack];
//    double dy_dtrack[kNumTrack];
//    for ( j = 0;  j < kNumTrack;  j++ ) {
//       dx_dtrack[j] = 0;
//       dy_dtrack[j] = 0;
//    }

//    for ( i = 0;  i < kNumCirc;  i++ )
//       for ( j = 0;  j < kNumTrack;  j++ ) {
// 	 dx_dtrack[j] += dx_dcirc[i] * dcirc_dtrack[i][j];
// 	 dy_dtrack[j] += dy_dcirc[i] * dcirc_dtrack[i][j];
//       }

//    // Propogating errors is a similarity transform. First we need a
//    // transformation matrix.
//    double sigma[kNumTrack][kNumTrack];
//    for ( i = 0;  i < kNumTrack;  i++ )
//       for ( j = 0;  j < kNumTrack;  j++ )
// 	 sigma[i][j] = 0;

//    sigma[kCurva][kCurva] = a.errorMatrix()(KTHelix::kCurvature, KTHelix::kCurvature);
//    sigma[kCurva][kD0a]   = a.errorMatrix()(KTHelix::kCurvature, KTHelix::kD0);
//    sigma[kCurva][kPhi0a] = a.errorMatrix()(KTHelix::kCurvature, KTHelix::kPhi0);
//    sigma[kD0a][kCurva]   = a.errorMatrix()(KTHelix::kD0, KTHelix::kCurvature);
//    sigma[kD0a][kD0a]     = a.errorMatrix()(KTHelix::kD0, KTHelix::kD0);
//    sigma[kD0a][kPhi0a]   = a.errorMatrix()(KTHelix::kD0, KTHelix::kPhi0);
//    sigma[kPhi0a][kCurva] = a.errorMatrix()(KTHelix::kPhi0, KTHelix::kCurvature);
//    sigma[kPhi0a][kD0a]   = a.errorMatrix()(KTHelix::kPhi0, KTHelix::kD0);
//    sigma[kPhi0a][kPhi0a] = a.errorMatrix()(KTHelix::kPhi0, KTHelix::kPhi0);

//    sigma[kCurvb][kCurvb] = b.errorMatrix()(KTHelix::kCurvature, KTHelix::kCurvature);
//    sigma[kCurvb][kD0b]   = b.errorMatrix()(KTHelix::kCurvature, KTHelix::kD0);
//    sigma[kCurvb][kPhi0b] = b.errorMatrix()(KTHelix::kCurvature, KTHelix::kPhi0);
//    sigma[kD0b][kCurvb]   = b.errorMatrix()(KTHelix::kD0, KTHelix::kCurvature);
//    sigma[kD0b][kD0b]     = b.errorMatrix()(KTHelix::kD0, KTHelix::kD0);
//    sigma[kD0b][kPhi0b]   = b.errorMatrix()(KTHelix::kD0, KTHelix::kPhi0);
//    sigma[kPhi0b][kCurvb] = b.errorMatrix()(KTHelix::kPhi0, KTHelix::kCurvature);
//    sigma[kPhi0b][kD0b]   = b.errorMatrix()(KTHelix::kPhi0, KTHelix::kD0);
//    sigma[kPhi0b][kPhi0b] = b.errorMatrix()(KTHelix::kPhi0, KTHelix::kPhi0);

//    double x_std[kNumTrack];
//    double y_std[kNumTrack];
//    for ( j = 0;  j < kNumTrack;  j++ ) {
//       x_std[j] = 0;
//       y_std[j] = 0;
//    }   

//    for ( i = 0;  i < kNumTrack;  i++ )
//       for ( j = 0;  j < kNumTrack;  j++ ) {
// 	 if ( dx_dtrack[i] != 0. )
// 	    x_std[j] += sigma[i][j] * dx_dtrack[i];
// 	 if ( dy_dtrack[i] != 0. )
// 	    y_std[j] += sigma[i][j] * dy_dtrack[i];
//       }

//    double sig2x(0.);
//    double sig2y(0.);

//    for ( i = 0;  i < kNumTrack;  i++ ) {
//       sig2x += dx_dtrack[i] * x_std[i];
//       sig2y += dy_dtrack[i] * y_std[i];
//    }

//    sigx = sqrt(sig2x);
//    sigy = sqrt(sig2y);

   // Calculate where the intersection happens in Z

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
   sigz = sqrt(1. / (1./sig2_az + 1./sig2_bz));
   zsep = az - bz;

   return true;
}

DABoolean Sneak::arclength_test( double x, double y, KTHelix& a, KTHelix& b )
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

//
// const member functions
//

//
// static member functions
//
