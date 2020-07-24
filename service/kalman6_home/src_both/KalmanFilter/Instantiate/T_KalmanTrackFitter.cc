// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      T_KalmanTrackFitter
// 
// Description: Instantiate Classes for KalmanTrackFitter
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon May 10 17:59:14 EDT 1999
// $Id: T_KalmanTrackFitter.cc,v 1.1 1999/05/14 01:14:07 wsun Exp $
//
// Revision history
//
// $Log: T_KalmanTrackFitter.cc,v $
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
#include "KalmanFilter/KalmanTrackFitter.h"

// STL classes
#include <vector>
#include <map>

#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< KalmanTrackFitter >;

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( KalmanTrackFitter )
//Use the following if 'KalmanTrackFitter' contains a ','
// also make sure there are no blank spaces within 'KalmanTrackFitter'
/* INSTANTIATE_ONE_1_COMMA( KalmanTrackFitter ) */
#include "FrameAccess/instantiate_one.h"

