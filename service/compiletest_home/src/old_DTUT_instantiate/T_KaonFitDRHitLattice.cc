// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_KaonFitDRHitLattice
// 
// Description: Instantiate Classes for KaonFitDRHitLattice
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
INSTANTIATE_ONE( KaonFitDRHitLattice )
//Use the following if 'KaonFitDRHitLattice' contains a ','
// also make sure there are no blank spaces within 'KaonFitDRHitLattice'
/* INSTANTIATE_ONE_1_COMMA( KaonFitDRHitLattice ) */
#include "FrameAccess/instantiate_one.h"

