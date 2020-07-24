#if !defined(NAVIGATION_NAVETAPRIMETOGG_H)
#define NAVIGATION_NAVETAPRIMETOGG_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavEtaPrimeToGG
// 
/**\class NavEtaPrimeToGG NavEtaPrimeToGG.h Navigation/NavEtaPrimeToGG.h
*/
// Description:
//
// Usage:
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:08:29 EDT 1999
// $Id: NavEtaPrimeToGG.h,v 1.2 2001/09/07 18:04:01 cleo3 Exp $
//
// Revision history
//
// $Log: NavEtaPrimeToGG.h,v $
// Revision 1.2  2001/09/07 18:04:01  cleo3
// removed forward declaration of ostream
//
// Revision 1.1  2001/05/10 19:55:17  bkh
// Addition of new etaPrime class
//
// Revision 1.2  2000/03/15 22:02:59  bkh
// add minimal doxygen trigger
//
// Revision 1.1  1999/08/09 16:25:38  bkh
// Massive renaming
//

// system include files

// user include files

#include "PhotonDecays/PhdEtaPrimeToGG.h"

// forward declarations
#include "C++Std/fwd_ostream.h"

class NavShower ;

class NavEtaPrimeToGG ;

ostream& operator<<( ostream& os, const NavEtaPrimeToGG& aEtaPrime ) ;

class NavEtaPrimeToGG
{
      // ---------- friend classes and functions ---------------
   public:
      // ---------- constants, enums and typedefs --------------

      typedef PhdEtaPrimeToGG::Identifier Identifier ;

      // ---------- Constructors and destructor ----------------

      NavEtaPrimeToGG( const PhdEtaPrimeToGG* aEtaPrime  ,
		       const NavShower*       aHiEn      ,
		       const NavShower*       aLoEn        ) ;

      virtual ~NavEtaPrimeToGG();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      Identifier             identifier() const ;

      const PhdEtaPrimeToGG& etaPrime()   const ;

      const NavShower&       hiEnShower() const ;
      const NavShower&       loEnShower() const ;

      // ---------- static member functions --------------------
   protected:
      // ---------- protected member functions -----------------
      // ---------- protected const member functions -----------
   private:
      // ---------- Constructors and destructor ----------------
      NavEtaPrimeToGG();
      NavEtaPrimeToGG( const NavEtaPrimeToGG& ); // stop default
      // ---------- assignment operator(s) ---------------------
      const NavEtaPrimeToGG& operator=( const NavEtaPrimeToGG& ); 
      // ---------- private member functions -------------------
      // ---------- private const member functions -------------
      // ---------- data members -------------------------------

      const PhdEtaPrimeToGG* m_etaPrime ;
      const NavShower*       m_hi ;
      const NavShower*       m_lo ;

      // ---------- static data members ------------------------
};

// inline function definitions
#endif /* NAVIGATION_NAVETAPRIMETOGG_H */
