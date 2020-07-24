// -*- C++ -*-
//
// Package:     ChisqFitProducer
// Module:      T_ChisqFitSVRHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Jun 20 17:59:23 EDT 1999
// $Id: T_ChisqFitSVRHitLatticeProxy.cc,v 1.2 1999/07/17 20:28:46 cdj Exp $
//
// Revision history
//
// $Log: T_ChisqFitSVRHitLatticeProxy.cc,v $
// Revision 1.2  1999/07/17 20:28:46  cdj
// now compiles when CC optimization is on
//
// Revision 1.1  1999/06/20 22:48:16  wsun
// Added proxies for all ten fit hypotheses.
//
//
//
#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackFitter/TrackFitSVRHitLink.h"
#include "ChisqFitProducer/ChisqFitSVRHitLatticeProxy.h"
#include "ChisqFitProducer/Template/ChisqFitSVRHitLatticeProxy.cc"

#include <vector>
#include <map>

template class ChisqFitSVRHitLatticeProxy< DBCandidate::kElectron > ;
template class ChisqFitSVRHitLatticeProxy< DBCandidate::kMuon > ;
template class ChisqFitSVRHitLatticeProxy< DBCandidate::kChargedPion > ;
template class ChisqFitSVRHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class ChisqFitSVRHitLatticeProxy< DBCandidate::kProton > ;
template class ChisqFitSVRHitLatticeProxy< DBCandidate::kExitElectron > ;
template class ChisqFitSVRHitLatticeProxy< DBCandidate::kExitMuon > ;
template class ChisqFitSVRHitLatticeProxy< DBCandidate::kExitPion > ;
template class ChisqFitSVRHitLatticeProxy< DBCandidate::kExitKaon > ;
template class ChisqFitSVRHitLatticeProxy< DBCandidate::kExitProton > ;
