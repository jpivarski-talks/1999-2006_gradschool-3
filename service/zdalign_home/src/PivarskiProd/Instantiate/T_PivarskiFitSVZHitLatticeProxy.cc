// -*- C++ -*-
//
// Package:     PivarskiProd
// Module:      T_PivarskiFitSVZHitLatticeProxy
// 
// Description: Instantiate Classes for TRHelixKaonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_PivarskiFitSVZHitLatticeProxy.cc,v 1.1.1.1 1999/09/10 21:58:59 wsun Exp $
//
// Revision history
//
// $Log: T_PivarskiFitSVZHitLatticeProxy.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:59  wsun
// Transfered from PivarskiFilter
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
#include "PivarskiProd/PivarskiFitSVZHitLatticeProxy.h"
#include "PivarskiProd/Template/PivarskiFitSVZHitLatticeProxy.cc"

#include <vector>
#include <map>


template class PivarskiFitSVZHitLatticeProxy< DBCandidate::kElectron > ;
template class PivarskiFitSVZHitLatticeProxy< DBCandidate::kMuon > ;
template class PivarskiFitSVZHitLatticeProxy< DBCandidate::kChargedPion > ;
template class PivarskiFitSVZHitLatticeProxy< DBCandidate::kChargedKaon > ;
template class PivarskiFitSVZHitLatticeProxy< DBCandidate::kProton > ;
template class PivarskiFitSVZHitLatticeProxy< DBCandidate::kExitElectron > ;
template class PivarskiFitSVZHitLatticeProxy< DBCandidate::kExitMuon > ;
template class PivarskiFitSVZHitLatticeProxy< DBCandidate::kExitPion > ;
template class PivarskiFitSVZHitLatticeProxy< DBCandidate::kExitKaon > ;
template class PivarskiFitSVZHitLatticeProxy< DBCandidate::kExitProton > ;
