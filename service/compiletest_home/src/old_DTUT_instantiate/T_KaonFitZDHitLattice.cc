// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_KaonFitZDHitLattice
// 
// Description: Instantiate Classes for KaonFitZDHitLattice
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
INSTANTIATE_ONE( KaonFitZDHitLattice )
//Use the following if 'KaonFitZDHitLattice' contains a ','
// also make sure there are no blank spaces within 'KaonFitZDHitLattice'
/* INSTANTIATE_ONE_1_COMMA( KaonFitZDHitLattice ) */
#include "FrameAccess/instantiate_one.h"

