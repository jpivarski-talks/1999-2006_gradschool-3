// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      T_MuonFitDRHitLattice
// 
// Description: Template instantiation of FAItem< MuonFitDRHitLattice >
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Tue Dec 15 14:10:23 EST 1998
// $Id: T_MuonFitDRHitLattice.cc,v 1.2 2000/02/24 17:38:30 wsun Exp $
//
// Revision history
//
// $Log: T_MuonFitDRHitLattice.cc,v $
// Revision 1.2  2000/02/24 17:38:30  wsun
// Updated for storage helpers.
//
// Revision 1.1  1998/12/15 20:33:31  marsh
// Added definitions of fit track to hit Lattices and link data.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackFitter/TrackFitDRHitLink.h"

#include <vector>
#include <set>

#include "TrackFitter/Storage/instantiatestore_trackhitlattice.h"
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( MuonFitDRHitLattice )
#include "FrameAccess/instantiate_one.h"

#include "StorageManagement/SMLatticeStorageHelper.h"
#include "TrackFitter/Storage/LatticeLinkTrackFitDRHitLatticeStorageHelper.h"

INSTANTIATESTORE_TRACKHITLATTICE(Muon,DR)

#include "StorageManagement/instantiate_latticehelper.h"
