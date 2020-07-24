// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrackMaster
// 
// Description: Master block for NavTrack object, inherits from TDTrackMaster.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Oct  8 13:10:07 EDT 2003
// $Id: NavTrackMaster_zdSeedHits.cc,v 1.1 2003/10/16 18:42:08 mccann Exp $
//
// Revision history
//
// $Log: NavTrackMaster_zdSeedHits.cc,v $
// Revision 1.1  2003/10/16 18:42:08  mccann
// adding ZD files
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "Navigation/NavTrackMaster.h"

#include "FrameAccess/extract.h"

// STL classes
#include <map>
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrackMaster" ;

const NavTrack::SeedZDHitLinkTable*
NavTrackMaster::seedZDHitLinks( NavTrack::Identifier aId ) const
{
   NavTrack::SeedZDHitLinkTable* kNone( 0 );
   NavTrack::SeedZDHitLinkTable* hitLinkTable;

   // If the cache is empty...
   if ( 0 == m_seedZDHitLattice )
   {
      FAItem< SeedTrackZDHitLattice > lattice;
      extract( dict() , lattice );
      // cast away const to fill cache
      ((NavTrackMaster*)this)->m_seedZDHitLattice = &*lattice;
   }
   hitLinkTable = new NavTrack::SeedZDHitLinkTable;
   const SeedTrackZDHitLattice::Links& linksGivenLeft =
      m_seedZDHitLattice->linksGivenLeft( aId );
   SeedTrackZDHitLattice::Links::const_iterator finished =
      linksGivenLeft.end();
   for ( SeedTrackZDHitLattice::Links::const_iterator link =
            linksGivenLeft.begin() ;
         link != finished ;
         ++link )
   {
      hitLinkTable->push_back( *link );
   }
   return hitLinkTable;
}

const NavTrack::ZDHitTable*
NavTrackMaster::seedZDHitTable( NavTrack::Identifier aId ) const
{
   NavTrack::ZDHitTable* kNone( 0 );
   NavTrack::ZDHitTable* hitTable;

   // If the cache is empty...
   if ( 0 == m_seedZDHitLattice )
   {
      FAItem< SeedTrackZDHitLattice > lattice;
      extract( dict() , lattice );
      // cast away const to fill cache
      ((NavTrackMaster*)this)->m_seedZDHitLattice = &*lattice;
   }
   FATable< CalibratedZDHit > calibratedHitTable;
   extract( dict() , calibratedHitTable );
   hitTable = new NavTrack::ZDHitTable;
   const SeedTrackZDHitLattice::VectorRightID* farData =
      m_seedZDHitLattice->vRightGivenLeft( aId );
   if ( 0 != farData )
   {
      SeedTrackZDHitLattice::VectorRightID::const_iterator finished =
         farData->end();
      for ( SeedTrackZDHitLattice::VectorRightID::const_iterator hits =
               farData->begin() ;
            hits != finished ;
            ++hits )
      {
         if ( calibratedHitTable.valid() )
         {
            hitTable->push_back(
               &*(calibratedHitTable.find( *hits ) ) );
         }
         else
         {
            hitTable->push_back( 0 );
         }
      }
   }
   return hitTable;
}
