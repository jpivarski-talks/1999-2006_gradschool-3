// -*- C++ -*-
//
// Package:     PivarskiProd
// Module:      T_PivarskiFitZDHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_PivarskiFitZDHitLatticeProxy.cc,v 1.1 2003/04/17 17:05:07 bkh Exp $
//
// Revision history
//
// $Log: T_PivarskiFitZDHitLatticeProxy.cc,v $
// Revision 1.1  2003/04/17 17:05:07  bkh
// Add ZD Lattice Proxy & throw exceptions if wrong config
//
// Revision 1.1.1.1  1999/09/10 21:58:59  wsun
// Transfered from PivarskiFilter
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
#include "PivarskiProd/PivarskiFitZDHitLatticeProxy.h"
#include "PivarskiProd/Template/PivarskiFitZDHitLatticeProxy.cc"

#include <vector>
#include <map>

template class PivarskiFitZDHitLatticeProxy< DBCandidate::kElectron > ;
template class PivarskiFitZDHitLatticeProxy< DBCandidate::kMuon > ;
template class PivarskiFitZDHitLatticeProxy< DBCandidate::kChargedPion > ;
template class PivarskiFitZDHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class PivarskiFitZDHitLatticeProxy< DBCandidate::kProton > ;
template class PivarskiFitZDHitLatticeProxy< DBCandidate::kExitElectron > ;
template class PivarskiFitZDHitLatticeProxy< DBCandidate::kExitMuon > ;
template class PivarskiFitZDHitLatticeProxy< DBCandidate::kExitPion > ;
template class PivarskiFitZDHitLatticeProxy< DBCandidate::kExitKaon > ;
template class PivarskiFitZDHitLatticeProxy< DBCandidate::kExitProton > ;
