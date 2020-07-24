// -*- C++ -*-
//
// Package:     ChisqFitProducer
// Module:      T_ChisqBaseQualityProxy
// 
// Description:
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Jun 20 17:59:23 EDT 1999
// $Id: T_ChisqBaseQualityProxy.cc,v 1.2 1999/07/17 20:28:38 cdj Exp $
//
// Revision history
//
// $Log: T_ChisqBaseQualityProxy.cc,v $
// Revision 1.2  1999/07/17 20:28:38  cdj
// now compiles when CC optimization is on
//
// Revision 1.1  1999/06/20 22:48:09  wsun
// Added proxies for all ten fit hypotheses.
//
//
//
#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "ChisqFitProducer/ChisqBaseQualityProxy.h"
#include "ChisqFitProducer/Template/ChisqBaseQualityProxy.cc"
#include "ProxyBind/Template/ProxyBindableTemplate.cc"

#include <vector>
#include <map>

template class ChisqBaseQualityProxy< DBCandidate::kElectron > ;
template class ChisqBaseQualityProxy< DBCandidate::kMuon > ;
template class ChisqBaseQualityProxy< DBCandidate::kChargedPion > ;
template class ChisqBaseQualityProxy< DBCandidate::kChargedKaon > ;
template class ChisqBaseQualityProxy< DBCandidate::kProton > ;
template class ChisqBaseQualityProxy< DBCandidate::kExitElectron > ;
template class ChisqBaseQualityProxy< DBCandidate::kExitMuon > ;
template class ChisqBaseQualityProxy< DBCandidate::kExitPion > ;
template class ChisqBaseQualityProxy< DBCandidate::kExitKaon > ;
template class ChisqBaseQualityProxy< DBCandidate::kExitProton > ;
