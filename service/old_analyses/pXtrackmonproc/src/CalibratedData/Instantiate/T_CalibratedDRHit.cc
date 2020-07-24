// -*- C++ -*-
//
// Package:     CalibratedData
// Module:      T_CalibratedDRHit
// 
// Description: Instantiate template classes and funtions for CalibratedDRHit
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Inga Karliner
// Created:     Mon Feb 16 16:25:23 EDT 1997
//              Thu Apr 2 1998 replace Calib* with Calibrated*
//
// Revision history
//
//
//

#include "Experiment/Experiment.h"
#include "Experiment/types.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <string>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "CalibratedData/CalibratedDRHit.h"

// Instantiate the storage helper
#include "CalibratedData/ManyCalibratedDRHitStorageHelper.h"
SM_REGISTER_STORAGE(ManyCalibratedDRHitStorageHelper)

// instantiate FrameAccess templates (Extractor etc.)
#include "FrameAccess/instantiate_macros.h" 
INSTANTIATE_MANY( CalibratedDRHit ) 

#include "FrameAccess/instantiate_many.h"

