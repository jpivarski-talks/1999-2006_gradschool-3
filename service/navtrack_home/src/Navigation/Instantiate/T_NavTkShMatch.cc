// -*- C++ -*-
//
// Package:     Navigation
// Module:      T_NavTkShMatch
// 
// Description: Instantiate Classes for NavTkShMatch
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Brian K. Heltsley
// Created:     Tue Jun 22 17:00:56 EDT 1999
// $Id: T_NavTkShMatch.cc,v 1.1 1999/08/09 16:25:26 bkh Exp $
//
// Revision history
//
// $Log: T_NavTkShMatch.cc,v $
// Revision 1.1  1999/08/09 16:25:26  bkh
// Massive renaming
//
// Revision 1.1  1999/06/29 21:10:50  bkh
// New classes associated with analysis-level shower object
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Navigation/NavTkShMatch.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( NavTkShMatch )
//Use the following if 'NavTkShMatch' contains a ','
// also make sure there are no blank spaces within 'NavTkShMatch'
/* INSTANTIATE_MANY_1_COMMA( NavTkShMatch ) */
#include "FrameAccess/instantiate_many.h"
