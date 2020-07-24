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
// $Id: NavTrack_svHits.cc,v 1.1 2000/04/05 15:43:03 marsh Exp $
//
// Revision history
//
// $Log: NavTrack_svHits.cc,v $
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

const NavTrack::SVRHitLinkTable*
NavTrack::svrHitLinks( DBCandidate::Hypo aHypo ) const
{
   // If the cache is empty...
   if ( 0 == m_svrHitLinks[ aHypo ] )
   {
      // cast away const to fill cache from master block
      ((NavTrack*)this)->m_svrHitLinks[ aHypo ] =
         ((const NavTrackMaster&)masterBlock()).svrHitLinks( identifier(),
                                                             aHypo );
   }

   // return the object in the cache
   return m_svrHitLinks[ aHypo ];
}

const NavTrack::SVRHitLinkTable*
NavTrack::pionSVRHitLinks() const
{
   return svrHitLinks( DBCandidate::kChargedPion );
}

const NavTrack::SVRHitLinkTable*
NavTrack::kaonSVRHitLinks() const
{
   return svrHitLinks( DBCandidate::kChargedKaon );
}

const NavTrack::SVRHitLinkTable*
NavTrack::electronSVRHitLinks() const
{
   return svrHitLinks( DBCandidate::kElectron );
}

const NavTrack::SVRHitLinkTable*
NavTrack::muonSVRHitLinks() const
{
   return svrHitLinks( DBCandidate::kMuon );
}

const NavTrack::SVRHitLinkTable*
NavTrack::protonSVRHitLinks() const
{
   return svrHitLinks( DBCandidate::kProton );
}

const NavTrack::SVRHitLinkTable*
NavTrack::exitElectronSVRHitLinks() const
{
   return svrHitLinks( DBCandidate::kExitElectron );
}

const NavTrack::SVRHitLinkTable*
NavTrack::exitMuonSVRHitLinks() const
{
   return svrHitLinks( DBCandidate::kExitMuon );
}

const NavTrack::SVRHitLinkTable*
NavTrack::exitPionSVRHitLinks() const
{
   return svrHitLinks( DBCandidate::kExitPion );
}

const NavTrack::SVRHitLinkTable*
NavTrack::exitKaonSVRHitLinks() const
{
   return svrHitLinks( DBCandidate::kExitKaon );
}

const NavTrack::SVRHitLinkTable*
NavTrack::exitProtonSVRHitLinks() const
{
   return svrHitLinks( DBCandidate::kExitProton );
}

const NavTrack::SVZHitLinkTable*
NavTrack::svzHitLinks( DBCandidate::Hypo aHypo ) const
{
   // If the cache is empty...
   if ( 0 == m_svzHitLinks[ aHypo ] )
   {
      // cast away const to fill cache from master block
      ((NavTrack*)this)->m_svzHitLinks[ aHypo ] =
         ((const NavTrackMaster&)masterBlock()).svzHitLinks( identifier(),
                                                             aHypo );
   }

   // return the object in the cache
   return m_svzHitLinks[ aHypo ];
}

const NavTrack::SVZHitLinkTable*
NavTrack::pionSVZHitLinks() const
{
   return svzHitLinks( DBCandidate::kChargedPion );
}

const NavTrack::SVZHitLinkTable*
NavTrack::kaonSVZHitLinks() const
{
   return svzHitLinks( DBCandidate::kChargedKaon );
}

const NavTrack::SVZHitLinkTable*
NavTrack::electronSVZHitLinks() const
{
   return svzHitLinks( DBCandidate::kElectron );
}

const NavTrack::SVZHitLinkTable*
NavTrack::muonSVZHitLinks() const
{
   return svzHitLinks( DBCandidate::kMuon );
}

const NavTrack::SVZHitLinkTable*
NavTrack::protonSVZHitLinks() const
{
   return svzHitLinks( DBCandidate::kProton );
}

const NavTrack::SVZHitLinkTable*
NavTrack::exitElectronSVZHitLinks() const
{
   return svzHitLinks( DBCandidate::kExitElectron );
}

const NavTrack::SVZHitLinkTable*
NavTrack::exitMuonSVZHitLinks() const
{
   return svzHitLinks( DBCandidate::kExitMuon );
}

const NavTrack::SVZHitLinkTable*
NavTrack::exitPionSVZHitLinks() const
{
   return svzHitLinks( DBCandidate::kExitPion );
}

const NavTrack::SVZHitLinkTable*
NavTrack::exitKaonSVZHitLinks() const
{
   return svzHitLinks( DBCandidate::kExitKaon );
}

const NavTrack::SVZHitLinkTable*
NavTrack::exitProtonSVZHitLinks() const
{
   return svzHitLinks( DBCandidate::kExitProton );
}


const NavTrack::SVRHitTable*
NavTrack::svrHits( DBCandidate::Hypo aHypo ) const
{
   // If the cache is empty...
   if ( 0 == m_svrHitTables[ aHypo ] )
   {
      // cast away const to fill cache from master block
      ((NavTrack*)this)->m_svrHitTables[ aHypo ] =
         ((const NavTrackMaster&)masterBlock()).svrHits( identifier(), aHypo );
   }

   // return the object in the cache
   return m_svrHitTables[ aHypo ];
}

const NavTrack::SVRHitTable*
NavTrack::pionSVRHits() const
{
   return svrHits( DBCandidate::kChargedPion );
}

const NavTrack::SVRHitTable*
NavTrack::kaonSVRHits() const
{
   return svrHits( DBCandidate::kChargedKaon );
}

const NavTrack::SVRHitTable*
NavTrack::electronSVRHits() const
{
   return svrHits( DBCandidate::kElectron );
}

const NavTrack::SVRHitTable*
NavTrack::muonSVRHits() const
{
   return svrHits( DBCandidate::kMuon );
}

const NavTrack::SVRHitTable*
NavTrack::protonSVRHits() const
{
   return svrHits( DBCandidate::kProton );
}

const NavTrack::SVRHitTable*
NavTrack::exitElectronSVRHits() const
{
   return svrHits( DBCandidate::kExitElectron );
}

const NavTrack::SVRHitTable*
NavTrack::exitMuonSVRHits() const
{
   return svrHits( DBCandidate::kExitMuon );
}

const NavTrack::SVRHitTable*
NavTrack::exitPionSVRHits() const
{
   return svrHits( DBCandidate::kExitPion );
}

const NavTrack::SVRHitTable*
NavTrack::exitKaonSVRHits() const
{
   return svrHits( DBCandidate::kExitKaon );
}

const NavTrack::SVRHitTable*
NavTrack::exitProtonSVRHits() const
{
   return svrHits( DBCandidate::kExitProton );
}

const NavTrack::SVZHitTable*
NavTrack::svzHits( DBCandidate::Hypo aHypo ) const
{
   // If the cache is empty...
   if ( 0 == m_svzHitTables[ aHypo ] )
   {
      // cast away const to fill cache from master block
      ((NavTrack*)this)->m_svzHitTables[ aHypo ] =
         ((const NavTrackMaster&)masterBlock()).svzHits( identifier(), aHypo );
   }

   // return the object in the cache
   return m_svzHitTables[ aHypo ];
}

const NavTrack::SVZHitTable*
NavTrack::pionSVZHits() const
{
   return svzHits( DBCandidate::kChargedPion );
}

const NavTrack::SVZHitTable*
NavTrack::kaonSVZHits() const
{
   return svzHits( DBCandidate::kChargedKaon );
}

const NavTrack::SVZHitTable*
NavTrack::electronSVZHits() const
{
   return svzHits( DBCandidate::kElectron );
}

const NavTrack::SVZHitTable*
NavTrack::muonSVZHits() const
{
   return svzHits( DBCandidate::kMuon );
}

const NavTrack::SVZHitTable*
NavTrack::protonSVZHits() const
{
   return svzHits( DBCandidate::kProton );
}

const NavTrack::SVZHitTable*
NavTrack::exitElectronSVZHits() const
{
   return svzHits( DBCandidate::kExitElectron );
}

const NavTrack::SVZHitTable*
NavTrack::exitMuonSVZHits() const
{
   return svzHits( DBCandidate::kExitMuon );
}

const NavTrack::SVZHitTable*
NavTrack::exitPionSVZHits() const
{
   return svzHits( DBCandidate::kExitPion );
}

const NavTrack::SVZHitTable*
NavTrack::exitKaonSVZHits() const
{
   return svzHits( DBCandidate::kExitKaon );
}

const NavTrack::SVZHitTable*
NavTrack::exitProtonSVZHits() const
{
   return svzHits( DBCandidate::kExitProton );
}
