// -*- C++ -*-
//
// Package:     PivarskiProd
// Module:      T_PivarskiFitSVRHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_PivarskiFitSVRHitLatticeProxy.cc,v 1.1.1.1 1999/09/10 21:58:59 wsun Exp $
//
// Revision history
//
// $Log: T_PivarskiFitSVRHitLatticeProxy.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:59  wsun
// Transfered from PivarskiFilter
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
#include "PivarskiProd/PivarskiFitSVRHitLatticeProxy.h"
#include "PivarskiProd/Template/PivarskiFitSVRHitLatticeProxy.cc"

#include <vector>
#include <map>

template class PivarskiFitSVRHitLatticeProxy< DBCandidate::kElectron > ;
template class PivarskiFitSVRHitLatticeProxy< DBCandidate::kMuon > ;
template class PivarskiFitSVRHitLatticeProxy< DBCandidate::kChargedPion > ;
template class PivarskiFitSVRHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class PivarskiFitSVRHitLatticeProxy< DBCandidate::kProton > ;
template class PivarskiFitSVRHitLatticeProxy< DBCandidate::kExitElectron > ;
template class PivarskiFitSVRHitLatticeProxy< DBCandidate::kExitMuon > ;
template class PivarskiFitSVRHitLatticeProxy< DBCandidate::kExitPion > ;
template class PivarskiFitSVRHitLatticeProxy< DBCandidate::kExitKaon > ;
template class PivarskiFitSVRHitLatticeProxy< DBCandidate::kExitProton > ;

