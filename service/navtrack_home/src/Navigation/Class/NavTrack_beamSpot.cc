// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrack_beamSpot
// 
// Description: User-level navigation object for tracks - beam spot fcns
//
// Implementation:
//
// Author:      Brian Heltsley
// Created:     Mon Jun  5 14:31:23 EDT 2000
// $Id: NavTrack_beamSpot.cc,v 1.1 2003/07/29 20:18:29 bkh Exp $
//
// Revision history
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
#include "BeamSpot/BeamSpot.h"
#include "Navigation/NavTrackMaster.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrack" ;

//
// static data member definitions
//


double 
NavTrack::signedDCABeamSpot( const KTHelix& aHelix ) const 
{
   return aHelix.dcaBend( *m_master.beamSpot() ) ;
}

HepPoint3D 
NavTrack::pcaBeamSpot( const KTHelix& aHelix ) const 
{
   return aHelix.pcaBend( *m_master.beamSpot() ) ;
}

