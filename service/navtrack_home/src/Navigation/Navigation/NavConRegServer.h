#if !defined(NAVIGATION_NAVCONREGSERVER_H)
#define NAVIGATION_NAVCONREGSERVER_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavConRegServer
// 
// Description: Helper class for lazy evaluation of NavConReg's
//
// Usage: Only by NavConReg and NavConRegProxy
//
// Author:      Brian K. Heltsley
// Created:     Thu Jun 24 15:03:44 EDT 1999
// $Id: NavConRegServer.h,v 1.2 2002/03/25 11:53:06 bkh Exp $
//
// Revision history
//
// $Log: NavConRegServer.h,v $
// Revision 1.2  2002/03/25 11:53:06  bkh
// various optimizations for speed
//
// Revision 1.1  1999/08/09 16:25:37  bkh
// Massive renaming
//
// Revision 1.1  1999/06/29 21:10:58  bkh
// New classes associated with analysis-level shower object
//

// system include files

// user include files

#include "FrameAccess/FATable.h"
#include "Navigation/NavShower.h"
#include "C3cc/CcConRegAttributes.h"
#include "TrackShowerMatching/TrackShowerLattice.h"

// forward declarations

class Record ;

class NavConRegServer
{
      // ---------- friend classes and functions ---------------
   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      NavConRegServer( const Record& aRecord ) ;

      virtual ~NavConRegServer() ;

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      const NavShower& navShower(
	 const NavShower::Identifier aNavShowerId ) const ;

      const CcConRegAttributes& attributes(
	 const CcConRegAttributes::Identifier aId ) const ;

      const TrackShowerLattice& trackShowerLattice() const ;
      
      // ---------- static member functions --------------------
   protected:
      // ---------- protected member functions -----------------
      // ---------- protected const member functions -----------
   private:
      // ---------- Constructors and destructor ----------------
      NavConRegServer();
      NavConRegServer( const NavConRegServer& ); // stop default
      // ---------- assignment operator(s) ---------------------
      const NavConRegServer& operator=( const NavConRegServer& );
      // ---------- private member functions -------------------
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      const Record&                 m_record    ;
      FATable< NavShower          > m_navShower ;
      FATable< CcConRegAttributes > m_atts      ;

      const TrackShowerLattice*     m_lattice   ;

      // ---------- static data members ------------------------
};

// inline function definitions

#endif /* NAVIGATION_NAVCONREGSERVER_H */
