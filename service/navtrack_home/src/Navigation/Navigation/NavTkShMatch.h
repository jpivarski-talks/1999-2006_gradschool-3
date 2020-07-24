#if !defined(NAVIGATION_NAVTKSHMATCH_H)
#define NAVIGATION_NAVTKSHMATCH_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTkShMatch
// 
/**\class NavTkShMatch NavTkShMatch.h Navigation/NavTkShMatch.h
*/
// Description: Charged track - calorimeter shower match object
//
// Usage: 
//
// Author:      Brian K. Heltsley
// Created:     Mon Jun 21 17:28:48 EDT 1999
// $Id: NavTkShMatch.h,v 1.7 2001/12/13 20:54:37 bkh Exp $
//
// Revision history
//
// $Log: NavTkShMatch.h,v $
// Revision 1.7  2001/12/13 20:54:37  bkh
// Implement eta-->gamgam access from NavShower just as with pi0
//
// Revision 1.6  2001/09/07 18:04:04  cleo3
// removed forward declaration of ostream
//
// Revision 1.5  2001/04/03 16:58:12  bkh
// Implement lists of nearby showers/tracks based on distance
//
// Revision 1.4  2000/08/11 00:21:09  bkh
// Add operator<< functionality to these classes
//
// Revision 1.3  2000/03/15 22:03:00  bkh
// add minimal doxygen trigger
//
// Revision 1.2  1999/12/06 18:13:07  bkh
// Change interface to use FATables where possible not vectors
//
// Revision 1.1  1999/08/09 16:25:41  bkh
// Massive renaming
//
// Revision 1.1  1999/06/29 21:11:02  bkh
// New classes associated with analysis-level shower object
//

// system include files

// user include files
#include "C3cc/CcTypes.h"

#include "Navigation/NavTrack.h"
#include "Navigation/NavShower.h"
#include "TrackShowerMatching/TrackShowerLattice.h"

// forward declarations

//class NavShower ;
class NavTkShMatchServer ;
template <class T> class FAConstPtrTable ;
template <class T> class FATable         ;

#include "STLUtility/fwd_vector.h"
#include "C++Std/fwd_ostream.h"

class NavTkShMatch ;


ostream& operator<<( ostream& os, const NavTkShMatch& nTSM ) ;


class NavTkShMatch
{
      // ---------- friend classes and functions ---------------
   public:
      // ---------- constants, enums and typedefs --------------

      typedef UInt16                         Identifier   ;
      typedef NavTrack::Identifier           TrackId      ;
      typedef CcShowerIdType                 ShowerId     ;
      typedef STL_VECTOR( ShowerId )         ShowerIdList ;
      typedef TrackShowerLattice::Link       LatticeLink  ;

      // ---------- Constructors and destructor ----------------

      NavTkShMatch( const Identifier              aId     ,
		    const LatticeLink&            aLink   ,
		    const NavTkShMatchServer& aServer   ) ;

      virtual ~NavTkShMatch();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      Identifier           identifier()    const ;
      const NavTrack&      navTrack()      const ;
      FATable< NavShower > navShowers()    const ;
      CcGeV                matchedEnergy() const ;
      CcReal               eOverP()        const ;
      CcMeters             distance()      const ; // to nearest shower ctr

      TrackId              trackId()       const ; // doesn't access NavTrack
      const ShowerIdList&  showerIds()     const ; // doesn't access NavShower

      // vect of links to unmatched showers, sorted by distance? 

      // ---------- static member functions --------------------
   protected:
      // ---------- protected member functions -----------------
      // ---------- protected const member functions -----------
   private:
      // ---------- Constructors and destructor ----------------
      NavTkShMatch( const NavTkShMatch& ); // stop default
      // ---------- assignment operator(s) ---------------------
      const NavTkShMatch& operator=( const NavTkShMatch& );
      // ---------- private member functions -------------------
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      Identifier                    m_identifier    ;
      const LatticeLink&            m_link          ;
      const NavTkShMatchServer&     m_server        ;
      const NavTrack*               m_navTrack      ;
      FAConstPtrTable< NavShower >* m_showers       ;

      CcGeV                         m_ener          ;
      double                        m_eOverP        ;

      // ---------- static data members ------------------------
};

// inline function definitions

#endif /* NAVIGATION_NAVTKSHMATCH_H */
