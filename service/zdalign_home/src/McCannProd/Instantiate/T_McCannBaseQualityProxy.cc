// -*- C++ -*-
//
// Package:     McCannFilter
// Module:      T_McCannBaseQualityProxy
// 
// Description:
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_McCannBaseQualityProxy.cc,v 1.1 1999/09/11 19:37:57 wsun Exp $
//
// Revision history
//
// $Log: T_McCannBaseQualityProxy.cc,v $
// Revision 1.1  1999/09/11 19:37:57  wsun
// Moved from McCannFilter.
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

#include "McCannProd/McCannBaseQualityProxy.h"
#include "McCannProd/Template/McCannBaseQualityProxy.cc"

#include <vector>
#include <map>

template class McCannBaseQualityProxy< DBCandidate::kElectron > ;
template class McCannBaseQualityProxy< DBCandidate::kMuon > ;
template class McCannBaseQualityProxy< DBCandidate::kChargedPion > ;
template class McCannBaseQualityProxy< DBCandidate::kChargedKaon > ;
template class McCannBaseQualityProxy< DBCandidate::kProton > ;
template class McCannBaseQualityProxy< DBCandidate::kExitElectron > ;
template class McCannBaseQualityProxy< DBCandidate::kExitMuon > ;
template class McCannBaseQualityProxy< DBCandidate::kExitPion > ;
template class McCannBaseQualityProxy< DBCandidate::kExitKaon > ;
template class McCannBaseQualityProxy< DBCandidate::kExitProton > ;

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
