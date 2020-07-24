#if !defined(TRACKFITTER_TRACKFITCATHODEHITLINK_H)
#define TRACKFITTER_TRACKFITCATHODEHITLINK_H
// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      TrackFitCathodeHitLink
// 
/**\class TrackFitCathodeHitLink TrackFitCathodeHitLink.h TrackFitter/TrackFitCathodeHitLink.h

 Description: Link data for fit track \<--\> Cathode hit Lattices, and typedefs.

 Usage:
    This defines the link data between fit tracks and cathode hits.

    There are also several typedefs:
\code
       PionFitCathodeHitLattice
       KaonFitCathodeHitLattice
       ElectronFitCathodeHitLattice
       MuonFitCathodeHitLattice
       ProtonFitCathodeHitLattice
       ExitElectronFitCathodeHitLattice
       ExitMuonFitCathodeHitLattice
       ExitPionFitCathodeHitLattice
       ExitKaonFitCathodeHitLattice
       ExitProtonFitCathodeHitLattice
\endcode
    These are what the user should refer to, since the link data type
    is included in these.

    To construct a Lattice, the appropriate syntax is:
\code
       PionFitCathodeHitLattice( left_table,
                                 right_table,
                                 PionFitCathodeHitLattice::LNodeMulti,
                                 PionFitCathodeHitLattice::RNodeMulti );
\endcode
    This will set up the topology such that each track can have multiple
    hits, and each hit can be on multiple tracks.  Each hit/track
    combination only has one link, however.

*/
//
// Author:      Werner Sun
// Created:     Mon Sep  6 14:34:19 EDT 1999
// $Id: TrackFitCathodeHitLink.h,v 1.5 2002/06/12 20:38:12 cleo3 Exp $
//
// Revision history
//
// $Log: TrackFitCathodeHitLink.h,v $
// Revision 1.5  2002/06/12 20:38:12  cleo3
// inlined the entire class and let the compiler write what it can
//
// Revision 1.4  2000/08/16 19:33:51  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.3  2000/02/24 17:34:08  wsun
// Updated for storage helpers.
//
// Revision 1.2  1999/10/07 18:00:40  marsh
// Added doxygen-style comments.
//
// Revision 1.1  1999/09/10 20:23:22  wsun
// First submission.
//
//

// system include files

// user include files
#include "TrackFitter/TrackFitHitLink.h"
#include "TrackRoot/TRHelixCandidateFit.h"
#include "CleoDB/DBCandidate.h"
#include "CalibratedData/CalibratedCathodeHit.h"
#include "Lattice/Lattice.h"

// forward declarations

class TrackFitCathodeHitLink : public TrackFitHitLink
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      TrackFitCathodeHitLink( DABoolean used,
			      double residual,
			      double residualError,
//			      double curvature,
			      double momentum ):
	 TrackFitHitLink( used, residual, residualError, momentum )
      {
      }

      TrackFitCathodeHitLink() :
	 TrackFitHitLink( false, 0, 1E10, 0 )
      {
      }


      //Allow the compiler to write these
      //TrackFitCathodeHitLink( const TrackFitCathodeHitLink& );
      // const TrackFitCathodeHitLink& operator=( const TrackFitCathodeHitLink& );
      //virtual ~TrackFitCathodeHitLink();

      // member functions

      // const member functions

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor (stop defaults) -- default ok

      // assignment operator(s)

      // private member functions

      // private const member functions

      // data members

      // static data members

};

typedef Lattice< TRHelixCandidateFit< DBCandidate::kChargedPion > ,
                 CalibratedCathodeHit ,
                 TrackFitCathodeHitLink > PionFitCathodeHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kChargedKaon > ,
                 CalibratedCathodeHit ,
                 TrackFitCathodeHitLink > KaonFitCathodeHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kElectron > ,
                 CalibratedCathodeHit ,
                 TrackFitCathodeHitLink > ElectronFitCathodeHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kMuon > ,
                 CalibratedCathodeHit ,
                 TrackFitCathodeHitLink > MuonFitCathodeHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kProton > ,
                 CalibratedCathodeHit ,
                 TrackFitCathodeHitLink > ProtonFitCathodeHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitElectron > ,
                 CalibratedCathodeHit ,
                 TrackFitCathodeHitLink > ExitElectronFitCathodeHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitMuon > ,
                 CalibratedCathodeHit ,
                 TrackFitCathodeHitLink > ExitMuonFitCathodeHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitPion > ,
                 CalibratedCathodeHit ,
                 TrackFitCathodeHitLink > ExitPionFitCathodeHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitKaon > ,
                 CalibratedCathodeHit ,
                 TrackFitCathodeHitLink > ExitKaonFitCathodeHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitProton > ,
                 CalibratedCathodeHit ,
                 TrackFitCathodeHitLink > ExitProtonFitCathodeHitLattice ;

// inline function definitions

#endif /* TRACKFITTER_TRACKFITCATHODEHITLINK_H */
