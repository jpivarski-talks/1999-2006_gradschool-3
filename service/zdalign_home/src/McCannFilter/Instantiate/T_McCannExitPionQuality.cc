// -*- C++ -*-
//
// Package:     McCannFilter
// Module:      McCannCandidateQuality
// 
// Description: Instantiate McCannCandidateQuality< kExitPion >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 15:18:23 EDT 2000
// $Id: T_McCannExitPionQuality.cc,v 1.1 2000/08/16 20:09:29 wsun Exp $
//
// Revision history
//
// $Log: T_McCannExitPionQuality.cc,v $
// Revision 1.1  2000/08/16 20:09:29  wsun
// First submission.
//
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "McCannFilter/McCannCandidateQuality.h"
#include "McCannFilter/Template/McCannCandidateQuality.cc"

//
// static data member definitions
//

#if ( defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX) )
// for AXP
#pragma define_template McCannCandidateQuality< DBCandidate::kExitPion >
#else
template class McCannCandidateQuality< DBCandidate::kExitPion > ;
#endif /* (defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX)) */
