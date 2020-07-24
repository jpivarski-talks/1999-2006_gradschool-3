// -*- C++ -*-
//
// Package:     PivarskiProd
// Module:      T_PivarskiHelixFitProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_PivarskiHelixFitProxy.cc,v 1.1.1.1 1999/09/10 21:58:58 wsun Exp $
//
// Revision history
//
// $Log: T_PivarskiHelixFitProxy.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:58  wsun
// Transfered from PivarskiFilter
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
#include "PivarskiProd/PivarskiHelixFitProxy.h"
#include "PivarskiProd/Template/PivarskiHelixFitProxy.cc"

#include <vector>
#include <map>

template class PivarskiHelixFitProxy< DBCandidate::kElectron > ;
template class PivarskiHelixFitProxy< DBCandidate::kMuon > ;
template class PivarskiHelixFitProxy< DBCandidate::kChargedPion > ;
template class PivarskiHelixFitProxy< DBCandidate::kChargedKaon > ;
template class PivarskiHelixFitProxy< DBCandidate::kProton > ;
template class PivarskiHelixFitProxy< DBCandidate::kExitElectron > ;
template class PivarskiHelixFitProxy< DBCandidate::kExitMuon > ;
template class PivarskiHelixFitProxy< DBCandidate::kExitPion > ;
template class PivarskiHelixFitProxy< DBCandidate::kExitKaon > ;
template class PivarskiHelixFitProxy< DBCandidate::kExitProton > ;
