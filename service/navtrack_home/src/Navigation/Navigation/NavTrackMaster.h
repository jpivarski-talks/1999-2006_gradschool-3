#if !defined(NAVIGATION_NAVTRACKMASTER_H)
#define NAVIGATION_NAVTRACKMASTER_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrackMaster
// 
// Description: Master block for NavTrack, inherits from TDTrackMaster.
//
// Usage:
//    <usage>
//
// Author:      Michael Marsh
// Created:     Mon Jul  6 14:31:17 EDT 1998
// $Id: NavTrackMaster.h,v 1.14 2003/10/08 23:08:11 mccann Exp $
//
// Revision history
//
// $Log: NavTrackMaster.h,v $
// Revision 1.14  2003/10/08 23:08:11  mccann
// added ZD NavTrack interface
//
// Revision 1.13  2003/07/29 20:18:33  bkh
// Add trkman, pca-BeamSpot fcns to NavTrack
//
// Revision 1.12  2003/02/03 20:16:40  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.10  2002/11/07 17:42:24  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
// Revision 1.9  2002/08/06 17:34:54  cleo3
// NavTrackMaster rather than NavTrack now owns any memory inorder to decrease dependencies seen on Solaris 8
//
// Revision 1.8  2002/03/19 20:25:10  cdj
// simplified memory handling for NavTrack
//
// Revision 1.7  2001/10/26 21:54:02  bkh
// Add features to showers and connected region objects
//
// Revision 1.6  2000/09/26 21:21:45  lkg
// NavTrackMaster now takes production and usage tags for specification
// of the seed tracks (track finder) and fit tracks (track fitter) to be
// used by the TDTrackMaster base class
//
// Revision 1.5  2000/07/27 01:32:06  ts
// NavRich added
//
// Revision 1.4  2000/06/06 18:39:00  bkh
// Install dedx, electron id, and muon id interfaces
//
// Revision 1.3  1999/09/16 20:38:21  bkh
// Add track shower match info
//
// Revision 1.2  1999/05/08 00:45:18  marsh
// Added remaining outgoing hypotheses.
//
// Revision 1.1.1.1  1998/12/31 18:18:30  marsh
// New library for navigation objects.
//
// Revision 1.2  1998/12/29 22:20:53  marsh
// Mods for providing Lattice info.
//
// Revision 1.1.1.1  1998/07/13 23:26:20  marsh
// New library for user-level analysis objects
//

// system include files
#include <vector>

// user include files
//#include "FrameAccess/FATable.h"
//#include "FrameAccess/FAConstPtrTable.h"
#include "TrackDelivery/TDTrackMaster.h" // superclass
#include "Navigation/NavTrack.h"
//#include "Navigation/NavTkShMatch.h"
//#include "Navigation/NavRich.h"
//#include "Navigation/NavShower.h"
#include "TrackFitter/TrackFitDRHitLink.h"
#include "TrackFitter/TrackFitSVRHitLink.h"
#include "TrackFitter/TrackFitSVZHitLink.h"
#include "TrackFinder/SeedDRHitLink.h"
#include "TrackFinder/SeedTrackSVRHitLattice.h"
#include "TrackFinder/SeedTrackSVZHitLattice.h"

//#include "DedxInfo/DedxInfo.h"
//#include "C3Mu/MuTrack.h"

#include "STLUtility/fwd_vector.h"

// forward declarations
class NavTkShMatch;
class NavKs;
class NavLambda;
class NavRich;
class NavShower;
class DedxInfo;
class MuTrack;
class MCParticle ;
class MCTrackTagsByTrack ;
class TrkmanInfo ;

template <class T> class FATable;
template <class T> class FAItem;
template <class T> class FAConstPtrTable;
template <class T> class FAPtrTable;

class NavTrackMaster : public TDTrackMaster
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      NavTrackMaster( const Record& iRecord,
		      TagPair fitterTags = TagPair( UsageTag(),ProductionTag() ),
		      TagPair finderTags = TagPair( UsageTag(),ProductionTag() )
 );
      virtual ~NavTrackMaster();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      const NavTkShMatch* trackShowerMatch(
	 const NavTrack::Identifier aId ) const ;

      const FAPtrTable<NavShower>* bremShowers(
	 const NavTrack::Identifier aId ) const ;

      const NavRich* richInfo(
         const NavTrack::Identifier aId ) const ;

      const MuTrack* muTrack(
	 const NavTrack::Identifier aId ) const ;

      const DedxInfo* dedx(
	 const NavTrack::Identifier aId ) const ;

      const TrkmanInfo* trkmanInfo(
	 const NavTrack::Identifier aId ) const ;

      const MCParticle* mcTag(
	 const NavTrack::Identifier aId ) const ;

      const MCTrackTagsByTrack* mcTags(
	 const NavTrack::Identifier aId ) const ;

      const NavKs* navKs(
	 const NavTrack::Identifier aId ) const ;

      const NavLambda* navLambda(
	 const NavTrack::Identifier aId ) const ;

      FATable< NavShower >   navShowers() const ;

      const HepPoint3D* beamSpot() const ;

      const NavTrack::DRHitLinkTable* drHitLinks(
         NavTrack::Identifier aId,
         DBCandidate::Hypo aHypo ) const;
      const NavTrack::ZDHitLinkTable* zdHitLinks(
         NavTrack::Identifier aId,
         DBCandidate::Hypo aHypo ) const;
      const NavTrack::SVRHitLinkTable* svrHitLinks(
         NavTrack::Identifier aId,
         DBCandidate::Hypo aHypo ) const;
      const NavTrack::SVZHitLinkTable* svzHitLinks(
         NavTrack::Identifier aId,
         DBCandidate::Hypo aHypo ) const;
      const NavTrack::DRHitTable* drHits(
         NavTrack::Identifier aId,
         DBCandidate::Hypo aHypo ) const;
      const NavTrack::ZDHitTable* zdHits(
         NavTrack::Identifier aId,
         DBCandidate::Hypo aHypo ) const;
      const NavTrack::SVRHitTable* svrHits(
         NavTrack::Identifier aId,
         DBCandidate::Hypo aHypo ) const;
      const NavTrack::SVZHitTable* svzHits(
         NavTrack::Identifier aId,
         DBCandidate::Hypo aHypo ) const;
      const NavTrack::SeedDRHitLinkTable* seedDRHitLinks(
         NavTrack::Identifier aId ) const;
      const NavTrack::SeedZDHitLinkTable* seedZDHitLinks(
         NavTrack::Identifier aId ) const;
      const NavTrack::SeedSVRHitLinkTable* seedSVRHitLinks(
         NavTrack::Identifier aId ) const;
      const NavTrack::SeedSVZHitLinkTable* seedSVZHitLinks(
         NavTrack::Identifier aId ) const;
      const NavTrack::DRHitTable* seedDRHitTable(
         NavTrack::Identifier aId ) const;
      const NavTrack::ZDHitTable* seedZDHitTable(
         NavTrack::Identifier aId ) const;
      const NavTrack::SVRHitTable* seedSVRHitTable(
         NavTrack::Identifier aId ) const;
      const NavTrack::SVZHitTable* seedSVZHitTable(
         NavTrack::Identifier aId ) const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      NavTrackMaster( const NavTrackMaster& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const NavTrackMaster& operator=( const NavTrackMaster& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      const FAPtrTable< NavShower    >* m_showers     ;      
      const FAPtrTable< NavTkShMatch >* m_showerMatch ;      
      const FAPtrTable< NavRich      >* m_rich        ;
      const FAPtrTable< MuTrack      >* m_muTrack     ;      
      const FAPtrTable< DedxInfo     >* m_dedx        ;      
      const FAPtrTable< TrkmanInfo   >* m_trkmanInfo  ;      
      const FAPtrTable< NavKs        >* m_navKs       ;      
      const FAPtrTable< NavLambda    >* m_navLambda   ;      
      const FAPtrTable< MCTrackTagsByTrack  >* m_mcTags      ;      

      const PionFitDRHitLattice*          m_pionDRHitLattice;
      const KaonFitDRHitLattice*          m_kaonDRHitLattice;
      const ElectronFitDRHitLattice*      m_electronDRHitLattice;
      const MuonFitDRHitLattice*          m_muonDRHitLattice;
      const ProtonFitDRHitLattice*        m_protonDRHitLattice;
      const ExitElectronFitDRHitLattice*  m_exitElectronDRHitLattice;
      const ExitMuonFitDRHitLattice*      m_exitMuonDRHitLattice;
      const ExitPionFitDRHitLattice*      m_exitPionDRHitLattice;
      const ExitKaonFitDRHitLattice*      m_exitKaonDRHitLattice;
      const ExitProtonFitDRHitLattice*    m_exitProtonDRHitLattice;

      const PionFitZDHitLattice*          m_pionZDHitLattice;
      const KaonFitZDHitLattice*          m_kaonZDHitLattice;
      const ElectronFitZDHitLattice*      m_electronZDHitLattice;
      const MuonFitZDHitLattice*          m_muonZDHitLattice;
      const ProtonFitZDHitLattice*        m_protonZDHitLattice;
      const ExitElectronFitZDHitLattice*  m_exitElectronZDHitLattice;
      const ExitMuonFitZDHitLattice*      m_exitMuonZDHitLattice;
      const ExitPionFitZDHitLattice*      m_exitPionZDHitLattice;
      const ExitKaonFitZDHitLattice*      m_exitKaonZDHitLattice;
      const ExitProtonFitZDHitLattice*    m_exitProtonZDHitLattice;

      const PionFitSVRHitLattice*          m_pionSVRHitLattice;
      const KaonFitSVRHitLattice*          m_kaonSVRHitLattice;
      const ElectronFitSVRHitLattice*      m_electronSVRHitLattice;
      const MuonFitSVRHitLattice*          m_muonSVRHitLattice;
      const ProtonFitSVRHitLattice*        m_protonSVRHitLattice;
      const ExitElectronFitSVRHitLattice*  m_exitElectronSVRHitLattice;
      const ExitMuonFitSVRHitLattice*      m_exitMuonSVRHitLattice;
      const ExitPionFitSVRHitLattice*      m_exitPionSVRHitLattice;
      const ExitKaonFitSVRHitLattice*      m_exitKaonSVRHitLattice;
      const ExitProtonFitSVRHitLattice*    m_exitProtonSVRHitLattice;

      const PionFitSVZHitLattice*          m_pionSVZHitLattice;
      const KaonFitSVZHitLattice*          m_kaonSVZHitLattice;
      const ElectronFitSVZHitLattice*      m_electronSVZHitLattice;
      const MuonFitSVZHitLattice*          m_muonSVZHitLattice;
      const ProtonFitSVZHitLattice*        m_protonSVZHitLattice;
      const ExitElectronFitSVZHitLattice*  m_exitElectronSVZHitLattice;
      const ExitMuonFitSVZHitLattice*      m_exitMuonSVZHitLattice;
      const ExitPionFitSVZHitLattice*      m_exitPionSVZHitLattice;
      const ExitKaonFitSVZHitLattice*      m_exitKaonSVZHitLattice;
      const ExitProtonFitSVZHitLattice*    m_exitProtonSVZHitLattice;

      const SeedTrackDRHitLattice*   m_seedDRHitLattice;
      const SeedTrackZDHitLattice*   m_seedZDHitLattice;
      const SeedTrackSVRHitLattice*  m_seedSVRHitLattice;
      const SeedTrackSVZHitLattice*  m_seedSVZHitLattice;

      const HepPoint3D* m_beamSpot ;

      STL_VECTOR(FAConstPtrTable<NavShower>*) m_bremShowers;
      STL_VECTOR( const MCParticle* )*  m_mcTag ;

      // ---------- static data members ------------------------

   public:
      const Record& record() const ;
};

// inline function definitions

#endif /* NAVIGATION_NAVTRACKMASTER_H */
