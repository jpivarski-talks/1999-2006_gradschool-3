// -*- C++ -*-
//
// Package:     KaonMonitorProc
// Module:      KaonMonitorProc
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Oct 30 17:46:24 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "KaonMonitorProc/KaonMonitorProc.h"
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

#include "MCInfo/MCDecayTree/MCDecayTree.h"
#include "MCInfo/MCDecayTree/MCParticle.h"
#include "MCInfo/MCParticleProperty/MCParticleProperty.h"
#include "MCInfo/MCParticleProperty/MCParticlePropertyStore.h"

#include "BeamEnergy/BeamEnergy.h"

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
static const char* const kFacilityString = "Processor.KaonMonitorProc" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

static const double M_KMASS  = 0.497672;
static const double M_PIMASS = 0.13957018;
static const double M_KLIFE = 0.8935e-10;
static const double M_C = 299792458.;

//
// static data member definitions
//



//
// constructors and destructor
//
KaonMonitorProc::KaonMonitorProc( void )               // anal1
   : Processor( "KaonMonitorProc" )
     , m_printoutFrequency("printoutFrequency", this, 10000)
     , m_startSkip("startSkip", this, 10000)
     , m_beamX_sum(0.), m_beamX_N(0)
     , m_beamY_sum(0.), m_beamY_N(0)
     , m_beamZ_sum(0.), m_beamZ_N(0)
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &KaonMonitorProc::event,    Stream::kEvent );
   bind( &KaonMonitorProc::beginRun, Stream::kBeginRun );
   //bind( &KaonMonitorProc::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

KaonMonitorProc::~KaonMonitorProc()                    // anal5
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
KaonMonitorProc::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

   m_kaons = m_events = m_hadronic = m_beamgas = 0;
}

// -------------------- terminate method ----------------------------
void
KaonMonitorProc::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
   printout(SYSTEM);
}


// ---------------- standard place to book histograms ---------------
void
KaonMonitorProc::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   m_direction = iHistoManager.histogram(
      100, "cos(kaon flight path, two pion momentum)", 100, 0.9, 1.);

   m_mass = iHistoManager.histogram(
      200, "kaon mass", 100, M_KMASS - 0.050, M_KMASS + 0.050);

   m_lifetime = iHistoManager.histogram(
      300, "kaon lifetime", 100, 0., 0.050e-8);

   m_radial_efficiency = iHistoManager.histogram(
      400, "efficiency VS radius (unnormalized)", 100, 0., 0.050e-8);

   m_phi = iHistoManager.histogram(
      500, "kaon phi", 100, -M_PI, M_PI);

   m_cosTheta = iHistoManager.histogram(
      600, "kaon cos(theta)", 100, -1., 1.);

   m_kaon_count = iHistoManager.number(string("kaons"), 0.);
   m_event_count = iHistoManager.number(string("events"), 0.);
   m_hadronic_count = iHistoManager.number(string("hadronicEvents"), 0.);
   m_beamgas_count = iHistoManager.number(string("beamgasEvents"), 0.);
   m_kaons_per_event = iHistoManager.numberWithErrors(string("kaonsPerEvent"), 0., 0., 0.);
   m_kaons_per_hadronic = iHistoManager.numberWithErrors(string("kaonsPerHadronic"), 0., 0., 0.);
   m_beamgas_per_hadronic = iHistoManager.numberWithErrors(string("beamgasPerHadronic"), 0., 0., 0.);
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
KaonMonitorProc::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks);
   FATable<NavTrack>::const_iterator tracks_end = tracks.end();
   FATable<NavTrack>::const_iterator track_iter;
   FATable<NavTrack>::const_iterator second_pos;  // a positive secondary
   FATable<NavTrack>::const_iterator second_neg;  // a negative secondary

   FAItem<BeamEnergy> beam_energy;
   extract(iFrame.record(Stream::kStartRun), beam_energy);

   if (tracks.size() <= 3) return ActionBase::kPassed;

   vector<KTHelix> origin_tracks;
   for ( track_iter = tracks.begin();  track_iter != tracks_end;  ++track_iter ) {
      if ( ! track_iter->seedQuality()->originUsed()             &&
           ! track_iter->pionQuality()->fitAbort()               &&
           1./fabs(track_iter->pionHelix()->curvature()) > 0.42  &&
           ! track_iter->pionHelix()->hasNullErrorMatrix()          ) {
         origin_tracks.push_back(*track_iter->pionHelix());
      }
   }
   HepPoint3D origin(event_vertex(origin_tracks));

   if (m_beamY_N < m_startSkip.value()) return ActionBase::kPassed;

// FOR THE MCSTUDY
//    int real_kaons = 0;
//    int observed_kaons = 0;
//    FAItem<MCDecayTree> decayTree;
//    extract(iFrame.record(Stream::kEvent), decayTree);
//    MCDecayTree::const_vIterator vertex_end = decayTree->vEnd();
//    HepPoint3D mc_origin = decayTree->topParticle().deathVertex()->position();
//    for (MCDecayTree::const_vIterator vertex_iter = decayTree->vBegin();
// 	vertex_iter != vertex_end;
// 	++vertex_iter) {
//       if (vertex_iter->parent().properties().name() == "K0S"  &&
// 	  (mc_origin - vertex_iter->position()).mag() > 0.003   ) {  // this restraint did little
// 	 int piplus = 0;
// 	 int piminus = 0;
// 	 for (MCVertex::const_pIterator child_iter = vertex_iter->pBegin();
// 	      child_iter != vertex_iter->pEnd();
// 	      ++child_iter) {
// 	    if (child_iter->properties().name() == "PI+") piplus++;
// 	    if (child_iter->properties().name() == "PI-") piminus++;
// 	 }
// 	 if (piplus == 1  &&  piminus == 1) real_kaons++;
//       }
//    }
// END MCSTUDY (FOR NOW)

   m_events++;
   m_event_count->update(m_events);

   if (origin.perp() < 0.005  &&  fabs(origin.z()) > 0.05) {
      m_beamgas++;
      m_beamgas_count->update(m_beamgas);
   }

   if (tracks.size() >= 3 &&
       origin.perp() < 0.005  &&  fabs(origin.z()) < 0.05) {
      double biggest_p = 0.;
      for (track_iter = tracks.begin();  track_iter != tracks_end;  ++track_iter) {
	 double p = track_iter->pionFit()->momentum().mag();
	 if (p > biggest_p) biggest_p = p;
      }
      if (biggest_p / beam_energy->value() < 0.8) {
	 m_hadronic++;
	 m_hadronic_count->update(m_hadronic);

	 for ( second_pos = tracks.begin();  second_pos != tracks_end;  ++second_pos )
	    if ( second_pos->pionHelix()->curvature() > 0. ) {
         
	       second_neg = second_pos;  ++second_neg;
	       for ( ;  second_neg != tracks_end;  ++second_neg )
		  if ( second_neg->pionHelix()->curvature() < 0. ) {

		     HepPoint3D kaon_death(-1000., -1000., -1000.);
		     double kd_zsep(-1000.), kd_sigz(-1000.), secPosPhi(-1000.), secNegPhi(-1000.);
		     KTHelix secPos(*second_pos->pionHelix());
		     KTHelix secNeg(*second_neg->pionHelix());
		     if ( calc_intersection(kaon_death,
					    kd_zsep, kd_sigz, secPosPhi, secNegPhi,
					    secPos, secNeg)    &&
			  fabs(kd_zsep) < 0.005                 &&
			  (kaon_death - origin).mag() > 0.003     ) {

			double secPosE(sqrt(second_pos->pionFit()->momentum().mag2() + sqr(M_PIMASS)));
			double secNegE(sqrt(second_neg->pionFit()->momentum().mag2() + sqr(M_PIMASS)));
			Hep3Vector secPosP(rotate_momentum(second_pos->pionFit()->momentum(), secPosPhi));
			Hep3Vector secNegP(rotate_momentum(second_neg->pionFit()->momentum(), secNegPhi));
			double kaonEfromPis(secPosE + secNegE);
			Hep3Vector kaonPfromPis(secPosP + secNegP);
			double kaonMfromPis(sqrt(sqr(kaonEfromPis) - kaonPfromPis.mag2()));
			if ( fabs(kaonMfromPis - M_KMASS) < 0.050 ) {
			   Hep3Vector kaon_flight(kaon_death - origin);
			   double direction_cosine(kaonPfromPis.dot(kaon_flight)
						   / kaonPfromPis.mag() / kaon_flight.mag());

			   m_direction->fill(direction_cosine);
			   if ( direction_cosine > 0.98 ) {
			      m_mass->fill(kaonMfromPis);
			      if ( fabs(kaonMfromPis - M_KMASS) < 0.010 ) {
				 // Now we know it's a kaon

				 // lifetime is (distance travelled) / c times mass_K / momentum_K
				 double lifetime = kaon_flight.mag() / M_C * M_KMASS / kaonPfromPis.mag();
				 if (lifetime > 0.2e-10) {
				    m_lifetime->fill(lifetime);
				    m_radial_efficiency->fill(lifetime, exp(lifetime/M_KLIFE));
				 }

				 m_phi->fill(kaonPfromPis.phi());
				 m_cosTheta->fill(cos(kaonPfromPis.theta()));

				 m_kaons++;
				 m_kaon_count->update(m_kaons);

// FOR THE MCSTUDY
// 			   observed_kaons++;
// END MCSTUDY (FOR NOW)

			      } // tight kaon mass cut
			   } // kaon momentum direction is along flight path
			} // loose kaon mass cut
		     } // secondary vertex is outside of 3 mm
		  } // second_neg is actually negative
	    } // second_pos is actually positive

      }} // end if this is a hadronic event

   if (m_kaons > 0) {
      double kaons_per_event = double(m_kaons) / double(m_events);
      double fractional_error = sqrt(1./double(m_kaons) + 1./double(m_events));
      double real_error = kaons_per_event * fractional_error;
      m_kaons_per_event->update(kaons_per_event, real_error, real_error);
   }

   if (m_kaons > 0  &&  m_hadronic > 0) {
      double kaons_per_hadronic = double(m_kaons) / double(m_hadronic);
      double fractional_error = sqrt(1./double(m_kaons) + 1./double(m_hadronic));
      double real_error = kaons_per_hadronic * fractional_error;
      m_kaons_per_hadronic->update(kaons_per_hadronic, real_error, real_error);
   }

   if (m_beamgas > 0  &&  m_hadronic > 0) {
      double beamgas_per_hadronic = double(m_beamgas) / double(m_hadronic);
      double fractional_error = sqrt(1./double(m_beamgas) + 1./double(m_hadronic));
      double real_error = beamgas_per_hadronic * fractional_error;
      m_beamgas_per_hadronic->update(beamgas_per_hadronic, real_error, real_error);
   }

   if (m_events % m_printoutFrequency.value() == 0) printout(INFO);

// FOR THE MCSTUDY
//    cout << "MCSTUDY: " << real_kaons << " " << observed_kaons << endl;
// END MCSTUDY

   return ActionBase::kPassed;
}

ActionBase::ActionResult
KaonMonitorProc::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   m_kaons = m_events = m_hadronic = m_beamgas = 0;

   return ActionBase::kPassed;
}

ActionBase::ActionResult
KaonMonitorProc::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   printout(SYSTEM);

   return ActionBase::kPassed;
}

void
KaonMonitorProc::printout(Severity severity)
{
   report(severity, kFacilityString)
      << "Kaons/Event = " << m_kaon_count->value() << " / "
      << m_event_count->value() << " = "
      << m_kaons_per_event->value() << " +/- " << m_kaons_per_event->positiveError() << endl;
   report(severity, kFacilityString)
      << "Kaons/Hadronic Event = " << m_kaon_count->value() << " / "
      << m_hadronic_count->value() << " = "
      << m_kaons_per_hadronic->value() << " +/- " << m_kaons_per_hadronic->positiveError() << endl;
   report(severity, kFacilityString)
      << "Beamgas Events/Hadronic Events = " << m_beamgas_count->value() << " / "
      << m_hadronic_count->value() << " = "
      << m_beamgas_per_hadronic->value() << " +/- " << m_beamgas_per_hadronic->positiveError() << endl;
}

HepPoint3D KaonMonitorProc::event_vertex(vector<KTHelix>& good_tracks)
{
   double beamX(0.), beamY(0.);
   if (m_beamX_N > 0) beamX = m_beamX_sum / double(m_beamX_N);
   if (m_beamY_N > 0) beamY = m_beamY_sum / double(m_beamY_N);

   double weighted_z(1000.);
   double znumer(0.);
   double zdenom(0.);

   int i, j, num_tracks(good_tracks.size());
   for ( i = 0;  i < num_tracks;  i++ )
      for ( j = i+1;  j < num_tracks;  j++ ) {
         HepPoint3D trial(-1000., -1000., -1000.);
         double tzsep(-1000.), tsigz(-1000.), taphi(-1000.), tbphi(-1000.);

         if ( calc_intersection(trial, tzsep, tsigz, taphi, tbphi,
                                good_tracks[i], good_tracks[j]) ) {

            // z-quality of intersection is uncertainty in the z intersection +
            // mismatch in z + distance from the beamspot (added in quadrature)
            double zqual( sqr(tsigz) + sqr(tzsep) +
                          sqr(trial.x() - beamX) + sqr(trial.y() - beamY) );

            znumer += trial.z() / zqual;
            zdenom += 1. / zqual;

         } // end if successful intersection
      } // end loop over tracks

   if ( zdenom != 0. ) weighted_z = znumer / zdenom;

   double weighted_x(1000.);
   double xnumer(0.);
   double xdenom(0.);

   double weighted_y(1000.);
   double ynumer(0.);
   double ydenom(0.);

   for ( i = 0;  i < num_tracks;  i++ )
      for ( j = i+1;  j < num_tracks;  j++ ) {
         HepPoint3D trial(-1000., -1000., -1000.);
         double tzsep(-1000.), tsigz(-1000.), taphi(-1000.), tbphi(-1000.);
         
         if ( calc_intersection(trial, tzsep, tsigz, taphi, tbphi,
                                good_tracks[i], good_tracks[j]) ) {
            
            // xy-quality of intersection is uncertainty in the x intersection +
            // mismatch in z + distance from already-determined weighted_z
            // (added in quadrature with weights to normalize the distribution)
            double qual( sqr(tzsep)/sqr(0.002) +
			 sqr(trial.z() - weighted_z)/sqr(0.025) );
            
            xnumer += trial.x() / qual;
            xdenom += 1. / qual;

            ynumer += trial.y() / qual;
            ydenom += 1. / qual;
            
         } // end if successful intersection
      } // end loop over tracks

   if ( xdenom != 0. ) weighted_x = xnumer / xdenom;
   if ( ydenom != 0. ) weighted_y = ynumer / ydenom;

   if (weighted_x != 1000.) {
      m_beamX_sum += weighted_x;
      m_beamX_N++;
   }
   if (weighted_y != 1000.) {
      m_beamY_sum += weighted_y;
      m_beamY_N++;
   }
   if (weighted_z != 1000.) {
      m_beamZ_sum += weighted_z;
      m_beamZ_N++;
   }

   // the beam is a ribbon: no need for event-by-event determinations of y
   return HepPoint3D(weighted_x, beamY, weighted_z);
}

DABoolean KaonMonitorProc::calc_intersection( HepPoint3D& intersection,
                                     double& zsep, double& sigz,
                                     double& aphi, double& bphi,
                                     KTHelix& a, KTHelix& b )
{
   double x, y, z;
   x = y = z = zsep = sigz = aphi = bphi = 1000.;  // To more easily find unchecked failures

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
   if ( fabs( sqr(xsol1 - xa) + sqr(ysol1a - ya) - sqr(ra) ) <
        fabs( sqr(xsol1 - xa) + sqr(ysol1b - ya) - sqr(ra) )   )
      ysol1 = ysol1a;
   else
      ysol1 = ysol1b;

   double ysol2a( yb + sqrt( sqr(rb) - sqr(xsol2 - xb) ) );
   double ysol2b( yb - sqrt( sqr(rb) - sqr(xsol2 - xb) ) );
   double ysol2;
   if ( fabs( sqr(xsol2 - xa) + sqr(ysol2a - ya) - sqr(ra) ) <
        fabs( sqr(xsol2 - xa) + sqr(ysol2b - ya) - sqr(ra) )   )
      ysol2 = ysol2a;
   else
      ysol2 = ysol2b;

   // Calculate the arclength of each solution along the two tracks.
   // Throw out a solution if the particle needs to travel (too far)
   // backward to get to it. If both solutions are okay, take the one
   // closest to the origin.
   double aarclength1, barclength1, aarclength2, barclength2;
   DABoolean arclength1_ok( arclength_test( xsol1, ysol1, a, b, aarclength1, barclength1 ) );
   DABoolean arclength2_ok( arclength_test( xsol2, ysol2, a, b, aarclength2, barclength2 ) );
   
   double aarclength, barclength;
   if ( !arclength1_ok  &&  !arclength2_ok ) return false;

   else if ( arclength1_ok  &&  !arclength2_ok ) {
      x = xsol1;
      y = ysol1;
      aarclength = aarclength1;
      barclength = barclength1;
   }

   else if ( !arclength1_ok  &&  arclength2_ok ) {
      x = xsol2;
      y = ysol2;
      aarclength = aarclength2;
      barclength = barclength2;
   }      

   else if ( sqr(xsol1) + sqr(ysol1) < sqr(xsol2) + sqr(ysol2) ) {
      x = xsol1;
      y = ysol1;
      aarclength = aarclength1;
      barclength = barclength1;
   }

   else {
      x = xsol2;
      y = ysol2;
      aarclength = aarclength2;
      barclength = barclength2;
   }      

   // Calculate where the intersection happens in Z
   double az( a.cotTheta() * aarclength + a.z0() );
   double bz( b.cotTheta() * barclength + b.z0() );

   double sig2_az( a.errorMatrix()(KTHelix::kCotTheta, KTHelix::kCotTheta) * sqr(aarclength)
                   + a.errorMatrix()(KTHelix::kZ0, KTHelix::kZ0)
                   + 2. * aarclength * a.errorMatrix()(KTHelix::kCotTheta, KTHelix::kZ0) );
   double sig2_bz( b.errorMatrix()(KTHelix::kCotTheta, KTHelix::kCotTheta) * sqr(barclength)
                   + b.errorMatrix()(KTHelix::kZ0, KTHelix::kZ0)
                   + 2. * barclength * b.errorMatrix()(KTHelix::kCotTheta, KTHelix::kZ0) );

   z = az;
   sigz = sqrt(1. / (1./sig2_az + 1./sig2_bz));
   zsep = bz - az;

   intersection = HepPoint3D(x, y, z);

   aphi = a.phi0() + fabs(aarclength) * 2. * a.curvature();
   bphi = b.phi0() + fabs(barclength) * 2. * b.curvature();

   return true;
}

DABoolean KaonMonitorProc::arclength_test( double x, double y, KTHelix& a, KTHelix& b,
					   double& aarclength, double& barclength )
{
   const double tailCut = 0.005;

   double asinphi( 2. * a.curvature() * (x * cos(a.phi0()) + y * sin(a.phi0())) );
   double acosphi( 2. * a.curvature() * (x * sin(a.phi0()) - y * cos(a.phi0())
                                          + a.d0()) + 1. );
   aarclength = atan2(asinphi, acosphi) / 2. / a.curvature();

   double bsinphi( 2. * b.curvature() * (x * cos(b.phi0()) + y * sin(b.phi0())) );
   double bcosphi( 2. * b.curvature() * (x * sin(b.phi0()) - y * cos(b.phi0())
                                          + b.d0()) + 1. );
   barclength = atan2(bsinphi, bcosphi) / 2. / b.curvature();

   return ( aarclength > -tailCut  &&  aarclength < M_PI/2./fabs(a.curvature()) )  &&
      ( barclength > -tailCut  &&  barclength < M_PI/2./fabs(b.curvature()) );
}

Hep3Vector KaonMonitorProc::rotate_momentum(Hep3Vector momentum, double angle)
{
   return Hep3Vector(cos(angle) * momentum.perp(),
                     sin(angle) * momentum.perp(),
                     momentum.z());
}

//
// const member functions
//

//
// static member functions
//
