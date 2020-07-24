// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrack_mcTag
// 
// Description: User-level navigation object for tracks - mc tagging only
//
// Implementation:
//
// Author:      Brian Heltsley
// Created:     Mon Jun  5 14:31:23 EDT 2000
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
#include "MCInfo/MCDecayTree/MCParticle.h"
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

FAItem<MCParticle>
NavTrack::mcTag() const
{
   return FAItem< MCParticle > ( m_master.mcTag( identifier() ) ) ;
}

FAItem<MCTrackTagsByTrack>
NavTrack::mcTags() const
{
   return FAItem< MCTrackTagsByTrack > ( m_master.mcTags( identifier() ) ) ;
}
