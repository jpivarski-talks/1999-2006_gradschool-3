// -*- C++ -*-
//
// Package:     DualtrackProd
// Module:      T_HIHelix.cc
// 
// Description: Instantiate Classes for HIHelix
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 10:18:52 EST 2003
// $Id: 
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "HelixIntersection/HIHelix.h"

typedef HIHelix _many_element_ ;
#include "FrameAccess/instantiate_many.h"
