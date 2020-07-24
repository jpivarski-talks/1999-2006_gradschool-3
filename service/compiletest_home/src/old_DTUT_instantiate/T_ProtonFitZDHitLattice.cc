// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_ProtonFitZDHitLattice
// 
// Description: Instantiate Classes for ProtonFitZDHitLattice
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
INSTANTIATE_ONE( ProtonFitZDHitLattice )
//Use the following if 'ProtonFitZDHitLattice' contains a ','
// also make sure there are no blank spaces within 'ProtonFitZDHitLattice'
/* INSTANTIATE_ONE_1_COMMA( ProtonFitZDHitLattice ) */
#include "FrameAccess/instantiate_one.h"

