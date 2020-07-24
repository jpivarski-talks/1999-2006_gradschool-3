// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      T_ElectronFitZDHitLattice
// 
// Description: Template instantiation of FAItem< ElectronFitZDHitLattice >
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Tue Dec 15 14:10:23 EST 1998
// $Id: T_ElectronFitZDHitLattice.cc,v 1.1 2003/04/17 17:03:12 bkh Exp $
//
// Revision history
//
// $Log: T_ElectronFitZDHitLattice.cc,v $
// Revision 1.1  2003/04/17 17:03:12  bkh
// Add ZD Lattice Object & Instantiations
//
// Revision 1.2  2000/02/24 17:38:18  wsun
// Updated for storage helpers.
//
// Revision 1.1  1998/12/15 20:32:54  marsh
// Added definitions of fit track to hit Lattices and link data.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackFitter/TrackFitZDHitLink.h"

#include <vector>
#include <set>

#include "TrackFitter/Storage/instantiatestore_trackhitlattice.h"
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( ElectronFitZDHitLattice )
#include "FrameAccess/instantiate_one.h"

#include "StorageManagement/SMLatticeStorageHelper.h"
#include "TrackFitter/Storage/LatticeLinkTrackFitZDHitLatticeStorageHelper.h"

INSTANTIATESTORE_TRACKHITLATTICE(Electron,ZD)

#include "StorageManagement/instantiate_latticehelper.h"
