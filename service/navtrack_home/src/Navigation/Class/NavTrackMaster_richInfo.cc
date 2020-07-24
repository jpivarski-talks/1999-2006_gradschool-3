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
// Author:      Tomasz Skwarnicki
// Created:     Sun Jul 23 06:21:00 EDT 2000
// $Id: NavTrackMaster_richInfo.cc,v 1.3 2002/08/06 17:35:03 cleo3 Exp $
//
// Revision history
//
// $Log: NavTrackMaster_richInfo.cc,v $
// Revision 1.3  2002/08/06 17:35:03  cleo3
// NavTrackMaster rather than NavTrack now owns any memory inorder to decrease dependencies seen on Solaris 8
//
// Revision 1.2  2001/12/21 20:46:02  cdj
// modified NavRich related code so no longer need to load RichTracks
//
// Revision 1.1  2000/07/27 01:31:46  ts
// NavRich added
//
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <set>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "Navigation/NavTrackMaster.h"
#include "RichTracksProd/RichTracksProd.h"
#include "RichTracksProd/RichTrack.h"

#include "FrameAccess/extract.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAPtrTable.h"
#include "Navigation/NavRich.h"


// STL classes
#include <map>
#include <vector>
#include <set>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrackMaster" ;

const NavRich* 
NavTrackMaster::richInfo( const NavTrack::Identifier aId ) const 
{
   const NavRich* richInfoPtr ( 0 ) ;
   if( 0 == m_rich )
   {
      FATable<NavRich> navRichs;
      extract( dict(), navRichs );

      const_cast< NavTrackMaster* >( this )->m_rich = navRichs.getContents();
   }
   FATable<NavRich>::const_iterator itEnd = m_rich->end();
   for( FATable< NavRich >::const_iterator iM = m_rich->begin(); 
	iM != itEnd ; 
	++iM )
   {
      const NavRich& richInfo ( *iM ) ;
      if( aId == richInfo.trackId() ){
	 richInfoPtr = &richInfo ;
	 break;
      }
   }
   return richInfoPtr ;
}
