// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      T_TrackMany
// 
// Description: Instantiate Classes for TRTrackFitQuality
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Fri Dec 18  1998
// $Id: T_TRTrackFitQualityField.cc,v 1.1 1998/12/22 20:43:48 marsh Exp $
//
// Revision history
//
// $Log: T_TRTrackFitQualityField.cc,v $
// Revision 1.1  1998/12/22 20:43:48  marsh
// Instantiation file for track quality object.
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackRoot/TRTrackFitQuality.h"

#include "FrameAccess/instantiate_macros.h" 
INSTANTIATE_MANY( TRTrackFitQuality ) 

#include "FrameAccess/instantiate_many.h"
