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
// $Id: NavTrackMaster_drHits.cc,v 1.3 2001/07/13 17:20:10 cdj Exp $
//
// Revision history
//
// $Log: NavTrackMaster_drHits.cc,v $
// Revision 1.3  2001/07/13 17:20:10  cdj
// fixed type problem caused by previous change
//
// Revision 1.2  2001/06/26 18:01:42  cdj
// modified to work with new Lattice interface
//
// Revision 1.1  2000/04/05 15:43:00  marsh
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

template<class LatticeT>
inline
const NavTrack::DRHitLinkTable*
hitLinks( NavTrack::Identifier aId,
          const Record& iRecord, 
          const LatticeT* ioCachePtr)
{
   if( 0 == ioCachePtr ) {
      FAItem< LatticeT> lattice;
      extract( iRecord , lattice );
      ioCachePtr = &*lattice;
   }  

   NavTrack::DRHitLinkTable* hitLinkTable = new NavTrack::DRHitLinkTable;

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
      hitLinkTable->push_back( reinterpret_cast<NavTrack::DRHitLink* const>(*link) );
   }
   return hitLinkTable;
}

const NavTrack::DRHitLinkTable*
NavTrackMaster::drHitLinks( NavTrack::Identifier aId,
                            DBCandidate::Hypo aHypo ) const
{
   NavTrack::DRHitLinkTable* kNone( 0 );
   NavTrack::DRHitLinkTable* hitLinkTable;

   switch( aHypo )
   {
      case DBCandidate::kChargedPion :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
                 const_cast<NavTrackMaster*>(this)->m_pionDRHitLattice );
      }
      case DBCandidate::kChargedKaon :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
                 const_cast<NavTrackMaster*>(this)->m_kaonDRHitLattice );
      }
      case DBCandidate::kElectron :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
                 const_cast<NavTrackMaster*>(this)->m_electronDRHitLattice );
      }
      case DBCandidate::kMuon :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
                 const_cast<NavTrackMaster*>(this)->m_muonDRHitLattice );
      }
      case DBCandidate::kProton :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
                 const_cast<NavTrackMaster*>(this)->m_protonDRHitLattice );
      }
      case DBCandidate::kExitElectron :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitElectronDRHitLattice );
      }
      case DBCandidate::kExitMuon :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitMuonDRHitLattice );
      }
      case DBCandidate::kExitPion :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitPionDRHitLattice );
      }
      case DBCandidate::kExitKaon :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitKaonDRHitLattice );
      }
      case DBCandidate::kExitProton :
      {
         // cast away const to fill cache
         return hitLinks( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitProtonDRHitLattice );
      }
      default :
         return ( kNone );
   }
//   return ( kNone );
}

template<class LatticeT>
inline
const NavTrack::DRHitTable*
drHitsImp( NavTrack::Identifier aId,
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

   FATable< CalibratedDRHit > calibratedHitTable;
   extract( iRecord , calibratedHitTable );
   NavTrack::DRHitTable* hitTable = new NavTrack::DRHitTable;
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

const NavTrack::DRHitTable*
NavTrackMaster::drHits( NavTrack::Identifier aId,
                        DBCandidate::Hypo aHypo ) const
{
   NavTrack::DRHitTable* kNone( 0 );
   switch( aHypo )
   {
      case DBCandidate::kChargedPion :
      {
         // cast away const to fill cache
         return drHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_pionDRHitLattice );
      }
      case DBCandidate::kChargedKaon :
      {
         // cast away const to fill cache
         return drHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_kaonDRHitLattice );
      }
      case DBCandidate::kElectron :
      {
         // cast away const to fill cache
         return drHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_electronDRHitLattice );
      }
      case DBCandidate::kMuon :
      {
         // cast away const to fill cache
         return drHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_muonDRHitLattice );
      }
      case DBCandidate::kProton :
      {
         // cast away const to fill cache
         return drHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_protonDRHitLattice );
      }
      case DBCandidate::kExitElectron :
      {
         // cast away const to fill cache
         return drHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitElectronDRHitLattice );
      }
      case DBCandidate::kExitMuon :
      {
         // cast away const to fill cache
         return drHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitMuonDRHitLattice );
      }
      case DBCandidate::kExitPion :
      {
         // cast away const to fill cache
         return drHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitPionDRHitLattice );
      }
      case DBCandidate::kExitKaon :
      {
         // cast away const to fill cache
         return drHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitKaonDRHitLattice );
      }
      case DBCandidate::kExitProton :
      {
         // cast away const to fill cache
         return drHitsImp( aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitProtonDRHitLattice );
      }
      default :
         return ( kNone );
   }
//   return ( kNone );
}
