// -*- C++ -*-
//
// Package:     PivarskiFilter
// Module:      PivarskiCandidateQuality
// 
// Description: Instantiate PivarskiCandidateQuality< kChargedPion >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 15:18:23 EDT 2000
// $Id: T_PivarskiPionQuality.cc,v 1.1 2000/08/16 20:09:34 wsun Exp $
//
// Revision history
//
// $Log: T_PivarskiPionQuality.cc,v $
// Revision 1.1  2000/08/16 20:09:34  wsun
// First submission.
//
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "PivarskiFilter/PivarskiCandidateQuality.h"
#include "PivarskiFilter/Template/PivarskiCandidateQuality.cc"

//
// static data member definitions
//

#if ( defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX) )
// for AXP
#pragma define_template PivarskiCandidateQuality< DBCandidate::kChargedPion >
#else
template class PivarskiCandidateQuality< DBCandidate::kChargedPion > ;
#endif /* (defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX)) */
