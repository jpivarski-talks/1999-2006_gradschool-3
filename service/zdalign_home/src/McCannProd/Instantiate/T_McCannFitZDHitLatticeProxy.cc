// -*- C++ -*-
//
// Package:     McCannProd
// Module:      T_McCannFitZDHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_McCannFitZDHitLatticeProxy.cc,v 1.1 2003/04/17 17:05:07 bkh Exp $
//
// Revision history
//
// $Log: T_McCannFitZDHitLatticeProxy.cc,v $
// Revision 1.1  2003/04/17 17:05:07  bkh
// Add ZD Lattice Proxy & throw exceptions if wrong config
//
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

#include "TrackFitter/TrackFitZDHitLink.h"
#include "McCannProd/McCannFitZDHitLatticeProxy.h"
#include "McCannProd/Template/McCannFitZDHitLatticeProxy.cc"

#include <vector>
#include <map>

template class McCannFitZDHitLatticeProxy< DBCandidate::kElectron > ;
template class McCannFitZDHitLatticeProxy< DBCandidate::kMuon > ;
template class McCannFitZDHitLatticeProxy< DBCandidate::kChargedPion > ;
template class McCannFitZDHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class McCannFitZDHitLatticeProxy< DBCandidate::kProton > ;
template class McCannFitZDHitLatticeProxy< DBCandidate::kExitElectron > ;
template class McCannFitZDHitLatticeProxy< DBCandidate::kExitMuon > ;
template class McCannFitZDHitLatticeProxy< DBCandidate::kExitPion > ;
template class McCannFitZDHitLatticeProxy< DBCandidate::kExitKaon > ;
template class McCannFitZDHitLatticeProxy< DBCandidate::kExitProton > ;
