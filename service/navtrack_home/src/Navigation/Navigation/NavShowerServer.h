#if !defined(NAVIGATION_NAVSHOWERSERVER_H)
#define NAVIGATION_NAVSHOWERSERVER_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavShowerServer
// 
// Description: Helper class for lazy evaluation of fcns in NavShower
//
// Usage: Only by NavShower and NavShowerProxy
//
// Author:      Brian K. Heltsley
// Created:     Fri Jun 25 17:28:56 EDT 1999
// $Id: NavShowerServer.h,v 1.17 2003/02/03 20:16:39 bkh Exp $
//
// Revision history
//
// $Log: NavShowerServer.h,v $
// Revision 1.17  2003/02/03 20:16:39  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.15  2002/11/21 16:52:50  bkh
// Add new NavShower fcn giving back track matches in same con reg
// Require NavTrack mcTag to have same charge as track
//
// Revision 1.14  2002/11/07 17:42:23  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
// Revision 1.13  2002/08/08 16:55:14  cleo3
// NavShower no longer owns any additional memory
//
// Revision 1.12  2002/04/10 18:46:26  bkh
// Speed up way connected region information is obtained with direct
// lattice access
//
// Revision 1.11  2002/03/21 01:51:48  cdj
// NavShower::noTrackMatch now just looks at Lattice
//
// Revision 1.10  2001/12/13 20:54:37  bkh
// Implement eta-->gamgam access from NavShower just as with pi0
//
// Revision 1.9  2001/11/13 16:06:05  bkh
// Separated off pi0 pieces to avoid making everyone link to PhotonDecays
//
// Revision 1.8  2001/11/09 20:26:50  bkh
// Added in shower-->pi0  functionality
//
// Revision 1.7  2001/10/26 21:54:01  bkh
// Add features to showers and connected region objects
//
// Revision 1.6  2001/04/03 16:58:11  bkh
// Implement lists of nearby showers/tracks based on distance
//
// Revision 1.5  2000/10/04 20:00:30  bkh
// Remove pi0-vetoing from NavShower; bad idea
//
// Revision 1.4  2000/10/03 18:40:59  bkh
// Add pi0/eta access to NavShower for vetoing, etc.
//
// Revision 1.3  1999/12/06 18:13:06  bkh
// Change interface to use FATables where possible not vectors
//
// Revision 1.2  1999/08/09 16:25:41  bkh
// Massive renaming
//
// Revision 1.1  1999/06/29 21:11:01  bkh
// New classes associated with analysis-level shower object
//

// system include files
#include <vector>

// user include files

#include "FrameAccess/FATable.h"
#include "Navigation/NavShower.h"
#include "Navigation/NavConReg.h"
#include "C3cc/CcEnergyHit.h"
#include "C3cc/CcHitShowerLattice.h"
#include "Navigation/NavTkShMatch.h"
#include "Navigation/NavTrack.h"

#include "TrackShowerMatching/TrackShowerLattice.h"
#include "C3cc/CcConRegShowerLattice.h"
// forward declarations
#include "STLUtility/fwd_vector.h"

class Record ;
template <class T> class FAConstPtrTable;

class MCParticle ;
class MCCCTagsByShower   ;

class NavShowerServer
{
      // ---------- friend classes and functions ---------------
   public:
      // ---------- constants, enums and typedefs --------------
      // ---------- Constructors and destructor ----------------

      NavShowerServer( const Record& aRecord ) ;

      virtual ~NavShowerServer();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      const NavShower&                   navShower(
	 const NavShower::Identifier aNavShowerId     ) const ;
						      
      FATable< NavShower >               navShowers()   const ;
						      
      FATable< NavTrack >                navTracks()    const ;
						      
      FATable< NavTkShMatch >            tkShMatches()  const ;

      DABoolean                          hasTrackMatch(
	 const NavShower::Identifier aNavShowerId     ) const;

      const NavTrack*                    simpleMatch(
	 const NavShower::Identifier aNavShowerId     ) const ;

      double                             angSimpleMatch(
	 const NavShower::Identifier aNavShowerId     ) const ;

      const NavConReg&                   conReg(      
	 const NavShower::Identifier aNavShowerId     ) const ;
						      
      const CcEnergyHit&                 energyHit(   
	 const CcEnergyHit::Identifier aHitId         ) const ;
						      
      const CcHitShowerLattice::Links&   links(	      
	 const NavShower::Identifier aNavShowerId     ) const ;
						      
      const FAPtrTable<NavPi0ToGG>* matchedPi0s(
	 const NavShower::Identifier aNavShowerId     ) const ;

      const FAPtrTable<NavEtaToGG>* matchedEtas(
	 const NavShower::Identifier aNavShowerId     ) const ;

      const FAPtrTable<NavTkShMatch>* matchedTracks(
	 const NavShower::Identifier aNavShowerId     ) const;

      const FAPtrTable<NavTkShMatch>* conRegMatchedTracks(
	 const NavShower::Identifier aNavShowerId     ) const;

      const FAPtrTable<NavPi0ToGG>* navPi0s()  const ;
						      
      const FAPtrTable<NavEtaToGG>* navEtas()  const ;

      const MCParticle*       mcTag( 
	 const NavShower::Identifier aNavShowerId ) const ;

      const MCCCTagsByShower* mcTags( 
	 const NavShower::Identifier aNavShowerId ) const ;

      const Record& record() const ;

      // ---------- static member functions --------------------
   protected:
      // ---------- protected member functions -----------------
      // ---------- protected const member functions -----------
   private:
      // ---------- Constructors and destructor ----------------
      NavShowerServer(); // stop default
      NavShowerServer( const NavShowerServer& ); // stop default
      // ---------- assignment operator(s) ---------------------
      const NavShowerServer& operator=( const NavShowerServer& );
      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      double simpleMatch ( const NavShower*& aShower ,
			   const NavTrack&   aTrack    ) const ;

      // ---------- data members -------------------------------

      const Record& m_record ;

      FATable< NavShower          > m_navShower  ;
      FATable< NavTkShMatch       > m_match      ;
      const TrackShowerLattice*     m_tkShLattice;
      FATable< NavConReg          > m_conReg     ;
      FATable< CcEnergyHit        > m_energyHits ;
      FAItem<  CcHitShowerLattice > m_lattice    ;
      FATable< NavTrack           > m_navTrack   ;
      const FAPtrTable<NavPi0ToGG>*    m_navPi0     ;
      const FAPtrTable<NavEtaToGG>*    m_navEta     ; 
      STL_VECTOR(FAConstPtrTable<NavPi0ToGG>* ) m_matchedPi0s;
      STL_VECTOR(FAConstPtrTable<NavEtaToGG>* ) m_matchedEtas;
      STL_VECTOR(FAConstPtrTable<NavTkShMatch>* ) m_matchedTracks;
      STL_VECTOR(FAConstPtrTable<NavTkShMatch>* ) m_conRegMatchedTracks;

      STL_VECTOR( const MCParticle* )* m_mcTag ;
      const FAPtrTable< MCCCTagsByShower >*    m_mcTags ;

      STL_VECTOR( const NavTrack* )*  m_simpleMatch ;
      STL_VECTOR( double )*           m_angSimpleMatch ;

      const CcConRegShowerLattice* m_crLattice ;
      // ---------- static data members ------------------------
};

// inline function definitions

#endif /* NAVIGATION_NAVSHOWERSERVER_H */
