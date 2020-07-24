// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrack_shower
// 
// Description: User-level navigation object for tracks - showers id only
//
// Implementation:
//
// Author:      Brian Heltsley
// Created:     Mon Jun  5 14:31:23 EDT 2000
// $Id: NavTrack_shower.cc,v 1.3 2002/08/06 17:35:06 cleo3 Exp $
//
// Revision history
//
// $Log: NavTrack_shower.cc,v $
// Revision 1.3  2002/08/06 17:35:06  cleo3
// NavTrackMaster rather than NavTrack now owns any memory inorder to decrease dependencies seen on Solaris 8
//
// Revision 1.2  2002/03/25 11:52:57  bkh
// various optimizations for speed
//
// Revision 1.1  2001/10/26 21:53:54  bkh
// Add features to showers and connected region objects
//
// Revision 1.1  2000/06/06 18:38:46  bkh
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "FrameAccess/FAPtrTable.h"
#include "FrameAccess/FAConstPtrTable.h"
#include "FrameAccess/FATable.h"
#include "Navigation/NavTrack.h"
#include "Navigation/NavShower.h"
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

FATable< NavShower >
NavTrack::bremShowers() const
{
   if( 0 == m_bremShowers )
   {
      const_cast< NavTrack* >( this )->m_bremShowers =
	 m_master.bremShowers( identifier() );
   }
   return FATable< NavShower >( m_bremShowers ) ;
}







