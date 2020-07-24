// -*- C++ -*-
//
// Package:     DSneak
// Module:      DSneak
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Mar 17 15:12:46 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "DSneak/DSneak.h"
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

#include "Navigation/NavShower.h"
#include "C3cc/CcShowerAttributes.h"
#include "Navigation/NavTkShMatch.h"

#include "BeamSpot/BeamSpot.h"
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
static const char* const kFacilityString = "Processor.DSneak" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

static const double M_KMASS  = 0.497672;
static const double M_PIMASS = 0.13957018;
static const double M_DMASS  = 1.8645;

//
// static data member definitions
//



//
// constructors and destructor
//
DSneak::DSneak( void )               // anal1
   : Processor( "DSneak" )
   , m_zeroBeamSpot( "zeroBeamSpot", this, false )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &DSneak::event,    Stream::kEvent );
   //bind( &DSneak::beginRun, Stream::kBeginRun );
   //bind( &DSneak::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

DSneak::~DSneak()                    // anal5
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
DSneak::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
DSneak::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
DSneak::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   const char* label[kNumEntries] = {
      "p1px", "p1py", "p1pz", "n1px", "n1py", "n1pz",
      "p2px", "p2py", "p2pz", "n2px", "n2py", "n2pz",
      "kmass",
      "kbx", "kby", "kbz", "kbzsep",
      "kdx", "kdy", "kdz", "kdzsep",
      "kdircos",
      "dmassbc", "dmass",
      "dpx", "dpy", "dpz", "de",
      "beame", "misspx", "misspy", "misspz", "misse"
   };

   m_ntuple = iHistoManager.ntuple(
      1, "dsneak", kNumEntries, 262144, label );
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
DSneak::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;
   DABoolean passed(false);

   double beamX(0.), beamY(0.), beamZ(0.);
   if ( ! m_zeroBeamSpot.value() ) {
      FAItem<BeamSpot> spot;
      extract(iFrame.record(Stream::kBeginRun), spot);
      beamX = spot->center().x();
      beamY = spot->center().y();
      beamZ = spot->center().z();
   }

   FAItem<BeamEnergy> beamEnergy;
   extract(iFrame.record(Stream::kStartRun), beamEnergy);

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks, "TrkmanApproved");
   FATable<NavTrack>::const_iterator tracks_end = tracks.end();
   FATable<NavTrack>::const_iterator track_iter;
   FATable<NavTrack>::const_iterator second_pos;
   FATable<NavTrack>::const_iterator second_neg;
   FATable<NavTrack>::const_iterator prompt_pos;
   FATable<NavTrack>::const_iterator prompt_neg;

   if ( tracks.size() < 4 ) return ActionBase::kFailed;

   int charge(0);
   for (track_iter = tracks.begin();  track_iter != tracks_end;  ++track_iter)
      charge += (track_iter->pionHelix()->curvature() > 0. ? 1 : -1);
   if ( charge != 0 ) return ActionBase::kFailed;

   FATable<NavShower> showers;
   extract(iFrame.record(Stream::kEvent), showers);
   FATable<NavShower>::const_iterator showers_end = showers.end();
   FATable<NavShower>::const_iterator shower_iter;

   vector<KTHelix> origin_tracks;
   for ( track_iter = tracks.begin();  track_iter != tracks_end;  ++track_iter )
      if ( ! track_iter->seedQuality()->originUsed()             &&
	   ! track_iter->pionQuality()->fitAbort()               &&
	   1./fabs(track_iter->pionHelix()->curvature()) > 0.42  &&
	   ! track_iter->pionHelix()->hasNullErrorMatrix()          )
	 origin_tracks.push_back(*track_iter->pionHelix());
   HepPoint3D origin(event_vertex(origin_tracks, HepPoint3D(beamX, beamY, beamZ)));
   if ( origin.x() - beamX > 0.005  ||  origin.z() - beamZ > 0.05 ) return ActionBase::kFailed;

   second_pos = tracks.begin();
   for ( ;  second_pos != tracks_end;  ++second_pos )
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
		    abs(kd_zsep) < 0.005                 &&
		    (kaon_death - origin).mag() > 0.003     ) {

		  double secPosE(sqrt(second_pos->pionFit()->momentum().mag2() + sqr(M_PIMASS)));
		  double secNegE(sqrt(second_neg->pionFit()->momentum().mag2() + sqr(M_PIMASS)));
		  Hep3Vector secPosP(rotate_momentum(second_pos->pionFit()->momentum(), secPosPhi));
		  Hep3Vector secNegP(rotate_momentum(second_neg->pionFit()->momentum(), secNegPhi));
		  double kaonEfromPis(secPosE + secNegE);
		  Hep3Vector kaonPfromPis(secPosP + secNegP);
		  double kaonMfromPis(sqrt(sqr(kaonEfromPis) - kaonPfromPis.mag2()));
		  if ( abs(kaonMfromPis - M_KMASS) < 0.010 ) {

		     prompt_pos = second_neg;
		     for ( ;  prompt_pos != tracks_end;  ++prompt_pos )
			if ( prompt_pos->pionHelix()->curvature() > 0. ) {

			   prompt_neg = prompt_pos;  ++prompt_neg;
			   for ( ;  prompt_neg != tracks_end;  ++prompt_neg )
			      if ( prompt_neg->pionHelix()->curvature() < 0. ) {
	       
				 HepPoint3D kaon_birth(-1000., -1000., -1000.);
				 double kb_zsep(-1000.), kb_sigz(-1000.), proPosPhi(-1000.), proNegPhi(-1000.);
				 KTHelix proPos(*prompt_pos->pionHelix());
				 KTHelix proNeg(*prompt_neg->pionHelix());
				 if ( calc_intersection(kaon_birth,
							kb_zsep, kb_sigz, proPosPhi, proNegPhi,
							proPos, proNeg)    &&
				      abs(kb_zsep) < 0.005                 &&
				      (kaon_birth - origin).mag() < 0.003     ) {

				    Hep3Vector kaon_flight(kaon_death - kaon_birth);
				    double direction_cosine(kaonPfromPis.dot(kaon_flight)
							    / kaonPfromPis.mag() / kaon_flight.mag());
				    if ( direction_cosine > 0.98 ) {

				       double proPosE(sqrt(prompt_pos->pionFit()->momentum().mag2() + sqr(M_PIMASS)));
				       double proNegE(sqrt(prompt_neg->pionFit()->momentum().mag2() + sqr(M_PIMASS)));
				       Hep3Vector proPosP(rotate_momentum(prompt_pos->pionFit()->momentum(), proPosPhi));
				       Hep3Vector proNegP(rotate_momentum(prompt_neg->pionFit()->momentum(), proNegPhi));

 				       double deeEfromKpipi(proPosE + proNegE + secPosE + secNegE);
 				       Hep3Vector deePfromKpipi(proPosP + proNegP + secPosP + secNegP);
 				       double deeMfromKpipi(sqrt(sqr(deeEfromKpipi) - deePfromKpipi.mag2()));
 				       double deeMfromBC(sqrt(sqr(beamEnergy->value()) - deePfromKpipi.mag2()));
				       
				       if ( deeEfromKpipi > 0.4 * beamEnergy->value() )
					  cout << "NTUPLE5 " << deeEfromKpipi
					       << " " << beamEnergy->value()
					       << " " << deeMfromKpipi
					       << " " << deeMfromBC << endl;





// 				       cout << "compareme " << (proPosE + proNegE + secPosE + secNegE)
// 					    << " " << (track_energy + shower_energy)
// 					    << " " << 2. * beamEnergy->value() << endl;




// 				       double shower_energy(0.);
// 				       Hep3Vector shower_momentum(0., 0., 0.);
// 				       for ( shower_iter = showers.begin();  shower_iter != showers_end;  ++shower_iter )
// 					  if ( shower_iter->attributes().status()                               &&
// 					       ! shower_iter->attributes().hot()                                &&
// 					       shower_iter->attributes().energy() > 0.01 * beamEnergy->value()     ) {
// 					     shower_energy += shower_iter->attributes().energy();
// 					     shower_momentum += shower_iter->attributes().momentum();
// 					  }








				       


// 				       double tagDenergy(0.);
// 				       Hep3Vector tagDmomentum(0., 0., 0.);
// 				       for ( FATable<NavShower>::const_iterator shower_iter = showers.begin();
// 					     shower_iter != showers_end;
// 					     ++shower_iter )
// 					  if ( shower_iter->attributes().status()                               &&
// 					       !shower_iter->attributes().hot()                                 &&
// 					       shower_iter->attributes().energy() > 0.01 * beamEnergy->value()  &&
// 					       shower_iter->noTrackMatch()                                      &&
// 					       shower_iter->noTrackMatchConReg()                                &&
// 					       ( !shower_iter->bremTrack().valid()  ||
// 						 ( shower_iter->bremTrack()->trackShowerMatch().valid()  &&
// 						   shower_iter->bremTrack()->trackShowerMatch()->eOverP() < 0.5 )) ) {
// 					     tagDenergy += shower_iter->attributes().energy();
// 					     tagDmomentum += shower_iter->attributes().momentum();
// 					  }

// 				       for ( track_iter = tracks.begin();  track_iter != tracks_end;  ++track_iter )
// 					  if ( ! track_iter->seedQuality()->originUsed()  &&
// 					       ! track_iter->pionQuality()->fitAbort()    &&
// 					       track_iter != prompt_pos                   &&
// 					       track_iter != prompt_neg  		  &&
// 					       track_iter != second_pos  		  &&
// 					       track_iter != second_neg                      ) {
// 					     tagDenergy += track_iter->pionFit()->energy();
// 					     tagDmomentum += track_iter->pionFit()->momentum();
// 					     if ( track_iter->trackShowerMatch().valid()          &&
// 						  track_iter->trackShowerMatch()->eOverP() > 0.5  &&
// 						  track_iter->bremShowers().size() > 0               ) {
// 						FATable<NavShower> bremShowers(track_iter->bremShowers());
// 						FATable<NavShower>::const_iterator bremShowers_end = bremShowers.end();
// 						for ( FATable<NavShower>::const_iterator brem_iter = bremShowers.begin();
// 						      brem_iter != bremShowers_end;
// 						      ++brem_iter )
// 						   if ( brem_iter->attributes().energy() > 0.01 * beamEnergy->value()  &&
// 							!brem_iter->attributes().hot()                                    ) {
// 						      tagDenergy += brem_iter->attributes().energy();
// 						      tagDmomentum += brem_iter->attributes().momentum();
// 						   } // end loop over bremshowers
// 					     } // end if there are showers associated with this track
// 					  } // end if this track is good and is not one of the Four

// 				       double tagDeeM(sqrt(sqr(tagDenergy) - tagDmomentum.mag2()));
// 				       double tagDeeMfromBC(sqrt(sqr(beamEnergy->value()) - tagDmomentum.mag2()));

// 				       report(DEBUG, kFacilityString)
// 					  << "Found a potential D -> K pi pi:" << endl
// 					  << "    Kaon flight is " << kaon_birth << " to " << kaon_death << endl
// 					  << "    Kaon mass is " << kaonMfromPis << endl
// 					  << "    Beam-constrained D mass is " << tagDeeMfromBC << endl;

// // 				       if ( 1.66 < tagDeeMfromBC  &&  tagDeeMfromBC < 2.06 ) {

// 					  double deeEfromTag(tagDenergy);
// 					  Hep3Vector deePfromTag(Hep3Vector(-0.0261 * beamEnergy->value() / 5.29, 0., 0.)
// 								 - tagDmomentum);

// 					  Hep3Vector missingP(deePfromTag - proPosP - proNegP - kaonPfromPis);
// 					  double missingE(beamEnergy->value() - proPosE - proNegE - kaonEfromPis);










// 					  int i;
// 					  float v[kNumEntries];  for ( i = 0;  i < kNumEntries;  i++ ) v[i] = -2000.;
// 					  v[kProPosPx] = proPosP.x();
// 					  v[kProPosPy] = proPosP.y();
// 					  v[kProPosPz] = proPosP.z();
// 					  v[kProNegPx] = proNegP.x();
// 					  v[kProNegPy] = proNegP.y();
// 					  v[kProNegPz] = proNegP.z();
// 					  v[kSecPosPx] = secPosP.x();
// 					  v[kSecPosPy] = secPosP.y();
// 					  v[kSecPosPz] = secPosP.z();
// 					  v[kSecNegPx] = secNegP.x();
// 					  v[kSecNegPy] = secNegP.y();
// 					  v[kSecNegPz] = secNegP.z();
// 					  v[kKaonMass] = kaonMfromPis;
// 					  v[kKaonBirthX] = kaon_birth.x();
// 					  v[kKaonBirthY] = kaon_birth.y();
// 					  v[kKaonBirthZ] = kaon_birth.z();
// 					  v[kKaonBirthZsep] = kb_zsep;
// 					  v[kKaonDeathX] = kaon_death.x();
// 					  v[kKaonDeathY] = kaon_death.y();
// 					  v[kKaonDeathZ] = kaon_death.z();
// 					  v[kKaonDeathZsep] = kd_zsep;
// 					  v[kDirectionCosine] = direction_cosine;
// 					  v[kDeeMassFromBC] = tagDeeMfromBC;
// 					  v[kDeeMass] = tagDeeM;
// 					  v[kDeePx] = deePfromTag.x();
// 					  v[kDeePy] = deePfromTag.y();
// 					  v[kDeePz] = deePfromTag.z();
// 					  v[kDeeE] = deeEfromTag;
					  
// 					  v[kEBeam] = beamEnergy->value();
// 					  v[kMissingPx] = missingP.x();
// 					  v[kMissingPy] = missingP.y();
// 					  v[kMissingPz] = missingP.z();
// 					  v[kMissingE] = missingE;

// 					  m_ntuple->fill(v);

// 					  report(INFO, kFacilityString) << "NTUPLE ";
// 					  for ( i = 0;  i < kNumEntries;  i++ )
// 					     report(INFO, kFacilityString) << v[i] << " ";
// 					  report(INFO, kFacilityString) << endl;

// 					  passed = true;

//				       } // beam-constrained D mass cut
				    } // kaon momentum direction is along flight path
				 } // prompt vertex is within 3 mm
			      } // prompt_neg is actually negatively-charged
			} // prompt_pos is actually positively-charged
		  } // kaon mass cut
	       } // secondary vertex is outside of 3 mm
	    } // second_neg is actually negative
      } // second_pos is actually positive

   if ( passed ) return ActionBase::kPassed;
   else return ActionBase::kFailed;
}

/*
ActionBase::ActionResult
DSneak::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
DSneak::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   return ActionBase::kPassed;
}
*/

HepPoint3D DSneak::event_vertex( vector<KTHelix>& good_tracks, HepPoint3D& beamSpot )
{
   double beamX(beamSpot.x()), beamY(beamSpot.y());

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

   for ( i = 0;  i < num_tracks;  i++ )
      for ( j = i+1;  j < num_tracks;  j++ ) {
	 HepPoint3D trial(-1000., -1000., -1000.);
	 double tzsep(-1000.), tsigz(-1000.), taphi(-1000.), tbphi(-1000.);
	 
	 if ( calc_intersection(trial, tzsep, tsigz, taphi, tbphi,
				good_tracks[i], good_tracks[j]) ) {
	    
	    // x-quality of intersection is uncertainty in the x intersection +
	    // mismatch in z + distance from already-determined weighted_z
	    // (added in quadrature with weights to normalize the distribution)
	    double xqual( sqr(tzsep)/sqr(0.002) +
			  sqr(trial.z() - weighted_z)/sqr(0.025) );
	    // The sigx criteria didn't do much anyway... I get to speed up code
// 	    double xqual( sqr(tsigx)/sqr(0.2) + sqr(tzsep)/sqr(0.002) +
// 			  sqr(tz - weighted_z)/sqr(0.025) );
	    
	    xnumer += trial.x() / xqual;
	    xdenom += 1. / xqual;
	    
	 } // end if successful intersection
      } // end loop over tracks

   if ( xdenom != 0. ) weighted_x = xnumer / xdenom;

   // the beam is a ribbon: no need for event-by-event determinations of y
   return HepPoint3D(weighted_x, beamY, weighted_z);
}

DABoolean DSneak::calc_intersection( HepPoint3D& intersection,
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

   aphi = a.phi0() + abs(aarclength) * 2. * a.curvature();
   bphi = b.phi0() + abs(barclength) * 2. * b.curvature();

   return true;
}

DABoolean DSneak::arclength_test( double x, double y, KTHelix& a, KTHelix& b,
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

   return ( aarclength > -tailCut  &&  aarclength < M_PI/2./abs(a.curvature()) )  &&
      ( barclength > -tailCut  &&  barclength < M_PI/2./abs(b.curvature()) );
}

Hep3Vector DSneak::rotate_momentum(Hep3Vector momentum, double angle)
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
