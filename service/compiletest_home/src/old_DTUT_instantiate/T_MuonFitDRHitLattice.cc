// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_MuonFitDRHitLattice
// 
// Description: Instantiate Classes for MuonFitDRHitLattice
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
INSTANTIATE_ONE( MuonFitDRHitLattice )
//Use the following if 'MuonFitDRHitLattice' contains a ','
// also make sure there are no blank spaces within 'MuonFitDRHitLattice'
/* INSTANTIATE_ONE_1_COMMA( MuonFitDRHitLattice ) */
#include "FrameAccess/instantiate_one.h"

