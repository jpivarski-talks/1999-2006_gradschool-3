// -*- C++ -*-
//
// Package:     FakeATrackProd
// Module:      T_SeedTrackDRHitLattice
// 
// Description: Instantiate Classes for SeedTrackDRHitLattice
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Jul  1 16:39:52 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// user include files
#include "TrackFinder/SeedDRHitLink.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( SeedTrackDRHitLattice )
//Use the following if 'SeedTrackDRHitLattice' contains a ','
// also make sure there are no blank spaces within 'SeedTrackDRHitLattice'
/* INSTANTIATE_ONE_1_COMMA( SeedTrackDRHitLattice ) */
#include "FrameAccess/instantiate_one.h"

