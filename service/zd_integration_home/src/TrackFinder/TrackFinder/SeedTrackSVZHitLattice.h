#if !defined(TRACKFINDER_SEEDTRACKSVZHITLATTICE_H)
#define TRACKFINDER_SEEDTRACKSVZHITLATTICE_H
// -*- C++ -*-
//
// Package:     TrackFinder
// Module:      SeedTrackSVZHitLattice
// 
// Description: define LatticeLinkData template specilization
// and typedef Lattice name. 
//
// Usage: Should be include by everyone wishing to use 
// Lattice< TRSeedTrack, CalibratedSVZHit > which is
// typedefed to SeedTrackSVZHitLattice.  LinkData specilization
// makes links which hold the relevant hit residual.
//
// Author:      Merlin Meyer-Mitchell
// Created:     Thu Oct 8 1998
// $Id: SeedTrackSVZHitLattice.h,v 1.4 1999/06/21 22:59:22 pg Exp $
//
// Revision history
//
// $Log: SeedTrackSVZHitLattice.h,v $
// Revision 1.4  1999/06/21 22:59:22  pg
//  Modifications for new Si hits
//
// Revision 1.3  1999/02/22 21:51:34  meyerm
// changed SeedTrackSV*HitLattice to use DefaultLinkData
//
// Revision 1.2  1998/12/17 00:33:41  meyerm
// moving SeedTrackSV*Lattice stuff to TrackFinder
//
// Revision 1.1  1998/12/16 23:39:24  meyerm
// moving SeedTrack-SV*Hit lattices to TrackFinder
//
// Revision 1.4  1998/11/09 20:55:16  meyerm
// fixed to work with new Lattice and renamed some files
//
// Revision 1.3  1998/10/29 01:15:54  meyerm
// removed internal geometry code
//
// Revision 1.2  1998/10/13 17:43:58  meyerm
// moved LinkData definition to generic SVLinkData.h
//
// Revision 1.1  1998/10/12 17:07:34  meyerm
// adding SeedTrackSVZHitLattice.h
//
//

//--------------------//
// user include files //
//--------------------//
#include "Lattice/Lattice.h"
#include "TrackRoot/TRSeedTrack.h"
#include "SiHits/CalibratedSVZHit.h"

typedef Lattice< TRSeedTrack, CalibratedSVZHit, DefaultLinkData >
                   SeedTrackSVZHitLattice;

#endif /* TRACKFINDER_SEEDTRACKSVZHITLATTICE_H */
