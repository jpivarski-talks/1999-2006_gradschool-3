// -*- C++ -*-
//
// Package:     ChisqFitProducer
// Module:      T_ChisqFitDRHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Jun 20 17:59:23 EDT 1999
// $Id: T_ChisqFitDRHitLatticeProxy.cc,v 1.2 1999/07/17 20:28:45 cdj Exp $
//
// Revision history
//
// $Log: T_ChisqFitDRHitLatticeProxy.cc,v $
// Revision 1.2  1999/07/17 20:28:45  cdj
// now compiles when CC optimization is on
//
// Revision 1.1  1999/06/20 22:48:15  wsun
// Added proxies for all ten fit hypotheses.
//
//
//
#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackFitter/TrackFitDRHitLink.h"
#include "ChisqFitProducer/ChisqFitDRHitLatticeProxy.h"
#include "ChisqFitProducer/Template/ChisqFitDRHitLatticeProxy.cc"

#include <vector>
#include <map>

template class ChisqFitDRHitLatticeProxy< DBCandidate::kElectron > ;
template class ChisqFitDRHitLatticeProxy< DBCandidate::kMuon > ;
template class ChisqFitDRHitLatticeProxy< DBCandidate::kChargedPion > ;
template class ChisqFitDRHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class ChisqFitDRHitLatticeProxy< DBCandidate::kProton > ;
template class ChisqFitDRHitLatticeProxy< DBCandidate::kExitElectron > ;
template class ChisqFitDRHitLatticeProxy< DBCandidate::kExitMuon > ;
template class ChisqFitDRHitLatticeProxy< DBCandidate::kExitPion > ;
template class ChisqFitDRHitLatticeProxy< DBCandidate::kExitKaon > ;
template class ChisqFitDRHitLatticeProxy< DBCandidate::kExitProton > ;
