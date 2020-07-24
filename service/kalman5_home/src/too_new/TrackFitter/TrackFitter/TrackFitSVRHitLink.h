#if !defined(TRACKFITTER_TRACKFITSVRHITLINK_H)
#define TRACKFITTER_TRACKFITSVRHITLINK_H
// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      TrackFitSVRHitLink
// 
/**\class TrackFitSVRHitLink TrackFitSVRHitLink.h TrackFitter/TrackFitSVRHitLink.h

 Description: Link data for fit track \<--\> SVR hit Lattices, and typedefs.

 Usage:
    This defines the link data between fit tracks and silicon r-phi hits.

    There are also several typedefs:
\code
       PionFitSVRHitLattice
       KaonFitSVRHitLattice
       ElectronFitSVRHitLattice
       MuonFitSVRHitLattice
       ProtonFitSVRHitLattice
       ExitElectronFitSVRHitLattice
       ExitMuonFitSVRHitLattice
       ExitPionFitSVRHitLattice
       ExitKaonFitSVRHitLattice
       ExitProtonFitSVRHitLattice
\endcode
    These are what the user should refer to, since the link data type
    is included in these.

    To construct a Lattice, the appropriate syntax is:
\code
       PionFitSVRHitLattice( left_table,
                             right_table,
                             PionFitSVRHitLattice::LNodeMulti,
                             PionFitSVRHitLattice::RNodeMulti );
\endcode
    This will set up the topology such that each track can have multiple
    hits, and each hit can be on multiple tracks.  Each hit/track
    combination only has one link, however.

*/
//
// Author:      Michael Marsh
// Created:     Wed Nov 18 15:01:59 EST 1998
// $Id: TrackFitSVRHitLink.h,v 1.10 2002/06/12 20:38:13 cleo3 Exp $
//
// Revision history
//
// $Log: TrackFitSVRHitLink.h,v $
// Revision 1.10  2002/06/12 20:38:13  cleo3
// inlined the entire class and let the compiler write what it can
//
// Revision 1.9  2000/08/16 19:33:53  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.8  2000/02/24 17:34:09  wsun
// Updated for storage helpers.
//
// Revision 1.7  1999/10/07 18:00:43  marsh
// Added doxygen-style comments.
//
// Revision 1.6  1999/06/21 22:53:46  pg
//  Modifications for new Si hits.
//
// Revision 1.5  1999/05/09 21:32:06  wsun
// Implemented operator=()
//
// Revision 1.4  1999/05/08 01:02:16  marsh
// Added remaining outgoing hypotheses.
//
// Revision 1.3  1999/04/26 20:35:22  marsh
// Made link data classes inherit a common base class.
//
// Revision 1.2  1999/01/26 22:44:39  marsh
// Added error on residual, curvature, and momentum.
//
// Revision 1.1  1998/12/15 20:33:50  marsh
// Added definitions of fit track to hit Lattices and link data.
//

// system include files

// user include files
#include "TrackFitter/TrackFitHitLink.h"
#include "TrackRoot/TRHelixCandidateFit.h"
#include "CleoDB/DBCandidate.h"
#include "SiHits/CalibratedSVRphiHit.h"
#include "Lattice/Lattice.h"

// forward declarations

class TrackFitSVRHitLink : public TrackFitHitLink
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      TrackFitSVRHitLink( DABoolean used,
                          double residual,
                          double residualError,
                          double momentum ):
	 TrackFitHitLink( used, residual, residualError, momentum )
      {
      }

      TrackFitSVRHitLink():
	 TrackFitHitLink( false, 0, 1E10, 0 )
      {
      }
      
      //Allow the compiler to write these
      //TrackFitSVRHitLink( const TrackFitSVRHitLink& );
      //const TrackFitSVRHitLink& operator=( const TrackFitSVRHitLink& );
      //virtual ~TrackFitSVRHitLink();

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
                 CalibratedSVRphiHit ,
                 TrackFitSVRHitLink > PionFitSVRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kChargedKaon > ,
                 CalibratedSVRphiHit ,
                 TrackFitSVRHitLink > KaonFitSVRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kElectron > ,
                 CalibratedSVRphiHit ,
                 TrackFitSVRHitLink > ElectronFitSVRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kMuon > ,
                 CalibratedSVRphiHit ,
                 TrackFitSVRHitLink > MuonFitSVRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kProton > ,
                 CalibratedSVRphiHit ,
                 TrackFitSVRHitLink > ProtonFitSVRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitElectron > ,
                 CalibratedSVRphiHit ,
                 TrackFitSVRHitLink > ExitElectronFitSVRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitMuon > ,
                 CalibratedSVRphiHit ,
                 TrackFitSVRHitLink > ExitMuonFitSVRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitPion > ,
                 CalibratedSVRphiHit ,
                 TrackFitSVRHitLink > ExitPionFitSVRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitKaon > ,
                 CalibratedSVRphiHit ,
                 TrackFitSVRHitLink > ExitKaonFitSVRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitProton > ,
                 CalibratedSVRphiHit ,
                 TrackFitSVRHitLink > ExitProtonFitSVRHitLattice ;

// inline function definitions

#endif /* TRACKFITTER_TRACKFITSVRHITLINK_H */
