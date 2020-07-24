#if !defined(TRACKFITTER_TRACKFITDRHITLINK_H)
#define TRACKFITTER_TRACKFITDRHITLINK_H
// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      TrackFitDRHitLink
// 
/**\class TrackFitDRHitLink TrackFitDRHitLink.h TrackFitter/TrackFitDRHitLink.h

 Description: Link data for fit track \<--\> DR hit Lattices, and typedefs.

 Usage:
    This defines the link data between fit tracks and drift chamber hits.

    There are also several typedefs:
\code
       PionFitDRHitLattice
       KaonFitDRHitLattice
       ElectronFitDRHitLattice
       MuonFitDRHitLattice
       ProtonFitDRHitLattice
       ExitElectronFitDRHitLattice
       ExitMuonFitDRHitLattice
       ExitPionFitDRHitLattice
       ExitKaonFitDRHitLattice
       ExitProtonFitDRHitLattice
\endcode
    These are what the user should refer to, since the link data type
    is included in these.

    To construct a Lattice, the appropriate syntax is:
\code
       PionFitDRHitLattice( left_table,
                            right_table,
                            PionFitDRHitLattice::LNodeMulti,
                            PionFitDRHitLattice::RNodeMulti );
\endcode
    This will set up the topology such that each track can have multiple
    hits, and each hit can be on multiple tracks.  Each hit/track
    combination only has one link, however.

*/
//
// Author:      Michael Marsh
// Created:     Wed Nov 18 15:01:59 EST 1998
// $Id: TrackFitDRHitLink.h,v 1.16 2002/06/12 20:40:20 cleo3 Exp $
//
// Revision history
//
// $Log: TrackFitDRHitLink.h,v $
// Revision 1.16  2002/06/12 20:40:20  cleo3
// inlined the constructors and let the compiler write the methods it can
//
// Revision 1.15  2001/09/26 08:01:18  wsun
// Added signed drift distance as data member.
//
// Revision 1.14  2000/10/11 06:31:18  wsun
// Added fittingWeight() to TrackFitDRHitLink.
//
// Revision 1.13  2000/08/16 19:33:52  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.12  2000/06/16 19:30:22  marsh
// Purged tool box typedefs.
//
// Revision 1.11  2000/06/09 00:19:15  wsun
// Added driftTime data member.
//
// Revision 1.10  2000/02/24 17:34:08  wsun
// Updated for storage helpers.
//
// Revision 1.9  1999/11/03 03:20:57  wsun
// Added signedDcaError(), signedDriftDistance(), and signedDriftError().
//
// Revision 1.8  1999/10/07 18:00:41  marsh
// Added doxygen-style comments.
//
// Revision 1.7  1999/08/27 19:59:46  wsun
// Added signed dca to wire and sin(entrance angle).
//
// Revision 1.6  1999/05/09 21:32:04  wsun
// Implemented operator=()
//
// Revision 1.5  1999/05/08 01:02:15  marsh
// Added remaining outgoing hypotheses.
//
// Revision 1.4  1999/04/26 20:35:21  marsh
// Made link data classes inherit a common base class.
//
// Revision 1.3  1999/03/13 16:16:45  marsh
// Added m_spacePoint
//
// Revision 1.2  1999/01/26 22:44:38  marsh
// Added error on residual, curvature, and momentum.
//
// Revision 1.1  1998/12/15 20:33:49  marsh
// Added definitions of fit track to hit Lattices and link data.
//

// system include files

// user include files
#include "TrackFitter/TrackFitHitLink.h"
#include "TrackRoot/TRHelixCandidateFit.h"
#include "CleoDB/DBCandidate.h"
#include "CalibratedData/CalibratedDRHit.h"
#include "Lattice/Lattice.h"

// forward declarations

class TrackFitDRHitLink : public TrackFitHitLink
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      TrackFitDRHitLink( DABoolean iUsed,
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
	 TrackFitHitLink( iUsed, iResidual, iResidualError, iMomentum ),
	 m_trackRefPt(       iTrackRefPt ),
	 m_dca(              iSignedDcaToWire ),
	 m_dcaError(         iSignedDcaError ),
	 m_sinTrackToRadial( iSinTrackToRadial ),
	 m_driftTime(        iDriftTime ),
	 m_fittingWeight(    iFittingWeight ),
	 m_signedDriftDistance( iSignedDriftDistance )
      {
      }

      TrackFitDRHitLink():
	 TrackFitHitLink( false, 0, 1E10, 0 ),
	 m_trackRefPt( HepPoint3D(0,0,0) ),
	 m_dca( 0 ),
	 m_dcaError( 0 ),
	 m_sinTrackToRadial( 0 ),
	 m_driftTime( 0 ),
	 m_fittingWeight(  0 ),
	 m_signedDriftDistance( 0 )
      {
      }

      
      //Allow the compiler to write these
      //TrackFitDRHitLink( const TrackFitDRHitLink& );
      //const TrackFitDRHitLink& operator=( const TrackFitDRHitLink& );
      //virtual ~TrackFitDRHitLink();

      DABoolean operator==( const TrackFitDRHitLink& ) const;
      DABoolean operator!=( const TrackFitDRHitLink& ) const;

      // member functions
      void setTrackRefPt( const HepPoint3D& aPoint )
      { m_trackRefPt = aPoint ; }
      void setSignedDcaToWire( Meters aDca ) { m_dca = aDca ; }
      void setSignedDcaError( Meters aError ) { m_dcaError = aError ; }
      void setSinTrackToRadial( double aSin ) { m_sinTrackToRadial = aSin ; }
      void setDriftTime( CalibratedTimeHit::PicoSec aDriftTime )
      { m_driftTime = aDriftTime ; }
      void setFittingWeight( double aFittingWeight )
      { m_fittingWeight = aFittingWeight ; }
      void setSignedDriftDistance( Meters aSignedDriftDistance )
      { m_signedDriftDistance = aSignedDriftDistance ; }

      // const member functions
      /**
       * \return const reference to a Hep3Vector.
       * Reference point for the track at this hit.  This will typically
       * be the wire position, since that is the most natural origin
       * for the local coordinate system.
       */
      const HepPoint3D& trackRefPt()    const { return m_trackRefPt; }

      /**
       * \return drift time, which may have track- and mass-dependent
       * corrections applied (to account for signal propagation and time
       *  of flight, etc.), so it may differ from CalibratedDRHit::time()
       * for the same hit.
       */
      CalibratedTimeHit::PicoSec driftTime() const { return m_driftTime ; }

      /**
       * \return signed distance of closest approach of the track to the wire
       * that this hit is on.
       */
      Meters signedDcaToWire()            const { return m_dca ; }

      /**
       * \return error on signed distance of closest approach of the track
       * to the wire that this hit is on.
       */
      Meters signedDcaError()            const { return m_dcaError ; }

      /**
       * \return signed drift distance used in the fit, which may have a
       * different sign from the quantity in the SeedTrackDRHitLattice
       * because the fitter is allowed to flip the sign.  This quantity
       * may also have track- and mass-dependent corrections applied
       * (to account for signal propagation and time of flight, etc.), so
       * it may differ from SeedDRHitLink::signedDriftDistance() for the
       * same hit.  This quantity is NOT the sum of the residual and the
       * DCA because these two quantities have been smoothed independently.
       */
      Meters signedDriftDistance()       const { return m_signedDriftDistance;}

      /**
       * \return error on signed drift distance.  This quantity is the same
       * as the 1. / sqrt( CalibratedDRHit::weight() ).
       */
      Meters signedDriftError()          const
      { return sqrt( sqr( residualError() ) - sqr( m_dcaError ) ) ; }

      /**
       * \return sin(entrance angle)
       */
      double sinTrackToRadial()           const { return m_sinTrackToRadial ; }

      /**
       * \return fitting weight with track- and mass-dependent corrections.
       */
      double fittingWeight()           const { return m_fittingWeight ; }

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor (stop defaults) -- default ok

      // private member functions

      // private const member functions

      // data members
      HepPoint3D                  m_trackRefPt ;
      Meters                      m_dca ;
      Meters                      m_dcaError ;
      double                      m_sinTrackToRadial ;
      CalibratedTimeHit::PicoSec  m_driftTime ;
      double                      m_fittingWeight ;
      Meters                      m_signedDriftDistance ;

      // static data members

};

typedef Lattice< TRHelixCandidateFit< DBCandidate::kChargedPion > ,
                 CalibratedDRHit ,
                 TrackFitDRHitLink > PionFitDRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kChargedKaon > ,
                 CalibratedDRHit ,
                 TrackFitDRHitLink > KaonFitDRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kElectron > ,
                 CalibratedDRHit ,
                 TrackFitDRHitLink > ElectronFitDRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kMuon > ,
                 CalibratedDRHit ,
                 TrackFitDRHitLink > MuonFitDRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kProton > ,
                 CalibratedDRHit ,
                 TrackFitDRHitLink > ProtonFitDRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitElectron > ,
                 CalibratedDRHit ,
                 TrackFitDRHitLink > ExitElectronFitDRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitMuon > ,
                 CalibratedDRHit ,
                 TrackFitDRHitLink > ExitMuonFitDRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitPion > ,
                 CalibratedDRHit ,
                 TrackFitDRHitLink > ExitPionFitDRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitKaon > ,
                 CalibratedDRHit ,
                 TrackFitDRHitLink > ExitKaonFitDRHitLattice ;
typedef Lattice< TRHelixCandidateFit< DBCandidate::kExitProton > ,
                 CalibratedDRHit ,
                 TrackFitDRHitLink > ExitProtonFitDRHitLattice ;

// inline function definitions

#endif /* TRACKFITTER_TRACKFITDRHITLINK_H */
