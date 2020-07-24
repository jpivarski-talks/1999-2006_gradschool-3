#if !defined(NAVIGATION_NAVTRACK_H)
#define NAVIGATION_NAVTRACK_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrack
// 
/**\class NavTrack NavTrack.h Navigation/NavTrack.h

 Description: User-level navigation object for tracks.

 Usage:
    NavTrack inherits TDTrack, adding connections to other types of
    data, such as hits, showers, and (where appropriate) Monte Carlo.

    \par Hits:
    NavTrack provides two type of methods to get DR and silicon hits attached
    to a track.  The first is a simple list of hits, of the general type
\code
   const DRHitTable*  drHits( DBCandidate::Hypo aHypo ) const;
\endcode
    where DRHitTable is a vector of pointers to constant CalibratedDRHits.
    In addition to drHits(), there are also svrHits() and svzHits().  Also,
    there is seedDRHits() (and the other hit-types), which provides the same
    information for the seed (pattern-recognition) track.  In principle,
    these should be the same, since discarded hits are kept and flagged as
    unused.
    The second is a list of links between the track and the hits, of the
    general type
\code
   const DRHitLinkTable*  drHitLinks( DBCandidate::Hypo aHypo ) const;
\endcode
    where DRHitLinkTable is a vector of pointers to constant Lattice links.
    As above, drHitLinks() is joined by svrHitLinks() and svzHitLinks().
    The functions for the seed track, seedDRHitLinks() et. al., return 
    slightly different objects, since the pattern-recognition information
    is different than the fitter information.

    \par Showers:
    NavTrack also provides a track-shower matching object via
\code
   FAItem< NavTkShMatch >  trackShowerMatch() const;
\endcode
*/
//
// Author:      Michael Marsh
// Created:     Mon Jul  6 14:31:11 EDT 1998
// $Id: NavTrack.h,v 1.18 2003/10/08 16:23:07 urner Exp $
//
// Revision history
//
// $Log: NavTrack.h,v $
// Revision 1.18  2003/10/08 16:23:07  urner
// included ZD lattice links
//
// Revision 1.17  2003/07/29 20:18:32  bkh
// Add trkman, pca-BeamSpot fcns to NavTrack
//
// Revision 1.16  2003/02/03 20:16:40  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.14  2002/11/07 17:42:24  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
// Revision 1.13  2002/08/06 17:34:52  cleo3
// NavTrackMaster rather than NavTrack now owns any memory inorder to decrease dependencies seen on Solaris 8
//
// Revision 1.12  2002/03/19 20:25:10  cdj
// simplified memory handling for NavTrack
//
// Revision 1.11  2001/10/26 21:54:01  bkh
// Add features to showers and connected region objects
//
// Revision 1.10  2000/07/27 01:32:06  ts
// NavRich added
//
// Revision 1.9  2000/06/07 21:53:10  llh14
// Deleted 'm_seed' member data in destructor to plug memory leaks
//
// Revision 1.8  2000/06/06 18:38:59  bkh
// Install dedx, electron id, and muon id interfaces
//
// Revision 1.7  1999/11/17 20:34:23  marsh
// Lattice information is now returned with pointers instead of references.
// Track-shower matches are returned as FAItems instead of pointers.
// There is no more hypothesis faking for missing data.
//
// Revision 1.6  1999/10/24 15:12:46  cdj
// removed inappropriate use of typename
//
// Revision 1.5  1999/10/06 22:16:54  marsh
// Removed "void" from argument lists.
// Added in-file documentation.
//
// Revision 1.4  1999/09/16 20:38:21  bkh
// Add track shower match info
//
// Revision 1.3  1999/05/08 00:45:17  marsh
// Added remaining outgoing hypotheses.
//
// Revision 1.2  1999/04/23 21:35:59  marsh
// Changed NavTrack to deliver links, not link data.
//
// Revision 1.1.1.1  1998/12/31 18:18:30  marsh
// New library for navigation objects.
//
// Revision 1.2  1998/12/29 22:20:52  marsh
// Mods for providing Lattice info.
//
// Revision 1.1.1.1  1998/07/13 23:26:20  marsh
// New library for user-level analysis objects
//

// system include files

// user include files
#include "TrackDelivery/TDTrack.h" // superclass
#include "TrackFitter/TrackFitDRHitLink.h"
#include "TrackFitter/TrackFitZDHitLink.h"
#include "TrackFitter/TrackFitSVRHitLink.h"
#include "TrackFitter/TrackFitSVZHitLink.h"
#include "TrackFinder/SeedDRHitLink.h"
#include "TrackFinder/SeedZDHitLink.h"
#include "TrackFinder/SeedTrackSVRHitLattice.h"
#include "TrackFinder/SeedTrackSVZHitLattice.h"

// forward declarations


template <class T> class FAPtrTable      ;
template <class T> class FATable         ;
template <class T> class FAItem          ;


class Record         ;
class NavTrackMaster ;
class NavTkShMatch   ;

class NavRich ;
class NavKs ;
class NavLambda ;

class DedxInfo  ;
class NavMuonId ;
class NavElecId ;
class NavShower ;
class MCParticle ;
class MCTrackTagsByTrack ;

class TrkmanInfo ;

class NavTrack : public TDTrack
{
      // ---------- friend classes and functions ---------------
   public:
      // ---------- constants, enums and typedefs --------------
      typedef 
   Lattice< TRHelixFit, CalibratedDRHit, TrackFitDRHitLink >::Link
   DRHitLink;
      typedef 
   Lattice< TRHelixFit, CalibratedZDHit, TrackFitZDHitLink >::Link
   ZDHitLink;
      typedef 
   Lattice< TRHelixFit, CalibratedSVRphiHit, TrackFitSVRHitLink >::Link
   SVRHitLink;
      typedef 
   Lattice< TRHelixFit, CalibratedSVZHit, TrackFitSVZHitLink >::Link
   SVZHitLink;
      typedef STL_VECTOR( const DRHitLink* ) DRHitLinkTable;
      typedef STL_VECTOR( const ZDHitLink* ) ZDHitLinkTable;
      typedef STL_VECTOR( const SVRHitLink* ) SVRHitLinkTable;
      typedef STL_VECTOR( const SVZHitLink* ) SVZHitLinkTable;
      typedef STL_VECTOR( const CalibratedDRHit* )  DRHitTable;
      typedef STL_VECTOR( const CalibratedZDHit* )  ZDHitTable;
      typedef STL_VECTOR( const CalibratedSVRphiHit* ) SVRHitTable;
      typedef STL_VECTOR( const CalibratedSVZHit* ) SVZHitTable;
      typedef STL_VECTOR( const SeedTrackDRHitLattice::Link* )
         SeedDRHitLinkTable;
      typedef STL_VECTOR( const SeedTrackZDHitLattice::Link* )
         SeedZDHitLinkTable;
      typedef STL_VECTOR( const SeedTrackSVRHitLattice::Link* )
         SeedSVRHitLinkTable;
      typedef STL_VECTOR( const SeedTrackSVZHitLattice::Link* )
         SeedSVZHitLinkTable;

      // ---------- Constructors and destructor ----------------
      NavTrack( const Identifier& aId ,
                const NavTrackMaster& aMaster );
      virtual ~NavTrack();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      /**
       * \return FAItem of TrkmanInfo.
       * If there is no such item this returns an invalid item.
       */
      FAItem< TrkmanInfo   >  trkmanInfo()  const ;

      /**
       * \return FAItem of NavTkShMatch.
       * If there is no matched shower this returns an invalid item.
       */
      FAItem< NavTkShMatch >  trackShowerMatch()  const ;


      /**
       * \return FATable of NavShowers
       * Contains showers **NOT** matched to this track which
       * line up with the pion fit momentum within 100 mrad
       */
      FATable< NavShower >    bremShowers()       const ;

      /**
       * \return signed distance of closest approach w.r.t. beam spot (dbcd)
       */
      double signedDCABeamSpot( const KTHelix& aHelix ) const ;

      /**
       * \return point closest approach w.r.t. beam spot
       */
      HepPoint3D pcaBeamSpot( const KTHelix& aHelix ) const ;

      /**
       * \return FAItem of NavRich.
       * If this track was not analyzed this returns an invalid item.
       */
      FAItem< NavRich >  richInfo()  const;
    

      FAItem< DedxInfo     >  dedxInfo()          const ;

      const NavMuonId& muonId() const ;
      const NavElecId& elecId() const ;

      FAItem< NavKs >     navKs()     const ; // best Ks incl this track if any

      FAItem< NavLambda > navLambda() const ; // best lambda incl this trk

      FAItem< MCParticle         >  mcTag()  const ; // best tag of same charge
      FAItem< MCTrackTagsByTrack >  mcTags() const ; // whole tag object

      /**
       * \param aHypo mass hypothesis for fit.
       * \return const pointer to NavTrack::DRHitLinkTable, a vector of
       * LatticeLinks connecting a track with CalibratedDRHits.
       *
       * drHitLinks() provides the links between a track and the hits from
       * the DR that were attached to the
       * specified mass-hypothesis fit.  There are also "shortcut"
       * functions that internally call this function with pre-set
       * hypotheses, ie. pionDRHitLinks().  All ten hypotheses (5 inward, 5
       * outward) are provided.  If a hypothesis is unavailable, you
       * will get a null pointer (0).
       *
       * The links themselves can be a little confusing.  There are only
       * two functions of the link that you'll need to worry about:
       * linkData() and rightID().
       *
       * linkData() returns a const reference to a TrackFitDRHitLink,
       * which contains information about how (and if) the hit was used
       * in this fit (eg. residual).
       *
       * rightID() returns the identifier of the hit.
       */
      const DRHitLinkTable*  drHitLinks( DBCandidate::Hypo aHypo ) const;
      const DRHitLinkTable*  pionDRHitLinks()         const;
      const DRHitLinkTable*  kaonDRHitLinks()         const;
      const DRHitLinkTable*  electronDRHitLinks()     const;
      const DRHitLinkTable*  muonDRHitLinks()         const;
      const DRHitLinkTable*  protonDRHitLinks()       const;
      const DRHitLinkTable*  exitElectronDRHitLinks() const;
      const DRHitLinkTable*  exitMuonDRHitLinks()     const;
      const DRHitLinkTable*  exitPionDRHitLinks()     const;
      const DRHitLinkTable*  exitKaonDRHitLinks()     const;
      const DRHitLinkTable*  exitProtonDRHitLinks()   const;


      /**
       * \param aHypo mass hypothesis for fit.
       * \return const pointer to NavTrack::ZDHitLinkTable, a vector of
       * LatticeLinks connecting a track with CalibratedZDHits.
       *
       * zdHitLinks() provides the links between a track and the hits from
       * the ZD that were attached to the
       * specified mass-hypothesis fit.  There are also "shortcut"
       * functions that internally call this function with pre-set
       * hypotheses, ie. pionZDHitLinks().  All ten hypotheses (5 inward, 5
       * outward) are provided.  If a hypothesis is unavailable, you
       * will get a null pointer (0).
       *
       * The links themselves can be a little confusing.  There are only
       * two functions of the link that you'll need to worry about:
       * linkData() and rightID().
       *
       * linkData() returns a const reference to a TrackFitZDHitLink,
       * which contains information about how (and if) the hit was used
       * in this fit (eg. residual).
       *
       * rightID() returns the identifier of the hit.
       */
      const ZDHitLinkTable*  zdHitLinks( DBCandidate::Hypo aHypo ) const;
      const ZDHitLinkTable*  pionZDHitLinks()         const;
      const ZDHitLinkTable*  kaonZDHitLinks()         const;
      const ZDHitLinkTable*  electronZDHitLinks()     const;
      const ZDHitLinkTable*  muonZDHitLinks()         const;
      const ZDHitLinkTable*  protonZDHitLinks()       const;
      const ZDHitLinkTable*  exitElectronZDHitLinks() const;
      const ZDHitLinkTable*  exitMuonZDHitLinks()     const;
      const ZDHitLinkTable*  exitPionZDHitLinks()     const;
      const ZDHitLinkTable*  exitKaonZDHitLinks()     const;
      const ZDHitLinkTable*  exitProtonZDHitLinks()   const;


      /**
       * \param aHypo mass hypothesis for fit.
       * \return const pointer to NavTrack::SVRHitLinkTable, a vector of
       * LatticeLinks connecting a track with CalibratedSVRphiHits.
       *
       * svrHitLinks() provides the links between a track and the r-phi
       * hits from the SVX that were attached to the
       * specified mass-hypothesis fit.  There are also "shortcut"
       * functions that internally call this function with pre-set
       * hypotheses, ie. pionSVRHitLinks().  All ten hypotheses (5 inward, 5
       * outward) are provided.  If a hypothesis is unavailable, you
       * will get a null pointer (0).
       *
       * The links themselves can be a little confusing.  There are only
       * two functions of the link that you'll need to worry about:
       * linkData() and rightID().
       *
       * linkData() returns a const reference to a TrackFitSVRHitLink,
       * which contains information about how (and if) the hit was used
       * in this fit (eg. residual).
       *
       * rightID() returns the identifier of the hit.
       */
      const SVRHitLinkTable*  svrHitLinks( DBCandidate::Hypo aHypo ) const;
      const SVRHitLinkTable*  pionSVRHitLinks()         const;
      const SVRHitLinkTable*  kaonSVRHitLinks()         const;
      const SVRHitLinkTable*  electronSVRHitLinks()     const;
      const SVRHitLinkTable*  muonSVRHitLinks()         const;
      const SVRHitLinkTable*  protonSVRHitLinks()       const;
      const SVRHitLinkTable*  exitElectronSVRHitLinks() const;
      const SVRHitLinkTable*  exitMuonSVRHitLinks()     const;
      const SVRHitLinkTable*  exitPionSVRHitLinks()     const;
      const SVRHitLinkTable*  exitKaonSVRHitLinks()     const;
      const SVRHitLinkTable*  exitProtonSVRHitLinks()   const;

      /**
       * \param aHypo mass hypothesis for fit.
       * \return const pointer to NavTrack::SVZHitLinkTable, a vector of
       * LatticeLinks connecting a track with CalibratedSVZHits.
       *
       * svzHitLinks() provides the links between a track and the z-side
       * hits from the SVX that were attached to the
       * specified mass-hypothesis fit.  There are also "shortcut"
       * functions that internally call this function with pre-set
       * hypotheses, ie. pionSVZHitLinks().  All ten hypotheses (5 inward, 5
       * outward) are provided.  If a hypothesis is unavailable, you
       * will get a null pointer (0).
       *
       * The links themselves can be a little confusing.  There are only
       * two functions of the link that you'll need to worry about:
       * linkData() and rightID().
       *
       * linkData() returns a const reference to a TrackFitSVZHitLink,
       * which contains information about how (and if) the hit was used
       * in this fit (eg. residual).
       *
       * rightID() returns the identifier of the hit.
       */
      const SVZHitLinkTable*  svzHitLinks( DBCandidate::Hypo aHypo ) const;
      const SVZHitLinkTable*  pionSVZHitLinks()         const;
      const SVZHitLinkTable*  kaonSVZHitLinks()         const;
      const SVZHitLinkTable*  electronSVZHitLinks()     const;
      const SVZHitLinkTable*  muonSVZHitLinks()         const;
      const SVZHitLinkTable*  protonSVZHitLinks()       const;
      const SVZHitLinkTable*  exitElectronSVZHitLinks() const;
      const SVZHitLinkTable*  exitMuonSVZHitLinks()     const;
      const SVZHitLinkTable*  exitPionSVZHitLinks()     const;
      const SVZHitLinkTable*  exitKaonSVZHitLinks()     const;
      const SVZHitLinkTable*  exitProtonSVZHitLinks()   const;

      /**
       * \param aHypo mass hypothesis for fit.
       * \return const pointer to NavTrack::DRHitTable, a vector of
       * CalibratedDRHits.
       *
       * drHits() provides the hits from the DR that were attached to the
       * specified mass-hypothesis fit.  There are also "shortcut"
       * functions that internally call this function with pre-set
       * hypotheses, ie. pionDRHits().  All ten hypotheses (5 inward, 5
       * outward) are provided.  If a hypothesis is unavailable, you
       * will get a null pointer (0).
       */
      const DRHitTable*  drHits( DBCandidate::Hypo aHypo ) const;
      const DRHitTable*  pionDRHits()         const;
      const DRHitTable*  kaonDRHits()         const;
      const DRHitTable*  electronDRHits()     const;
      const DRHitTable*  muonDRHits()         const;
      const DRHitTable*  protonDRHits()       const;
      const DRHitTable*  exitElectronDRHits() const;
      const DRHitTable*  exitMuonDRHits()     const;
      const DRHitTable*  exitPionDRHits()     const;
      const DRHitTable*  exitKaonDRHits()     const;
      const DRHitTable*  exitProtonDRHits()   const;

      /**
       * \param aHypo mass hypothesis for fit.
       * \return const pointer to NavTrack::ZDHitTable, a vector of
       * CalibratedZDHits.
       *
       * zdHits() provides the hits from the ZD that were attached to the
       * specified mass-hypothesis fit.  There are also "shortcut"
       * functions that internally call this function with pre-set
       * hypotheses, ie. pionZDHits().  All ten hypotheses (5 inward, 5
       * outward) are provided.  If a hypothesis is unavailable, you
       * will get a null pointer (0).
       */
      const ZDHitTable*  zdHits( DBCandidate::Hypo aHypo ) const;
      const ZDHitTable*  pionZDHits()         const;
      const ZDHitTable*  kaonZDHits()         const;
      const ZDHitTable*  electronZDHits()     const;
      const ZDHitTable*  muonZDHits()         const;
      const ZDHitTable*  protonZDHits()       const;
      const ZDHitTable*  exitElectronZDHits() const;
      const ZDHitTable*  exitMuonZDHits()     const;
      const ZDHitTable*  exitPionZDHits()     const;
      const ZDHitTable*  exitKaonZDHits()     const;
      const ZDHitTable*  exitProtonZDHits()   const;


      /**
       * \param aHypo mass hypothesis for fit.
       * \return const pointer to NavTrack::SVRHitTable, a vector of
       * CalibratedSVRphiHits.
       *
       * svrHits() provides the r-phi-side hits from the SVX that were
       * attached to the specified mass-hypothesis fit.  There are also
       * "shortcut" functions that internally call this function with pre-set
       * hypotheses, ie. pionSVRHits().  All ten hypotheses (5 inward, 5
       * outward) are provided.  If a hypothesis is unavailable, you will
       * get a null pointer (0).
       */
      const SVRHitTable*  svrHits( DBCandidate::Hypo aHypo ) const;
      const SVRHitTable*  pionSVRHits()         const;
      const SVRHitTable*  kaonSVRHits()         const;
      const SVRHitTable*  electronSVRHits()     const;
      const SVRHitTable*  muonSVRHits()         const;
      const SVRHitTable*  protonSVRHits()       const;
      const SVRHitTable*  exitElectronSVRHits() const;
      const SVRHitTable*  exitMuonSVRHits()     const;
      const SVRHitTable*  exitPionSVRHits()     const;
      const SVRHitTable*  exitKaonSVRHits()     const;
      const SVRHitTable*  exitProtonSVRHits()   const;

      /**
       * \param aHypo mass hypothesis for fit.
       * \return const pointer to NavTrack::SVZHitTable, a vector of
       * CalibratedSVZHits.
       *
       * svzHits() provides the z-side hits from the SVX that were attached
       * to the specified mass-hypothesis fit.  There are also "shortcut"
       * functions that internally call this function with pre-set
       * hypotheses, ie. pionSVZHits().  All ten hypotheses (5 inward, 5
       * outward) are provided.  If a hypothesis is unavailable, you will
       * get a null pointer (0).
       */
      const SVZHitTable*  svzHits( DBCandidate::Hypo aHypo ) const;
      const SVZHitTable*  pionSVZHits()         const;
      const SVZHitTable*  kaonSVZHits()         const;
      const SVZHitTable*  electronSVZHits()     const;
      const SVZHitTable*  muonSVZHits()         const;
      const SVZHitTable*  protonSVZHits()       const;
      const SVZHitTable*  exitElectronSVZHits() const;
      const SVZHitTable*  exitMuonSVZHits()     const;
      const SVZHitTable*  exitPionSVZHits()     const;
      const SVZHitTable*  exitKaonSVZHits()     const;
      const SVZHitTable*  exitProtonSVZHits()   const;

      /**
       * \return const pointer to a table of LatticeLinks.
       * linkData() returns a SeedDRHitLink.
       * \sa drHitLinks
       */
      const SeedDRHitLinkTable*  seedDRHitLinks() const;
      /**
       * \return const pointer to a table of LatticeLinks.
       * linkData() returns a SeedZDHitLink.
       * \sa zdHitLinks
       */
      const SeedZDHitLinkTable*  seedZDHitLinks() const;

      /**
       * \return const pointer to a vector of const CalibratedDRHit*
       * \sa drHits
       */
      const DRHitTable*          seedDRHits()     const;

      /**
       * \return const pointer to a vector of const CalibratedZDHit*
       * \sa zdHits
       */
      const ZDHitTable*          seedZDHits()     const;

      /**
       * \return const pointer to a table of LatticeLinks.
       * linkData() returns nothing.
       * \sa svrHitLinks
       */
      const SeedSVRHitLinkTable*  seedSVRHitLinks() const;
      /**
       * \return const pointer to a vector of const CalibratedSVRphiHit*
       * \sa svrHits
       */
      const SVRHitTable*          seedSVRHits()     const;

      /**
       * \return const pointer to a table of LatticeLinks.
       * linkData() returns nothing.
       * \sa svzHitLinks
       */
      const SeedSVZHitLinkTable*  seedSVZHitLinks() const;
      /**
       * \return const pointer to a vector of const CalibratedSVZHit*
       * \sa svzHits
       */
      const SVZHitTable*          seedSVZHits()     const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      NavTrack( const NavTrack& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const NavTrack& operator=( const NavTrack& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const DRHitLinkTable*    m_drHitLinks[DBCandidate::kMaxHypo];
      const ZDHitLinkTable*    m_zdHitLinks[DBCandidate::kMaxHypo];
      const SVRHitLinkTable*   m_svrHitLinks[DBCandidate::kMaxHypo];
      const SVZHitLinkTable*   m_svzHitLinks[DBCandidate::kMaxHypo];
      const DRHitTable*        m_drHitTables[DBCandidate::kMaxHypo];
      const ZDHitTable*        m_zdHitTables[DBCandidate::kMaxHypo];
      const SVRHitTable*       m_svrHitTables[DBCandidate::kMaxHypo];
      const SVZHitTable*       m_svzHitTables[DBCandidate::kMaxHypo];
      
      const SeedDRHitLinkTable*              m_seedDRHitLinks;
      const SeedZDHitLinkTable*              m_seedZDHitLinks;
      const SeedSVRHitLinkTable*             m_seedSVRHitLinks;
      const SeedSVZHitLinkTable*             m_seedSVZHitLinks;
      const DRHitTable*                      m_seedDRHitTable;
      const ZDHitTable*                      m_seedZDHitTable;
      const SVRHitTable*                     m_seedSVRHitTable;
      const SVZHitTable*                     m_seedSVZHitTable;

      const NavTrackMaster&  m_master ;

      NavElecId* m_elecId ;
      NavMuonId* m_muonId ;

      const FAPtrTable< NavShower >* m_bremShowers ;

      const NavKs*     m_navKs ;
      const NavLambda* m_navLambda ;

      // ---------- static data members ------------------------
   public:
      const Record& record() const ;
};

// inline function definitions

#endif /* NAVIGATION_NAVTRACK_H */
