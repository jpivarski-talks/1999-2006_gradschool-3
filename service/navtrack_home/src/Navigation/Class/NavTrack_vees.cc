// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrack_vees
// 
// Description: for ks or lambda access from navtrack
//
// Implementation:
//
// Author:      Brian Heltsley
// Created:     Mon Jun  5 14:31:27 EDT 2000
// $Id: NavTrack_vees.cc,v 1.1 2002/11/07 17:42:05 bkh Exp $
//
// Revision history
//
// $Log: NavTrack_vees.cc,v $
// Revision 1.1  2002/11/07 17:42:05  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
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
#include "Navigation/NavTrack.h"
#include "Navigation/NavTrackMaster.h"

#include "FrameAccess/extract.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAPtrTable.h"
#include "Navigation/NavKs.h"
#include "Navigation/NavLambda.h"

// STL classes
#include <map>
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrack_vees" ;

FAItem<NavKs>
NavTrack::navKs() const 
{
   return FAItem<NavKs> ( m_master.navKs( identifier() ) ) ;
}

FAItem<NavLambda>
NavTrack::navLambda() const 
{
   return FAItem<NavLambda> ( m_master.navLambda( identifier() ) ) ;
}
