// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_ElectronFitDRHitLattice
// 
// Description: Instantiate Classes for ElectronFitDRHitLattice
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
INSTANTIATE_ONE( ElectronFitDRHitLattice )
//Use the following if 'ElectronFitDRHitLattice' contains a ','
// also make sure there are no blank spaces within 'ElectronFitDRHitLattice'
/* INSTANTIATE_ONE_1_COMMA( ElectronFitDRHitLattice ) */
#include "FrameAccess/instantiate_one.h"

