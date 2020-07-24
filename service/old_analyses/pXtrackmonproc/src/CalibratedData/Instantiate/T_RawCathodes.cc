// -*- C++ -*-
//
// Package:     RawData
// Module:      T_RawCathodes
// 
// Description: Instantiate Classes for RawCathodes
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Mon Apr 20 17:12:08 EDT 1998
// $Id: T_RawCathodes.cc,v 1.2 1998/09/30 17:03:47 cdj Exp $
//
// Revision history
//
// $Log: T_RawCathodes.cc,v $
// Revision 1.2  1998/09/30 17:03:47  cdj
// use new instantiate macro
//
// Revision 1.1  1998/08/07 19:07:02  isabel
// for CalibratedCathodeHitProxy
//
// Revision 1.1  1998/06/02 19:58:50  bkh
// add dr cathodes
//
// Revision 1.1  1998/05/04 17:01:40  bkh
// grand rewrite of RawData classes
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include <string>

// user include files
#include "RawData/RawCathodes.h"

#include "FrameAccess/instantiate_macros.h" 
INSTANTIATE_ONE( RawCathodes ) 

typedef RawCathodes instantiate_one;

#include "FrameAccess/instantiate_one.h"
