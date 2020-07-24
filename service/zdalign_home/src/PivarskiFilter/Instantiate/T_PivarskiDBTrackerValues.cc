// -*- C++ -*-
//
// Package:     PivarskiFilter
// Module:      T_PivarskiDBTrackerValues
// 
// Description: Instantiate Classes for PivarskiDBTrackerValues
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 23:19:13 EDT 1999
// $Id: T_PivarskiDBTrackerValues.cc,v 1.1 1999/05/14 01:13:56 wsun Exp $
//
// Revision history
//
// $Log: T_PivarskiDBTrackerValues.cc,v $
// Revision 1.1  1999/05/14 01:13:56  wsun
// First submission of producer.
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "CleoDB/DBTrackerValues.h"

#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< DBTrackerValues >;

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( DBTrackerValues )
//Use the following if 'PivarskiDBTrackerValues' contains a ','
// also make sure there are no blank spaces within 'PivarskiDBTrackerValues'
/* INSTANTIATE_ONE_1_COMMA( DBTrackerValues ) */
#include "FrameAccess/instantiate_one.h"

