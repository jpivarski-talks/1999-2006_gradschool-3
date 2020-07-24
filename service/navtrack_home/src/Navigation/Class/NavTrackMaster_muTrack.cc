// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrackMaster_mutrack
// 
// Description: for muon track info
//
// Implementation:
//
// Author:      Brian Heltsley
// Created:     Mon Jun  5 14:31:27 EDT 2000
// $Id: NavTrackMaster_muTrack.cc,v 1.3 2002/08/06 17:35:02 cleo3 Exp $
//
// Revision history
//
// $Log: NavTrackMaster_muTrack.cc,v $
// Revision 1.3  2002/08/06 17:35:02  cleo3
// NavTrackMaster rather than NavTrack now owns any memory inorder to decrease dependencies seen on Solaris 8
//
// Revision 1.2  2000/06/08 20:17:38  bkh
// Find table item by identifier in consistent way
//
// Revision 1.1  2000/06/06 18:38:45  bkh
// Install dedx, electron id, and muon id interfaces
//
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
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAPtrTable.h"
#include "C3Mu/MuTrack.h"

// STL classes
#include <map>
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrackMaster" ;

const MuTrack* 
NavTrackMaster::muTrack( const NavTrack::Identifier aId ) const 
{
   const MuTrack* muPtr ( 0 ) ;
   if( 0 == m_muTrack )
   {
      FATable<MuTrack> muTracks;
      extract( dict(), muTracks );

      const_cast< NavTrackMaster* >( this )->m_muTrack = muTracks.getContents() ;
   }

   FATable< MuTrack >::const_iterator endMu ( m_muTrack->end() ) ;

   FATable< MuTrack >::const_iterator found ( m_muTrack->find( aId ) ) ;

   return ( found == endMu ? 0 : &(*found) ) ;
}
