// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_ExitKaonFitDRHitLattice
// 
// Description: Instantiate Classes for ExitKaonFitDRHitLattice
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// user include files
#include "TrackFitter/TrackFitDRHitLink.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( ExitKaonFitDRHitLattice )
//Use the following if 'ExitKaonFitDRHitLattice' contains a ','
// also make sure there are no blank spaces within 'ExitKaonFitDRHitLattice'
/* INSTANTIATE_ONE_1_COMMA( ExitKaonFitDRHitLattice ) */
#include "FrameAccess/instantiate_one.h"

