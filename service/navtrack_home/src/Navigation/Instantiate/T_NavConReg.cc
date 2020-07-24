// -*- C++ -*-
//
// Package:     Navigation
// Module:      T_NavConReg
// 
// Description: Instantiate Classes for NavConReg
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Brian K. Heltsley
// Created:     Fri Jun 25 12:00:26 EDT 1999
// $Id: T_NavConReg.cc,v 1.1 1999/08/09 16:25:24 bkh Exp $
//
// Revision history
//
// $Log: T_NavConReg.cc,v $
// Revision 1.1  1999/08/09 16:25:24  bkh
// Massive renaming
//
// Revision 1.1  1999/06/29 21:10:49  bkh
// New classes associated with analysis-level shower object
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Navigation/NavConReg.h"

#include "FrameAccess/FAPtrTable.h"

#include <vector>
#include <map>

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( NavConReg )
//Use the following if 'NavConReg' contains a ','
// also make sure there are no blank spaces within 'NavConReg'
/* INSTANTIATE_MANY_1_COMMA( NavConReg ) */
#include "FrameAccess/instantiate_many.h"
