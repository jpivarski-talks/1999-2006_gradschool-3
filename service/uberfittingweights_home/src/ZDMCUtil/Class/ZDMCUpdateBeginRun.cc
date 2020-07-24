// -*- C++ -*-
//
// Package:     <ZDMCUtil>
// Module:      ZDMCUpdateBeginRun
// 
// Description: C++ Function to make begin run calls for ZD MC simulation
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Guangshun Huang
// Created:     Tue Dec 17, 2002
//
// Revision history
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ZDMCUtil/ZDMCUpdateBeginRun.h"

// Frame related
#include "DataHandler/Frame.h"

// Initialize ZDMC fortran access to constants
#include "ZDMCUtil/InitZDMCFrameBrokers.h"

extern "C"{
   extern void zdmc_beginrun_();  // updates ZD MC resolution constants
}
   
// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ZDMCUtil.ZDMCUpdateBeginRun" ;

void ZDMCUpdateBeginRun( Frame& iFrame )
{
   report( ERROR, kFacilityString )
      << "dummy routine should not be called now !" << endl;
}
