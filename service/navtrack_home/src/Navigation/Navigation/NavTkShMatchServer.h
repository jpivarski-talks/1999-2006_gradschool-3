#if !defined(NAVIGATION_NAVTKSHMATCHSERVER_H)
#define NAVIGATION_NAVTKSHMATCHSERVER_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTkShMatchServer
// 
// Description: Serves up navTracks & navShowers for NavTkShMatch
//
// Usage:
//
// Author:      Brian K. Heltsley
// Created:     Tue Jun 22 12:18:35 EDT 1999
// $Id: NavTkShMatchServer.h,v 1.1 1999/08/09 16:25:42 bkh Exp $
//
// Revision history
//
// $Log: NavTkShMatchServer.h,v $
// Revision 1.1  1999/08/09 16:25:42  bkh
// Massive renaming
//
// Revision 1.1  1999/06/29 21:11:02  bkh
// New classes associated with analysis-level shower object
//

// system include files

// user include files

#include "FrameAccess/FATable.h"

#include "Navigation/NavTrack.h"
#include "Navigation/NavShower.h"

// forward declarations

class Record ;

class NavTkShMatchServer
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      NavTkShMatchServer( const Record& aRecord ) ;

      virtual ~NavTkShMatchServer();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      const NavTrack&  navTrack(
	 const NavTrack::Identifier  aNavTrackId  ) const ;

      const NavShower& navShower(
	 const NavShower::Identifier aNavShowerId ) const ;

      // ---------- static member functions --------------------
   protected:
      // ---------- protected member functions -----------------
      // ---------- protected const member functions -----------
   private:
      // ---------- Constructors and destructor ----------------
      NavTkShMatchServer( const NavTkShMatchServer& ); // stop default
      // ---------- assignment operator(s) ---------------------
      const NavTkShMatchServer& operator=( const NavTkShMatchServer& );
      // ---------- private member functions -------------------
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      const Record&        m_record    ;

      FATable< NavTrack  > m_navTrack  ;
      FATable< NavShower > m_navShower ;

      // ---------- static data members ------------------------
};

// inline function definitions

#endif /* NAVIGATION_NAVTKSHMATCHSERVER_H */
