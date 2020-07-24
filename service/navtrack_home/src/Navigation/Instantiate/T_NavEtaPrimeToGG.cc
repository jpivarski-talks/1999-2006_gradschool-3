// -*- C++ -*-
//
// Package:     Navigation
// Module:      T_NavEtaPrimeToGG
// 
// Description: Instantiate Classes for NavEtaPrimeToGG
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:20:44 EDT 1999
// $Id: T_NavEtaPrimeToGG.cc,v 1.1 2001/05/10 19:55:10 bkh Exp $
//
// Revision history
//
// $Log: T_NavEtaPrimeToGG.cc,v $
// Revision 1.1  2001/05/10 19:55:10  bkh
// Addition of new etaPrime class
//
// Revision 1.1  1999/08/09 16:25:25  bkh
// Massive renaming
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Navigation/NavEtaPrimeToGG.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( NavEtaPrimeToGG )
//Use the following if 'NavEtaPrimeToGG' contains a ','
// also make sure there are no blank spaces within 'NavEtaPrimeToGG'
/* INSTANTIATE_MANY_1_COMMA( NavEtaPrimeToGG ) */
#include "FrameAccess/instantiate_many.h"
