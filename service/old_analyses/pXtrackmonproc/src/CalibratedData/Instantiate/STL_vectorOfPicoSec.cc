// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      STL_vectorOfPicoSec
// 
// Description: Instantiate vector<PicoSec>
//
// Implementation:
//
// Author:      Jon Thaler
// Created:     Thu Jan 8 16:48:46 EST 1998
//              Thu Apr 2 1998 replace Calib* with Calibrated*
//
// Revision history
// Inga Karliner 3/18/98
//   Utility changed to STLUtility
// Revision  4/15/98 Inga Karliner
//   no code change, typedef float-> double CalibratedTimeHit::PicoSec 
//
#include "Experiment/Experiment.h"
#include "Experiment/types.h"

#include <stdlib.h>
#include <assert.h>
// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "CalibratedData/CalibratedTimeHit.h"

// STL classes
#include <vector>

typedef CalibratedTimeHit::PicoSec _vector_contents_;

#include "STLUtility/instantiate_vector.h"
