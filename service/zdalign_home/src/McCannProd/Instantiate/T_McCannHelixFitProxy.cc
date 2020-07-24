// -*- C++ -*-
//
// Package:     McCannProd
// Module:      T_McCannHelixFitProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_McCannHelixFitProxy.cc,v 1.1.1.1 1999/09/10 21:58:58 wsun Exp $
//
// Revision history
//
// $Log: T_McCannHelixFitProxy.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:58  wsun
// Transfered from McCannFilter
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
#include "McCannProd/McCannHelixFitProxy.h"
#include "McCannProd/Template/McCannHelixFitProxy.cc"

#include <vector>
#include <map>

template class McCannHelixFitProxy< DBCandidate::kElectron > ;
template class McCannHelixFitProxy< DBCandidate::kMuon > ;
template class McCannHelixFitProxy< DBCandidate::kChargedPion > ;
template class McCannHelixFitProxy< DBCandidate::kChargedKaon > ;
template class McCannHelixFitProxy< DBCandidate::kProton > ;
template class McCannHelixFitProxy< DBCandidate::kExitElectron > ;
template class McCannHelixFitProxy< DBCandidate::kExitMuon > ;
template class McCannHelixFitProxy< DBCandidate::kExitPion > ;
template class McCannHelixFitProxy< DBCandidate::kExitKaon > ;
template class McCannHelixFitProxy< DBCandidate::kExitProton > ;
