// -*- C++ -*-
//
// Package:     McCannProd
// Module:      T_McCannFitDRHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_McCannFitDRHitLatticeProxy.cc,v 1.1.1.1 1999/09/10 21:58:59 wsun Exp $
//
// Revision history
//
// $Log: T_McCannFitDRHitLatticeProxy.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:59  wsun
// Transfered from McCannFilter
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

#include "TrackFitter/TrackFitDRHitLink.h"
#include "McCannProd/McCannFitDRHitLatticeProxy.h"
#include "McCannProd/Template/McCannFitDRHitLatticeProxy.cc"

#include <vector>
#include <map>

template class McCannFitDRHitLatticeProxy< DBCandidate::kElectron > ;
template class McCannFitDRHitLatticeProxy< DBCandidate::kMuon > ;
template class McCannFitDRHitLatticeProxy< DBCandidate::kChargedPion > ;
template class McCannFitDRHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class McCannFitDRHitLatticeProxy< DBCandidate::kProton > ;
template class McCannFitDRHitLatticeProxy< DBCandidate::kExitElectron > ;
template class McCannFitDRHitLatticeProxy< DBCandidate::kExitMuon > ;
template class McCannFitDRHitLatticeProxy< DBCandidate::kExitPion > ;
template class McCannFitDRHitLatticeProxy< DBCandidate::kExitKaon > ;
template class McCannFitDRHitLatticeProxy< DBCandidate::kExitProton > ;
