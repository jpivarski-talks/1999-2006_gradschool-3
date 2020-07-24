// -*- C++ -*-
//
// Package:     DRAlignmentMod
// Module:      T_CalibratedHit.cc
// 
// Description: Instantiate Classes for CalibratedHit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Dec  8 11:57:24 EST 2003
// $Id: 
//
// Revision history
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// #include <string>
// #include <map>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// #include <string>
// #include <map>
#include <vector>

// user include files
#include "CalibratedData/CalibratedHit.h"

typedef CalibratedHit _many_element_ ;

#include "FrameAccess/instantiate_many.h"
