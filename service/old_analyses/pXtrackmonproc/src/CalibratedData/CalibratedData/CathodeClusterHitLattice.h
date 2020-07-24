#if !defined(CALIBRATEDDATA_CATHODECLUSTERHITLATTICE_H)
#define CALIBRATEDDATA_CATHODECLUSTERHITLATTICE_H
// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      CathodeClusterHitLattice
// 
/**\class CathodeClusterHitLattice CathodeClusterHitLattice.h CalibratedData/CathodeClusterHitLattice.h

 Description: Lattice connecting a cathode cluster (CalibratedCathodeHit) to 
its constituent pads (DRCACorrectHit)  [yes, the naming is bad! <sigh>]

 Usage:
    <usage>

*/
//
// Author:      Adam Lyon
// Created:     Thu Feb 24 00:04:05 EST 2000
// $Id: CathodeClusterHitLattice.h,v 1.1 2000/03/03 15:14:03 lyon Exp $
//
// Revision history
//
// $Log: CathodeClusterHitLattice.h,v $
// Revision 1.1  2000/03/03 15:14:03  lyon
// Storage helpers
//

#include "Lattice/Lattice.h"
#include "CalibratedData/CalibratedCathodeHit.h"
#include "DRCACorrectHit/DRCACorrectHit.h"

typedef Lattice< CalibratedCathodeHit, DRCACorrectHit, DefaultLinkData>
                 CathodeClusterHitLattice;

#endif /* CALIBRATEDDATA_CATHODECLUSTERHITLATTICE_H */
