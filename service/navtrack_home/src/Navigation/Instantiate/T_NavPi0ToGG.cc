// -*- C++ -*-
//
// Package:     Navigation
// Module:      T_NavPi0ToGG
// 
// Description: Instantiate Classes for NavPi0ToGG
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:20:44 EDT 1999
// $Id: T_NavPi0ToGG.cc,v 1.2 2001/03/26 20:13:51 urner Exp $
//
// Revision history
//
// $Log: T_NavPi0ToGG.cc,v $
// Revision 1.2  2001/03/26 20:13:51  urner
// added Nav Ks
//
// Revision 1.1  1999/08/09 16:25:26  bkh
// Massive renaming
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Navigation/NavPi0ToGG.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( NavPi0ToGG )
//Use the following if 'NavPi0ToGG' contains a ','
// also make sure there are no blank spaces within 'NavPi0ToGG'
/* INSTANTIATE_MANY_1_COMMA( NavPi0ToGG ) */
#include "FrameAccess/instantiate_many.h"

