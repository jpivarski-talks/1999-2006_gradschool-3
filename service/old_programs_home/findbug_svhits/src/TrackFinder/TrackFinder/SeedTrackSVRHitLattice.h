#if !defined(TRACKFINDER_SEEDTRACKSVRHITLATTICE_H)
#define TRACKFINDER_SEEDTRACKSVRHITLATTICE_H
// -*- C++ -*-
//
// Package:     TrackFinder
// Module:      SeedTrackSVRHitLattice
// 
// Description: define LatticeLinkData template specilization
// and typedef Lattice name. 
//
// Usage: Should be include by everyone wishing to use 
// Lattice< TRSeedTrack, CalibratedSVRphiHit > which is
// typedefed to SeedTrackSVRHitLattice.  LinkData specilization
// makes links which hold the relevant hit residual.
//
// Author:      Merlin Meyer-Mitchell
// Created:     Thu Sep 24 17:49:07 EDT 1998
// $Id: SeedTrackSVRHitLattice.h,v 1.4 1999/06/21 22:59:21 pg Exp $
//
// Revision history
//
// $Log: SeedTrackSVRHitLattice.h,v $
// Revision 1.4  1999/06/21 22:59:21  pg
//  Modifications for new Si hits
//
// Revision 1.3  1999/02/22 21:51:34  meyerm
// changed SeedTrackSV*HitLattice to use DefaultLinkData
//
// Revision 1.2  1998/12/17 00:33:40  meyerm
// moving SeedTrackSV*Lattice stuff to TrackFinder
//
// Revision 1.1  1998/12/16 23:39:23  meyerm
// moving SeedTrack-SV*Hit lattices to TrackFinder
//
// Revision 1.4  1998/11/09 20:55:16  meyerm
// fixed to work with new Lattice and renamed some files
//
// Revision 1.3  1998/10/29 01:15:53  meyerm
// removed internal geometry code
//
// Revision 1.2  1998/10/13 17:43:58  meyerm
// moved LinkData definition to generic SVLinkData.h
//
// Revision 1.1  1998/10/08 21:18:29  meyerm
// added SeedTrackSVZHitLattice and renamed .SVRh. to .SVRH.
//
// Revision 1.3  1998/10/07 23:43:50  meyerm
// Eliminated assignment of hits from same ladder to strings
//
// Revision 1.2  1998/10/02 18:22:33  meyerm
// updated all to work with SeedTrackSVRhitLattice.h
//
// Revision 1.1  1998/10/02 18:06:43  meyerm
// renamed and typedefed Lattice
//
// Revision 1.3  1998/10/01 23:31:39  meyerm
// gave all Id and Log
//
// Revision 1.1  1998/10/01 22:58:19  meyerm
// renamed file
//
// Revision 1.3  1998/10/01 18:58:35  meyerm
// updated to work with changes in LatticeLink.h
//
// Revision 1.1.1.1  1998/09/30 19:01:26  meyerm
// imported GetSvxHitsProc sources
//

//--------------------//
// user include files //
//--------------------//
#include "Lattice/Lattice.h"
#include "TrackRoot/TRSeedTrack.h"
#include "SiHits/CalibratedSVRphiHit.h"

typedef Lattice< TRSeedTrack, CalibratedSVRphiHit, DefaultLinkData >
                   SeedTrackSVRHitLattice;

#endif /* TRACKFINDER_SEEDTRACKSVRHITLATTICE_H */



