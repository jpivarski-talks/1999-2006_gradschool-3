// -*- C++ -*-
//
// Package:     MCTest
// Module:      MCTest
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri Feb 28 10:12:58 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "MCTest/MCTest.h"
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

//RICH example 
//Dedx example
//Event Shape example

#include "MCInfo/MCDecayTree/MCDecayTree.h"
#include "MCInfo/MCDecayTree/MCParticle.h"
#include "MCInfo/MCParticleProperty/MCParticleProperty.h"
#include "MCInfo/MCParticleProperty/MCParticlePropertyStore.h"

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
static const char* const kFacilityString = "Processor.MCTest" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.31 2002/06/12 20:06:57 cleo3 Exp $";
static const char* const kTagString = "$Name: v06_06_01 $";

// Some handy typedefs for accessing MC information
typedef MCDecayTree::const_vIterator MCVertexConstIt;
typedef MCDecayTree::const_pIterator MCPartConstIt;
typedef MCVertex::const_pIterator    MCChildConstIt;

//
// static data member definitions
//

//
// constructors and destructor
//
MCTest::MCTest( void )               // anal1
   : Processor( "MCTest" )
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

   bind( &MCTest::event,    Stream::kEvent );
   bind( &MCTest::beginRun, Stream::kBeginRun );
   //bind( &MCTest::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

MCTest::~MCTest()                    // anal5
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
MCTest::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
MCTest::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
MCTest::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here


   const char* label[kNumEntries] = {
      "fitx", "fity", "fitz",
      "mcx", "mcy", "mcz",
      "kx", "ky", "kz",
      "vertx", "verty", "vertz",
      "pispx", "pispy", "pispz", "pise",
      "kpx", "kpy", "kpz", "ke"
   };

   m_ntuple = iHistoManager.ntuple(
      1, "kaon", kNumEntries, 262144, label );
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
MCTest::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks);
   FATable< NavTrack >::const_iterator tracks_end = tracks.end();
   
   vector<KTHelix> good_tracks;
   for ( FATable< NavTrack >::const_iterator track_iter = tracks.begin();
	 track_iter != tracks_end;
	 ++track_iter )
      if ( ! track_iter->seedQuality()->originUsed()             &&
	   ! track_iter->pionQuality()->fitAbort()               &&
	   1./fabs(track_iter->pionHelix()->curvature()) > 0.42  &&
	   ! track_iter->pionHelix()->hasNullErrorMatrix()          )
	 good_tracks.push_back(KTHelix(*track_iter->pionHelix()));

   FAItem<MCDecayTree> tree;
   extract(iFrame.record(Stream::kEvent), tree);
   MCDecayTree::const_pIterator tree_end = tree->pEnd();

   HepPoint3D fit_origin(event_vertex(good_tracks));
   HepPoint3D mc_origin(tree->topParticle().position());

   const MCParticlePropertyStore& mcppstore =
     tree->topParticle().properties().store();
   int kshort_id = mcppstore.nameToQQId("K0S");
   int piplus_id = mcppstore.nameToQQId("PI+");
   int piminus_id = mcppstore.nameToQQId("PI-");
 
   const double pi_mass = 0.139570;
   const double k_mass = 0.497670;

   for( MCDecayTree::const_pIterator part_iter = tree->pBegin();
        part_iter != tree_end;
	++part_iter ) {

      if ( part_iter->properties().QQId() == kshort_id ) {

	 if ( part_iter->dead()  &&  part_iter->deathVertex()->nChildren() == 2 ) {

	    DABoolean seePiPlus(false), seePiMinus(false);
	    Hep3Vector piPlus_p, piMinus_p;
	    double piPlus_e, piMinus_e;
	    for( MCVertex::const_pIterator child_iter = part_iter->deathVertex()->pBegin();
		 child_iter != part_iter->deathVertex()->pEnd();
		 ++child_iter ) {
	       if ( child_iter->properties().QQId() == piplus_id ) {
		  seePiPlus = true;
		  piPlus_p = child_iter->momentum();
		  piPlus_e = sqrt(piPlus_p.mag2() + sqr(pi_mass));
	       }
	       else if ( child_iter->properties().QQId() == piminus_id ) {
		  seePiMinus = true;
		  piMinus_p = child_iter->momentum();
		  piMinus_e = sqrt(piMinus_p.mag2() + sqr(pi_mass));
	       }
	    }

	    if ( seePiPlus && seePiMinus ) {
// 	       cout << "I see a Kshort that started at " << part_iter->position()
// 		    << " and ended at " << part_iter->deathVertex()->position()
// 		    << " with pi+ pi-"
// 		    << (part_iter->deathVertex()->radiativeDecay()
// 			? " (ACK! RADIATIVE DECAY!)." : ".")
// 		    << endl << endl;

	       HepPoint3D k_origin(part_iter->position());
	       HepPoint3D vertex(part_iter->deathVertex()->position());
	       Hep3Vector pis_p(piPlus_p + piMinus_p);
	       double pis_e(piPlus_e + piMinus_e);
	       Hep3Vector k_p(part_iter->momentum());
	       double k_e(sqrt(k_p.mag2() + sqr(k_mass)));

	       float v[kNumEntries];
	       for ( int i = 0;  i < kNumEntries;  i++ ) v[i] = -10000.;
	       v[kFitX] = fit_origin.x();  v[kFitY] = fit_origin.y();  v[kFitZ] = fit_origin.z();
	       v[kMCX] = mc_origin.x();    v[kMCY] = mc_origin.y();    v[kMCZ] = mc_origin.z();
	       v[kKX] = k_origin.x();      v[kKY] = k_origin.y();      v[kKZ] = k_origin.z();
	       v[kVertexX] = vertex.x();   v[kVertexY] = vertex.y();   v[kVertexZ] = vertex.z();
	       v[kPIsPX] = pis_p.x();      v[kPIsPY] = pis_p.y();      v[kPIsPZ] = pis_p.z();
	       v[kPIsE] = pis_e;
	       v[kKPX] = k_p.x();          v[kKPY] = k_p.y();          v[kKPZ] = k_p.z();
	       v[kKE] = k_e;
	       m_ntuple->fill(v);

	    } // end the daughters are pi+ pi-
	 } // end particle decayed into two daughters
      } // end the particle is a Kshort
   } // end loop over particles

   return ActionBase::kPassed;
}

ActionBase::ActionResult
MCTest::beginRun( Frame& iFrame )       // anal2 equiv.
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
MCTest::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   return ActionBase::kPassed;
}
*/

HepPoint3D MCTest::event_vertex( vector<KTHelix> good_tracks )
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

DABoolean MCTest::calc_intersection( double& x, double& y, double& sigx, double& sigy,
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

DABoolean MCTest::arclength_test( double x, double y, KTHelix& a, KTHelix& b )
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

