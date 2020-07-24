// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrack
// 
// Description: User-level navigation object for tracks.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Oct  8 13:12:09 EDT 2003
// $Id: NavTrack_zdSeedHits.cc,v 1.1 2003/10/16 18:42:12 mccann Exp $
//
// Revision history
//
// $Log: NavTrack_zdSeedHits.cc,v $
// Revision 1.1  2003/10/16 18:42:12  mccann
// adding ZD files
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "Navigation/NavTrack.h"
#include "Navigation/NavTrackMaster.h"

// STL classes
#include <map>
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrack" ;

//
// static data member definitions
//

const NavTrack::SeedZDHitLinkTable*
NavTrack::seedZDHitLinks() const
{
   // If the cache is empty...
   if ( 0 == m_seedZDHitLinks )
   {
      // cast away const to fill cache from master block
      ((NavTrack*)this)->m_seedZDHitLinks =
         ((const NavTrackMaster&)masterBlock()).seedZDHitLinks(
            identifier() );
   }

   // return the object in the cache
   return m_seedZDHitLinks;
}

const NavTrack::ZDHitTable*
NavTrack::seedZDHits() const
{
   // If the cache is empty...
   if ( 0 == m_seedZDHitTable )
   {
      // cast away const to fill cache from master block
      ((NavTrack*)this)->m_seedZDHitTable =
         ((const NavTrackMaster&)masterBlock()).seedZDHitTable( identifier() );
   }

   // return the object in the cache
   return m_seedZDHitTable;
}
