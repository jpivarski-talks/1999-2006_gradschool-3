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
// $Id: NavTrack_drHits.cc,v 1.1 2000/04/05 15:43:03 marsh Exp $
//
// Revision history
//
// $Log: NavTrack_drHits.cc,v $
// Revision 1.1  2000/04/05 15:43:03  marsh
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

const NavTrack::DRHitLinkTable*
NavTrack::drHitLinks( DBCandidate::Hypo aHypo ) const
{
   // If the cache is empty...
   if ( 0 == m_drHitLinks[ aHypo ] )
   {
      // cast away const to fill cache from master block
      ((NavTrack*)this)->m_drHitLinks[ aHypo ] =
         ((const NavTrackMaster&)masterBlock()).drHitLinks( identifier(),
                                                            aHypo );
   }

   // return the object in the cache
   return m_drHitLinks[ aHypo ];
}

const NavTrack::DRHitLinkTable*
NavTrack::pionDRHitLinks() const
{
   return drHitLinks( DBCandidate::kChargedPion );
}

const NavTrack::DRHitLinkTable*
NavTrack::kaonDRHitLinks() const
{
   return drHitLinks( DBCandidate::kChargedKaon );
}

const NavTrack::DRHitLinkTable*
NavTrack::electronDRHitLinks() const
{
   return drHitLinks( DBCandidate::kElectron );
}

const NavTrack::DRHitLinkTable*
NavTrack::muonDRHitLinks() const
{
   return drHitLinks( DBCandidate::kMuon );
}

const NavTrack::DRHitLinkTable*
NavTrack::protonDRHitLinks() const
{
   return drHitLinks( DBCandidate::kProton );
}

const NavTrack::DRHitLinkTable*
NavTrack::exitElectronDRHitLinks() const
{
   return drHitLinks( DBCandidate::kExitElectron );
}

const NavTrack::DRHitLinkTable*
NavTrack::exitMuonDRHitLinks() const
{
   return drHitLinks( DBCandidate::kExitMuon );
}

const NavTrack::DRHitLinkTable*
NavTrack::exitPionDRHitLinks() const
{
   return drHitLinks( DBCandidate::kExitPion );
}

const NavTrack::DRHitLinkTable*
NavTrack::exitKaonDRHitLinks() const
{
   return drHitLinks( DBCandidate::kExitKaon );
}

const NavTrack::DRHitLinkTable*
NavTrack::exitProtonDRHitLinks() const
{
   return drHitLinks( DBCandidate::kExitProton );
}


const NavTrack::DRHitTable*
NavTrack::drHits( DBCandidate::Hypo aHypo ) const
{
   // If the cache is empty...
   if ( 0 == m_drHitTables[ aHypo ] )
   {
      // cast away const to fill cache from master block
      ((NavTrack*)this)->m_drHitTables[ aHypo ] =
         ((const NavTrackMaster&)masterBlock()).drHits( identifier(), aHypo );
   }

   // return the object in the cache
   return m_drHitTables[ aHypo ];
}

const NavTrack::DRHitTable*
NavTrack::pionDRHits() const
{
   return drHits( DBCandidate::kChargedPion );
}

const NavTrack::DRHitTable*
NavTrack::kaonDRHits() const
{
   return drHits( DBCandidate::kChargedKaon );
}

const NavTrack::DRHitTable*
NavTrack::electronDRHits() const
{
   return drHits( DBCandidate::kElectron );
}

const NavTrack::DRHitTable*
NavTrack::muonDRHits() const
{
   return drHits( DBCandidate::kMuon );
}

const NavTrack::DRHitTable*
NavTrack::protonDRHits() const
{
   return drHits( DBCandidate::kProton );
}

const NavTrack::DRHitTable*
NavTrack::exitElectronDRHits() const
{
   return drHits( DBCandidate::kExitElectron );
}

const NavTrack::DRHitTable*
NavTrack::exitMuonDRHits() const
{
   return drHits( DBCandidate::kExitMuon );
}

const NavTrack::DRHitTable*
NavTrack::exitPionDRHits() const
{
   return drHits( DBCandidate::kExitPion );
}

const NavTrack::DRHitTable*
NavTrack::exitKaonDRHits() const
{
   return drHits( DBCandidate::kExitKaon );
}

const NavTrack::DRHitTable*
NavTrack::exitProtonDRHits() const
{
   return drHits( DBCandidate::kExitProton );
}
