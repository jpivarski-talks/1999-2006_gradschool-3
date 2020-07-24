// -*- C++ -*-
//
// Package:     <Suez>
// Module:      testBoolean
// 
// Description: test if Boolean is properly implemented
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Martin Lohner, Brian Heltsley
// Created:     Thu Mar 12 10:40:25 EST 1998
// $Id: forceLinkStorageManagement.cc,v 1.1 1999/11/05 17:43:55 mkl Exp $
//
// Revision history
//
// $Log: forceLinkStorageManagement.cc,v $
// Revision 1.1  1999/11/05 17:43:55  mkl
// move force-link to StorageManagement into its own file (for better maintainance)
//
// Revision 1.1  1998/03/12 16:16:17  mkl
// added testing for Boolean problems (true .neq. .not.false)
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "StorageManagement/SMStorageHelperManager.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Suez.testBoolean" ;

void 
forceLinkStorageManagement()
{
   // force-link StorageManager, so that nobody has else to
   SMStorageHelperManager::instance();
}
