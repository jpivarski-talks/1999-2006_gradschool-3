// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_PionFitDRHitLattice
// 
// Description: Instantiate Classes for PionFitDRHitLattice
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
INSTANTIATE_ONE( PionFitDRHitLattice )
//Use the following if 'PionFitDRHitLattice' contains a ','
// also make sure there are no blank spaces within 'PionFitDRHitLattice'
/* INSTANTIATE_ONE_1_COMMA( PionFitDRHitLattice ) */
#include "FrameAccess/instantiate_one.h"

