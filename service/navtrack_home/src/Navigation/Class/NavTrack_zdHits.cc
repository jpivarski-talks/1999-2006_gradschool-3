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
// Created:     Wed Oct  8 13:11:08 EDT 2003
// $Id: NavTrack_zdHits.cc,v 1.1 2003/10/16 18:42:11 mccann Exp $
//
// Revision history
//
// $Log: NavTrack_zdHits.cc,v $
// Revision 1.1  2003/10/16 18:42:11  mccann
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

const NavTrack::ZDHitLinkTable*
NavTrack::zdHitLinks( DBCandidate::Hypo aHypo ) const
{
   // If the cache is empty...
   if ( 0 == m_zdHitLinks[ aHypo ] )
   {
      // cast away const to fill cache from master block
      ((NavTrack*)this)->m_zdHitLinks[ aHypo ] =
         ((const NavTrackMaster&)masterBlock()).zdHitLinks( identifier(),
                                                            aHypo );
   }

   // return the object in the cache
   return m_zdHitLinks[ aHypo ];
}

const NavTrack::ZDHitLinkTable*
NavTrack::pionZDHitLinks() const
{
   return zdHitLinks( DBCandidate::kChargedPion );
}

const NavTrack::ZDHitLinkTable*
NavTrack::kaonZDHitLinks() const
{
   return zdHitLinks( DBCandidate::kChargedKaon );
}

const NavTrack::ZDHitLinkTable*
NavTrack::electronZDHitLinks() const
{
   return zdHitLinks( DBCandidate::kElectron );
}

const NavTrack::ZDHitLinkTable*
NavTrack::muonZDHitLinks() const
{
   return zdHitLinks( DBCandidate::kMuon );
}

const NavTrack::ZDHitLinkTable*
NavTrack::protonZDHitLinks() const
{
   return zdHitLinks( DBCandidate::kProton );
}

const NavTrack::ZDHitLinkTable*
NavTrack::exitElectronZDHitLinks() const
{
   return zdHitLinks( DBCandidate::kExitElectron );
}

const NavTrack::ZDHitLinkTable*
NavTrack::exitMuonZDHitLinks() const
{
   return zdHitLinks( DBCandidate::kExitMuon );
}

const NavTrack::ZDHitLinkTable*
NavTrack::exitPionZDHitLinks() const
{
   return zdHitLinks( DBCandidate::kExitPion );
}

const NavTrack::ZDHitLinkTable*
NavTrack::exitKaonZDHitLinks() const
{
   return zdHitLinks( DBCandidate::kExitKaon );
}

const NavTrack::ZDHitLinkTable*
NavTrack::exitProtonZDHitLinks() const
{
   return zdHitLinks( DBCandidate::kExitProton );
}


const NavTrack::ZDHitTable*
NavTrack::zdHits( DBCandidate::Hypo aHypo ) const
{
   // If the cache is empty...
   if ( 0 == m_zdHitTables[ aHypo ] )
   {
      // cast away const to fill cache from master block
      ((NavTrack*)this)->m_zdHitTables[ aHypo ] =
         ((const NavTrackMaster&)masterBlock()).zdHits( identifier(), aHypo );
   }

   // return the object in the cache
   return m_zdHitTables[ aHypo ];
}

const NavTrack::ZDHitTable*
NavTrack::pionZDHits() const
{
   return zdHits( DBCandidate::kChargedPion );
}

const NavTrack::ZDHitTable*
NavTrack::kaonZDHits() const
{
   return zdHits( DBCandidate::kChargedKaon );
}

const NavTrack::ZDHitTable*
NavTrack::electronZDHits() const
{
   return zdHits( DBCandidate::kElectron );
}

const NavTrack::ZDHitTable*
NavTrack::muonZDHits() const
{
   return zdHits( DBCandidate::kMuon );
}

const NavTrack::ZDHitTable*
NavTrack::protonZDHits() const
{
   return zdHits( DBCandidate::kProton );
}

const NavTrack::ZDHitTable*
NavTrack::exitElectronZDHits() const
{
   return zdHits( DBCandidate::kExitElectron );
}

const NavTrack::ZDHitTable*
NavTrack::exitMuonZDHits() const
{
   return zdHits( DBCandidate::kExitMuon );
}

const NavTrack::ZDHitTable*
NavTrack::exitPionZDHits() const
{
   return zdHits( DBCandidate::kExitPion );
}

const NavTrack::ZDHitTable*
NavTrack::exitKaonZDHits() const
{
   return zdHits( DBCandidate::kExitKaon );
}

const NavTrack::ZDHitTable*
NavTrack::exitProtonZDHits() const
{
   return zdHits( DBCandidate::kExitProton );
}
