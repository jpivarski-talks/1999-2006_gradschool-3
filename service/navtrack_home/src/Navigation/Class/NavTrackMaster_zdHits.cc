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
// Created:     Wed Oct  8 13:07:42 EDT 2003
// $Id: NavTrackMaster_zdHits.cc,v 1.1 2003/10/16 18:42:06 mccann Exp $
//
// Revision history
//
// $Log: NavTrackMaster_zdHits.cc,v $
// Revision 1.1  2003/10/16 18:42:06  mccann
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

template<class LatticeT>
inline
const NavTrack::ZDHitLinkTable*
hitLinks( NavTrack::Identifier aId,
          const Record& iRecord, 
          const LatticeT* ioCachePtr)
{
   if( 0 == ioCachePtr ) {
      FAItem< LatticeT> lattice;
      extract( iRecord , lattice );
      ioCachePtr = &*lattice;
   }  

   NavTrack::ZDHitLinkTable* hitLinkTable = new NavTrack::ZDHitLinkTable;

   typedef typename LatticeT::Links Links;
   const Links& linksGivenLeft =
            ioCachePtr->linksGivenLeft( aId );

   typename Links::const_iterator finished =
               linksGivenLeft.end();
   for ( typename Links::const_iterator link =
         linksGivenLeft.begin() ;
         link != finished ;
         ++link )
   {
      hitLinkTable->push_back( reinterpret_cast<NavTrack::ZDHitLink* const>(*link) );
   }
   return hitLinkTable;
}

const NavTrack::ZDHitLinkTable*
NavTrackMaster::zdHitLinks( NavTrack::Identifier aId,
                            DBCandidate::Hypo aHypo ) const
{
   NavTrack::ZDHitLinkTable* kNone( 0 );
   NavTrack::ZDHitLinkTable* hitLinkTable;

   switch( aHypo )
   {
      case DBCandidate::kChargedPion :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
                 const_cast<NavTrackMaster*>(this)->m_pionZDHitLattice );
      }
      case DBCandidate::kChargedKaon :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
                 const_cast<NavTrackMaster*>(this)->m_kaonZDHitLattice );
      }
      case DBCandidate::kElectron :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
                 const_cast<NavTrackMaster*>(this)->m_electronZDHitLattice );
      }
      case DBCandidate::kMuon :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
                 const_cast<NavTrackMaster*>(this)->m_muonZDHitLattice );
      }
      case DBCandidate::kProton :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
                 const_cast<NavTrackMaster*>(this)->m_protonZDHitLattice );
      }
      case DBCandidate::kExitElectron :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitElectronZDHitLattice );
      }
      case DBCandidate::kExitMuon :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitMuonZDHitLattice );
      }
      case DBCandidate::kExitPion :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitPionZDHitLattice );
      }
      case DBCandidate::kExitKaon :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitKaonZDHitLattice );
      }
      case DBCandidate::kExitProton :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitProtonZDHitLattice );
      }
      default :
         return ( kNone );
   }
//   return ( kNone );
}

template<class LatticeT>
inline
const NavTrack::ZDHitTable*
zdHitsImp( NavTrack::Identifier aId,
          const Record& iRecord, 
          const LatticeT* ioCachePtr)
{

   // If the cache is empty...
   if ( 0 == ioCachePtr )
   {
      FAItem< LatticeT > lattice;
      extract( iRecord , lattice );
      ioCachePtr = &*lattice;
   }

   FATable< CalibratedZDHit > calibratedHitTable;
   extract( iRecord , calibratedHitTable );
   NavTrack::ZDHitTable* hitTable = new NavTrack::ZDHitTable;
   typedef typename LatticeT::VectorRightID VectorRightID;
   const VectorRightID* farData =
            ioCachePtr->vRightGivenLeft( aId );
   if ( 0 != farData )
   {
      typename VectorRightID::const_iterator finished =
            farData->end();
      for ( typename VectorRightID::const_iterator hits =
            farData->begin() ;
            hits != finished ;
            ++hits )
      {
         hitTable->push_back(
            &*(calibratedHitTable.find( *hits ) ) );
      }
   }
   return hitTable;
}

const NavTrack::ZDHitTable*
NavTrackMaster::zdHits( NavTrack::Identifier aId,
                        DBCandidate::Hypo aHypo ) const
{
   NavTrack::ZDHitTable* kNone( 0 );
   switch( aHypo )
   {
      case DBCandidate::kChargedPion :
      {
         // cast away const to fill cache
         return zdHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_pionZDHitLattice );
      }
      case DBCandidate::kChargedKaon :
      {
         // cast away const to fill cache
         return zdHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_kaonZDHitLattice );
      }
      case DBCandidate::kElectron :
      {
         // cast away const to fill cache
         return zdHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_electronZDHitLattice );
      }
      case DBCandidate::kMuon :
      {
         // cast away const to fill cache
         return zdHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_muonZDHitLattice );
      }
      case DBCandidate::kProton :
      {
         // cast away const to fill cache
         return zdHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_protonZDHitLattice );
      }
      case DBCandidate::kExitElectron :
      {
         // cast away const to fill cache
         return zdHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitElectronZDHitLattice );
      }
      case DBCandidate::kExitMuon :
      {
         // cast away const to fill cache
         return zdHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitMuonZDHitLattice );
      }
      case DBCandidate::kExitPion :
      {
         // cast away const to fill cache
         return zdHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitPionZDHitLattice );
      }
      case DBCandidate::kExitKaon :
      {
         // cast away const to fill cache
         return zdHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitKaonZDHitLattice );
      }
      case DBCandidate::kExitProton :
      {
         // cast away const to fill cache
         return zdHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitProtonZDHitLattice );
      }
      default :
         return ( kNone );
   }
//   return ( kNone );
}
