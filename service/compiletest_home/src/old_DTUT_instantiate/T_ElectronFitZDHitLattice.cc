// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_ElectronFitZDHitLattice
// 
// Description: Instantiate Classes for ElectronFitZDHitLattice
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
#include "TrackFitter/TrackFitZDHitLink.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( ElectronFitZDHitLattice )
//Use the following if 'ElectronFitZDHitLattice' contains a ','
// also make sure there are no blank spaces within 'ElectronFitZDHitLattice'
/* INSTANTIATE_ONE_1_COMMA( ElectronFitZDHitLattice ) */
#include "FrameAccess/instantiate_one.h"

