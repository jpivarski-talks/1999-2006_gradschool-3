#if !defined(TRACKFINDER_SEEDTRACKCATHODEHITLATTICE_H)
#define TRACKFINDER_SEEDTRACKCATHODEHITLATTICE_H
// -*- C++ -*-
//
// Package:     <TrackFinder>
// Module:      SeedTrackCathodeHitLattice
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sun Sep  5 22:14:36 EDT 1999
// $Id: SeedTrackCathodeHitLattice.h,v 1.1 1999/09/10 20:22:00 wsun Exp $
//
// Revision history
//
// $Log: SeedTrackCathodeHitLattice.h,v $
// Revision 1.1  1999/09/10 20:22:00  wsun
// First submission.
//

// system include files

// user include files

// forward declarations

#include "Lattice/Lattice.h"
#include "TrackRoot/TRSeedTrack.h"
#include "CalibratedData/CalibratedCathodeHit.h"

typedef Lattice< TRSeedTrack, CalibratedCathodeHit, DefaultLinkData >
SeedTrackCathodeHitLattice;

#endif /* TRACKFINDER_SEEDTRACKCATHODEHITLATTICE_H */
