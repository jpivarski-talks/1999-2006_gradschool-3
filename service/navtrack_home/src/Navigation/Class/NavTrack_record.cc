// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrack_record
// 
// Description: User-level navigation object for tracks.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Mon Jul  6 14:31:23 EDT 1998
// $Id: NavTrack_record.cc,v 1.1 2003/02/04 18:13:10 bkh Exp $
//
// Revision history
//
// $Log: NavTrack_record.cc,v $
// Revision 1.1  2003/02/04 18:13:10  bkh
// Separate off record function
//
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
#include "DataHandler/Record.h"

// STL classes
#include <map>
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrack_record" ;

//
// static data member definitions
//

//
// constructors and destructor
//

const Record& 
NavTrack::record() const 
{
   return m_master.record() ;
}
