// -*- C++ -*-
//
// Package:     Sneaky
// Module:      Sneaky
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Mar 18 15:25:18 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "Sneaky/Sneaky.h"
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
#include "Navigation/NavRich.h"
#include "DedxInfo/DedxInfo.h"

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
static const char* const kFacilityString = "Processor.Sneaky" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

static const double M_K0MASS  = 0.497672;
static const double M_KMASS   = 0.493677;
static const double M_PIMASS  = 0.13957018;
static const double M_D0MASS  = 1.8645;

//
// static data member definitions
//



//
// constructors and destructor
//
Sneaky::Sneaky( void )               // anal1
   : Processor( "Sneaky" )
   , m_zeroBeamSpot( "zeroBeamSpot", this, false )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &Sneaky::event,    Stream::kEvent );
   //bind( &Sneaky::beginRun, Stream::kBeginRun );
   //bind( &Sneaky::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

Sneaky::~Sneaky()                    // anal5
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
Sneaky::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
Sneaky::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
Sneaky::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

//    const char* label[kNumEntries] = {
//    };

//    m_ntuple = iHistoManager.ntuple(
//       1, "sneaky", kNumEntries, 262144, label );
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
Sneaky::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;
   int i, j;

   HepPoint3D beamSpot(0., 0., 0.);
   if ( ! m_zeroBeamSpot.value() ) {
      FAItem<BeamSpot> spot;
      extract(iFrame.record(Stream::kBeginRun), spot);
      beamSpot = spot->center();
   }

   FAItem<BeamEnergy> beamEnergy;
   extract(iFrame.record(Stream::kStartRun), beamEnergy);

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks, "TrkmanApproved");
   FATable<NavTrack>::const_iterator tracks_end = tracks.end();
   FATable<NavTrack>::const_iterator track_iter;
   FATable<NavTrack>::const_iterator other_iter;
   FATable<NavTrack>::const_iterator kaon = tracks_end;
   FATable<NavTrack>::const_iterator secondary1;
   FATable<NavTrack>::const_iterator secondary2;
   FATable<NavTrack>::const_iterator sameSignAsKaon;
   FATable<NavTrack>::const_iterator heads = tracks_end;
   FATable<NavTrack>::const_iterator tails = tracks_end;

   if ( tracks.size() != 6 ) return ActionBase::kFailed;

   // I would think Trkman should get rid of these... just in case it doesn't...
   for (track_iter = tracks.begin();  track_iter != tracks_end;  ++track_iter)
      if ( track_iter->seedQuality()->originUsed()  ||
	   track_iter->pionQuality()->fitAbort()       )
	 return ActionBase::kFailed;

   // Charge balance, of course
   int charge(0);
   for (track_iter = tracks.begin();  track_iter != tracks_end;  ++track_iter)
      charge += (track_iter->pionHelix()->curvature() > 0. ? 1 : -1);
   if ( charge != 0 ) return ActionBase::kFailed;

   // Make sure they all have dedx and RICH info!
   for (track_iter = tracks.begin();  track_iter != tracks_end;  ++track_iter)
      if ( !track_iter->richInfo().valid()  ||
	   !track_iter->dedxInfo().valid()     ) return ActionBase::kFailed;

   // Find the kaon
   for (track_iter = tracks.begin();  track_iter != tracks_end;  ++track_iter)
      // Be sure this is a kaon and not a pion
      if ( (track_iter->richInfo()->kaonLogLikelihood() -
	    track_iter->richInfo()->pionLogLikelihood() < 0.)   &&
	   abs(track_iter->dedxInfo()->kSigma()) < 3.           &&
	   abs(track_iter->dedxInfo()->piSigma()) > 3.             ) {
	 if ( kaon == tracks_end ) kaon = track_iter;
	 else return ActionBase::kFailed;
      } // end if this is a kaon and not a pion

   // Be sure the rest are pions
   for (track_iter = tracks.begin();  track_iter != tracks_end;  ++track_iter)
      if ( track_iter != kaon                                           &&
	   ! ((track_iter->richInfo()->pionLogLikelihood() -
	       track_iter->richInfo()->kaonLogLikelihood() < 0.)   &&
	      abs(track_iter->dedxInfo()->piSigma()) < 3.          &&
	      abs(track_iter->dedxInfo()->kSigma()) > 3.              )    )
	 return ActionBase::kFailed;

   // Find the most distant intersection and the tracks involved in it
   HepPoint3D secondary_vertex;
   double secondary_distance(0.);
   double secondary1_phi, secondary2_phi;
   for (track_iter = tracks.begin();  track_iter != tracks_end;  ++track_iter)
      for (other_iter = track_iter;  other_iter != tracks_end;  ++other_iter)
	 if ( track_iter != other_iter ) {
	    HepPoint3D intersection;
	    double zsep(-1000.), aphi(-1000.), bphi(-1000.), distance(-1000.);
	    if ( calc_intersection(intersection, zsep, aphi, bphi,
				   &*track_iter->pionHelix(), &*other_iter->pionHelix())  &&
		 abs(zsep) < 0.005                                                        &&
		 (distance = (intersection - beamSpot).mag()) > secondary_distance           ) {
	       secondary_vertex = intersection;
	       secondary_distance = distance;
	       secondary1 = track_iter;
	       secondary2 = other_iter;
	       secondary1_phi = aphi;
	       secondary2_phi = bphi;
	    } // end if this intersection is the furthest we have seen yet
	 } // end loop over pairs of tracks
   if ( secondary_distance < 0.003 ) return ActionBase::kFailed;
   if ( secondary1->pionHelix()->curvature() * secondary2->pionHelix()->curvature() > 0. )
      return ActionBase::kFailed;
   double secondary1_energy(secondary1->pionFit()->energy());
   double secondary2_energy(secondary2->pionFit()->energy());
   Hep3Vector secondary1_momentum(rotate_momentum(secondary1->pionFit()->momentum(), secondary1_phi));
   Hep3Vector secondary2_momentum(rotate_momentum(secondary2->pionFit()->momentum(), secondary2_phi));
   double kshort_energy(secondary1_energy + secondary2_energy);
   Hep3Vector kshort_momentum(secondary1_momentum + secondary2_momentum);
   double kshort_mass(sqrt(sqr(kshort_energy) + kshort_momentum.mag2()));
   if ( abs(kshort_mass - M_K0MASS) > 0.010 ) return ActionBase::kFailed;

   report(INFO, kFacilityString) << "Looking good: we have 6 tracks where two of them describe a kaon..." << endl;

   // Be sure that the others are close together
   HepPoint3D primary_vertex(0., 0., 0.);
   int vertices_in_average(0);
   vector<HepPoint3D> sub_vertex;
   for (track_iter = tracks.begin();  track_iter != tracks_end;  ++track_iter)
      for (other_iter = track_iter;  other_iter != tracks_end;  ++other_iter)
	 if ( track_iter != other_iter                                &&
	      track_iter != secondary1  &&  track_iter != secondary2  &&
	      other_iter != secondary1  &&  other_iter != secondary2     ) {
	    HepPoint3D intersection;
	    double zsep(-1000.), aphi(-1000.), bphi(-1000.), distance(-1000.);
	    if ( calc_intersection(intersection, zsep, aphi, bphi,
				   &*track_iter->pionHelix(), &*other_iter->pionHelix())  &&
		 abs(zsep) < 0.005                                                           ) {
	       primary_vertex += intersection;
	       vertices_in_average++;
	       sub_vertex.push_back(intersection);
	    } // end if this is a good intersection
	 } // end loop over pairs of tracks other than the two secondaries I identified earlier
   if ( vertices_in_average != 6 ) return ActionBase::kFailed;
   report(INFO, kFacilityString) << "The 6 primary vertices requirement didn't kill my efficiency." << endl;

   primary_vertex = HepPoint3D(primary_vertex.x() / double(vertices_in_average),
			       primary_vertex.y() / double(vertices_in_average),
			       primary_vertex.z() / double(vertices_in_average));
   if ( (primary_vertex - beamSpot).mag() > 0.005 ) return ActionBase::kFailed;
   report(INFO, kFacilityString) << "The non-beamwall requirement didn't kill my efficiency." << endl;

   if ( kshort_momentum.dot(secondary_vertex - primary_vertex) < 0.98 ) return ActionBase::kFailed;
   report(INFO, kFacilityString) << "The kshort momentum/flight vector alignment didn't kill my efficiency." << endl;

   vector<HepPoint3D>::const_iterator sub_vertex_end = sub_vertex.end();
   for ( vector<HepPoint3D>::const_iterator sub_iter = sub_vertex.begin();
	 sub_iter != sub_vertex_end;
	 ++sub_iter )
      if ( (*sub_iter - primary_vertex).mag() > 0.0003 ) return ActionBase::kFailed;
   report(INFO, kFacilityString) << "The requirement that the primary vertices be very close together"
				 << " didn't kill my efficiency." << endl;

   // Find the prompt track with the same charge as the kaon
   for (track_iter = tracks.begin();  track_iter != tracks_end;  ++track_iter)
      if ( track_iter != kaon        &&
	   track_iter != secondary1  &&  track_iter != secondary2  &&
	   track_iter->pionHelix()->curvature() * kaon->kaonHelix()->curvature() > 0. )
	 sameSignAsKaon = track_iter;

   // Now just identify the other two
   for (track_iter = tracks.begin();  track_iter != tracks_end;  ++track_iter)
      if ( track_iter != kaon  &&
	   track_iter != secondary1  &&  track_iter != secondary2  &&
	   track_iter != sameSignAsKaon ) {
	 if (heads == tracks_end) heads = track_iter;
	 else tails = track_iter;
      }
   
   FATable<NavShower> showers;
   extract(iFrame.record(Stream::kEvent), showers);
   FATable<NavShower>::const_iterator showers_end = showers.end();
   
   double pi0senergy(0.);
   Hep3Vector pi0smomentum(0., 0., 0.);
   int numpi0showers(0);
   for ( FATable<NavShower>::const_iterator shower_iter = showers.begin();
	 shower_iter != showers_end;
	 ++shower_iter )
      if ( shower_iter->attributes().status()                                      &&
	   shower_iter->attributes().energy() > 0.01 * beamEnergy->value()         &&
	   shower_iter->noTrackMatch()                                             &&
	   shower_iter->noTrackMatchConReg()                                       &&
	   ! shower_iter->bremTrack().valid()                                         ) {
// 	   ( ! shower_iter->bremTrack().valid()  ||
// 	     ( shower_iter->bremTrack()->trackShowerMatch().valid()         &&
// 	       shower_iter->bremTrack()->trackShowerMatch()->eOverP() < 0.5    ) )    )
	 pi0senergy += shower_iter->attributes().energy();
	 pi0smomentum += shower_iter->attributes().momentum();
	 numpi0showers++;
      }
   report(INFO, kFacilityString) << "Exploratory: number of pi0 showers: " << numpi0showers << endl;

   report(INFO, kFacilityString) << "Exploratory: total energy: "
				 << (pi0senergy + kshort_energy + 
				     kaon->kaonFit()->energy() + sameSignAsKaon->pionFit()->energy() +
				     heads->pionFit()->energy() + tails->pionFit()->energy()) << endl;

   report(INFO, kFacilityString) << "Exploratory: total momentum: "
				 << (pi0smomentum + kshort_momentum + 
				     kaon->kaonFit()->momentum() + sameSignAsKaon->pionFit()->momentum() +
				     heads->pionFit()->momentum() + tails->pionFit()->momentum()) << endl;

   double headsD0tagenergy(kaon->kaonFit()->energy() + heads->pionFit()->energy());
   Hep3Vector headsD0tagmomentum(kaon->kaonFit()->momentum() + heads->pionFit()->momentum());
   double headsD0tagmass(sqrt(sqr(headsD0tagenergy) + headsD0tagmomentum.mag2()));
   double headsD0tagmassBC(sqrt(sqr(beamEnergy->value()) + headsD0tagmomentum.mag2()));

   double tailsD0tagenergy(kaon->kaonFit()->energy() + tails->pionFit()->energy());
   Hep3Vector tailsD0tagmomentum(kaon->kaonFit()->momentum() + tails->pionFit()->momentum());
   double tailsD0tagmass(sqrt(sqr(tailsD0tagenergy) + tailsD0tagmomentum.mag2()));
   double tailsD0tagmassBC(sqrt(sqr(beamEnergy->value()) + tailsD0tagmomentum.mag2()));

   DABoolean bestIsHeads(abs(headsD0tagmass - M_D0MASS) < abs(tailsD0tagmass - M_D0MASS));

   report(INFO, kFacilityString)
      << "D0MASS " << (bestIsHeads ? headsD0tagmass : tailsD0tagmass)
      << " " << (bestIsHeads ? headsD0tagmassBC : tailsD0tagmassBC)
      << " " << (bestIsHeads ? headsD0tagenergy : tailsD0tagenergy)
      << " " << (bestIsHeads ? headsD0tagmomentum : tailsD0tagmomentum)
      << " " << (!bestIsHeads ? headsD0tagmass : tailsD0tagmass)
      << " " << (!bestIsHeads ? headsD0tagmassBC : tailsD0tagmassBC)
      << " " << (!bestIsHeads ? headsD0tagenergy : tailsD0tagenergy)
      << " " << (!bestIsHeads ? headsD0tagmomentum : tailsD0tagmomentum)
      << " " << kaon->kaonFit()->energy()
      << " " << kaon->kaonFit()->momentum()
      << " " << (bestIsHeads ? heads->pionFit()->energy() : tails->pionFit()->energy())
      << " " << sameSignAsKaon->pionFit()->energy()
      << " " << sameSignAsKaon->pionFit()->momentum()
      << " " << (bestIsHeads ? tails->pionFit()->energy() : heads->pionFit()->energy())
      << " " << secondary1->pionFit()->energy()
      << " " << secondary1->pionFit()->momentum()
      << " " << secondary2->pionFit()->energy()
      << " " << secondary2->pionFit()->momentum()
      << " " << kshort_mass
      << " " << primary_vertex
      << " " << secondary_vertex
      << endl;

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
Sneaky::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
Sneaky::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   return ActionBase::kPassed;
}
*/

DABoolean Sneaky::calc_intersection( HepPoint3D& intersection,
				     double& zsep,
				     double& aphi, double& bphi,
				     const KTHelix* a, const KTHelix* b )
{
   double x, y, z;
   x = y = z = zsep = aphi = bphi = 1000.;  // To more easily find unchecked failures

   // Express the two helices in terms of (x - xa)^2 + (y - ya)^2 == ra^2
   double ra( 1./2./a->curvature() );
   double xa( -(a->d0() + ra) * sin(a->phi0()) );
   double ya( (a->d0() + ra) * cos(a->phi0()) );

   double rb( 1./2./b->curvature() );
   double xb( -(b->d0() + rb) * sin(b->phi0()) );
   double yb( (b->d0() + rb) * cos(b->phi0()) );

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
   double az( a->cotTheta() * aarclength + a->z0() );
   double bz( b->cotTheta() * barclength + b->z0() );

   z = az;
   zsep = bz - az;

   intersection = HepPoint3D(x, y, z);

   aphi = a->phi0() + abs(aarclength) * 2. * a->curvature();
   bphi = b->phi0() + abs(barclength) * 2. * b->curvature();

   return true;
}

DABoolean Sneaky::arclength_test( double x, double y, const KTHelix* a, const KTHelix* b,
				  double& aarclength, double& barclength )
{
   const double tailCut = 0.005;

   double asinphi( 2. * a->curvature() * (x * cos(a->phi0()) + y * sin(a->phi0())) );
   double acosphi( 2. * a->curvature() * (x * sin(a->phi0()) - y * cos(a->phi0())
					  + a->d0()) + 1. );
   aarclength = atan2(asinphi, acosphi) / 2. / a->curvature();

   double bsinphi( 2. * b->curvature() * (x * cos(b->phi0()) + y * sin(b->phi0())) );
   double bcosphi( 2. * b->curvature() * (x * sin(b->phi0()) - y * cos(b->phi0())
					  + b->d0()) + 1. );
   barclength = atan2(bsinphi, bcosphi) / 2. / b->curvature();

   return ( aarclength > -tailCut  &&  aarclength < M_PI/2./abs(a->curvature()) )  &&
      ( barclength > -tailCut  &&  barclength < M_PI/2./abs(b->curvature()) );
}

Hep3Vector Sneaky::rotate_momentum(Hep3Vector momentum, double angle)
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
