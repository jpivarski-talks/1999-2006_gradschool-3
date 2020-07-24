// -*- C++ -*-
//
// Package:     ChisqFitProducer
// Module:      T_ChisqFitSVZHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Jun 20 17:59:23 EDT 1999
// $Id: T_ChisqFitSVZHitLatticeProxy.cc,v 1.2 1999/07/17 20:28:46 cdj Exp $
//
// Revision history
//
// $Log: T_ChisqFitSVZHitLatticeProxy.cc,v $
// Revision 1.2  1999/07/17 20:28:46  cdj
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

#include "TrackFitter/TrackFitSVZHitLink.h"
#include "ChisqFitProducer/ChisqFitSVZHitLatticeProxy.h"
#include "ChisqFitProducer/Template/ChisqFitSVZHitLatticeProxy.cc"

#include <vector>
#include <map>


template class ChisqFitSVZHitLatticeProxy< DBCandidate::kElectron > ;
template class ChisqFitSVZHitLatticeProxy< DBCandidate::kMuon > ;
template class ChisqFitSVZHitLatticeProxy< DBCandidate::kChargedPion > ;
template class ChisqFitSVZHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class ChisqFitSVZHitLatticeProxy< DBCandidate::kProton > ;
template class ChisqFitSVZHitLatticeProxy< DBCandidate::kExitElectron > ;
template class ChisqFitSVZHitLatticeProxy< DBCandidate::kExitMuon > ;
template class ChisqFitSVZHitLatticeProxy< DBCandidate::kExitPion > ;
template class ChisqFitSVZHitLatticeProxy< DBCandidate::kExitKaon > ;
template class ChisqFitSVZHitLatticeProxy< DBCandidate::kExitProton > ;
