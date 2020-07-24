// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_ExitPionFitDRHitLattice
// 
// Description: Instantiate Classes for ExitPionFitDRHitLattice
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
INSTANTIATE_ONE( ExitPionFitDRHitLattice )
//Use the following if 'ExitPionFitDRHitLattice' contains a ','
// also make sure there are no blank spaces within 'ExitPionFitDRHitLattice'
/* INSTANTIATE_ONE_1_COMMA( ExitPionFitDRHitLattice ) */
#include "FrameAccess/instantiate_one.h"

