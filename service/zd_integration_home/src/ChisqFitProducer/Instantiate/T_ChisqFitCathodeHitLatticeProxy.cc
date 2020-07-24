// -*- C++ -*-
//
// Package:     ChisqFitProducer
// Module:      T_ChisqFitCathodeHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Sep 10 19:14:08 EDT 1999
// $Id: T_ChisqFitCathodeHitLatticeProxy.cc,v 1.1 1999/09/11 19:34:24 wsun Exp $
//
// Revision history
//
// $Log: T_ChisqFitCathodeHitLatticeProxy.cc,v $
// Revision 1.1  1999/09/11 19:34:24  wsun
// Added DR cathodes.
//
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackFitter/TrackFitCathodeHitLink.h"
#include "ChisqFitProducer/ChisqFitCathodeHitLatticeProxy.h"
#include "ChisqFitProducer/Template/ChisqFitCathodeHitLatticeProxy.cc"

#include <vector>
#include <map>

template class ChisqFitCathodeHitLatticeProxy< DBCandidate::kElectron > ;
template class ChisqFitCathodeHitLatticeProxy< DBCandidate::kMuon > ;
template class ChisqFitCathodeHitLatticeProxy< DBCandidate::kChargedPion > ;
template class ChisqFitCathodeHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class ChisqFitCathodeHitLatticeProxy< DBCandidate::kProton > ;
template class ChisqFitCathodeHitLatticeProxy< DBCandidate::kExitElectron > ;
template class ChisqFitCathodeHitLatticeProxy< DBCandidate::kExitMuon > ;
template class ChisqFitCathodeHitLatticeProxy< DBCandidate::kExitPion > ;
template class ChisqFitCathodeHitLatticeProxy< DBCandidate::kExitKaon > ;
template class ChisqFitCathodeHitLatticeProxy< DBCandidate::kExitProton > ;
