// -*- C++ -*-
//
// Package:     McCannProd
// Module:      T_McCannFitSVZHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_McCannFitSVZHitLatticeProxy.cc,v 1.1.1.1 1999/09/10 21:58:59 wsun Exp $
//
// Revision history
//
// $Log: T_McCannFitSVZHitLatticeProxy.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:59  wsun
// Transfered from McCannFilter
//
// Revision 1.2  1999/07/15 23:01:18  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:14:03  wsun
// First submission of producer.
//
//
//
#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackFitter/TrackFitSVZHitLink.h"
#include "McCannProd/McCannFitSVZHitLatticeProxy.h"
#include "McCannProd/Template/McCannFitSVZHitLatticeProxy.cc"

#include <vector>
#include <map>


template class McCannFitSVZHitLatticeProxy< DBCandidate::kElectron > ;
template class McCannFitSVZHitLatticeProxy< DBCandidate::kMuon > ;
template class McCannFitSVZHitLatticeProxy< DBCandidate::kChargedPion > ;
template class McCannFitSVZHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class McCannFitSVZHitLatticeProxy< DBCandidate::kProton > ;
template class McCannFitSVZHitLatticeProxy< DBCandidate::kExitElectron > ;
template class McCannFitSVZHitLatticeProxy< DBCandidate::kExitMuon > ;
template class McCannFitSVZHitLatticeProxy< DBCandidate::kExitPion > ;
template class McCannFitSVZHitLatticeProxy< DBCandidate::kExitKaon > ;
template class McCannFitSVZHitLatticeProxy< DBCandidate::kExitProton > ;
