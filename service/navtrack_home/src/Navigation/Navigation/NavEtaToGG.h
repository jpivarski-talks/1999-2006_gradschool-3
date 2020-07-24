#if !defined(NAVIGATION_NAVETATOGG_H)
#define NAVIGATION_NAVETATOGG_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavEtaToGG
// 
/**\class NavEtaToGG NavEtaToGG.h Navigation/NavEtaToGG.h
*/
// Description:
//
// Usage:
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:08:29 EDT 1999
// $Id: NavEtaToGG.h,v 1.4 2003/02/03 20:16:38 bkh Exp $
//
// Revision history
//
// $Log: NavEtaToGG.h,v $
// Revision 1.4  2003/02/03 20:16:38  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.3  2001/09/07 18:04:01  cleo3
// removed forward declaration of ostream
//
// Revision 1.2  2000/03/15 22:02:59  bkh
// add minimal doxygen trigger
//
// Revision 1.1  1999/08/09 16:25:38  bkh
// Massive renaming
//

// system include files

// user include files

#include "PhotonDecays/PhdEtaToGG.h"

// forward declarations
#include "C++Std/fwd_ostream.h"

class NavShower ;

class MCParticle ;

template <class T> class FAItem;

class NavEtaToGG ;

ostream& operator<<( ostream& os, const NavEtaToGG& aEta ) ;

class NavEtaToGG
{
      // ---------- friend classes and functions ---------------
   public:
      // ---------- constants, enums and typedefs --------------

      typedef PhdEtaToGG::Identifier Identifier ;

      // ---------- Constructors and destructor ----------------

      NavEtaToGG( const PhdEtaToGG* aEta  ,
		  const NavShower*  aHiEn ,
		  const NavShower*  aLoEn   ) ;

      virtual ~NavEtaToGG();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      Identifier        identifier() const ;

      const PhdEtaToGG& eta()        const ;

      const NavShower&  hiEnShower() const ;
      const NavShower&  loEnShower() const ;

      FAItem< MCParticle > mcTag()   const ;

      // ---------- static member functions --------------------
   protected:
      // ---------- protected member functions -----------------
      // ---------- protected const member functions -----------
   private:
      // ---------- Constructors and destructor ----------------
      NavEtaToGG();
      NavEtaToGG( const NavEtaToGG& ); // stop default
      // ---------- assignment operator(s) ---------------------
      const NavEtaToGG& operator=( const NavEtaToGG& ); // stop default
      // ---------- private member functions -------------------
      // ---------- private const member functions -------------
      // ---------- data members -------------------------------

      const PhdEtaToGG* m_eta ;
      const NavShower*  m_hi ;
      const NavShower*  m_lo ;

      const MCParticle* m_mcTag ;

      // ---------- static data members ------------------------
   public:
      enum { k_QQId = 52 } ;
};

// inline function definitions
#endif /* NAVIGATION_NAVETATOGG_H */
