// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      T_TrackMany
// 
// Description: Instantiate Classes for TRTrackCandidateFitQuality< kProton >
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Fri Dec 18  1998
// $Id: T_TRTrackProtonFitQualityField.cc,v 1.3 2000/02/25 01:10:06 wsun Exp $
//
// Revision history
//
// $Log: T_TRTrackProtonFitQualityField.cc,v $
// Revision 1.3  2000/02/25 01:10:06  wsun
// Updated for storage helpers.
//
// Revision 1.2  1999/07/15 22:59:35  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1998/12/18 19:37:18  marsh
// Added templated fit track quality object to provide type-differentiation
// for Frame storage.
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackRoot/Storage/instantiate_quality_field.h" 
INSTANTIATE_QUALITY_FIELD( DBCandidate::kProton, Proton )

#include "FrameAccess/instantiate_many.h" 

#include "StorageManagement/instantiate_faptrtablehelper.h" 
