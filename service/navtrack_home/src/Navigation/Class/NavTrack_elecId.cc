// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrack_elecId
// 
// Description: User-level navigation object for tracks - elec id only
//
// Implementation:
//
// Author:      Brian Heltsley
// Created:     Mon Jun  5 14:31:23 EDT 2000
// $Id: NavTrack_elecId.cc,v 1.1 2000/06/06 18:38:46 bkh Exp $
//
// Revision history
//
// $Log: NavTrack_elecId.cc,v $
// Revision 1.1  2000/06/06 18:38:46  bkh
// Install dedx, electron id, and muon id interfaces
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
#include "Navigation/NavElecId.h"
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

const NavElecId&
NavTrack::elecId() const
{
   if( 0 == m_elecId )
   {
      const_cast< NavTrack* >( this )->m_elecId = new NavElecId ( *this ) ;
   }
   return ( *m_elecId ) ;
}
