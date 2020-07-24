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
// Author:      Michael Marsh
// Created:     Mon Jul  6 14:31:23 EDT 1998
// $Id: NavTrack_svSeedHits.cc,v 1.1 2000/04/05 15:43:04 marsh Exp $
//
// Revision history
//
// $Log: NavTrack_svSeedHits.cc,v $
// Revision 1.1  2000/04/05 15:43:04  marsh
// Split class implementations into separate files based on closely-related
// functions.  This will greatly reduce the amount of linking that most
// users will have to do.
//
// Revision 1.7  1999/11/17 20:34:19  marsh
// Lattice information is now returned with pointers instead of references.
// Track-shower matches are returned as FAItems instead of pointers.
// There is no more hypothesis faking for missing data.
//
// Revision 1.6  1999/10/06 22:16:20  marsh
// Removed "void" from argument lists.
//
// Revision 1.5  1999/09/16 20:38:11  bkh
// Add track shower match info
//
// Revision 1.4  1999/05/08 00:45:13  marsh
// Added remaining outgoing hypotheses.
//
// Revision 1.3  1999/01/19 21:27:58  marsh
// Added tests on caches before returning.  If the caches are still
// empty, return references to empty tables.
//
// Revision 1.2  1998/12/31 22:27:46  marsh
// Release memory that was dynamically allocated.
//
// Revision 1.1.1.1  1998/12/31 18:18:31  marsh
// New library for navigation objects.
//
// Revision 1.2  1998/12/29 22:20:55  marsh
// Mods for providing Lattice info.
//
// Revision 1.1.1.1  1998/07/13 23:26:21  marsh
// New library for user-level analysis objects
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

const NavTrack::SeedSVRHitLinkTable*
NavTrack::seedSVRHitLinks() const
{
   // If the cache is empty...
   if ( 0 == m_seedSVRHitLinks )
   {
      // cast away const to fill cache from master block
      ((NavTrack*)this)->m_seedSVRHitLinks =
         ((const NavTrackMaster&)masterBlock()).seedSVRHitLinks(
            identifier() );
   }

   // return the object in the cache
   return m_seedSVRHitLinks;
}

const NavTrack::SeedSVZHitLinkTable*
NavTrack::seedSVZHitLinks() const
{
   // If the cache is empty...
   if ( 0 == m_seedSVZHitLinks )
   {
      // cast away const to fill cache from master block
      ((NavTrack*)this)->m_seedSVZHitLinks =
         ((const NavTrackMaster&)masterBlock()).seedSVZHitLinks(
            identifier() );
   }

   // return the object in the cache
   return m_seedSVZHitLinks;
}

const NavTrack::SVRHitTable*
NavTrack::seedSVRHits() const
{
   // If the cache is empty...
   if ( 0 == m_seedSVRHitTable )
   {
      // cast away const to fill cache from master block
      ((NavTrack*)this)->m_seedSVRHitTable =
         ((const NavTrackMaster&)masterBlock()).seedSVRHitTable(
            identifier() );
   }

   // return the object in the cache
   return m_seedSVRHitTable;
}

const NavTrack::SVZHitTable*
NavTrack::seedSVZHits() const
{
   // If the cache is empty...
   if ( 0 == m_seedSVZHitTable )
   {
      // cast away const to fill cache from master block
      ((NavTrack*)this)->m_seedSVZHitTable =
         ((const NavTrackMaster&)masterBlock()).seedSVZHitTable(
            identifier() );
   }

   // return the object in the cache
   return m_seedSVZHitTable;
}
