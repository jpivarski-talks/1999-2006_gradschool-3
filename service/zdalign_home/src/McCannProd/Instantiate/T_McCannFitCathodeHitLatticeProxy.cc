// -*- C++ -*-
//
// Package:     McCannProd
// Module:      T_McCannFitCathodeHitLatticeProxy
// 
// Description:
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Sep  7 01:08:39 EDT 1999
// $Id: T_McCannFitCathodeHitLatticeProxy.cc,v 1.1.1.1 1999/09/10 21:58:59 wsun Exp $
//
// Revision history
//
// $Log: T_McCannFitCathodeHitLatticeProxy.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:59  wsun
// Transfered from McCannFilter
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackFitter/TrackFitCathodeHitLink.h"
#include "McCannProd/McCannFitCathodeHitLatticeProxy.h"
#include "McCannProd/Template/McCannFitCathodeHitLatticeProxy.cc"

#include <vector>
#include <map>

template class McCannFitCathodeHitLatticeProxy< DBCandidate::kElectron > ;
template class McCannFitCathodeHitLatticeProxy< DBCandidate::kMuon > ;
template class McCannFitCathodeHitLatticeProxy< DBCandidate::kChargedPion > ;
template class McCannFitCathodeHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class McCannFitCathodeHitLatticeProxy< DBCandidate::kProton > ;
template class McCannFitCathodeHitLatticeProxy< DBCandidate::kExitElectron > ;
template class McCannFitCathodeHitLatticeProxy< DBCandidate::kExitMuon > ;
template class McCannFitCathodeHitLatticeProxy< DBCandidate::kExitPion > ;
template class McCannFitCathodeHitLatticeProxy< DBCandidate::kExitKaon > ;
template class McCannFitCathodeHitLatticeProxy< DBCandidate::kExitProton > ;
