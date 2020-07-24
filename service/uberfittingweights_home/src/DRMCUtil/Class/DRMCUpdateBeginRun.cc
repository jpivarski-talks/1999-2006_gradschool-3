// -*- C++ -*-
//
// Package:     <DRMCUtil>
// Module:      DRMCUpdateBeginRun
// 
// Description: C++ Function to make begin run calls for DR MC simulation
//
// Implementation:
//     <Notes on implementation>
//
// Author:      John J O'Neill
// Created:     Mon Sep 20 14:49:24 EDT 1999
// $Id: DRMCUpdateBeginRun.cc,v 1.3 2000/02/15 00:47:22 jjo Exp $
//
// Revision history
//
// $Log: DRMCUpdateBeginRun.cc,v $
// Revision 1.3  2000/02/15 00:47:22  jjo
// No longer use this routine but use InitDRMCFrameBrokers directly.
//
// Revision 1.2  1999/10/20 21:42:56  jjo
// Call drmc_beginrun_ to update DR MC constants every begin run
//
// Revision 1.1  1999/09/20 22:45:27  jjo
// Import into cvs
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "DRMCUtil/DRMCUpdateBeginRun.h"

// Frame related
#include "DataHandler/Frame.h"


// Initialize DRMC fortran access to constants
#include "DRMCUtil/InitDRMCFrameBrokers.h"

extern "C"{
   extern void drmc_beginrun_();  // updates DR MC resolution constants
}
   
// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRMCUtil.DRMCUpdateBeginRun" ;

void DRMCUpdateBeginRun( Frame& iFrame )
{
   report( ERROR, kFacilityString )
      << "dummy routine should not be called now !" << endl;
}

