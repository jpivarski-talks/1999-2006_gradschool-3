#if !defined(NAVIGATION_NAVSHOWER_H)
#define NAVIGATION_NAVSHOWER_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavShower
// 
/**\class NavShower NavShower.h Navigation/NavShower.h
*/
// Description: Navigation object for calorimeter showers
//
// Usage:       Use through member functions.
//
//              Aside from CcShowerAttributes, items are not
//              actually fetched unless asked for explicitly.
//
// Author:      Brian K. Heltsley
// Created:     Tue Jun 22 10:46:34 EDT 1999
// $Id: NavShower.h,v 1.19 2003/02/03 20:16:39 bkh Exp $
//
// Revision history
//
// $Log: NavShower.h,v $
// Revision 1.19  2003/02/03 20:16:39  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.17  2002/11/21 16:52:49  bkh
// Add new NavShower fcn giving back track matches in same con reg
// Require NavTrack mcTag to have same charge as track
//
// Revision 1.16  2002/11/07 17:42:23  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
// Revision 1.15  2002/08/08 16:55:13  cleo3
// NavShower no longer owns any additional memory
//
// Revision 1.14  2002/03/26 22:46:58  bkh
// Speed up with inlining per cdj request
//
// Revision 1.13  2002/03/21 01:51:47  cdj
// NavShower::noTrackMatch now just looks at Lattice
//
// Revision 1.12  2001/12/13 20:54:36  bkh
// Implement eta-->gamgam access from NavShower just as with pi0
//
// Revision 1.11  2001/11/09 20:26:49  bkh
// Added in shower-->pi0  functionality
//
// Revision 1.10  2001/10/26 21:54:01  bkh
// Add features to showers and connected region objects
//
// Revision 1.9  2001/09/07 18:04:03  cleo3
// removed forward declaration of ostream
//
// Revision 1.8  2001/04/03 16:58:11  bkh
// Implement lists of nearby showers/tracks based on distance
//
// Revision 1.7  2000/10/04 20:00:29  bkh
// Remove pi0-vetoing from NavShower; bad idea
//
// Revision 1.6  2000/10/03 18:40:59  bkh
// Add pi0/eta access to NavShower for vetoing, etc.
//
// Revision 1.5  2000/08/11 00:21:09  bkh
// Add operator<< functionality to these classes
//
// Revision 1.4  2000/03/15 22:03:00  bkh
// add minimal doxygen trigger
//
// Revision 1.3  1999/12/06 18:13:06  bkh
// Change interface to use FATables where possible not vectors
//
// Revision 1.2  1999/08/09 16:25:39  bkh
// Massive renaming
//
// Revision 1.1  1999/06/29 21:11:01  bkh
// New classes associated with analysis-level shower object
//

// system include files

// user include files

#include "C3cc/CcShowerAttributes.h"

//#include "Navigation/NavTrack.h"
//#include "Navigation/NavTkShMatch.h"

// forward declarations


class Record              ;
class NavPi0ToGG          ;
class NavEtaToGG          ;
class NavTkShMatch        ;
class NavShowerServer     ;
class NavConReg           ;
class KTKinematicData     ;
class CcAssignedEnergyHit ;
class NavTrack            ;
class MCParticle          ;
class MCCCTagsByShower    ;

template <class T> class FAPtrTable      ;
template <class T> class FAConstPtrTable ;
template <class T> class FATable         ;
template <class T> class FAItem          ;

#include "STLUtility/fwd_vector.h"
#include "C++Std/fwd_ostream.h"
class NavShower ;


ostream& operator<<( ostream& os, const NavShower& nSh ) ;

class NavShower
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      typedef CcShowerIdType          Identifier     ;

      typedef STL_VECTOR( const NavShower* ) ShowerPtrList ;
      typedef STL_VECTOR( const NavTrack* )  TrackPtrList  ;

      // ---------- Constructors and destructor ----------------

      NavShower( const CcShowerAttributes& aAtts   ,
		 const NavShowerServer&    aServer   ) ;

      virtual ~NavShower();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      Identifier                     identifier()   const ;

      const CcShowerAttributes&      attributes()   const ;
      const NavConReg&               conReg()       const ;
      const KTKinematicData&         photon()       const ;

      DABoolean                      noTrackMatch() const ; // true= no matches
      FATable< NavTkShMatch >        trackMatches() const ; // empty= no match

      DABoolean                      noTrackMatchConReg() const ; // in con reg
      FATable< NavTkShMatch >        conRegTrackMatches() const ;

      // like above, but allows thru if not directly matched &&
      // is greater than 50% of the energy of the matched shower in the CR
      DABoolean                      noTrackMatchConReg50() const ;

      DABoolean                      noSimpleMatch() const ; // alternate
      FAItem< NavTrack >             simpleMatch()   const ; //   trk-matching
      double                         angSimpleMatch() const ;

      // the mcTag() fcn gives the MCParticle (which eventually
      // itself or through its children is responsible for the shower)
      // whose birth occurs  before it exits the DR 
      // (e.g. does not give a child of an interaction in RICH material)
      FAItem< MCParticle       >           mcTag()        const ; 

      // gives all direct tags, not unwound to parent particles
      FAItem< MCCCTagsByShower >           mcTags()       const ;

      // track which is **NOT** matched to this (unmatched) shower,
      //   which has a pion fit 3-vector pointing within 100mrad
      //   and closer than any other track
      // this is meant to signal the possibility of bremmstrahlung
      // FAItem is !valid() if this shower is matched to any track
      //   or if no track points close enough
      FAItem< NavTrack >             bremTrack()               const ;

      FATable< CcAssignedEnergyHit > assignedHits() const ;

      // container of showers that are matched to tracks, ordered by
      // distance from this shower: see distance fcn below.

      const ShowerPtrList&           nearestMatchedShowers()   const ;

      // container of showers that are not matched to tracks, ordered by
      // distance from this shower: see distance fcn below
      const ShowerPtrList&           nearestUnmatchedShowers() const ;

      // container of tracks that have a shower match, ordered by
      // distance from this shower to the 1st (highest energy) 
      // shower matched to the track: see distance fcn below
      const TrackPtrList&            nearestTracks()           const ;

      CcMeters                       distance( const NavShower& aSh ) const ;
      CcMeters                       distance( const NavTrack&  aTr ) const ;

      // a table of pi0's which have this shower as one element
      FATable< NavPi0ToGG >          pi0s()                    const ;

      // a table of eta's which have this shower as one element
      FATable< NavEtaToGG >          etas()                    const ;
      
      // flags to tell if there are any pi0s (etas) with this shower that
      // satisfy given cuts on the mass pull (sigma units) or mass itself

      // note that the sig's are signed quantities (i.e. a side band
      // could be selected with a choice of (4.5,6.5)

      DABoolean                      inPi0Pull( const double aLoSig,
						const double aHiSig ) const ;

      // straight mass cuts in GeV
      DABoolean                      inPi0GeV( const double aLoGeV,
					       const double aHiGeV ) const ;

      DABoolean                      inEtaPull( const double aLoSig,
						const double aHiSig ) const ;

      // straight mass cuts in GeV
      DABoolean                      inEtaGeV( const double aLoGeV,
					       const double aHiGeV ) const ;

      // ---------- static member functions --------------------
   protected:
      // ---------- protected member functions -----------------
      // ---------- protected const member functions -----------
   private:
      // ---------- Constructors and destructor ----------------
      NavShower( const NavShower& ); // stop default
      // ---------- assignment operator(s) ---------------------
      const NavShower& operator=( const NavShower& ); // stop default
      // ---------- private member functions -------------------
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      enum MatchedValue {kMatchFalse, kMatchTrue, kMatchUnknown};

      const CcShowerAttributes&          m_attributes   ;
      const NavShowerServer&             m_server       ;
      const NavConReg*                   m_conRegPtr    ;
      KTKinematicData*                   m_photonPtr    ;
      MatchedValue                       m_hasTrackMatch;
      const FAPtrTable< NavTkShMatch >*  m_trackMatches ;
      const FAPtrTable< NavTkShMatch >*  m_conRegTrackMatches ;
      FAPtrTable< CcAssignedEnergyHit >* m_assignedHits ;

      ShowerPtrList*                     m_nearestMatchedShowers  ;
      ShowerPtrList*                     m_nearestUnmatchedShowers  ;
      TrackPtrList*                      m_nearestTracks  ;

      const NavTrack*                    m_bremTrack ;

      DABoolean                          m_hasSimpleMatch ;
      double                             m_angSimpleMatch ;
      const NavTrack*                    m_simpleMatch ;

      const FAPtrTable< NavPi0ToGG >*     m_pi0s ;

      const FAPtrTable< NavEtaToGG >*     m_etas ;

      DABoolean                           m_conReg50 ;
      DABoolean                           m_noConReg50 ;

      // ---------- static data members ------------------------

   public:

      class CompareShowersByDistance
      {
	 public:
	    CompareShowersByDistance( const NavShower& iSh ) : sh ( iSh ){} ;
	    DABoolean operator()( const NavShower* n1 ,
				  const NavShower* n2 ) {
	       return sh.distance( *n1 ) < sh.distance( *n2 ) ;
	    }
	 private:
	    const NavShower& sh ;
      } ;

      class CompareShowerToTrack
      {
	 public:
	    CompareShowerToTrack( const NavShower& iSh ) : sh ( iSh ) {} ;
	    DABoolean operator()( const NavTrack* n1 ,
				  const NavTrack* n2   ) {
	       return sh.distance( *n1 ) < sh.distance( *n2 ) ;
	    }
	 private:
	    const NavShower& sh ;
      } ;

      const Record& record() const ;

} ;

// inline function definitions

#include "Navigation/NavShower.icc"

#endif /* NAVIGATION_NAVSHOWER_H */
