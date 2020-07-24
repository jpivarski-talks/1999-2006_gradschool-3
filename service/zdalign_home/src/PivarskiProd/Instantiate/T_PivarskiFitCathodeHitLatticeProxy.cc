// -*- C++ -*-
//
// Package:     PivarskiProd
// Module:      T_PivarskiFitCathodeHitLatticeProxy
// 
// Description:
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Sep  7 01:08:39 EDT 1999
// $Id: T_PivarskiFitCathodeHitLatticeProxy.cc,v 1.1.1.1 1999/09/10 21:58:59 wsun Exp $
//
// Revision history
//
// $Log: T_PivarskiFitCathodeHitLatticeProxy.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:59  wsun
// Transfered from PivarskiFilter
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackFitter/TrackFitCathodeHitLink.h"
#include "PivarskiProd/PivarskiFitCathodeHitLatticeProxy.h"
#include "PivarskiProd/Template/PivarskiFitCathodeHitLatticeProxy.cc"

#include <vector>
#include <map>

template class PivarskiFitCathodeHitLatticeProxy< DBCandidate::kElectron > ;
template class PivarskiFitCathodeHitLatticeProxy< DBCandidate::kMuon > ;
template class PivarskiFitCathodeHitLatticeProxy< DBCandidate::kChargedPion > ;
template class PivarskiFitCathodeHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class PivarskiFitCathodeHitLatticeProxy< DBCandidate::kProton > ;
template class PivarskiFitCathodeHitLatticeProxy< DBCandidate::kExitElectron > ;
template class PivarskiFitCathodeHitLatticeProxy< DBCandidate::kExitMuon > ;
template class PivarskiFitCathodeHitLatticeProxy< DBCandidate::kExitPion > ;
template class PivarskiFitCathodeHitLatticeProxy< DBCandidate::kExitKaon > ;
template class PivarskiFitCathodeHitLatticeProxy< DBCandidate::kExitProton > ;
