#if !defined(NAVIGATION_NAVPI0TOGG_H)
#define NAVIGATION_NAVPI0TOGG_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavPi0ToGG
// 
/**\class NavPi0ToGG NavPi0ToGG.h Navigation/NavPi0ToGG.h
*/
// Description:
//
// Usage:
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:08:29 EDT 1999
// $Id: NavPi0ToGG.h,v 1.6 2003/02/03 20:16:38 bkh Exp $
//
// Revision history
//
// $Log: NavPi0ToGG.h,v $
// Revision 1.6  2003/02/03 20:16:38  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.5  2002/11/07 17:42:22  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
// Revision 1.4  2001/11/13 16:06:04  bkh
// Separated off pi0 pieces to avoid making everyone link to PhotonDecays
//
// Revision 1.3  2001/09/07 18:04:02  cleo3
// removed forward declaration of ostream
//
// Revision 1.2  2000/03/15 22:03:00  bkh
// add minimal doxygen trigger
//
// Revision 1.1  1999/08/09 16:25:38  bkh
// Massive renaming
//

// system include files

// user include files

//#include "PhotonDecays/PhdPi0.h"

// forward declarations
#include "C++Std/fwd_ostream.h"

class PhdPi0 ;

class NavShower ;

class MCParticle ;

class NavPi0ToGG ;

ostream& operator<<( ostream& os, const NavPi0ToGG& aPi0 ) ;

template <class T> class FAItem;

class NavPi0ToGG
{
      // ---------- friend classes and functions ---------------
   public:
      // ---------- constants, enums and typedefs --------------

      typedef UInt16 Identifier ;

      // ---------- Constructors and destructor ----------------

      NavPi0ToGG( const PhdPi0*    aPi0  ,
		  const NavShower* aHiEn ,
		  const NavShower* aLoEn   ) ;

      virtual ~NavPi0ToGG();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      Identifier       identifier() const ;

      const PhdPi0&    pi0()        const ;

      const NavShower& hiEnShower() const ;
      const NavShower& loEnShower() const ;

      FAItem< MCParticle > mcTag()  const ;

      // ---------- static member functions --------------------
   protected:
      // ---------- protected member functions -----------------
      // ---------- protected const member functions -----------
   private:
      // ---------- Constructors and destructor ----------------
      NavPi0ToGG();
      NavPi0ToGG( const NavPi0ToGG& ); // stop default
      // ---------- assignment operator(s) ---------------------
      const NavPi0ToGG& operator=( const NavPi0ToGG& ); // stop default
      // ---------- private member functions -------------------
      // ---------- private const member functions -------------
      // ---------- data members -------------------------------

      const PhdPi0*    m_pi0 ;
      const NavShower* m_hi ;
      const NavShower* m_lo ;

      const MCParticle* m_mcTag ;

      // ---------- static data members ------------------------
   public:
      enum { k_QQId = 51 } ;
};

// inline function definitions
#endif /* NAVIGATION_NAVPI0TOGG_H */
