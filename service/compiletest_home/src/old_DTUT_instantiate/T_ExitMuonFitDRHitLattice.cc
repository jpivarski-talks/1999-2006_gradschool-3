// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_ExitMuonFitDRHitLattice
// 
// Description: Instantiate Classes for ExitMuonFitDRHitLattice
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
INSTANTIATE_ONE( ExitMuonFitDRHitLattice )
//Use the following if 'ExitMuonFitDRHitLattice' contains a ','
// also make sure there are no blank spaces within 'ExitMuonFitDRHitLattice'
/* INSTANTIATE_ONE_1_COMMA( ExitMuonFitDRHitLattice ) */
#include "FrameAccess/instantiate_one.h"

