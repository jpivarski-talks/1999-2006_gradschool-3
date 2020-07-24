// -*- C++ -*-
//
// Package:     Navigation
// Module:      T_NavEtaToGG
// 
// Description: Instantiate Classes for NavEtaToGG
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:20:44 EDT 1999
// $Id: T_NavEtaToGG.cc,v 1.1 1999/08/09 16:25:25 bkh Exp $
//
// Revision history
//
// $Log: T_NavEtaToGG.cc,v $
// Revision 1.1  1999/08/09 16:25:25  bkh
// Massive renaming
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Navigation/NavEtaToGG.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( NavEtaToGG )
//Use the following if 'NavEtaToGG' contains a ','
// also make sure there are no blank spaces within 'NavEtaToGG'
/* INSTANTIATE_MANY_1_COMMA( NavEtaToGG ) */
#include "FrameAccess/instantiate_many.h"
