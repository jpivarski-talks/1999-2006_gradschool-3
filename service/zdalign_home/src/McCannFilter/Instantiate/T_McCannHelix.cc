// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      T_McCannHelix.cc
// 
// Description: Instantiate Classes for McCannHelix
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Apr  5 21:37:36 EDT 1999
// $Id: T_McCannHelix.cc,v 1.1 1999/04/27 22:21:30 wsun Exp $
//
// Revision history
//
// $Log: T_McCannHelix.cc,v $
// Revision 1.1  1999/04/27 22:21:30  wsun
// Major reorganization and structural changes.
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "McCannFilter/McCannHelix.h"

typedef McCannHelix _many_element_ ;
#include "FrameAccess/instantiate_many.h"
