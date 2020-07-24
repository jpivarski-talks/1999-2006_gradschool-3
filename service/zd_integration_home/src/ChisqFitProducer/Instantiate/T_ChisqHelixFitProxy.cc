// -*- C++ -*-
//
// Package:     ChisqFitProducer
// Module:      T_ChisqHelixFitProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Jun 20 17:59:23 EDT 1999
// $Id: T_ChisqHelixFitProxy.cc,v 1.2 1999/07/17 20:28:49 cdj Exp $
//
// Revision history
//
// $Log: T_ChisqHelixFitProxy.cc,v $
// Revision 1.2  1999/07/17 20:28:49  cdj
// now compiles when CC optimization is on
//
// Revision 1.1  1999/06/20 22:48:17  wsun
// Added proxies for all ten fit hypotheses.
//
//
//
#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackRoot/TRHelixCandidateFit.h"
#include "ChisqFitProducer/ChisqHelixFitProxy.h"
#include "ChisqFitProducer/Template/ChisqHelixFitProxy.cc"

#include <vector>
#include <map>

template class ChisqHelixFitProxy< DBCandidate::kElectron > ;
template class ChisqHelixFitProxy< DBCandidate::kMuon > ;
template class ChisqHelixFitProxy< DBCandidate::kChargedPion > ;
template class ChisqHelixFitProxy< DBCandidate::kChargedKaon > ;
template class ChisqHelixFitProxy< DBCandidate::kProton > ;
template class ChisqHelixFitProxy< DBCandidate::kExitElectron > ;
template class ChisqHelixFitProxy< DBCandidate::kExitMuon > ;
template class ChisqHelixFitProxy< DBCandidate::kExitPion > ;
template class ChisqHelixFitProxy< DBCandidate::kExitKaon > ;
template class ChisqHelixFitProxy< DBCandidate::kExitProton > ;

