// -*- C++ -*-
//
// Package:     KalmanProd
// Module:      T_KalmanFitCathodeHitLatticeProxy
// 
// Description:
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Sep  7 01:08:39 EDT 1999
// $Id: T_KalmanFitCathodeHitLatticeProxy.cc,v 1.1.1.1 1999/09/10 21:58:59 wsun Exp $
//
// Revision history
//
// $Log: T_KalmanFitCathodeHitLatticeProxy.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:59  wsun
// Transfered from KalmanFilter
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackFitter/TrackFitCathodeHitLink.h"
#include "KalmanProd/KalmanFitCathodeHitLatticeProxy.h"
#include "KalmanProd/Template/KalmanFitCathodeHitLatticeProxy.cc"

#include <vector>
#include <map>

template class KalmanFitCathodeHitLatticeProxy< DBCandidate::kElectron > ;
template class KalmanFitCathodeHitLatticeProxy< DBCandidate::kMuon > ;
template class KalmanFitCathodeHitLatticeProxy< DBCandidate::kChargedPion > ;
template class KalmanFitCathodeHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class KalmanFitCathodeHitLatticeProxy< DBCandidate::kProton > ;
template class KalmanFitCathodeHitLatticeProxy< DBCandidate::kExitElectron > ;
template class KalmanFitCathodeHitLatticeProxy< DBCandidate::kExitMuon > ;
template class KalmanFitCathodeHitLatticeProxy< DBCandidate::kExitPion > ;
template class KalmanFitCathodeHitLatticeProxy< DBCandidate::kExitKaon > ;
template class KalmanFitCathodeHitLatticeProxy< DBCandidate::kExitProton > ;
