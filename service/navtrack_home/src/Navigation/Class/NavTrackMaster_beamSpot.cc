// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrackMaster_beamSpot
// 
// Description: for beamSpot fcn
//
// Implementation:
//
// Author:      Brian Heltsley
// Created:     Mon Jun  5 14:31:27 EDT 2000
// $Id: NavTrackMaster_beamSpot.cc,v 1.2 2003/07/30 21:19:50 bkh Exp $
//
// Revision history
//
//
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>

// user include files
//#include "Experiment/report.h"
#include "Navigation/NavTrackMaster.h"

#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "DataHandler/Frame.h"
#include "BeamSpot/BeamSpot.h"
#include "DAException/DAException.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrackMaster_beamSpot" ;

const HepPoint3D*
NavTrackMaster::beamSpot() const 
{
   if( 0 == m_beamSpot )
   {
      FAItem< BeamSpot > bs ;
      extract( dict().frame().record( Stream::kBeginRun ), bs ) ;

      if( bs.valid() )
      {
	 const_cast< NavTrackMaster* >( this )->m_beamSpot = &(bs->center()) ;
      }
      else
      {
	 throw DAException("NavTrackMaster: Invalid BeamSpot extracted") ;
      }
   }
   return m_beamSpot ;
}
