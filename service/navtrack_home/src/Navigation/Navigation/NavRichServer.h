#if !defined(NAVIGATION_NAVRICHSERVER_H)
#define NAVIGATION_NAVRICHSERVER_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavRichServer
// 
// Description: Helper class for lazy evaluation of fcns in NavRich
//
// Usage: Only by NavRich and NavRichProxy
//
// Author:      Tomasz Skwarnicki
// Created:     Fri Jul 20 17:28:56 EDT 2000
// $Id: NavRichServer.h,v 1.3 2001/10/01 00:37:14 cdj Exp $
//
// Revision history
//
// $Log: NavRichServer.h,v $
// Revision 1.3  2001/10/01 00:37:14  cdj
// switched to RichAngles package
//
// Revision 1.2  2000/11/20 00:05:59  ts
// build NavRich on top of RichTrackPrototype
//
// Revision 1.1  2000/07/27 01:32:05  ts
// NavRich added
//
//

// system include files

// user include files

#include "FrameAccess/FATable.h"
#include "FrameAccess/FAItem.h"
//#include "Navigation/NavRich.h"
#include "RichTracksProd/RichTrack.h"
#include "RichImprovedTracksProd/RichImprovedTrack.h"
#include "Navigation/NavTrack.h"
#include "RichAverageAnglesProd/RichTrackAveAngles.h"
#include "RichTrackClusterMatchProd/RichTrackChamberCoord.h"
#include "RichTrackClusterMatchProd/RichTrackCRegionMatch.h"
#include "RichClustChamberCoord/RichCRegionChamberCoord.h"
#include "RichClustering/RichConnectedRegion.h"
#include "RichAngles/RichTrackRadiators.h"
#include "RichAngles/RichTrackPhotons.h"
#include "RichPhotonsProd/RichPhoton.h"
#include "RichClustering/RichBump.h"
#include "RichCalibratedHits/RichCalibratedHit.h"
#include "RichTrackPrototypesProd/RichTracksMethod.h"

// forward declarations

class Record ;

class NavRichServer
{
      // ---------- friend classes and functions ---------------
   public:
      // ---------- constants, enums and typedefs --------------
      // ---------- Constructors and destructor ----------------

      NavRichServer( const Record& aRecord ) ;

      virtual ~NavRichServer();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

						      
      FAItem< RichTrack > richTrack(UInt32 aId) const;

      FAItem< RichImprovedTrack > richImprovedTrack(UInt32 aId) const;

      FAItem< NavTrack > navTrack(UInt32 aId) const;

      FAItem< RichTrackAveAngles > richTrackAveAngles(UInt32 aId) const; 

      FAItem< RichTrackChamberCoord > richTrackChamberCoord(UInt32 aId) const;

      FAItem< RichTrackCRegionMatch > richTrackCRegionMatch(UInt32 aId) const;

      FAItem< RichCRegionChamberCoord > richCRegionChamberCoord(UInt32 aId) const;

      FAItem< RichConnectedRegion > richConnectedRegion(UInt32 aId) const;

      FAItem< RichTrackRadiators > richTrackRadiators(UInt32 aId) const;

      FAItem< RichTrackPhotons > richTrackPhotons(UInt32 aId) const;

      FAItem< RichPhoton > richPhoton(UInt32 aId) const;

      FAItem< RichBump > richBump(UInt32 aId) const;

      FAItem< RichCalibratedHit > richCalibratedHit(UInt32 aId) const;
 
      FAItem< RichTracksMethod > richTracksMethod() const;

      void geomUpdate() const;

      // ---------- static member functions --------------------
   protected:
      // ---------- protected member functions -----------------
      // ---------- protected const member functions -----------
   private:
      // ---------- Constructors and destructor ----------------
      NavRichServer(); // stop default
      NavRichServer( const NavRichServer& ); // stop default
      // ---------- assignment operator(s) ---------------------
      const NavRichServer& operator=( const NavRichServer& );
      // ---------- private member functions -------------------
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      const Record& m_record ;


      FATable< RichTrack > m_RichTrack ;
      FATable< RichImprovedTrack > m_RichImprovedTrack ;
      FATable< NavTrack > m_NavTrack ;
      FATable< RichTrackAveAngles > m_RichTrackAveAngles;
      FATable< RichTrackChamberCoord > m_RichTrackChamberCoord;
      FATable< RichTrackCRegionMatch > m_RichTrackCRegionMatch; 
      FATable< RichCRegionChamberCoord > m_RichCRegionChamberCoord;
      FATable< RichConnectedRegion > m_RichConnectedRegion;
      FATable< RichTrackRadiators > m_RichTrackRadiators;

      FATable< RichTrackPhotons > m_RichTrackPhotons;
      FATable< RichPhoton > m_RichPhoton; 
      FATable< RichBump > m_RichBump; 
      FATable< RichCalibratedHit > m_RichCalibratedHit; 

      FAItem< RichTracksMethod> m_RichTracksMethod;

      DABoolean m_RichGeometryUpdated;

      // ---------- static data members ------------------------
};

// inline function definitions

#endif /* NAVIGATION_NAVRICHSERVER_H */
