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
// $Id: NavTrackMaster_svHits.cc,v 1.4 2001/07/24 15:35:54 cdj Exp $
//
// Revision history
//
// $Log: NavTrackMaster_svHits.cc,v $
// Revision 1.4  2001/07/24 15:35:54  cdj
// inlined a templated function to avoid missing symbol on OSF
//
// Revision 1.3  2001/07/13 17:20:10  cdj
// fixed type problem caused by previous change
//
// Revision 1.2  2001/06/26 18:01:42  cdj
// modified to work with new Lattice interface
//
// Revision 1.1  2000/04/05 15:43:01  marsh
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
template <class LatticeT, class HitLink>
inline
const STL_VECTOR(const HitLink*)*
navSvHitLinks( NavTrack::Identifier aId,
                const Record& iRecord,
                const LatticeT* ioCachePtr,
                HitLink* iDummy )
{
   // If the cache is empty...
   if ( 0 == ioCachePtr )
   {
      FAItem< LatticeT > lattice;
      extract( iRecord , lattice );
      ioCachePtr = &*lattice;
   }

   STL_VECTOR(const HitLink*)* hitLinkTable = new STL_VECTOR(const HitLink*);
   typedef typename LatticeT::Links Links;
   const Links& linksGivenLeft =
            ioCachePtr->linksGivenLeft( aId );
   typename Links::const_iterator finished = linksGivenLeft.end();
   for ( typename Links::const_iterator link =
            linksGivenLeft.begin() ;
         link != finished ;
         ++link )
   {
         hitLinkTable->push_back( 
            reinterpret_cast<HitLink* const>(*link) );
   }
   return hitLinkTable;
}

static const char* const kFacilityString = "Navigation.NavTrackMaster" ;

const NavTrack::SVRHitLinkTable*
NavTrackMaster::svrHitLinks( NavTrack::Identifier aId,
                             DBCandidate::Hypo aHypo ) const
{
   NavTrack::SVRHitLinkTable* kNone( 0 );
   NavTrack::SVRHitLink* kDummy(0);
   switch( aHypo )
   {
      case DBCandidate::kChargedPion :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
                  const_cast<NavTrackMaster*>(this)->m_pionSVRHitLattice,
                  kDummy);
      }
      case DBCandidate::kChargedKaon :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
           const_cast<NavTrackMaster*>(this)->m_kaonSVRHitLattice,
           kDummy );
      }
      case DBCandidate::kElectron :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_electronSVRHitLattice,
            kDummy );
      }
      case DBCandidate::kMuon :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_muonSVRHitLattice,
            kDummy );
      }
      case DBCandidate::kProton :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_protonSVRHitLattice,
            kDummy );
      }
      case DBCandidate::kExitElectron :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitElectronSVRHitLattice,
            kDummy );
      }
      case DBCandidate::kExitMuon :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitMuonSVRHitLattice,
            kDummy );
      }
      case DBCandidate::kExitPion :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitPionSVRHitLattice,
            kDummy );
      }
      case DBCandidate::kExitKaon :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitKaonSVRHitLattice,
            kDummy );
      }
      case DBCandidate::kExitProton :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitProtonSVRHitLattice,
            kDummy );
      }
      default :
         return ( kNone );
   }
//   return ( kNone );
}

const NavTrack::SVZHitLinkTable*
NavTrackMaster::svzHitLinks( NavTrack::Identifier aId,
                             DBCandidate::Hypo aHypo ) const
{
   NavTrack::SVZHitLinkTable* kNone( 0 );
   NavTrack::SVZHitLinkTable* hitLinkTable;
   NavTrack::SVZHitLink* kDummy(0);

   switch( aHypo )
   {
      case DBCandidate::kChargedPion :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
                  const_cast<NavTrackMaster*>(this)->m_pionSVZHitLattice,
                  kDummy);
      }
      case DBCandidate::kChargedKaon :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
           const_cast<NavTrackMaster*>(this)->m_kaonSVZHitLattice,
           kDummy );
      }
      case DBCandidate::kElectron :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_electronSVZHitLattice,
            kDummy );
      }
      case DBCandidate::kMuon :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_muonSVZHitLattice,
            kDummy );
      }
      case DBCandidate::kProton :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_protonSVZHitLattice,
            kDummy );
      }
      case DBCandidate::kExitElectron :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitElectronSVZHitLattice,
            kDummy );
      }
      case DBCandidate::kExitMuon :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitMuonSVZHitLattice,
            kDummy );
      }
      case DBCandidate::kExitPion :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitPionSVZHitLattice,
            kDummy );
      }
      case DBCandidate::kExitKaon :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitKaonSVZHitLattice,
            kDummy );
      }
      case DBCandidate::kExitProton :
      {
         // cast away const to fill cache
         return navSvHitLinks(aId, dict(), 
            const_cast<NavTrackMaster*>(this)->m_exitProtonSVZHitLattice,
            kDummy );
      }
      default :
         return ( kNone );
   }
//   return ( kNone );
}


const NavTrack::SVRHitTable*
NavTrackMaster::svrHits( NavTrack::Identifier aId,
                         DBCandidate::Hypo aHypo ) const
{
   NavTrack::SVRHitTable* kNone( 0 );
   NavTrack::SVRHitTable* hitTable;

   switch( aHypo )
   {
      case DBCandidate::kChargedPion :
      {
         // If the cache is empty...
         if ( 0 == m_pionSVRHitLattice )
         {
            FAItem< PionFitSVRHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_pionSVRHitLattice = &*lattice;
         }
         FATable< CalibratedSVRphiHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVRHitTable;
         const PionFitSVRHitLattice::VectorRightID* farData =
            m_pionSVRHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            PionFitSVRHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( PionFitSVRHitLattice::VectorRightID::const_iterator hits =
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
      case DBCandidate::kChargedKaon :
      {
         // If the cache is empty...
         if ( 0 == m_kaonSVRHitLattice )
         {
            FAItem< KaonFitSVRHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_kaonSVRHitLattice = &*lattice;
         }
         FATable< CalibratedSVRphiHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVRHitTable;
         const KaonFitSVRHitLattice::VectorRightID* farData =
            m_kaonSVRHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            KaonFitSVRHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( KaonFitSVRHitLattice::VectorRightID::const_iterator hits =
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
      case DBCandidate::kElectron :
      {
         // If the cache is empty...
         if ( 0 == m_electronSVRHitLattice )
         {
            FAItem< ElectronFitSVRHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_electronSVRHitLattice = &*lattice;
         }
         FATable< CalibratedSVRphiHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVRHitTable;
         const ElectronFitSVRHitLattice::VectorRightID* farData =
            m_electronSVRHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            ElectronFitSVRHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( ElectronFitSVRHitLattice::VectorRightID::const_iterator
                     hits = farData->begin() ;
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
      case DBCandidate::kMuon :
      {
         // If the cache is empty...
         if ( 0 == m_muonSVRHitLattice )
         {
            FAItem< MuonFitSVRHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_muonSVRHitLattice = &*lattice;
         }
         FATable< CalibratedSVRphiHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVRHitTable;
         const MuonFitSVRHitLattice::VectorRightID* farData =
            m_muonSVRHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            MuonFitSVRHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( MuonFitSVRHitLattice::VectorRightID::const_iterator hits =
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
      case DBCandidate::kProton :
      {
         // If the cache is empty...
         if ( 0 == m_protonSVRHitLattice )
         {
            FAItem< ProtonFitSVRHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_protonSVRHitLattice = &*lattice;
         }
         FATable< CalibratedSVRphiHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVRHitTable;
         const ProtonFitSVRHitLattice::VectorRightID* farData =
            m_protonSVRHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            ProtonFitSVRHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( ProtonFitSVRHitLattice::VectorRightID::const_iterator hits =
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
      case DBCandidate::kExitElectron :
      {
         // If the cache is empty...
         if ( 0 == m_exitElectronSVRHitLattice )
         {
            FAItem< ExitElectronFitSVRHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_exitElectronSVRHitLattice = &*lattice;
         }
         FATable< CalibratedSVRphiHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVRHitTable;
         const ExitElectronFitSVRHitLattice::VectorRightID* farData =
            m_exitElectronSVRHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            ExitElectronFitSVRHitLattice::VectorRightID::const_iterator
               finished = farData->end();
            for ( ExitElectronFitSVRHitLattice::VectorRightID::const_iterator
                     hits = farData->begin() ;
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
      case DBCandidate::kExitMuon :
      {
         // If the cache is empty...
         if ( 0 == m_exitMuonSVRHitLattice )
         {
            FAItem< ExitMuonFitSVRHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_exitMuonSVRHitLattice = &*lattice;
         }
         FATable< CalibratedSVRphiHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVRHitTable;
         const ExitMuonFitSVRHitLattice::VectorRightID* farData =
            m_exitMuonSVRHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            ExitMuonFitSVRHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( ExitMuonFitSVRHitLattice::VectorRightID::const_iterator
                     hits = farData->begin() ;
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
      case DBCandidate::kExitPion :
      {
         // If the cache is empty...
         if ( 0 == m_exitPionSVRHitLattice )
         {
            FAItem< ExitPionFitSVRHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_exitPionSVRHitLattice = &*lattice;
         }
         FATable< CalibratedSVRphiHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVRHitTable;
         const ExitPionFitSVRHitLattice::VectorRightID* farData =
            m_exitPionSVRHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            ExitPionFitSVRHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( ExitPionFitSVRHitLattice::VectorRightID::const_iterator
                     hits = farData->begin() ;
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
      case DBCandidate::kExitKaon :
      {
         // If the cache is empty...
         if ( 0 == m_exitKaonSVRHitLattice )
         {
            FAItem< ExitKaonFitSVRHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_exitKaonSVRHitLattice = &*lattice;
         }
         FATable< CalibratedSVRphiHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVRHitTable;
         const ExitKaonFitSVRHitLattice::VectorRightID* farData =
            m_exitKaonSVRHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            ExitKaonFitSVRHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( ExitKaonFitSVRHitLattice::VectorRightID::const_iterator
                     hits = farData->begin() ;
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
      case DBCandidate::kExitProton :
      {
         // If the cache is empty...
         if ( 0 == m_exitProtonSVRHitLattice )
         {
            FAItem< ExitProtonFitSVRHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_exitProtonSVRHitLattice = &*lattice;
         }
         FATable< CalibratedSVRphiHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVRHitTable;
         const ExitProtonFitSVRHitLattice::VectorRightID* farData =
            m_exitProtonSVRHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            ExitProtonFitSVRHitLattice::VectorRightID::const_iterator
               finished = farData->end();
            for ( ExitProtonFitSVRHitLattice::VectorRightID::const_iterator
                     hits = farData->begin() ;
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
      default :
         return ( kNone );
   }
//   return ( kNone );
}

const NavTrack::SVZHitTable*
NavTrackMaster::svzHits( NavTrack::Identifier aId,
                         DBCandidate::Hypo aHypo ) const
{
   NavTrack::SVZHitTable* kNone( 0 );
   NavTrack::SVZHitTable* hitTable;

   switch( aHypo )
   {
      case DBCandidate::kChargedPion :
      {
         // If the cache is empty...
         if ( 0 == m_pionSVZHitLattice )
         {
            FAItem< PionFitSVZHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_pionSVZHitLattice = &*lattice;
         }
         FATable< CalibratedSVZHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVZHitTable;
         const PionFitSVZHitLattice::VectorRightID* farData =
            m_pionSVZHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            PionFitSVZHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( PionFitSVZHitLattice::VectorRightID::const_iterator hits =
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
      case DBCandidate::kChargedKaon :
      {
         // If the cache is empty...
         if ( 0 == m_kaonSVZHitLattice )
         {
            FAItem< KaonFitSVZHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_kaonSVZHitLattice = &*lattice;
         }
         FATable< CalibratedSVZHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVZHitTable;
         const KaonFitSVZHitLattice::VectorRightID* farData =
            m_kaonSVZHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            KaonFitSVZHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( KaonFitSVZHitLattice::VectorRightID::const_iterator hits =
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
      case DBCandidate::kElectron :
      {
         // If the cache is empty...
         if ( 0 == m_electronSVZHitLattice )
         {
            FAItem< ElectronFitSVZHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_electronSVZHitLattice = &*lattice;
         }
         FATable< CalibratedSVZHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVZHitTable;
         const ElectronFitSVZHitLattice::VectorRightID* farData =
            m_electronSVZHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            ElectronFitSVZHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( ElectronFitSVZHitLattice::VectorRightID::const_iterator
                     hits = farData->begin() ;
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
      case DBCandidate::kMuon :
      {
         // If the cache is empty...
         if ( 0 == m_muonSVZHitLattice )
         {
            FAItem< MuonFitSVZHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_muonSVZHitLattice = &*lattice;
         }
         FATable< CalibratedSVZHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVZHitTable;
         const MuonFitSVZHitLattice::VectorRightID* farData =
            m_muonSVZHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            MuonFitSVZHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( MuonFitSVZHitLattice::VectorRightID::const_iterator hits =
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
      case DBCandidate::kProton :
      {
         // If the cache is empty...
         if ( 0 == m_protonSVZHitLattice )
         {
            FAItem< ProtonFitSVZHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_protonSVZHitLattice = &*lattice;
         }
         FATable< CalibratedSVZHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVZHitTable;
         const ProtonFitSVZHitLattice::VectorRightID* farData =
            m_protonSVZHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            ProtonFitSVZHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( ProtonFitSVZHitLattice::VectorRightID::const_iterator hits =
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
      case DBCandidate::kExitElectron :
      {
         // If the cache is empty...
         if ( 0 == m_exitElectronSVZHitLattice )
         {
            FAItem< ExitElectronFitSVZHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_exitElectronSVZHitLattice = &*lattice;
         }
         FATable< CalibratedSVZHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVZHitTable;
         const ExitElectronFitSVZHitLattice::VectorRightID* farData =
            m_exitElectronSVZHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            ExitElectronFitSVZHitLattice::VectorRightID::const_iterator
               finished = farData->end();
            for ( ExitElectronFitSVZHitLattice::VectorRightID::const_iterator
                     hits = farData->begin() ;
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
      case DBCandidate::kExitMuon :
      {
         // If the cache is empty...
         if ( 0 == m_exitMuonSVZHitLattice )
         {
            FAItem< ExitMuonFitSVZHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_exitMuonSVZHitLattice = &*lattice;
         }
         FATable< CalibratedSVZHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVZHitTable;
         const ExitMuonFitSVZHitLattice::VectorRightID* farData =
            m_exitMuonSVZHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            ExitMuonFitSVZHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( ExitMuonFitSVZHitLattice::VectorRightID::const_iterator
                     hits = farData->begin() ;
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
      case DBCandidate::kExitPion :
      {
         // If the cache is empty...
         if ( 0 == m_exitPionSVZHitLattice )
         {
            FAItem< ExitPionFitSVZHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_exitPionSVZHitLattice = &*lattice;
         }
         FATable< CalibratedSVZHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVZHitTable;
         const ExitPionFitSVZHitLattice::VectorRightID* farData =
            m_exitPionSVZHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            ExitPionFitSVZHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( ExitPionFitSVZHitLattice::VectorRightID::const_iterator
                     hits = farData->begin() ;
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
      case DBCandidate::kExitKaon :
      {
         // If the cache is empty...
         if ( 0 == m_exitKaonSVZHitLattice )
         {
            FAItem< ExitKaonFitSVZHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_exitKaonSVZHitLattice = &*lattice;
         }
         FATable< CalibratedSVZHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVZHitTable;
         const ExitKaonFitSVZHitLattice::VectorRightID* farData =
            m_exitKaonSVZHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            ExitKaonFitSVZHitLattice::VectorRightID::const_iterator finished =
               farData->end();
            for ( ExitKaonFitSVZHitLattice::VectorRightID::const_iterator
                     hits = farData->begin() ;
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
      case DBCandidate::kExitProton :
      {
         // If the cache is empty...
         if ( 0 == m_exitProtonSVZHitLattice )
         {
            FAItem< ExitProtonFitSVZHitLattice > lattice;
            extract( dict() , lattice );
            // cast away const to fill cache
            ((NavTrackMaster*)this)->m_exitProtonSVZHitLattice = &*lattice;
         }
         FATable< CalibratedSVZHit > calibratedHitTable;
         extract( dict() , calibratedHitTable );
         hitTable = new NavTrack::SVZHitTable;
         const ExitProtonFitSVZHitLattice::VectorRightID* farData =
            m_exitProtonSVZHitLattice->vRightGivenLeft( aId );
         if ( 0 != farData )
         {
            ExitProtonFitSVZHitLattice::VectorRightID::const_iterator
               finished = farData->end();
            for ( ExitProtonFitSVZHitLattice::VectorRightID::const_iterator
                     hits = farData->begin() ;
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
      default :
         return ( kNone );
   }
//   return ( kNone );
}
