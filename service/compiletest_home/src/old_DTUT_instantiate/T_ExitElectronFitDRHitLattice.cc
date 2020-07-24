// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_ExitElectronFitDRHitLattice
// 
// Description: Instantiate Classes for ExitElectronFitDRHitLattice
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
INSTANTIATE_ONE( ExitElectronFitDRHitLattice )
//Use the following if 'ExitElectronFitDRHitLattice' contains a ','
// also make sure there are no blank spaces within 'ExitElectronFitDRHitLattice'
/* INSTANTIATE_ONE_1_COMMA( ExitElectronFitDRHitLattice ) */
#include "FrameAccess/instantiate_one.h"

