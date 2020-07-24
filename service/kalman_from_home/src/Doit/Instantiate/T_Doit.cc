// -*- C++ -*-
//
// Package:     DoitProd
// Module:      T_Doit
// 
// Description: Instantiate Classes for Doit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Mon Aug 23 14:43:58 EDT 1999
// $Id: T_Doit.cc,v 1.1 2000/02/08 18:33:22 lyon Exp $
//
// Revision history
//
// $Log: T_Doit.cc,v $
// Revision 1.1  2000/02/08 18:33:22  lyon
// Moved from DoitProd -- belong here
//
// Revision 1.1.1.1  1999/09/08 18:53:33  lyon
// imported DoitProd sources
//
//

#include "Experiment/Experiment.h"
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Doit/DoitTrackFinder.h"

//#include "ProxyBind/Template/ProxyBindableTemplate.cc"
//template class ProxyBindableTemplate< DoitTrackFinder >;

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( DoitTrackFinder )
//Use the following if 'Doit' contains a ','
// also make sure there are no blank spaces within 'Doit'
/* INSTANTIATE_ONE_1_COMMA( Doit ) */
#include "FrameAccess/instantiate_one.h"

