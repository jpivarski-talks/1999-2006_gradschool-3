// -*- C++ -*-
//
// Package:     PivarskiFilter
// Module:      T_PivarskiTrackFitter
// 
// Description: Instantiate Classes for PivarskiTrackFitter
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon May 10 17:59:14 EDT 1999
// $Id: T_PivarskiTrackFitter.cc,v 1.1 1999/05/14 01:14:07 wsun Exp $
//
// Revision history
//
// $Log: T_PivarskiTrackFitter.cc,v $
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
#include "PivarskiFilter/PivarskiTrackFitter.h"

// STL classes
#include <vector>
#include <map>

#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< PivarskiTrackFitter >;

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( PivarskiTrackFitter )
//Use the following if 'PivarskiTrackFitter' contains a ','
// also make sure there are no blank spaces within 'PivarskiTrackFitter'
/* INSTANTIATE_ONE_1_COMMA( PivarskiTrackFitter ) */
#include "FrameAccess/instantiate_one.h"

