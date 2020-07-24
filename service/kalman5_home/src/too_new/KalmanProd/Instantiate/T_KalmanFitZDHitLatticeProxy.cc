// -*- C++ -*-
//
// Package:     KalmanProd
// Module:      T_KalmanFitZDHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_KalmanFitZDHitLatticeProxy.cc,v 1.1 2003/04/17 17:05:07 bkh Exp $
//
// Revision history
//
// $Log: T_KalmanFitZDHitLatticeProxy.cc,v $
// Revision 1.1  2003/04/17 17:05:07  bkh
// Add ZD Lattice Proxy & throw exceptions if wrong config
//
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

#include "TrackFitter/TrackFitZDHitLink.h"
#include "KalmanProd/KalmanFitZDHitLatticeProxy.h"
#include "KalmanProd/Template/KalmanFitZDHitLatticeProxy.cc"

#include <vector>
#include <map>

template class KalmanFitZDHitLatticeProxy< DBCandidate::kElectron > ;
template class KalmanFitZDHitLatticeProxy< DBCandidate::kMuon > ;
template class KalmanFitZDHitLatticeProxy< DBCandidate::kChargedPion > ;
template class KalmanFitZDHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class KalmanFitZDHitLatticeProxy< DBCandidate::kProton > ;
template class KalmanFitZDHitLatticeProxy< DBCandidate::kExitElectron > ;
template class KalmanFitZDHitLatticeProxy< DBCandidate::kExitMuon > ;
template class KalmanFitZDHitLatticeProxy< DBCandidate::kExitPion > ;
template class KalmanFitZDHitLatticeProxy< DBCandidate::kExitKaon > ;
template class KalmanFitZDHitLatticeProxy< DBCandidate::kExitProton > ;
