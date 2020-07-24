// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      T_CalibratedHit.cc
// 
// Description: Instantiate Classes for CalibratedHit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Wed Oct 14 01:18:24 EDT 1998
// $Id: T_CalibratedHit.cc,v 1.2 1999/04/27 22:11:01 wsun Exp $
//
// Revision history
//
// $Log: T_CalibratedHit.cc,v $
// Revision 1.2  1999/04/27 22:11:01  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:35  wsun
// First submission.
//
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
