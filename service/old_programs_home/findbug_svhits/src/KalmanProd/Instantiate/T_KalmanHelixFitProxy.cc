// -*- C++ -*-
//
// Package:     KalmanProd
// Module:      T_KalmanHelixFitProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_KalmanHelixFitProxy.cc,v 1.1.1.1 1999/09/10 21:58:58 wsun Exp $
//
// Revision history
//
// $Log: T_KalmanHelixFitProxy.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:58  wsun
// Transfered from KalmanFilter
//
// Revision 1.2  1999/07/15 23:01:19  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:14:04  wsun
// First submission of producer.
//
//
//
#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackRoot/TRHelixCandidateFit.h"
#include "KalmanProd/KalmanHelixFitProxy.h"
#include "KalmanProd/Template/KalmanHelixFitProxy.cc"

#include <vector>
#include <map>

template class KalmanHelixFitProxy< DBCandidate::kElectron > ;
template class KalmanHelixFitProxy< DBCandidate::kMuon > ;
template class KalmanHelixFitProxy< DBCandidate::kChargedPion > ;
template class KalmanHelixFitProxy< DBCandidate::kChargedKaon > ;
template class KalmanHelixFitProxy< DBCandidate::kProton > ;
template class KalmanHelixFitProxy< DBCandidate::kExitElectron > ;
template class KalmanHelixFitProxy< DBCandidate::kExitMuon > ;
template class KalmanHelixFitProxy< DBCandidate::kExitPion > ;
template class KalmanHelixFitProxy< DBCandidate::kExitKaon > ;
template class KalmanHelixFitProxy< DBCandidate::kExitProton > ;
