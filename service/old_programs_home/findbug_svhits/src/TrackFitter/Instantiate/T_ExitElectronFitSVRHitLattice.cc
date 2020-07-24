// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      T_ExitElectronFitSVRHitLattice
// 
// Description: Template instantiation of FAItem< ExitElectronFitSVRHitLattice >
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Tue Dec 15 14:10:23 EST 1998
// $Id: T_ExitElectronFitSVRHitLattice.cc,v 1.3 2000/02/24 17:38:20 wsun Exp $
//
// Revision history
//
// $Log: T_ExitElectronFitSVRHitLattice.cc,v $
// Revision 1.3  2000/02/24 17:38:20  wsun
// Updated for storage helpers.
//
// Revision 1.2  1999/06/21 22:53:15  pg
//  Modifications for new Si hits.
//
// Revision 1.1  1999/05/08 01:00:58  marsh
// Added remaining outgoing hypotheses.
//
// Revision 1.1  1998/12/15 20:32:57  marsh
// Added definitions of fit track to hit Lattices and link data.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <set>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackFitter/TrackFitSVRHitLink.h"

#include <vector>
#include <set>
#include <map>

#include "TrackFitter/Storage/instantiatestore_trackhitlattice.h"
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( ExitElectronFitSVRHitLattice )
#include "FrameAccess/instantiate_one.h"

#include "StorageManagement/SMLatticeStorageHelper.h"
#include "TrackFitter/Storage/LatticeLinkTrackFitSVRHitLatticeStorageHelper.h"

INSTANTIATESTORE_TRACKHITLATTICE(ExitElectron,SVR)

#include "StorageManagement/instantiate_latticehelper.h"
