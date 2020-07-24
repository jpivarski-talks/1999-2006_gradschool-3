// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrackMaster
// 
// Description: Master block for NavTrack object, inherits from TDTrackMaster.
//
// Implementation:
//
// Author:      Brian Heltsley
// Created:     Mon Jun  5 14:31:27 EDT 2000
// $Id: NavTrackMaster_trkman.cc,v 1.1 2003/07/29 20:18:28 bkh Exp $
//
// Revision history
//
// $Log $
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

#include "Trkman/TrkmanInfo.h"

// STL classes
#include <map>
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrackMaster_trkman" ;

const TrkmanInfo* 
NavTrackMaster::trkmanInfo( const NavTrack::Identifier aId ) const 
{
   if( 0 == m_trkmanInfo )
   {
      FATable<TrkmanInfo > infos;
      extract( dict(), infos );

      const_cast< NavTrackMaster* >( this )->m_trkmanInfo = infos.getContents();
   }

   FATable< TrkmanInfo >::const_iterator endTrkman ( m_trkmanInfo->end() ) ;

   FATable< TrkmanInfo >::const_iterator found   ( m_trkmanInfo->find( aId ) ) ;

   return ( found == endTrkman ? 0 : &(*found) ) ;
}
