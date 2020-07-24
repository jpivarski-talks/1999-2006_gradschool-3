// -*- C++ -*-
//
// Package:     ChisqFitter
// Module:      T_TrackMany
// 
// Description: Instantiate Classes for
//              TRTrackCandidateFitQuality< kChargedKaon >
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 01:45:38 EDT 2000
// $Id: T_ChisqKaonQualityField.cc,v 1.1 2000/09/05 20:36:25 wsun Exp $
//
// Revision history
//
// $Log: T_ChisqKaonQualityField.cc,v $
// Revision 1.1  2000/09/05 20:36:25  wsun
// First submission.
//
// Revision 1.1  2000/08/16 20:09:32  wsun
// First submission.
//
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ChisqFitter/ChisqCandidateQuality.h"

typedef ChisqCandidateQuality< DBCandidate::kChargedKaon > ChisqKaonQuality;
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( ChisqKaonQuality )
#include "FrameAccess/instantiate_many.h"