// -*- C++ -*-
//
// Package:     PivarskiFilter
// Module:      T_TrackMany
// 
// Description: Instantiate Classes for TRTrackCandidateFitQuality< kMuon >
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 01:45:38 EDT 2000
// $Id: T_PivarskiMuonQualityField.cc,v 1.1 2000/08/16 20:09:34 wsun Exp $
//
// Revision history
//
// $Log: T_PivarskiMuonQualityField.cc,v $
// Revision 1.1  2000/08/16 20:09:34  wsun
// First submission.
//
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "PivarskiFilter/Storage/instantiate_quality_field.h" 
INSTANTIATE_QUALITY_FIELD( DBCandidate::kMuon, Muon )

#include "FrameAccess/instantiate_many.h" 

#include "StorageManagement/instantiate_faptrtablehelper.h" 
