// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrackMaster_record
// 
// Description: For record() fcn
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Mon Jul  6 14:31:27 EDT 1998
// $Id: NavTrackMaster_record.cc,v 1.1 2003/02/04 18:13:09 bkh Exp $
//
// Revision history
//
// $Log: NavTrackMaster_record.cc,v $
// Revision 1.1  2003/02/04 18:13:09  bkh
// Separate off record function
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
#include "Navigation/NavTrackMaster.h"

#include "FrameAccess/extract.h"

// STL classes
#include <map>
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrackMaster_record" ;

//
// static data member definitions
//

//
// constructors and destructor
//

const Record& 
NavTrackMaster::record() const
{
   return dict() ;
}


