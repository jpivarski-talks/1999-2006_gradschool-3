// -*- C++ -*-
//
// Package:     ChisqFitProducer
// Module:      T_ChisqExitElectronQualityProxy
// 
// Description: Instantiate Classes for ChisqCandidateQuality
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Jun 20 17:59:23 EDT 1999
// $Id: T_ChisqExitElectronQualityProxy.cc,v 1.4 2000/09/05 20:42:36 wsun Exp $
//
// Revision history
//
// $Log: T_ChisqExitElectronQualityProxy.cc,v $
// Revision 1.4  2000/09/05 20:42:36  wsun
// Moved Quality FATable instantiation to ChisqFitter.
//
// Revision 1.3  1999/09/11 19:34:20  wsun
// Added DR cathodes.
//
// Revision 1.2  1999/07/17 20:28:42  cdj
// now compiles when CC optimization is on
//
// Revision 1.1  1999/06/20 22:48:10  wsun
// Added proxies for all ten fit hypotheses.
//
//
//
//
#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "FrameAccess/FAPtrTable.h"
#include "ChisqFitter/ChisqCandidateQuality.h"
#include "ChisqFitProducer/ChisqCandidateQualityProxy.h"
#include "ChisqFitProducer/Template/ChisqCandidateQualityProxy.cc"

#include <vector>
#include <map>

template class ChisqCandidateQualityProxy< DBCandidate::kExitElectron > ;

// typedef ChisqCandidateQuality< DBCandidate::kExitElectron > ChisqExitElectronQuality;
// #include "FrameAccess/instantiate_macros.h"
// INSTANTIATE_MANY( ChisqExitElectronQuality )
// #include "FrameAccess/instantiate_many.h"
