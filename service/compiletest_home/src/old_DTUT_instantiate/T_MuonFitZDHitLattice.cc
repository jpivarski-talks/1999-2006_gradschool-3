// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_MuonFitZDHitLattice
// 
// Description: Instantiate Classes for MuonFitZDHitLattice
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
INSTANTIATE_ONE( MuonFitZDHitLattice )
//Use the following if 'MuonFitZDHitLattice' contains a ','
// also make sure there are no blank spaces within 'MuonFitZDHitLattice'
/* INSTANTIATE_ONE_1_COMMA( MuonFitZDHitLattice ) */
#include "FrameAccess/instantiate_one.h"

