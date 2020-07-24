// -*- C++ -*-
//
// Package:     McCannProd
// Module:      T_McCannFitSVRHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_McCannFitSVRHitLatticeProxy.cc,v 1.1.1.1 1999/09/10 21:58:59 wsun Exp $
//
// Revision history
//
// $Log: T_McCannFitSVRHitLatticeProxy.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:59  wsun
// Transfered from McCannFilter
//
// Revision 1.2  1999/07/15 23:01:18  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:14:02  wsun
// First submission of producer.
//
//
//
#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackFitter/TrackFitSVRHitLink.h"
#include "McCannProd/McCannFitSVRHitLatticeProxy.h"
#include "McCannProd/Template/McCannFitSVRHitLatticeProxy.cc"

#include <vector>
#include <map>

template class McCannFitSVRHitLatticeProxy< DBCandidate::kElectron > ;
template class McCannFitSVRHitLatticeProxy< DBCandidate::kMuon > ;
template class McCannFitSVRHitLatticeProxy< DBCandidate::kChargedPion > ;
template class McCannFitSVRHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class McCannFitSVRHitLatticeProxy< DBCandidate::kProton > ;
template class McCannFitSVRHitLatticeProxy< DBCandidate::kExitElectron > ;
template class McCannFitSVRHitLatticeProxy< DBCandidate::kExitMuon > ;
template class McCannFitSVRHitLatticeProxy< DBCandidate::kExitPion > ;
template class McCannFitSVRHitLatticeProxy< DBCandidate::kExitKaon > ;
template class McCannFitSVRHitLatticeProxy< DBCandidate::kExitProton > ;

