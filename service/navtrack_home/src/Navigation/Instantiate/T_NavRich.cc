// -*- C++ -*-
//
// Package:     Navigation
// Module:      T_NavRich
// 
// Description: Instantiate Classes for NavRich
//
// Implementation:
//
// Author:      Tomasz Skwarnicki
// Created:     Sun Jul 23 05:50:34 EDT 2000
// $Id: T_NavRich.cc,v 1.1 2000/07/27 01:32:00 ts Exp $
//
// Revision history
//
// $Log: T_NavRich.cc,v $
// Revision 1.1  2000/07/27 01:32:00  ts
// NavRich added
//
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Navigation/NavRich.h"

#include <map>

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( NavRich )
//Use the following if 'NavRich' contains a ','
// also make sure there are no blank spaces within 'NavRich'
/* INSTANTIATE_MANY_1_COMMA( NavRich ) */
#include "FrameAccess/instantiate_many.h"
