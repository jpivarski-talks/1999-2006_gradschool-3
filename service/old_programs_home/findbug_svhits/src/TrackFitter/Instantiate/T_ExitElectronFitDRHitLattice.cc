// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      T_ExitElectronFitDRHitLattice
// 
// Description: Template instantiation of FAItem< ExitElectronFitDRHitLattice >
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Tue Dec 15 14:10:23 EST 1998
// $Id: T_ExitElectronFitDRHitLattice.cc,v 1.2 2000/02/24 17:38:19 wsun Exp $
//
// Revision history
//
// $Log: T_ExitElectronFitDRHitLattice.cc,v $
// Revision 1.2  2000/02/24 17:38:19  wsun
// Updated for storage helpers.
//
// Revision 1.1  1999/05/08 01:00:56  marsh
// Added remaining outgoing hypotheses.
//
// Revision 1.1  1998/12/15 20:32:56  marsh
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
INSTANTIATE_ONE( ExitElectronFitDRHitLattice )
#include "FrameAccess/instantiate_one.h"

#include "StorageManagement/SMLatticeStorageHelper.h"
#include "TrackFitter/Storage/LatticeLinkTrackFitDRHitLatticeStorageHelper.h"

INSTANTIATESTORE_TRACKHITLATTICE(ExitElectron,DR)

#include "StorageManagement/instantiate_latticehelper.h"
