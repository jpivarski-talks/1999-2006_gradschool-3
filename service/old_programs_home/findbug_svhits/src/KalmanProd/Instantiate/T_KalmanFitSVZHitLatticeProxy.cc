// -*- C++ -*-
//
// Package:     KalmanProd
// Module:      T_KalmanFitSVZHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_KalmanFitSVZHitLatticeProxy.cc,v 1.1.1.1 1999/09/10 21:58:59 wsun Exp $
//
// Revision history
//
// $Log: T_KalmanFitSVZHitLatticeProxy.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:59  wsun
// Transfered from KalmanFilter
//
// Revision 1.2  1999/07/15 23:01:18  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:14:03  wsun
// First submission of producer.
//
//
//
#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackFitter/TrackFitSVZHitLink.h"
#include "KalmanProd/KalmanFitSVZHitLatticeProxy.h"
#include "KalmanProd/Template/KalmanFitSVZHitLatticeProxy.cc"

#include <vector>
#include <map>


template class KalmanFitSVZHitLatticeProxy< DBCandidate::kElectron > ;
template class KalmanFitSVZHitLatticeProxy< DBCandidate::kMuon > ;
template class KalmanFitSVZHitLatticeProxy< DBCandidate::kChargedPion > ;
template class KalmanFitSVZHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class KalmanFitSVZHitLatticeProxy< DBCandidate::kProton > ;
template class KalmanFitSVZHitLatticeProxy< DBCandidate::kExitElectron > ;
template class KalmanFitSVZHitLatticeProxy< DBCandidate::kExitMuon > ;
template class KalmanFitSVZHitLatticeProxy< DBCandidate::kExitPion > ;
template class KalmanFitSVZHitLatticeProxy< DBCandidate::kExitKaon > ;
template class KalmanFitSVZHitLatticeProxy< DBCandidate::kExitProton > ;
