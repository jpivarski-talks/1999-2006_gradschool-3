#if !defined(TRACKFITTER_TRACKFITZDHITLINK_H)
#define TRACKFITTER_TRACKFITZDHITLINK_H
// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      TrackFitZDHitLink
// 
/**\class TrackFitZDHitLink TrackFitZDHitLink.h TrackFitter/TrackFitZDHitLink.h

 Description: Link data for fit track \<--\> DR hit Lattices, and typedefs.

 Usage:
    This defines the link data between fit tracks and drift chamber hits.

    There are also several typedefs:
\code
       PionFitZDHitLattice
       KaonFitZDHitLattice
       ElectronFitZDHitLattice
       MuonFitZDHitLattice
       ProtonFitZDHitLattice
       ExitElectronFitZDHitLattice
       ExitMuonFitZDHitLattice
       ExitPionFitZDHitLattice
       ExitKaonFitZDHitLattice
       ExitProtonFitZDHitLattice
\endcode
    These are what the user should refer to, since the link data type
    is included in these.

    To construct a Lattice, the appropriate syntax is:
\code
       PionFitZDHitLattice( left_table,
                            right_table,
                            PionFitZDHitLattice::LNodeMulti,
                            PionFitZDHitLattice::RNodeMulti );
\endcode
    This will set up the topology such that each track can have multiple
    hits, and each hit can be on multiple tracks.  Each hit/track
    combination only has one link, however.

*/
//
// Author:      B. Heltsley
// Created:     Wed Nov 18 15:01:59 EST 1998
// $Id: TrackFitZDHitLink.h,v 1.1 2003/04/17 17:03:33 bkh Exp $
//
// Revision history
//
// $Log: TrackFitZDHitLink.h,v $
// Revision 1.1  2003/04/17 17:03:33  bkh
// Add ZD Lattice Object & Instantiations
//

// system include files

// user include files
#include "TrackFitter/TrackFitDRHitLink.h"
#include "CalibratedData/CalibratedZDHit.h"

// forward declarations

class TrackFitZDHitLink : public TrackFitDRHitLink
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      TrackFitZDHitLink( DABoolean iUsed,
                         double iResidual,
                         double iResidualError,
                         double iMomentum,
                         const HepPoint3D& iTrackRefPt,
			 Meters iSignedDcaToWire,
			 Meters iSignedDcaError,
			 double iSinTrackToRadial,
			 CalibratedTimeHit::PicoSec iDriftTime,
			 double iFittingWeight,
			 Meters iSignedDriftDistance ) :
	 TrackFitDRHitLink( iUsed,
			    iResidual,
			    iResidualError,
			    iMomentum,
			    iTrackRefPt,
			    iSignedDcaToWire,
			    iSignedDcaError,
			    iSinTrackToRadial,
			    iDriftTime,
			    iFittingWeight,
			    iSignedDriftDistance )
      {
      }

      TrackFitZDHitLink(): TrackFitDRHitLink()
      {
      }

      
      //Allow the compiler to write these
      //TrackFitZDHitLink( const TrackFitZDHitLink& );
      //const TrackFitZDHitLink& operator=( const TrackFitZDHitLink& );
      //virtual ~TrackFitZDHitLink();

      DABoolean operator==( const TrackFitZDHitLink& rhs ) const {
	 return TrackFitDRHitLink::operator==( rhs ) ; };
      DABoolean operator!=( const TrackFitZDHitLink& rhs ) const {
	 return TrackFitDRHitLink::operator!=( rhs ) ; };

      // member functions

      // const member functions

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor (stop defaults) -- default ok

      // private member functions

      // private const member functions

      // data members
      // static data members

};

typedef Lattice< TRHelixCandidateFit< DBCandidate::kChargedPion > ,
                 CalibratedZDHit ,
                 TrackFitZDHitLink > PionFitZDHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kChargedKaon > ,
                 CalibratedZDHit ,
                 TrackFitZDHitLink > KaonFitZDHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kElectron > ,
                 CalibratedZDHit ,
                 TrackFitZDHitLink > ElectronFitZDHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kMuon > ,
                 CalibratedZDHit ,
                 TrackFitZDHitLink > MuonFitZDHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kProton > ,
                 CalibratedZDHit ,
                 TrackFitZDHitLink > ProtonFitZDHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitElectron > ,
                 CalibratedZDHit ,
                 TrackFitZDHitLink > ExitElectronFitZDHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitMuon > ,
                 CalibratedZDHit ,
                 TrackFitZDHitLink > ExitMuonFitZDHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitPion > ,
                 CalibratedZDHit ,
                 TrackFitZDHitLink > ExitPionFitZDHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitKaon > ,
                 CalibratedZDHit ,
                 TrackFitZDHitLink > ExitKaonFitZDHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitProton > ,
                 CalibratedZDHit ,
                 TrackFitZDHitLink > ExitProtonFitZDHitLattice ;

// inline function definitions

#endif /* TRACKFITTER_TRACKFITZDHITLINK_H */
