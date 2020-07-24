// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrack_muonId
// 
// Description: User-level navigation object for tracks - muon id only
//
// Implementation:
//
// Author:      Brian Heltsley
// Created:     Mon Jun  5 14:31:23 EDT 2000
// $Id: NavTrack_muonId.cc,v 1.1 2000/06/06 18:38:47 bkh Exp $
//
// Revision history
//
// $Log: NavTrack_muonId.cc,v $
// Revision 1.1  2000/06/06 18:38:47  bkh
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
#include "Navigation/NavMuonId.h"
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

const NavMuonId&
NavTrack::muonId() const
{
   if( 0 == m_muonId )
   {
      const MuTrack* muPtr ( m_master.muTrack( identifier() ) ) ;
      const_cast< NavTrack* >( this )->m_muonId =
	 new NavMuonId ( *this, muPtr ) ;
   }
   return *m_muonId ;
}
