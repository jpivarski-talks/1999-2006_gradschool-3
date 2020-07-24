// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_ExitKaonFitZDHitLattice
// 
// Description: Instantiate Classes for ExitKaonFitZDHitLattice
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
INSTANTIATE_ONE( ExitKaonFitZDHitLattice )
//Use the following if 'ExitKaonFitZDHitLattice' contains a ','
// also make sure there are no blank spaces within 'ExitKaonFitZDHitLattice'
/* INSTANTIATE_ONE_1_COMMA( ExitKaonFitZDHitLattice ) */
#include "FrameAccess/instantiate_one.h"
