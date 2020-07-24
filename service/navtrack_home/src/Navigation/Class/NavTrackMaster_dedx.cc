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
// $Id: NavTrackMaster_dedx.cc,v 1.3 2002/08/06 17:35:01 cleo3 Exp $
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

#include "DedxInfo/DedxInfo.h"

// STL classes
#include <map>
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrackMaster" ;

const DedxInfo* 
NavTrackMaster::dedx( const NavTrack::Identifier aId ) const 
{
   if( 0 == m_dedx )
   {
      FATable<DedxInfo > infos;
      extract( dict(), infos );

      const_cast< NavTrackMaster* >( this )->m_dedx  = infos.getContents();
   }

   FATable< DedxInfo >::const_iterator endDedx ( m_dedx->end() ) ;

   FATable< DedxInfo >::const_iterator found   ( m_dedx->find( aId ) ) ;

   return ( found == endDedx ? 0 : &(*found) ) ;
}
