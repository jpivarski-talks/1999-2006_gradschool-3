// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_ProtonFitDRHitLattice
// 
// Description: Instantiate Classes for ProtonFitDRHitLattice
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
INSTANTIATE_ONE( ProtonFitDRHitLattice )
//Use the following if 'ProtonFitDRHitLattice' contains a ','
// also make sure there are no blank spaces within 'ProtonFitDRHitLattice'
/* INSTANTIATE_ONE_1_COMMA( ProtonFitDRHitLattice ) */
#include "FrameAccess/instantiate_one.h"

