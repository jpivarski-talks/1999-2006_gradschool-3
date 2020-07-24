// -*- C++ -*-
//
// Package:     ChisqFitProducer
// Module:      T_ChisqFitZDHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Jun 20 17:59:23 EDT 1999
// $Id: T_ChisqFitZDHitLatticeProxy.cc,v 1.1 2003/04/17 17:00:50 bkh Exp $
//
// Revision history
//
// $Log: T_ChisqFitZDHitLatticeProxy.cc,v $
// Revision 1.1  2003/04/17 17:00:50  bkh
// Add ZD options & throw exceptions for wrong config
//
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

#include "TrackFitter/TrackFitZDHitLink.h"
#include "ChisqFitProducer/ChisqFitZDHitLatticeProxy.h"
#include "ChisqFitProducer/Template/ChisqFitZDHitLatticeProxy.cc"

#include <vector>
#include <map>

template class ChisqFitZDHitLatticeProxy< DBCandidate::kElectron > ;
template class ChisqFitZDHitLatticeProxy< DBCandidate::kMuon > ;
template class ChisqFitZDHitLatticeProxy< DBCandidate::kChargedPion > ;
template class ChisqFitZDHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class ChisqFitZDHitLatticeProxy< DBCandidate::kProton > ;
template class ChisqFitZDHitLatticeProxy< DBCandidate::kExitElectron > ;
template class ChisqFitZDHitLatticeProxy< DBCandidate::kExitMuon > ;
template class ChisqFitZDHitLatticeProxy< DBCandidate::kExitPion > ;
template class ChisqFitZDHitLatticeProxy< DBCandidate::kExitKaon > ;
template class ChisqFitZDHitLatticeProxy< DBCandidate::kExitProton > ;
