// -*- C++ -*-
//
// Package:     PivarskiProd
// Module:      T_PivarskiFitDRHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_PivarskiFitDRHitLatticeProxy.cc,v 1.1.1.1 1999/09/10 21:58:59 wsun Exp $
//
// Revision history
//
// $Log: T_PivarskiFitDRHitLatticeProxy.cc,v $
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

#include "TrackFitter/TrackFitDRHitLink.h"
#include "PivarskiProd/PivarskiFitDRHitLatticeProxy.h"
#include "PivarskiProd/Template/PivarskiFitDRHitLatticeProxy.cc"

#include <vector>
#include <map>

template class PivarskiFitDRHitLatticeProxy< DBCandidate::kElectron > ;
template class PivarskiFitDRHitLatticeProxy< DBCandidate::kMuon > ;
template class PivarskiFitDRHitLatticeProxy< DBCandidate::kChargedPion > ;
template class PivarskiFitDRHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class PivarskiFitDRHitLatticeProxy< DBCandidate::kProton > ;
template class PivarskiFitDRHitLatticeProxy< DBCandidate::kExitElectron > ;
template class PivarskiFitDRHitLatticeProxy< DBCandidate::kExitMuon > ;
template class PivarskiFitDRHitLatticeProxy< DBCandidate::kExitPion > ;
template class PivarskiFitDRHitLatticeProxy< DBCandidate::kExitKaon > ;
template class PivarskiFitDRHitLatticeProxy< DBCandidate::kExitProton > ;
