// -*- C++ -*-
//
// Package:     Navigation
// Module:      T_NavShower
// 
// Description: Instantiate Classes for NavShower
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Sun Jun 27 19:58:56 EDT 1999
// $Id: T_NavShower.cc,v 1.2 2003/09/02 01:53:10 cdj Exp $
//
// Revision history
//
// $Log: T_NavShower.cc,v $
// Revision 1.2  2003/09/02 01:53:10  cdj
// can now store NavKs and NavShowers using templated storage helper
//
// Revision 1.1  1999/06/29 21:10:50  bkh
// New classes associated with analysis-level shower object
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Navigation/NavShower.h"

#include <map>

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( NavShower )
//Use the following if 'NavShower' contains a ','
// also make sure there are no blank spaces within 'NavShower'
/* INSTANTIATE_MANY_1_COMMA( NavShower ) */
#include "FrameAccess/instantiate_many.h"

#include "Navigation/Storage/ManyNavShowerStorageHelper.h"
SM_REGISTER_STORAGE(ManyNavShowerStorageHelper)
