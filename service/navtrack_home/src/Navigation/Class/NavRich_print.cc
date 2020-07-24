// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavRich
// 
// Description:
//
// Implementation:
//
// Author:      Tomasz Skwarnicki
// Created:     Tue Jul 20 10:46:38 EDT 2000
// $Id: NavRich_print.cc,v 1.5 2002/08/16 15:27:19 cleo3 Exp $
//
// Revision history
//
// $Log: NavRich_print.cc,v $
// Revision 1.5  2002/08/16 15:27:19  cleo3
// replaced iostream.h with C++/iostream.h
//
// Revision 1.4  2001/10/01 00:37:07  cdj
// switched to RichAngles package
//
// Revision 1.3  2000/11/20 00:05:54  ts
// build NavRich on top of RichTrackPrototype
//
// Revision 1.2  2000/07/27 04:42:37  ts
// fix spelling and no. of radiators
//
// Revision 1.1  2000/07/27 01:31:43  ts
// NavRich added
//
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#include "C++Std/iostream.h"
#include <iomanip.h>
#include "Experiment/cleo_math.h"
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "Navigation/NavRich.h"
#include "Navigation/NavRichServer.h"

#include "RichTrackPrototypesProd/RichTrackPrototype.h"
#include "RichAngles/RichRayAngle.h"

#include "KinematicTrajectory/KTHelix.h"


// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kReport = "Navigation.NavRich" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavRich_print.cc,v 1.5 2002/08/16 15:27:19 cleo3 Exp $";
static const char* const kTagString = "$Name:  $";
//
// static data member definitions
//


ostream& operator<<( ostream& os, const NavRich& nR ) 
{
   os << " NavRich ID=" << setw(3) << int(nR.identifier()) << " ================================ " << endl;

   if( nR.identifier() != nR.richTrack()->richTrkID() ){ os << " ERROR : inconsistent NaVRich and RichTrack identifiers " << endl; }

   FAItem<RichTrackPrototype> richTrackPrototype( nR.richTrackPrototype() );
   if( richTrackPrototype.valid() ){
      os << *richTrackPrototype << endl;
   } 
        
   RichHypothesisIDType iHyp( RichHypotheses::kPion );
   FAItem<KTHelix> helixPtr( nR.richTrajectory(iHyp) );
   if( ! helixPtr.valid() ){
      for( iHyp=0; iHyp<RichHypotheses::kHypMax; ++iHyp){
	 if( nR.richTrajectory(iHyp).valid() ){
               helixPtr = nR.richTrajectory(iHyp);
               break;
	 }
      }
   }
   if( !helixPtr.valid() ){  
      os << " None of hypotheses has trajectory " << endl;
      return os;
   }           
   const KTHelix& helix = *helixPtr;
   os << " Trajectory for " << RichHypotheses::kParticleName[iHyp] << " hypothesis " << endl;
   os << "   curvature=" << setw(8) << setprecision(4) << helix.curvature() << endl;
   os << "   phi0=" << setw(8) << setprecision(4) << helix.phi0() << endl; 
   os << "   d0=" << setw(8) << setprecision(4) << helix.d0() << endl; 
   os << "   cotTh=" << setw(8) << setprecision(4) << helix.cotTheta() << endl; 
   os << "   z0=" << setw(8) << setprecision(4) << helix.z0() << endl; 
   os << "   refPoint=" << helix.referencePoint() << endl; 

   if( true == nR.trajectoryWasImproved(iHyp) ){
 
   const KTHelix& helix = *(nR.richImprovedTrajectory(iHyp));
   os << " Trajectory was improved to: " << endl;
   os << "   curvature=" << setw(8) << setprecision(4) << helix.curvature() << endl;
   os << "   phi0=" << setw(8) << setprecision(4) << helix.phi0() << endl; 
   os << "   d0=" << setw(8) << setprecision(4) << helix.d0() << endl; 
   os << "   cotTh=" << setw(8) << setprecision(4) << helix.cotTheta() << endl; 
   os << "   z0=" << setw(8) << setprecision(4) << helix.z0() << endl; 
   os << "   refPoint=" << helix.referencePoint() << endl; 

   } else {

   os << " Trajectory was not improved." << endl;
       
   }

   os << " Track ID= " << int(nR.trackId());
   if( nR.navTrack().valid() ) { os << " NavTrack ID= " << nR.navTrack()->identifier(); } else { os << " NavTrack link invalid "; }
   os << endl;

   if( !nR.richTrackAveAngles().valid() ){ os << " WARNING: richTrackAngles is invalid " << endl; }
  

   os << " Most likely hypothesis " <<  RichHypotheses::kParticleName[nR.hypWithMaxLikelihood()] << endl;
   
   os << " Hypothesis :    " ;
   if(  nR.electronHypWasAnalyzed() ){
   os << "  Electron   " ;
   } else {
   os << " (Electron)  " ;
   }
   if(  nR.muonHypWasAnalyzed() ){
   os << "  Muon       " ;
   } else {
   os << " (Muon)      " ;
   }
   if(  nR.pionHypWasAnalyzed() ){
   os << "  Pion       " ;
   } else {
   os << " (Pion)      " ;
   }
   if(  nR.pionHypWasAnalyzed() ){
   os << "  Kaon       " ;
   } else {
   os << " (Kaon)      " ;
   }
   if(  nR.protonHypWasAnalyzed() ){
   os << "  Proton     " ;
   } else {
   os << " (Proton)    " ;
   }
//   for( iHyp=0; iHyp<RichHypotheses::kHypMax; ++iHyp){ os << "  " << RichHypotheses::kParticleName[iHyp]; } 
   os << endl;
  
   os << " LogLikelihood : " ;
   for( iHyp=0; iHyp<RichHypotheses::kHypMax; ++iHyp){ 
      os << " "  << setw(12) << setprecision(5) << nR.hypLogLikelihood(iHyp); }
   os << endl;   


   os << " No. of photons :" ;
   for( iHyp=0; iHyp<RichHypotheses::kHypMax; ++iHyp){ 
      os << " "  << setw(12) << nR.hypNumberOfPhotonsInAverage(iHyp); }
   os << endl;   

   os << " Std Deviation : " ;
   for( iHyp=0; iHyp<RichHypotheses::kHypMax; ++iHyp){ 
      os << " "  << setw(12) << setprecision(3) << nR.hypAverageAngleStandardDeviation(iHyp); }
   os << endl;   

   os << " Total Error mr: " ;
   for( iHyp=0; iHyp<RichHypotheses::kHypMax; ++iHyp){ 
      os << " "  << setw(12) << setprecision(3) << nR.hypAverageAngleTotalError(iHyp)*1000.0; }
   os << endl;   


   if( !nR.richTrackChamberCoord().valid() ){ os << " WARNING: richTrackChamberCoord is invalid " << endl; }
   if( !nR.richTrackCRegionMatch().valid() ){ os << " WARNING: richTrackCRegionMatch is invalid " << endl; }

   iHyp = nR.hypWithMaxLikelihood();

   os <<  " Matched Rich Connected Region Pulse Height (most likely hyp.) " 
       << setw(9) << setprecision(5) 
      <<  nR.matchedRichConnectedRegionPulseHeight(iHyp) << endl; 

   if( nR.matchedRichConnectedRegionPulseHeight(iHyp) > 0.0 ){

     if( !nR.richCRegionChamberCoord(iHyp).valid() ){ os << " WARNING: richCRegionChamberCoord is invalid " << endl; }

     if( !nR.richConnectedRegion(iHyp).valid() ){ os << " WARNING: richConnectedRegion " << endl; }

     os << " hit position (cm) "  << setw(8) << setprecision(4) 
        << " x " << nR.matchedRichConnectedRegionPosition(iHyp).x()*100.0
        << " y " << nR.matchedRichConnectedRegionPosition(iHyp).y()*100.0
        << " z " << nR.matchedRichConnectedRegionPosition(iHyp).z()*100.0 << endl;

     os << " +-                "  << setw(8) << setprecision(5) 
        << "   " << sqrt(nR.matchedRichConnectedRegionPositionCov(iHyp)(1,1))*100.0
        << "   " << sqrt(nR.matchedRichConnectedRegionPositionCov(iHyp)(2,2))*100.0
        << "   " << sqrt(nR.matchedRichConnectedRegionPositionCov(iHyp)(3,3))*100.0 << endl;

     os << " Delta(Z) "    << setw(8) << setprecision(3) <<  nR.trackRichHitDeltaZ(iHyp)*100.0 
        << " +- " <<  setw(8) <<setprecision(4) << nR.trackRichHitDeltaZError(iHyp)*100.0 
        << " Delta(Rphi) "     << setw(8) << setprecision(3) <<  nR.trackRichHitDeltaRphi(iHyp)*100.0 
        << " +- " <<  setw(8) << setprecision(4) << nR.trackRichHitDeltaRphiError(iHyp)*100.0 << endl;


   }

   if( !nR.richTrackRadiators().valid() ){ os << " WARNING: richTrackRadiators is invalid " << endl; }

   if( !nR.richTrajectoryRadiator(iHyp).valid() ){ os << " No radiator info for the most likely hypothesis " << endl; } 

   os << " Number of radiators intersected "  << setw(2) << int(nR.numberOfRadiators(iHyp))  
      << " sigmas to an edge "  << setw(8) << setprecision(4) <<  nR.sigmasToAnEdge(iHyp) 
      << " radiator id " << setw(3) << nR.radiatorId(iHyp) 
      << " length (cm) " << setw(7) << setprecision(3) << nR.radLength(iHyp)*100.0 << endl;


   os << " Local coordinates of track-radiator intersect        "
      << setw(8) << setprecision(3) 
      << " x " << nR.trackRadiatorIntersect(iHyp).x()*100.0
      << " y " << nR.trackRadiatorIntersect(iHyp).y()*100.0
      << " z " << nR.trackRadiatorIntersect(iHyp).z()*100.0 << endl;
   os << " Local direction cosines  of track-radiator intersect "
      << setw(8) << setprecision(4) 
      << " x " << nR.trackDirectionAtRadiatorIntersect(iHyp).x()
      << " y " << nR.trackDirectionAtRadiatorIntersect(iHyp).y()
      << " z " << nR.trackDirectionAtRadiatorIntersect(iHyp).z() << endl;


  if( !nR.richTrackPhotons().valid() ){ os << " No photon  info for the most likely hypothesis " << endl; }

  os  << setw(3) 
     << " Total number of photons linked to most likely hypothesis " << nR.numberOfPhotonsLinked(iHyp) << endl;

  os << " Number of RichPhotonAngles " << setw(3) << nR.richPhotonAnglesPtrs(iHyp).size() 
     << " Number of RichRayAngle " << setw(3) << nR.richRayAnglePtrs(iHyp).size() 
     << " Number of RichPhoton " << setw(3) << nR.richPhotonPtrs(iHyp).size() 
     << " Number of RichBump " << setw(3) << nR.richBumpPtrs(iHyp).size() 
     << " Number of RichCalibratedHit " << setw(3) << nR.richCalibratedHitPtrs(iHyp).size() << endl;

  os << " (photon)bump pulse heights : ";
  typedef NavRich::RichBumpTable RichBumpTable; 
  RichBumpTable bt = nR.richBumpPtrs(iHyp);
  RichBumpTable::const_iterator tableEnd( bt.end() );
  for( RichBumpTable::const_iterator item( bt.begin() ) ; item != tableEnd ; ++item ) {
     os << " "  << setw(9) << setprecision(5) << (*item)->pulseHeight();
  }
  os << endl;

  {
  os << " list of RichRayAngle for most likely optical path  : " << endl;
  typedef NavRich::RichRayAngleTable RichRayAngleTable;
  RichRayAngleTable rra = nR.richRayAnglePtrs(iHyp);
  RichRayAngleTable::const_iterator tableEnd( rra.end() );
  for( RichRayAngleTable::const_iterator item( rra.begin() ) ; item != tableEnd ; ++item ) {
     os << RichRayAngle(*(*item)) << endl;
  } }


  os << " ======================================================================= " << endl;

  return os ;

}

