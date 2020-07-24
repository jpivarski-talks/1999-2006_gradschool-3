// -*- C++ -*-
//
// Package:     DRAlignmentMod
// Module:      T_HIFitHelix.cc
// 
// Description: Instantiate Classes for HIFitHelix
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Dec  8 11:57:55 EST 2003
// $Id: 
//
// Revision history
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "HelixIntersection/HIHelix.h"

typedef HIHelix _many_element_ ;
#include "FrameAccess/instantiate_many.h"
