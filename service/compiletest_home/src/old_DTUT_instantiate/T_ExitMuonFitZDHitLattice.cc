// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_ExitMuonFitZDHitLattice
// 
// Description: Instantiate Classes for ExitMuonFitZDHitLattice
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
INSTANTIATE_ONE( ExitMuonFitZDHitLattice )
//Use the following if 'ExitMuonFitZDHitLattice' contains a ','
// also make sure there are no blank spaces within 'ExitMuonFitZDHitLattice'
/* INSTANTIATE_ONE_1_COMMA( ExitMuonFitZDHitLattice ) */
#include "FrameAccess/instantiate_one.h"

