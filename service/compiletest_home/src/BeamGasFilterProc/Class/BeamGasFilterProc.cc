// -*- C++ -*-
//
// Package:     BeamGasFilterProc
// Module:      BeamGasFilterProc
// 
// Description: Cuts out cosmics and beamwall events, and some beamgas (keeps 99.4% of signal, 43% of beamgas/beamwall/cosmics, AFTER hadronic subcollection cuts)
//
// Implementation:
//     Three cuts: number of tracks > 2 (redundant, I hope!)
//		   closest intersection in rphi,
//                 weighted z of intersections (special weighting factor)
//
// Author:      Jim Pivarski
// Created:     Wed Dec 18 10:02:28 EST 2002
// $Id: BeamGasFilterProc.cc,v 1.9 2004/04/12 17:04:44 mccann Exp $
//
// Revision history
//
// $Log: BeamGasFilterProc.cc,v $
// Revision 1.9  2004/04/12 17:04:44  mccann
// Fixed a division-by-zero bug.  (Thanks Istvan!)
//
// Revision 1.8  2003/05/06 14:10:28  vk
// Fix required type for sqrt
//
// Revision 1.7  2003/03/10 17:54:26  mccann
// included check on beamspot determination and fixed another inconsistancy between cuts in analyzed sample VS cuts in BeamGasFilterProc (BGFP now consistently subtracts m_beam_z from weighted_z)
//
// Revision 1.6  2003/03/10 15:36:33  mccann
// Added a cosmics count and more parameter control over output; fixed a bug in beamgas estimation (previously forgot to apply RPHI cut to shoulder events)
//
// Revision 1.5  2003/03/05 01:21:52  mccann
// Removed neutral energy cut, added run-time estimation of how much beamgas PASSES cuts.
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "BeamGasFilterProc/BeamGasFilterProc.h"
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
#include "BeamSpot/BeamSpot.h"

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
static const char* const kFacilityString = "Processor.BeamGasFilterProc" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: BeamGasFilterProc.cc,v 1.9 2004/04/12 17:04:44 mccann Exp $";
static const char* const kTagString = "$Name:  $";

static const double closestIntersectionDefault = 0.005;
static const double weightedZDefault = 0.05;
static const double weightedZClevernessDefault = 0.;

//
// static data member definitions
//



//
// constructors and destructor
//
BeamGasFilterProc::BeamGasFilterProc( void )               // anal1
   : Processor( "BeamGasFilterProc" )
   , m_closestIntersectionCut( "closestIntersectionCut", this, closestIntersectionDefault )
   , m_weightedZCut( "weightedZCut", this, weightedZDefault )
   , m_weightedZCleverness( "weightedZCleverness", this, weightedZClevernessDefault )
   , m_explicitBeamSpot( "explicitBeamSpot", this, false )
   , m_beamX( "beamX", this, 0. )
   , m_beamY( "beamY", this, 0. )
   , m_beamZ( "beamZ", this, 0. )
   , m_centerFactor( "centerFactor", this, 0.70 )
   , m_centerFactorUncert( "centerFactorUncert", this, 0.16 )
   , m_cosmicCenterFactor( "cosmicCenterFactor", this, 0.0089 )
   , m_cosmicCenterFactorUncert( "cosmicCenterFactorUncert", this, 0.0020 )
   , m_noExplaination( "noExplaination", this, false )
   , m_cumulativeFrequency( "cumulativeFrequency", this, 100. )
   , m_passed(0)
   , m_shoulder(0)
   , m_cosmic_shoulder(0)
   , m_total(0)
   , m_run(0)
   , m_run_passed(0)
   , m_run_shoulder(0)
   , m_run_cosmic_shoulder(0)
   , m_run_total(0)
   , m_run_rphi_mean(0.)
   , m_run_z_mean(0.)
   , m_beam_x(0.)
   , m_beam_y(0.)
   , m_beam_z(0.)
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &BeamGasFilterProc::event,    Stream::kEvent );
   bind( &BeamGasFilterProc::beginRun, Stream::kBeginRun );
   //bind( &BeamGasFilterProc::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

BeamGasFilterProc::~BeamGasFilterProc()                    // anal5
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
BeamGasFilterProc::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
BeamGasFilterProc::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
   printout(SYSTEM, true);
   printout(SYSTEM);
}


// ---------------- standard place to book histograms ---------------
void
BeamGasFilterProc::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   char closest_inter_cut[1024];
   char weighted_z_cut[1024];

   sprintf( closest_inter_cut, "%g", m_closestIntersectionCut.value() );
   sprintf( weighted_z_cut, "%g", m_weightedZCut.value() );

   m_hist_closest_inter = iHistoManager.histogram(
      100, (string("Closest intersection (cut at ") + string(closest_inter_cut) + string(")")).c_str(),
      100, 0., 0.10 );
   m_histclose_closest_inter = iHistoManager.histogram(
      101, (string("Closest intersection (cut at ") + string(closest_inter_cut) + string(")")).c_str(),
      100, 0., 0.005 );
   m_hist_weighted_z = iHistoManager.histogram(
      200, (string("Weighted Z (cut at ") + string(weighted_z_cut) + string(")")).c_str(),
      100, -1., 1. );
   m_histclose_weighted_z = iHistoManager.histogram(
      201, (string("Weighted Z (cut at ") + string(weighted_z_cut) + string(")")).c_str(),
      100, -0.10, 0.10 );
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
BeamGasFilterProc::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem< DBEventHeader > header;
   extract( iFrame.record( Stream::kEvent ), header );
   if ( m_run == 0 ) m_run = header->run();

   if ( m_run != header->run() ) {
      printout(SYSTEM, true);

      m_run = header->run();
      m_run_passed = 0;
      m_run_shoulder = 0;
      m_run_cosmic_shoulder = 0;
      m_run_total = 0;
      m_run_rphi_mean = 0.;
      m_run_z_mean = 0.;
   }

   FATable< NavTrack > tracks;
   extract( iFrame.record( Stream::kEvent ), tracks );
   FATable< NavTrack >::const_iterator tracks_end = tracks.end();

   if ( tracks.size() <= 2 ) {
      report(DEBUG, kFacilityString)
	 << "Failed because number of tracks == " << tracks.size() << " <= 2." << endl;
      return ActionBase::kFailed;
   }

   double ci_perp(1000.);

   double weighted_z(1000.);
   double znumer(0.);
   double zdenom(0.);

   int num_tracks(0.);

   FATable< NavTrack >::const_iterator track_iter;
   FATable< NavTrack >::const_iterator other_iter;
   for ( track_iter = tracks.begin();  track_iter != tracks_end;  track_iter++ ) {
      if ( ! track_iter->seedQuality()->originUsed()            &&
	   ! track_iter->pionQuality()->fitAbort()              &&
	   1./fabs(track_iter->pionHelix()->curvature()) > 0.42  &&
	   ! track_iter->pionHelix()->hasNullErrorMatrix()        
	 ) {
	 // Secondary track loop to calculate pairwise intersections
	 for ( other_iter = track_iter;  other_iter != tracks_end;  other_iter++ ) {
	    if ( other_iter != track_iter  &&
		 ! other_iter->seedQuality()->originUsed()            &&
		 ! other_iter->pionQuality()->fitAbort()              &&
		 1./fabs(other_iter->pionHelix()->curvature()) > 0.42  &&
		 ! other_iter->pionHelix()->hasNullErrorMatrix()        
	       ) { 
	       double tx(-1000.), ty(-1000.), tz(-1000.), tsig2z(-1000.), tzsep(-1000.);

	       KTHelix track( *track_iter->pionHelix() );
	       KTHelix other( *other_iter->pionHelix() );

	       if ( calc_intersection( tx, ty, tz, tsig2z, tzsep, track, other ) ) {

		  double perp( sqrt(sqr(tx - m_beam_x) + sqr(ty - m_beam_y)) );
		  if ( perp < ci_perp ) ci_perp = perp;

		  if ( fabs(m_weightedZCleverness.value() - 0.) < .1 ) {
		     // uncertainty in the z intersection + mismatch in z + mismatch in perp
		     double z_uncert2( tsig2z + sqr(tzsep) + sqr(tx - m_beam_x) + sqr(ty - m_beam_y) );
		     znumer += tz / z_uncert2;
		     zdenom += 1. / z_uncert2;
		  }
		  else if ( fabs(m_weightedZCleverness.value() - -1.) < .1 ) {
		     // uncertainty in the z intersection + mismatch in z, cut on perp
		     if ( perp > m_closestIntersectionCut.value() ) {
			double z_uncert2( tsig2z + sqr(tzsep) );
			znumer += tz / z_uncert2;
			zdenom += 1. / z_uncert2;
		     }
		  }
		  else {
		     report(EMERGENCY, kFacilityString)
			<< "unknown weightedZCleverness mode"
			<< m_weightedZCleverness.value() << endl;
		     assert(false);
		  }

	       } // end if successful intersection
	       num_tracks++;
	    } // end if secondary track passes quality cuts
	 } // end loop over secondary track

      } // end if primary track passes quality cuts
   } // end loop over tracks

   if ( zdenom != 0. ) {
      weighted_z = znumer / zdenom;
      weighted_z -= m_beam_z;
   }

   if ( m_total % int(m_cumulativeFrequency.value()) == 0 ) printout(INFO);

   m_total++;
   m_run_total++;
   if ( ci_perp < closestIntersectionDefault      &&
	fabs(weighted_z) > weightedZDefault       &&
	fabs(weighted_z) < 4. * weightedZDefault     ) {
      m_shoulder++;
      m_run_shoulder++;
   }
   if ( fabs(weighted_z) < weightedZDefault        &&
	ci_perp > 40.* closestIntersectionDefault  &&
	ci_perp < 80.* closestIntersectionDefault     ) {
      m_cosmic_shoulder++;
      m_run_cosmic_shoulder++;
   }

   m_hist_closest_inter->fill( ci_perp );
   m_histclose_closest_inter->fill( ci_perp );

   m_hist_weighted_z->fill( weighted_z );
   m_histclose_weighted_z->fill( weighted_z );

   if ( ci_perp < m_closestIntersectionCut.value()  &&  fabs(weighted_z) < m_weightedZCut.value() ) {
      m_passed++;
      m_run_passed++;
      m_run_rphi_mean += ci_perp;
      m_run_z_mean += weighted_z;
      return ActionBase::kPassed;
   } else {
      report( DEBUG, kFacilityString )
	 << "FAILED event; closest_inter == " << ci_perp << " > " << m_closestIntersectionCut.value()
	 << " |weighted_z| == |" << weighted_z << "| > " << m_weightedZCut.value() << endl;
      return ActionBase::kFailed;
   }
}

ActionBase::ActionResult
BeamGasFilterProc::beginRun( Frame& iFrame )       // anal2 equiv.
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
BeamGasFilterProc::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   return ActionBase::kPassed;
}
*/

void BeamGasFilterProc::printout(Severity severity, DABoolean runOnly)
{
   int passed, total, shoulder, cosmic_shoulder;
   double fraction;

   if ( runOnly ) {
      passed = m_run_passed;
      total = m_run_total;
      shoulder = m_run_shoulder;
      cosmic_shoulder = m_run_cosmic_shoulder;
   } else {
      passed = m_passed;
      total = m_total;
      shoulder = m_shoulder;
      cosmic_shoulder = m_cosmic_shoulder;
   }
   if (total == 0 || passed == 0) return;
   fraction = double(passed) / double(total);

   char run[7];
   sprintf(run, "%06d", m_run);

   if ( m_closestIntersectionCut.value() == closestIntersectionDefault  &&
	m_weightedZCut.value()           == weightedZDefault            &&
	m_weightedZCleverness.value()    == weightedZClevernessDefault     ) {

      double contamination(     shoulder * m_centerFactor.value() );
      double contaminationStat( sqrt((double)shoulder) * m_centerFactor.value() );
      double contaminationSyst( shoulder * m_centerFactorUncert.value() );
      
      double cosmic_contamination(     cosmic_shoulder * m_cosmicCenterFactor.value() );
      double cosmic_contaminationStat( sqrt((double)cosmic_shoulder) * m_cosmicCenterFactor.value() );
      double cosmic_contaminationSyst( cosmic_shoulder * m_cosmicCenterFactorUncert.value() );

      double real( double(passed) - contamination - cosmic_contamination );
      double realStat( sqrt(double(passed) + sqr(contaminationStat) + sqr(cosmic_contaminationStat)) );
      double realSyst( contaminationSyst + cosmic_contaminationSyst );

      report(severity, kFacilityString)
	 << (runOnly ? (string("RUN_SUMMARY for ") + string(run) + string(" : ")).c_str() : "CUMULATIVE_SUMMARY: ")
	 << "Passed " << passed << " out of " << total << " events, or " << fraction*100. << " %." << endl;

      report(severity, kFacilityString)
	 << (runOnly ? (string("RUN_BEAMGAS for ") + string(run) + string(" : ")).c_str() : "CUMULATIVE_BEAMGAS: ")
	 << "Shoulder of " << shoulder << " implies "
	 << contamination << " +/- " << contaminationStat << " +/- " << contaminationSyst
	 << " beamgas events passed the cuts." << endl;

      report(severity, kFacilityString)
	 << (runOnly ? (string("RUN_COSMICS for ") + string(run) + string(" : ")).c_str() : "CUMULATIVE_COSMICS: ")
	 << "Shoulder of " << cosmic_shoulder << " implies "
	 << cosmic_contamination << " +/- " << cosmic_contaminationStat << " +/- " << cosmic_contaminationSyst
	 << " cosmic rays passed the cuts." << endl;

      report(severity, kFacilityString)
	 << (runOnly ? (string("RUN_CONCLUSION for ") + string(run) + string(" : ")).c_str() : "CUMULATIVE_CONCLUSION: ")
	 << "Saw " << real << " +/- " << realStat << " +/- " << realSyst << " real e+e- collisions." << endl;

      if ( ! m_noExplaination.value() )
	 report(severity, kFacilityString)
	    << "EXPLAINATION: Beamgas shoulder events are events which pass the RPHI cut (closest intersection < 5mm)" << endl
	    << "EXPLAINATION: and are outside the Z cut (5cm < |event z| < 20cm).  The first uncertainty is" << endl
	    << "EXPLAINATION: statistical; disjoint datasets are independent.  The second uncertainty is common to" << endl
	    << "EXPLAINATION: all datasets." << endl
	    << "EXPLAINATION: Cosmic shoulder events are events which pass the Z cut (|event z| < 5cm) and have" << endl
	    << "EXPLAINATION: a closest intersection between 20cm and 40cm.  Again, the first uncertainty is" << endl
	    << "EXPLAINATION: statistical while the second is systematic, with the same dependencies." << endl
	    << "EXPLAINATION: The conclusion is just a convenient subtraction, with statistical uncertainties" << endl
	    << "EXPLAINATION: combined in quadrature as the first +/- and systematic uncertainties combined" << endl
	    << "EXPLAINATION: as a straight sum for the second +/-." << endl;
   }
   else
      report(severity, kFacilityString)
	 << (runOnly ? (string("RUN_SUMMARY for ") + string(run) + string(" : ")).c_str() : "CUMULATIVE_SUMMARY: ")
	 << "Passed " << passed << " out of " << total << " events, or " << fraction*100. << " %." << endl;

   if ( runOnly ) {
      m_run_rphi_mean /= double(m_run_passed);
      m_run_z_mean /= double(m_run_passed);

      report(severity, kFacilityString)
	 << "RUN_BEAMSPOTCHECK for " << run << " : RPHI mean is " << m_run_rphi_mean
	 << " and Z mean is " << m_run_z_mean << endl;
   }
}

DABoolean BeamGasFilterProc::calc_intersection( double& x, double& y, double& z, double& sig2z, double& zsep,
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
   zsep = az - bz;

   // Because it is not useful for the `closest_intersection'
   // algorithm, I had to throw out a great deal of lovely code that
   // calculates uncertanties in x and y.  If you want this, ask me
   // about it.
   return true;
}

DABoolean BeamGasFilterProc::arclength_test( double x, double y, KTHelix& a, KTHelix& b )
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

   return ( aarclength > -tailCut  &&  aarclength < M_PI/2./fabs(a.curvature()) )  &&
      ( barclength > -tailCut  &&  barclength < M_PI/2./fabs(b.curvature()) );
}

//
// const member functions
//

//
// static member functions
//
