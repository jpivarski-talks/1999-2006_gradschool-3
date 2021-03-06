// -*- C++ -*-
//
// Package:     ChisqFitter
// Module:      ChisqCandidateQuality
// 
// Description: Instantiate ChisqCandidateQuality< kElectron >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 15:18:23 EDT 2000
// $Id: T_ChisqElectronQuality.cc,v 1.1 2000/09/05 20:36:18 wsun Exp $
//
// Revision history
//
// $Log: T_ChisqElectronQuality.cc,v $
// Revision 1.1  2000/09/05 20:36:18  wsun
// First submission.
//
// Revision 1.1  2000/08/16 20:09:24  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "ChisqFitter/ChisqCandidateQuality.h"
// #include "ChisqFitter/Template/ChisqCandidateQuality.cc"

//
// static data member definitions
//

#if ( defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX) )
// for AXP
#pragma define_template ChisqCandidateQuality< DBCandidate::kElectron >
#else
template class ChisqCandidateQuality< DBCandidate::kElectron > ;
#endif /* (defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX)) */
