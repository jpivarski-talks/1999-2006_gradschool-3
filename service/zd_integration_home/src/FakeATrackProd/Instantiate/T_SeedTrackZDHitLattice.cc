// -*- C++ -*-
//
// Package:     FakeATrackProd
// Module:      T_SeedTrackZDHitLattice
// 
// Description: Instantiate Classes for SeedTrackZDHitLattice
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri Jul 11 11:46:22 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// user include files
#include "TrackFinder/SeedZDHitLink.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( SeedTrackZDHitLattice )
//Use the following if 'SeedTrackZDHitLattice' contains a ','
// also make sure there are no blank spaces within 'SeedTrackZDHitLattice'
/* INSTANTIATE_ONE_1_COMMA( SeedTrackZDHitLattice ) */
#include "FrameAccess/instantiate_one.h"

