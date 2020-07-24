// -*- C++ -*-
//
// Package:     PivarskiFilter
// Module:      T_PivarskiBaseQualityProxy
// 
// Description:
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_PivarskiBaseQualityProxy.cc,v 1.1 1999/09/11 19:37:57 wsun Exp $
//
// Revision history
//
// $Log: T_PivarskiBaseQualityProxy.cc,v $
// Revision 1.1  1999/09/11 19:37:57  wsun
// Moved from PivarskiFilter.
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

#include "PivarskiProd/PivarskiBaseQualityProxy.h"
#include "PivarskiProd/Template/PivarskiBaseQualityProxy.cc"

#include <vector>
#include <map>

template class PivarskiBaseQualityProxy< DBCandidate::kElectron > ;
template class PivarskiBaseQualityProxy< DBCandidate::kMuon > ;
template class PivarskiBaseQualityProxy< DBCandidate::kChargedPion > ;
template class PivarskiBaseQualityProxy< DBCandidate::kChargedKaon > ;
template class PivarskiBaseQualityProxy< DBCandidate::kProton > ;
template class PivarskiBaseQualityProxy< DBCandidate::kExitElectron > ;
template class PivarskiBaseQualityProxy< DBCandidate::kExitMuon > ;
template class PivarskiBaseQualityProxy< DBCandidate::kExitPion > ;
template class PivarskiBaseQualityProxy< DBCandidate::kExitKaon > ;
template class PivarskiBaseQualityProxy< DBCandidate::kExitProton > ;

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
