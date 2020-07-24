// -*- C++ -*-
//
// Package:     McCannFilter
// Module:      T_McCannTrackFitter
// 
// Description: Instantiate Classes for McCannTrackFitter
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon May 10 17:59:14 EDT 1999
// $Id: T_McCannTrackFitter.cc,v 1.1 1999/05/14 01:14:07 wsun Exp $
//
// Revision history
//
// $Log: T_McCannTrackFitter.cc,v $
// Revision 1.1  1999/05/14 01:14:07  wsun
// First submission of producer.
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "McCannFilter/McCannTrackFitter.h"

// STL classes
#include <vector>
#include <map>

#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< McCannTrackFitter >;

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( McCannTrackFitter )
//Use the following if 'McCannTrackFitter' contains a ','
// also make sure there are no blank spaces within 'McCannTrackFitter'
/* INSTANTIATE_ONE_1_COMMA( McCannTrackFitter ) */
#include "FrameAccess/instantiate_one.h"

