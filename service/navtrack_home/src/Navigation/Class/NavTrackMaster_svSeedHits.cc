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
// Author:      Michael Marsh
// Created:     Mon Jul  6 14:31:27 EDT 1998
// $Id: NavTrackMaster_svSeedHits.cc,v 1.2 2001/06/26 18:01:42 cdj Exp $
//
// Revision history
//
// $Log: NavTrackMaster_svSeedHits.cc,v $
// Revision 1.2  2001/06/26 18:01:42  cdj
// modified to work with new Lattice interface
//
// Revision 1.1  2000/04/05 15:43:02  marsh
// Split class implementations into separate files based on closely-related
// functions.  This will greatly reduce the amount of linking that most
// users will have to do.
//
// Revision 1.15  1999/12/31 23:14:27  bkh
// Eliminate obsolete checking of valid() after extracts
//
// Revision 1.14  1999/12/22 21:26:26  marsh
// Removed annoying messages when users ask for something that isn't there.
//
// Revision 1.13  1999/09/16 20:38:12  bkh
// Add track shower match info
//
// Revision 1.12  1999/08/02 19:15:21  wsun
// Initialize fit/hit Lattices for exit hypotheses.
//
// Revision 1.11  1999/06/29 21:10:40  bkh
// New classes associated with analysis-level shower object
//
// Revision 1.10  1999/05/24 16:35:52  marsh
// More Lattice interface changes.
//
// Revision 1.9  1999/05/23 21:31:39  marsh
// Updated for new Lattice interface.
//
// Revision 1.8  1999/05/11 13:07:13  marsh
// Changed LtoRmap to RightData
//
// Revision 1.7  1999/05/08 18:34:09  marsh
// Fixed Lattice interface.
//
// Revision 1.6  1999/05/08 00:45:14  marsh
// Added remaining outgoing hypotheses.
//
// Revision 1.5  1999/04/23 21:35:36  marsh
// Changed NavTrack to deliver links, not link data.
//
// Revision 1.4  1999/01/20 19:44:48  marsh
// Added missing '}'
//
// Revision 1.3  1999/01/20 19:23:35  marsh
// Protected against dereferencing null pointers.
//
// Revision 1.2  1998/12/31 22:27:49  marsh
// Release memory that was dynamically allocated.
//
// Revision 1.1.1.1  1998/12/31 18:18:31  marsh
// New library for navigation objects.
//
// Revision 1.2  1998/12/29 22:20:56  marsh
// Mods for providing Lattice info.
//
// Revision 1.1.1.1  1998/07/13 23:26:21  marsh
// New library for user-level analysis objects
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

const NavTrack::SeedSVRHitLinkTable*
NavTrackMaster::seedSVRHitLinks( NavTrack::Identifier aId ) const
{
   NavTrack::SeedSVRHitLinkTable* kNone( 0 );
   NavTrack::SeedSVRHitLinkTable* hitLinkTable;

   // If the cache is empty...
   if ( 0 == m_seedSVRHitLattice )
   {
      FAItem< SeedTrackSVRHitLattice > lattice;
      extract( dict() , lattice );
      // cast away const to fill cache
      ((NavTrackMaster*)this)->m_seedSVRHitLattice = &*lattice;
   }
   hitLinkTable = new NavTrack::SeedSVRHitLinkTable;
   const SeedTrackSVRHitLattice::Links& linksGivenLeft =
      m_seedSVRHitLattice->linksGivenLeft( aId );

   SeedTrackSVRHitLattice::Links::const_iterator finished =
         linksGivenLeft.end();
   for ( SeedTrackSVRHitLattice::Links::const_iterator link =
            linksGivenLeft.begin() ;
         link != finished ;
         ++link )
   {
      hitLinkTable->push_back( *link );
   }
   return hitLinkTable;
}

const NavTrack::SeedSVZHitLinkTable*
NavTrackMaster::seedSVZHitLinks( NavTrack::Identifier aId ) const
{
   NavTrack::SeedSVZHitLinkTable* kNone( 0 );
   NavTrack::SeedSVZHitLinkTable* hitLinkTable;

   // If the cache is empty...
   if ( 0 == m_seedSVZHitLattice )
   {
      FAItem< SeedTrackSVZHitLattice > lattice;
      extract( dict() , lattice );
      // cast away const to fill cache
      ((NavTrackMaster*)this)->m_seedSVZHitLattice = &*lattice;
   }
   hitLinkTable = new NavTrack::SeedSVZHitLinkTable;
   const SeedTrackSVZHitLattice::Links& linksGivenLeft =
      m_seedSVZHitLattice->linksGivenLeft( aId );

   SeedTrackSVZHitLattice::Links::const_iterator finished =
      linksGivenLeft.end();
   for ( SeedTrackSVZHitLattice::Links::const_iterator link =
            linksGivenLeft.begin() ;
         link != finished ;
         ++link )
   {
      hitLinkTable->push_back( *link );
   }
   return hitLinkTable;
}


const NavTrack::SVRHitTable*
NavTrackMaster::seedSVRHitTable( NavTrack::Identifier aId ) const
{
   NavTrack::SVRHitTable* kNone( 0 );
   NavTrack::SVRHitTable* hitTable;

   // If the cache is empty...
   if ( 0 == m_seedSVRHitLattice )
   {
      FAItem< SeedTrackSVRHitLattice > lattice;
      extract( dict() , lattice );
      // cast away const to fill cache
      ((NavTrackMaster*)this)->m_seedSVRHitLattice = &*lattice;
   }
   FATable< CalibratedSVRphiHit > calibratedHitTable;
   extract( dict() , calibratedHitTable );
   hitTable = new NavTrack::SVRHitTable;
   const SeedTrackSVRHitLattice::VectorRightID* farData =
      m_seedSVRHitLattice->vRightGivenLeft( aId );
   if ( 0 != farData )
   {
      SeedTrackSVRHitLattice::VectorRightID::const_iterator finished =
         farData->end();
      for ( SeedTrackSVRHitLattice::VectorRightID::const_iterator hits =
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

const NavTrack::SVZHitTable*
NavTrackMaster::seedSVZHitTable( NavTrack::Identifier aId ) const
{
   NavTrack::SVZHitTable* kNone( 0 );
   NavTrack::SVZHitTable* hitTable;

   // If the cache is empty...
   if ( 0 == m_seedSVZHitLattice )
   {
      FAItem< SeedTrackSVZHitLattice > lattice;
      extract( dict() , lattice );
      // cast away const to fill cache
      ((NavTrackMaster*)this)->m_seedSVZHitLattice = &*lattice;
   }
   FATable< CalibratedSVZHit > calibratedHitTable;
   extract( dict() , calibratedHitTable );
   hitTable = new NavTrack::SVZHitTable;
   const SeedTrackSVZHitLattice::VectorRightID* farData =
      m_seedSVZHitLattice->vRightGivenLeft( aId );
   if ( 0 != farData )
   {
      SeedTrackSVZHitLattice::VectorRightID::const_iterator finished =
         farData->end();
      for ( SeedTrackSVZHitLattice::VectorRightID::const_iterator hits =
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
