// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      T_HIFitHelix.cc
// 
// Description: Instantiate Classes for HIFitHelix
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Wed Oct 14 01:18:24 EDT 1998
// $Id: T_HIHelix.cc,v 1.1 1999/04/27 22:11:02 wsun Exp $
//
// Revision history
//
// $Log: T_HIHelix.cc,v $
// Revision 1.1  1999/04/27 22:11:02  wsun
// Major reorganization and structural changes.
//
// Revision 1.2  1999/01/11 19:43:06  wsun
// Cleanup.
//
// Revision 1.1.1.1  1998/12/07 04:58:35  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "HelixIntersection/HIHelix.h"

typedef HIHelix _many_element_ ;
#include "FrameAccess/instantiate_many.h"
