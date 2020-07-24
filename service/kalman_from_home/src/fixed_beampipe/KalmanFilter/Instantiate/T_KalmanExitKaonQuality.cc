// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      KalmanCandidateQuality
// 
// Description: Instantiate KalmanCandidateQuality< kExitKaon >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 15:18:23 EDT 2000
// $Id: T_KalmanExitKaonQuality.cc,v 1.1 2000/08/16 20:09:27 wsun Exp $
//
// Revision history
//
// $Log: T_KalmanExitKaonQuality.cc,v $
// Revision 1.1  2000/08/16 20:09:27  wsun
// First submission.
//
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "KalmanFilter/KalmanCandidateQuality.h"
#include "KalmanFilter/Template/KalmanCandidateQuality.cc"

//
// static data member definitions
//

#if ( defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX) )
// for AXP
#pragma define_template KalmanCandidateQuality< DBCandidate::kExitKaon >
#else
template class KalmanCandidateQuality< DBCandidate::kExitKaon > ;
#endif /* (defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX)) */
