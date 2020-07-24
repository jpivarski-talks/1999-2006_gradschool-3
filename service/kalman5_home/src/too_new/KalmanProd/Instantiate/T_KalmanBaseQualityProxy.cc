// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      T_KalmanBaseQualityProxy
// 
// Description:
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_KalmanBaseQualityProxy.cc,v 1.1 1999/09/11 19:37:57 wsun Exp $
//
// Revision history
//
// $Log: T_KalmanBaseQualityProxy.cc,v $
// Revision 1.1  1999/09/11 19:37:57  wsun
// Moved from KalmanFilter.
//
// Revision 1.2  1999/07/15 23:01:11  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:13:55  wsun
// First submission of producer.
//
//
//
#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "KalmanProd/KalmanBaseQualityProxy.h"
#include "KalmanProd/Template/KalmanBaseQualityProxy.cc"

#include <vector>
#include <map>

template class KalmanBaseQualityProxy< DBCandidate::kElectron > ;
template class KalmanBaseQualityProxy< DBCandidate::kMuon > ;
template class KalmanBaseQualityProxy< DBCandidate::kChargedPion > ;
template class KalmanBaseQualityProxy< DBCandidate::kChargedKaon > ;
template class KalmanBaseQualityProxy< DBCandidate::kProton > ;
template class KalmanBaseQualityProxy< DBCandidate::kExitElectron > ;
template class KalmanBaseQualityProxy< DBCandidate::kExitMuon > ;
template class KalmanBaseQualityProxy< DBCandidate::kExitPion > ;
template class KalmanBaseQualityProxy< DBCandidate::kExitKaon > ;
template class KalmanBaseQualityProxy< DBCandidate::kExitProton > ;

/*
template class
ProxyBindableTemplate< FAPtrTable<
TRTrackCandidateFitQuality< DBCandidate::kElectron > > >;
template class
ProxyBindableTemplate< FAPtrTable<
TRTrackCandidateFitQuality< DBCandidate::kMuon > > >;
template class
ProxyBindableTemplate< FAPtrTable<
TRTrackCandidateFitQuality< DBCandidate::kChargedPion > > >;
template class
ProxyBindableTemplate< FAPtrTable<
TRTrackCandidateFitQuality< DBCandidate::kChargedKaon > > >;
template class
ProxyBindableTemplate< FAPtrTable<
TRTrackCandidateFitQuality< DBCandidate::kProton > > >;
template class
ProxyBindableTemplate< FAPtrTable<
TRTrackCandidateFitQuality< DBCandidate::kExitElectron > > >;
template class
ProxyBindableTemplate< FAPtrTable<
TRTrackCandidateFitQuality< DBCandidate::kExitMuon > > >;
template class
ProxyBindableTemplate< FAPtrTable<
TRTrackCandidateFitQuality< DBCandidate::kExitPion > > >;
template class
ProxyBindableTemplate< FAPtrTable<
TRTrackCandidateFitQuality< DBCandidate::kExitKaon > > >;
template class
ProxyBindableTemplate< FAPtrTable<
TRTrackCandidateFitQuality< DBCandidate::kExitProton > > >;

*/
