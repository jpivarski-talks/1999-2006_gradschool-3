// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrack
// 
// Description: User-level navigation object for tracks.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Tomasz Skwarnicki
// Created:     Sun Jul 23 06:27:00 EDT 2000
// $Id: NavTrack_richInfo.cc,v 1.1 2000/07/27 01:31:47 ts Exp $
//
// Revision history
//
// $Log: NavTrack_richInfo.cc,v $
// Revision 1.1  2000/07/27 01:31:47  ts
// NavRich added
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

FAItem< NavRich >
NavTrack::richInfo() const
{
   return FAItem< NavRich >( m_master.richInfo( identifier() ) );
}
