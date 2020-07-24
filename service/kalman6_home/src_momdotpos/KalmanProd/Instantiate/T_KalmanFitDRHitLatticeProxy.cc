// -*- C++ -*-
//
// Package:     KalmanProd
// Module:      T_KalmanFitDRHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_KalmanFitDRHitLatticeProxy.cc,v 1.1.1.1 1999/09/10 21:58:59 wsun Exp $
//
// Revision history
//
// $Log: T_KalmanFitDRHitLatticeProxy.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:59  wsun
// Transfered from KalmanFilter
//
// Revision 1.2  1999/07/15 23:01:17  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:14:01  wsun
// First submission of producer.
//
//
//
#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackFitter/TrackFitDRHitLink.h"
#include "KalmanProd/KalmanFitDRHitLatticeProxy.h"
#include "KalmanProd/Template/KalmanFitDRHitLatticeProxy.cc"

#include <vector>
#include <map>

template class KalmanFitDRHitLatticeProxy< DBCandidate::kElectron > ;
template class KalmanFitDRHitLatticeProxy< DBCandidate::kMuon > ;
template class KalmanFitDRHitLatticeProxy< DBCandidate::kChargedPion > ;
template class KalmanFitDRHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class KalmanFitDRHitLatticeProxy< DBCandidate::kProton > ;
template class KalmanFitDRHitLatticeProxy< DBCandidate::kExitElectron > ;
template class KalmanFitDRHitLatticeProxy< DBCandidate::kExitMuon > ;
template class KalmanFitDRHitLatticeProxy< DBCandidate::kExitPion > ;
template class KalmanFitDRHitLatticeProxy< DBCandidate::kExitKaon > ;
template class KalmanFitDRHitLatticeProxy< DBCandidate::kExitProton > ;
