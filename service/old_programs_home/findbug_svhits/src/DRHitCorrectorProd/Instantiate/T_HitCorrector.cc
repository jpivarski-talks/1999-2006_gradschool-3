// -*- C++ -*-
//
// Package:     DRHitCorrectorProd
// Module:      T_HitCorrector
// 
// Description: Instantiate Classes for HitCorrector
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Oct  3 17:22:16 EDT 1999
// $Id: T_HitCorrector.cc,v 1.1 1999/10/04 18:19:32 wsun Exp $
//
// Revision history
//
// $Log: T_HitCorrector.cc,v $
// Revision 1.1  1999/10/04 18:19:32  wsun
// First submission.
//
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
//#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DRHitCorrectorProd/HitCorrector.h"

// STL classes
#include <vector>
//#include <map>

// #include "ProxyBind/Template/ProxyBindableTemplate.cc"
// template class ProxyBindableTemplate< KalmanTrackFitter >;

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( HitCorrector )
//Use the following if 'KalmanTrackFitter' contains a ','
// also make sure there are no blank spaces within 'KalmanTrackFitter'
/* INSTANTIATE_ONE_1_COMMA( KalmanTrackFitter ) */
#include "FrameAccess/instantiate_one.h"

