// -*- C++ -*-
//
// Package:     KalmanProd
// Module:      T_KalmanFitSVRHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_KalmanFitSVRHitLatticeProxy.cc,v 1.1.1.1 1999/09/10 21:58:59 wsun Exp $
//
// Revision history
//
// $Log: T_KalmanFitSVRHitLatticeProxy.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:59  wsun
// Transfered from KalmanFilter
//
// Revision 1.2  1999/07/15 23:01:18  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:14:02  wsun
// First submission of producer.
//
//
//
#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackFitter/TrackFitSVRHitLink.h"
#include "KalmanProd/KalmanFitSVRHitLatticeProxy.h"
#include "KalmanProd/Template/KalmanFitSVRHitLatticeProxy.cc"

#include <vector>
#include <map>

template class KalmanFitSVRHitLatticeProxy< DBCandidate::kElectron > ;
template class KalmanFitSVRHitLatticeProxy< DBCandidate::kMuon > ;
template class KalmanFitSVRHitLatticeProxy< DBCandidate::kChargedPion > ;
template class KalmanFitSVRHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class KalmanFitSVRHitLatticeProxy< DBCandidate::kProton > ;
template class KalmanFitSVRHitLatticeProxy< DBCandidate::kExitElectron > ;
template class KalmanFitSVRHitLatticeProxy< DBCandidate::kExitMuon > ;
template class KalmanFitSVRHitLatticeProxy< DBCandidate::kExitPion > ;
template class KalmanFitSVRHitLatticeProxy< DBCandidate::kExitKaon > ;
template class KalmanFitSVRHitLatticeProxy< DBCandidate::kExitProton > ;

