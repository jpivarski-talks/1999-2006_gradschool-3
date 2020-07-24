// -*- C++ -*-
//
// Package:     ZDHitCorrectorProd
// Module:      T_ZDHitCorrector
// 
// Description: Instantiate Classes for ZDHitCorrector
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Guangshun Huang
// Created:     Thu Feb 27 2003
// $Id: T_ZDHitCorrector.cc,v 1.1.1.1 2003/08/30 00:17:09 wsun Exp $
//
// Revision history
//
// $Log: T_ZDHitCorrector.cc,v $
// Revision 1.1.1.1  2003/08/30 00:17:09  wsun
// Imported ZDHitCorrectorProd sources.
//
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
#include "ZDHitCorrectorProd/ZDHitCorrector.h"

// STL classes
#include <vector>
//#include <map>

// #include "ProxyBind/Template/ProxyBindableTemplate.cc"
// template class ProxyBindableTemplate< KalmanTrackFitter >;

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( ZDHitCorrector )
//Use the following if 'KalmanTrackFitter' contains a ','
// also make sure there are no blank spaces within 'KalmanTrackFitter'
/* INSTANTIATE_ONE_1_COMMA( KalmanTrackFitter ) */
#include "FrameAccess/instantiate_one.h"

