// -*- C++ -*-
//
// Package:     FitterDiagnosticsProc
// Module:      FitterDiagnosticsProc
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Wed Apr 21 16:57:07 EDT 1999
// $Id FitterDiagnosticsProc.cc,v 1.1.1.1 1999/06/07 21:26:37 wsun Exp $
//
// Revision history
//
// $Id FitterDiagnosticsProc.cc,v 1.1.1.1 1999/06/07 21:26:37 wsun Exp $
//

#include "Experiment/Experiment.h"

extern "C" float prob_(const float&, const int&);

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "FitterDiagnosticsProc/FitterDiagnosticsProc.h"
#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "Navigation/NavTrack.h"
#include "FastMCTaggerProc/TrueTrackSeedTrackLattice.h"
#include "CleoDB/DBEventHeader.h"
#include "TrackFitter/TrackFitCathodeHitLink.h"

// #include "TrackRoot/TRTrackCandidateFitQuality.h"
// #include "CleoDB/DBCandidate.h"

#include "HelixIntersection/HIMagField.h"

// #include "TrackRoot/TRHelixCandidateFit.h"
// #include "TrackRoot/TRHelixMuonFit.h"
// #include "TrackRoot/TRHelixExitMuonFit.h"

#include "KalmanFilter/KalmanPionQuality.h"

//#include "TrackRoot/TRHelixElectronFit.h"

#include "DAException/DANoDataException.h"

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
static const char* const kFacilityString = "FitterDiagnosticsProc" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: FitterDiagnosticsProc.cc,v 1.7 2002/02/25 21:52:09 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
FitterDiagnosticsProc::FitterDiagnosticsProc( void )               // anal1
   : Processor( "FitterDiagnosticsProc" ),
     m_fitHypo( "FitHypo", this, "pion" ),
     m_beamSpotX( "BeamSpotX", this, 0. ),
     m_beamSpotY( "BeamSpotY", this, 0. )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &FitterDiagnosticsProc::event,    Stream::kEvent );
   bind( &FitterDiagnosticsProc::startRun, Stream::kStartRun );
   //bind( &FitterDiagnosticsProc::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

// FitterDiagnosticsProc::FitterDiagnosticsProc( const FitterDiagnosticsProc& iFitterDiagnosticsProc )
// {
//    *this = iFitterDiagnosticsProc;
// }

FitterDiagnosticsProc::~FitterDiagnosticsProc()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const FitterDiagnosticsProc& FitterDiagnosticsProc::operator=( const FitterDiagnosticsProc& iFitterDiagnosticsProc )
// {
//   if( this != &iFitterDiagnosticsProc ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iFitterDiagnosticsProc );
//   }
//
//   return *this;
// }

//
// member functions
//

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
FitterDiagnosticsProc::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
FitterDiagnosticsProc::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
FitterDiagnosticsProc::hist_book( TBHistoManager& aHistManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   const CharList ntupleFieldLabels[ kTracksNtupleSize ] =
   { "resid1", "resid2", "resid3", "resid4",
	"resid5", "resid6", "resid7", "resid8",
	"resid9", "resid10", "resid11", "resid12",
	"resid13", "resid14", "resid15", "resid16",
	"resid17", "resid18", "resid19", "resid20",
	"resid21", "resid22", "resid23", "resid24",
	"resid25", "resid26", "resid27", "resid28",
	"resid29", "resid30", "resid31", "resid32",
	"resid33", "resid34", "resid35", "resid36",
	"resid37", "resid38", "resid39", "resid40",
	"resid41", "resid42", "resid43", "resid44",
	"resid45", "resid46", "resid47",
	"reserr1", "reserr2", "reserr3", "reserr4",
	"reserr5", "reserr6", "reserr7", "reserr8",
	"reserr9", "reserr10", "reserr11", "reserr12",
	"reserr13", "reserr14", "reserr15", "reserr16",
	"reserr17", "reserr18", "reserr19", "reserr20",
	"reserr21", "reserr22", "reserr23", "reserr24",
	"reserr25", "reserr26", "reserr27", "reserr28",
	"reserr29", "reserr30", "reserr31", "reserr32",
	"reserr33", "reserr34", "reserr35", "reserr36",
	"reserr37", "reserr38", "reserr39", "reserr40",
	"reserr41", "reserr42", "reserr43", "reserr44",
	"reserr45", "reserr46", "reserr47",
	"drift1", "drift2", "drift3", "drift4",
	"drift5", "drift6", "drift7", "drift8",
	"drift9", "drift10", "drift11", "drift12",
	"drift13", "drift14", "drift15", "drift16",
	"drift17", "drift18", "drift19", "drift20",
	"drift21", "drift22", "drift23", "drift24",
	"drift25", "drift26", "drift27", "drift28",
	"drift29", "drift30", "drift31", "drift32",
	"drift33", "drift34", "drift35", "drift36",
	"drift37", "drift38", "drift39", "drift40",
	"drift41", "drift42", "drift43", "drift44",
	"drift45", "drift46", "drift47",
	"svr1", "svr2", "svr3", "svr4",
	"svrerr1", "svrerr2", "svrerr3", "svrerr4",
	"svz1", "svz2", "svz3", "svz4",
	"svzerr1", "svzerr2", "svzerr3", "svzerr4",
	"cathode", "catherr",
	"momtrue", "curvtrue", "phi0true", "d0true", "cotttrue", "z0true",
	"costtrue", "nhittrue",
	"momfit", "curvfit", "phi0fit", "d0fit", "cottfit", "z0fit", "costfit",
	"momerr", "curverr", "phi0err", "d0err", "cotterr", "z0err",
	"chisq", "ndof", "cl", "track", "event", "seedsev", "de", "arc" } ;

   m_fittedTrackNtuple = aHistManager.ntuple( 1,
					      "Fitted Tracks",
					      kTracksNtupleSize,
					      200000,
					      ntupleFieldLabels ) ;

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
FitterDiagnosticsProc::event( Frame& aFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   // Find the correct fit to extract.
   DBCandidate::Hypo fitHypo ;

   if( m_fitHypo.value() == "electron" )
   {
      fitHypo = DBCandidate::kElectron ;
   }
   else if( m_fitHypo.value() == "muon" )
   {
      fitHypo = DBCandidate::kMuon ;
   }
   else if( m_fitHypo.value() == "pion" )
   {
      fitHypo = DBCandidate::kChargedPion ;
   }
   else if( m_fitHypo.value() == "kaon" )
   {
      fitHypo = DBCandidate::kChargedKaon ;
   }
   else if( m_fitHypo.value() == "proton" )
   {
      fitHypo = DBCandidate::kProton ;
   }
   else if( m_fitHypo.value() == "exitElectron" )
   {
      fitHypo = DBCandidate::kExitElectron ;
   }
   else if( m_fitHypo.value() == "exitMuon" )
   {
      fitHypo = DBCandidate::kExitMuon ;
   }
   else if( m_fitHypo.value() == "exitPion" )
   {
      fitHypo = DBCandidate::kExitPion ;
   }
   else if( m_fitHypo.value() == "exitKaon" )
   {
      fitHypo = DBCandidate::kExitKaon ;
   }
   else if( m_fitHypo.value() == "exitProton" )
   {
      fitHypo = DBCandidate::kExitProton ;
   }
   else
   {
      report( ERROR, kFacilityString ) << "Unknown fit hypo given!" << endl ;
      return ActionBase::kFailed ;
   }


   // Extract fitted tracks.
   FATable< NavTrack > navTracks;
   extract( aFrame.record( Stream::kEvent ), navTracks );

   // Extract FATable of FMCTrueTracks and Lattice connecting TRSeedTracks.
   FATable< FMCTrueTrack > trueTracks ;
   try
   {
      extract( aFrame.record( Stream::kEvent ), trueTracks ) ;
   }
   catch( NO_TABLE_EXCEPTION( FMCTrueTrack )& aException )
   {
      try
      {
	 extract( aFrame.record( Stream::kEvent ), trueTracks,
		  "FastMCTaggerProc" ) ;
      }
      catch( NO_TABLE_EXCEPTION( FMCTrueTrack )& aException )
      {
//	 report( WARNING, kFacilityString ) << aException.what() << endl ;
      }
   }

   FAItem< TrueTrackSeedTrackLattice > trueSeedLattice ;
   try
   {
      extract( aFrame.record( Stream::kEvent ), trueSeedLattice ) ;
   }
   catch( NO_ITEM_EXCEPTION( TrueTrackSeedTrackLattice )& aException )
   {
      report( WARNING, kFacilityString ) << aException.what() << endl ;
   }

   if( !navTracks.valid() )
   {
      return ActionBase::kFailed;
   }

   // Extract Fit-Cathode hit lattice.  Use pion until NavTrack supports
   // cathodes.
   //   FAItem< MuonFitCathodeHitLattice > cathodeLattice ;
//   FAItem< PionFitCathodeHitLattice > cathodeLattice ;
   FAItem< ElectronFitCathodeHitLattice > cathodeLattice ;
   extract( aFrame.record( Stream::kEvent ), cathodeLattice ) ;
   

   float tmp[ kTracksNtupleSize ] ;

   int trackCounter = 0 ;

   // Event number.
   FAItem< DBEventHeader > eventHeader ;
   extract( aFrame.record( Stream::kEvent ), eventHeader ) ;

   // Loop over NavTracks
   FATableItr< NavTrack > navItr = navTracks.begin() ;
   FATableItr< NavTrack > endNav = navTracks.end() ;

   for( ; navItr != endNav ; ++navItr )
   {
      for( int j = 0 ; j < kTracksNtupleSize ; ++j )
      {
	 tmp[ j ] = -999. ;
      }

      // # of seed tracks in event.
      tmp[ kSeedsEv ] = navTracks.size() ;

      // If MC info is available, find the corresponding FMCTrueTrack
      // and fill ntuple.
      HepPoint3D trueRefPt( m_beamSpotX.value(),
			    m_beamSpotY.value(),
			    0. ) ;

      if( trueSeedLattice.valid() )
      {
	 const TrueTrackSeedTrackLattice::LeftID* trueID = 0 ;

	 FAItem< TRSeedTrack > seed = navItr->seedTrack() ;
	 if( seed.valid() )
	 {
	    trueID = trueSeedLattice->leftIDGivenRight( seed->identifier() );
	 }

	 if( trueID != 0 && trueTracks.valid() )
	 {
	    const FMCTrueTrack& trueTrack = trueTracks[ *trueID ] ;

	    const KTHelix* tmpHelix ;
	    if( DBCandidate::hypoIsInward( fitHypo ) )
	    {
	       tmpHelix = &trueTrack ;
	    }
	    else
	    {
	       tmpHelix = &trueTrack.exitHelix() ;
	    }
	    const KTHelix& trueHelix = *tmpHelix ;

	    trueRefPt = trueHelix.referencePoint() ;

	    tmp[ kMomTrue ]   = trueHelix.momentum(
	       HIMagField::instance()->bfield( trueHelix.position() ) ).mag() ;
	    tmp[ kCurvTrue ]  = trueHelix.curvature() ;
	    tmp[ kPhi0True ]  = trueHelix.phi0() ;
	    tmp[ kD0True ]    = trueHelix.d0() ;
	    tmp[ kCotthTrue ] = trueHelix.cotTheta() ;
	    tmp[ kZ0True ]    = trueHelix.z0() ;
	    tmp[ kCosthTrue ] =
	       tmp[ kCotthTrue ] / sqrt( 1. + sqr( tmp[ kCotthTrue ] ) ) ;
	    tmp[ kNhitTrue ]  = trueTrack.numberOfHitsGenerated() ;
	 }
      }

      cout << "BFIELD "
	   << HIMagField::instance()->bfield( HepPoint3D(0.,0.,0.) )
	   << endl;


      // Get the fitted helix.
      Meters fittedHelixArc = 0. ;

      FAItem< TRHelixFit > fittedTrack = navItr->helixFit( fitHypo ) ;
      if( !fittedTrack.valid() )
      {
	 continue ;
      }

      // Swim the inward fit to the right reference point.
      KTHelix fittedHelix( fittedTrack->curvature(),
			   fittedTrack->phi0(),
			   fittedTrack->d0(),
			   fittedTrack->cotTheta(),
			   fittedTrack->z0(),
			   fittedTrack->errorMatrix(),
			   fittedTrack->referencePoint() ) ;

      if( fittedHelix.moveToReferencePoint( trueRefPt, fittedHelixArc ) !=
	  KTMoveControl::kMoveOK )
      {
	 continue ;
      }

      tmp[ kMomFit ]   = fittedHelix.momentum(
	 HIMagField::instance()->bfield( fittedHelix.position() ) ).mag() ;
      tmp[ kCurvFit ]  = fittedHelix.curvature() ;
      tmp[ kPhi0Fit ]  = fittedHelix.phi0() ;
      tmp[ kD0Fit ]    = fittedHelix.d0() ;
      tmp[ kCotthFit ] = fittedHelix.cotTheta() ;
      tmp[ kZ0Fit ]    = fittedHelix.z0() ;
      tmp[ kCosthFit ] =
	 tmp[ kCotthFit ] / sqrt( 1. + sqr( tmp[ kCotthFit ] ) ) ;

      // Inward errors.
      const HepSymMatrix& err = fittedHelix.errorMatrix() ;

      double tmperr = err( KTHelix::kCurvature, KTHelix::kCurvature ) ;
      tmp[ kCurvErr ] = tmperr > 0. ? sqrt( tmperr ) : -99. ;

      tmperr = err( KTHelix::kPhi0, KTHelix::kPhi0 ) ;
      tmp[ kPhi0Err ] = tmperr > 0. ? sqrt( tmperr ) : -99. ;

       tmperr = err( KTHelix::kD0, KTHelix::kD0 ) ;
      tmp[ kD0Err ]   = tmperr > 0. ? sqrt( tmperr ) : -99. ;

      tmperr = err( KTHelix::kCotTheta, KTHelix::kCotTheta ) ;
      tmp[ kCotthErr ] = tmperr > 0. ? sqrt( tmperr ) : -99. ;

      tmperr = err( KTHelix::kZ0, KTHelix::kZ0 ) ;
      tmp[ kZ0Err ]   = tmperr > 0. ? sqrt( tmperr ) : -99. ;

      double K = tmp[ kCurvFit ] ;
      double dK = tmp[ kCurvErr ] ;
      double t = tmp[ kCotthFit ] ;
      double dt = tmp[ kCotthErr ] ;
      double dKdt = err( KTHelix::kCurvature, KTHelix::kCotTheta ) ;
      double sin2th = 1. / ( 1. + t * t ) ;

      if( K != 0. )
      {
	 tmp[ kMomErr ]  = tmp[ kMomFit ] *
	    sqrt( sqr( dK/K ) -
		  2. * sin2th * t * dKdt / K +
		  sqr( t * sin2th * dt ) ) ;
      }
      else
      {
	 tmp[ kMomErr ] = -99. ;
      }

      FAItem< TRTrackFitQuality > quality = navItr->quality( fitHypo ) ;
      if( quality.valid() && !quality->fitAbort() ) 
      {
	 tmp[ kChisq ]   = quality->chiSquare() ;
	 tmp[ kNdof ]    = quality->degreesOfFreedom() ;

	 if( tmp[ kNdof ] > 0 )
	 {
	    tmp[ kCL ]      = prob_( float( tmp[ kChisq ] ),
				     int(   tmp[ kNdof ] ) ) ;
	 }

 	 tmp[ kDeltaE ] = ( (KalmanPionQuality*) &*quality)->energyLoss();
 	 tmp[ kArcSwum ] = ( (KalmanPionQuality*) &*quality )->arcSwum();
      }

      tmp[ kTrack ] = ++trackCounter ;

      if( eventHeader.valid() )
      {
	 tmp[ kEvent ] = eventHeader->number() ;
      }

      cout << "Tr " << quality->identifier() << " true params: "
	   << tmp[ kCurvTrue ] << " "
	   << tmp[ kPhi0True ] << " "
	   << tmp[ kD0True ] << " "
	   << tmp[ kCotthTrue ] << " "
	   << tmp[ kZ0True ] << " / "
	   << tmp[ kMomTrue ] << "\n"
	   << "fit params:  "
	   << tmp[ kCurvFit ] << " "
	   << tmp[ kPhi0Fit ] << " "
	   << tmp[ kD0Fit ] << " "
	   << tmp[ kCotthFit ] << " "
	   << tmp[ kZ0Fit ] << " / "
	   << tmp[ kMomFit ] << "\n"
	   << "err: "
	   << tmp[ kCurvErr ] << " "
	   << tmp[ kPhi0Err ] << " "
	   << tmp[ kD0Err ] << " "
	   << tmp[ kCotthErr ] << " "
	   << tmp[ kZ0Err ] << " / "
	   << tmp[ kMomErr ] << "\n"
	   << "chisq " << tmp[ kChisq ]
	   << " ndof " << tmp[ kNdof ]
	   << " ref pt " << trueRefPt
	   << " dist " << fittedHelixArc
	   << " input/discarded " << quality->numberHitsInput()
	   << "/" << quality->numberHitsDropped()
	   << endl ;

//       cout << quality->numberSubdetectorHits(
// 	 TRSubdetectorLists::kEntireDetector ) << "/"
// 	   << quality->averageResidual(
// 	      TRSubdetectorLists::kEntireDetector ) << "/"
// 	   << quality->rmsResidual(
// 	      TRSubdetectorLists::kEntireDetector )
// 	   << " " << quality->numberSubdetectorHits(
// 	      TRSubdetectorLists::kDRAxial ) << "/"
// 	   << quality->averageResidual(
// 	      TRSubdetectorLists::kDRAxial ) << "/"
// 	   << quality->rmsResidual(
// 	      TRSubdetectorLists::kDRAxial )
// 	   << " " << quality->numberSubdetectorHits(
// 	      TRSubdetectorLists::kDRStereo ) << "/"
// 	   << quality->averageResidual(
// 	      TRSubdetectorLists::kDRStereo ) << "/"
// 	   << quality->rmsResidual(
// 	      TRSubdetectorLists::kDRStereo )
// 	   << " " << quality->numberSubdetectorHits(
// 	      TRSubdetectorLists::kDRCathode ) << "/"
// 	   << quality->averageResidual(
// 	      TRSubdetectorLists::kDRCathode ) << "/"
// 	   << quality->rmsResidual(
// 	      TRSubdetectorLists::kDRCathode )
// 	   << " " << quality->numberSubdetectorHits(
// 	      TRSubdetectorLists::kSVR ) << "/"
// 	   << quality->averageResidual(
// 	      TRSubdetectorLists::kSVR ) << "/"
// 	   << quality->rmsResidual(
// 	      TRSubdetectorLists::kSVR )
// 	   << " " << quality->numberSubdetectorHits(
// 	      TRSubdetectorLists::kSVZ ) << "/"
// 	   << quality->averageResidual(
// 	      TRSubdetectorLists::kSVZ ) << "/"
// 	   << quality->rmsResidual(
// 	      TRSubdetectorLists::kSVZ ) << endl ;

      // DR hit info
      const NavTrack::DRHitTable* hitsDR( navItr->drHits( fitHypo ) ) ;
      const NavTrack::DRHitLinkTable* drHitLinks =
	 navItr->drHitLinks( fitHypo ) ;

      if( hitsDR != 0 && drHitLinks != 0 )
      {
	 NavTrack::DRHitTable::const_iterator drHitItr = hitsDR->begin() ;
	 NavTrack::DRHitTable::const_iterator endDRHit = hitsDR->end() ;

	 for( ; drHitItr != endDRHit ; ++drHitItr )
	 {
	    NavTrack::DRHitLinkTable::const_iterator drLinkItr =
	       drHitLinks->begin() ;
	    NavTrack::DRHitLinkTable::const_iterator drLinkEnd =
	       drHitLinks->end() ;

	    for( ; drLinkItr != drLinkEnd ; ++drLinkItr )
	    {
	       if( ( *drLinkItr )->rightID() == 0 ||
		   *( *drLinkItr )->rightID() != ( *drHitItr )->identifier() )
	       {
		  continue ;
	       }

	       const TrackFitDRHitLink& drLinkData = (*drLinkItr)->linkData();

	       if( drLinkData.used() )
	       {
		  UInt16 layer = ( *drHitItr )->layer() ;
		  tmp[ layer - 1 ] = drLinkData.residual() ;
		  tmp[ layer - 1 + kResErr1 ] = drLinkData.residualError() ;
		  tmp[ layer - 1 + kDriftDist1 ] =
		     drLinkData.residual() + drLinkData.signedDcaToWire() ;
// 		  cout << "layer " << layer << " dca/drift error "
// 		       << drLinkData.signedDcaError() << "/"
// 		       << drLinkData.signedDriftError() << endl ;
	       }
	    }
	 }
      }


      // SVR hit info
      const NavTrack::SVRHitTable* hitsSVR( navItr->svrHits( fitHypo ) ) ;
      const NavTrack::SVRHitLinkTable* svrHitLinks =
	 navItr->svrHitLinks( fitHypo ) ;

      if( hitsSVR != 0 && svrHitLinks != 0 )
      {
	 NavTrack::SVRHitTable::const_iterator svrHitItr = hitsSVR->begin() ;
	 NavTrack::SVRHitTable::const_iterator endSVRHit = hitsSVR->end() ;

	 for( ; svrHitItr != endSVRHit ; ++svrHitItr )
	 {
	    NavTrack::SVRHitLinkTable::const_iterator svrLinkItr =
	       svrHitLinks->begin() ;
	    NavTrack::SVRHitLinkTable::const_iterator svrLinkEnd =
	       svrHitLinks->end() ;

	    for( ; svrLinkItr != svrLinkEnd ; ++svrLinkItr )
	    {
	       if( ( *svrLinkItr )->rightID() == 0 ||
		   *( *svrLinkItr )->rightID() !=
		   ( *svrHitItr )->identifier() )
	       {
		  continue ;
	       }

	       const TrackFitSVRHitLink& svrLinkData =
		  ( *svrLinkItr )->linkData();

	       if( svrLinkData.used() )
	       {
		  UInt16 layer = ( *svrHitItr )->layer() ;
		  tmp[ layer - 1 + kSVR1 ] = svrLinkData.residual() ;
		  tmp[ layer - 1 + kSVRErr1 ] = svrLinkData.residualError() ;
	       }
	    }
	 }
      }

      // SVZ hit info
      const NavTrack::SVZHitTable* hitsSVZ( navItr->svzHits( fitHypo ) ) ;
      const NavTrack::SVZHitLinkTable* svzHitLinks =
	 navItr->svzHitLinks( fitHypo ) ;

      if( hitsSVZ != 0 && svzHitLinks != 0 )
      {
	 NavTrack::SVZHitTable::const_iterator svzHitItr = hitsSVZ->begin() ;
	 NavTrack::SVZHitTable::const_iterator endSVZHit = hitsSVZ->end() ;

	 for( ; svzHitItr != endSVZHit ; ++svzHitItr )
	 {
	    NavTrack::SVZHitLinkTable::const_iterator svzLinkItr =
	       svzHitLinks->begin() ;
	    NavTrack::SVZHitLinkTable::const_iterator svzLinkEnd =
	       svzHitLinks->end() ;

	    for( ; svzLinkItr != svzLinkEnd ; ++svzLinkItr )
	    {
	       if( ( *svzLinkItr )->rightID() == 0 ||
		   *( *svzLinkItr )->rightID() !=
		   ( *svzHitItr )->identifier() )
	       {
		  continue ;
	       }

	       const TrackFitSVZHitLink& svzLinkData =
		  ( *svzLinkItr )->linkData();

	       if( svzLinkData.used() )
	       {
		  UInt16 layer = ( *svzHitItr )->layer() ;
		  tmp[ layer - 1 + kSVZ1 ] = svzLinkData.residual() ;
		  tmp[ layer - 1 + kSVZErr1 ] = svzLinkData.residualError() ;
	       }
	    }
	 }
      }

   FAItem< ElectronFitCathodeHitLattice > cathodeLattice ;
   extract( aFrame.record( Stream::kEvent ), cathodeLattice ) ;

      // Cathode hit info.
      //      const MuonFitCathodeHitLattice::Link* cathodeLink =
//      const PionFitCathodeHitLattice::Link* cathodeLink =
      const ElectronFitCathodeHitLattice::Link* cathodeLink =
	 cathodeLattice->linkGivenLeft( navItr->identifier() ) ;

      if( cathodeLink != 0 )
      {
	 const TrackFitCathodeHitLink& cathodeLinkData =
	    cathodeLink->linkData() ;
	    
	 tmp[ kCathode ] = cathodeLinkData.residual() ;
	 tmp[ kCathodeErr ] = cathodeLinkData.residualError() ;
      }

      m_fittedTrackNtuple->fill( tmp ) ;
   }

   return ActionBase::kPassed;
}

ActionBase::ActionResult
FitterDiagnosticsProc::startRun( Frame& aFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in startRun()" << endl;
   //   HIMagField::instance()->setBfield( aFrame.record( Stream::kStartRun ) ) ;
   //   cout << "BFIELD " << HIMagField::instance()->bfield( HepPoint3D(0.,0.,0.) )
   //	<< endl;

   FAItem< BeamEnergy > energy ;
   extract( aFrame.record( Stream::kStartRun ), energy ) ;
   cout << "BEAM ENERGY IS " << energy->value() << endl ;

   return ActionBase::kPassed;
}


// ActionBase::ActionResult
// FitterDiagnosticsProc::endRun( Frame& )         // anal4 equiv.
// {
//    report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

//    return ActionBase::kPassed;
// }


//
// const member functions
//

//
// static member functions
//
