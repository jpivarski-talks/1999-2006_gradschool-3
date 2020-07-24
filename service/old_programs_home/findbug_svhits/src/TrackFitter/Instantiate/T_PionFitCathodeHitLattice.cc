// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      T_PionFitCathodeHitLattice
// 
// Description: Template instantiation of FAItem< PionFitCathodeHitLattice >
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Sep  6 14:45:44 EDT 1999
// $Id: T_PionFitCathodeHitLattice.cc,v 1.2 2000/02/24 17:38:31 wsun Exp $
//
// Revision history
//
// $Log: T_PionFitCathodeHitLattice.cc,v $
// Revision 1.2  2000/02/24 17:38:31  wsun
// Updated for storage helpers.
//
// Revision 1.1  1999/09/10 20:24:22  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <set>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackFitter/TrackFitCathodeHitLink.h"

#include <vector>
#include <set>
#include <map>

#include "TrackFitter/Storage/instantiatestore_trackhitlattice.h"
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( PionFitCathodeHitLattice )
#include "FrameAccess/instantiate_one.h"

#include "StorageManagement/SMLatticeStorageHelper.h"
#include "TrackFitter/Storage/LatticeLinkTrackFitCathodeHitLatticeStorageHelper.h"

INSTANTIATESTORE_TRACKHITLATTICE(Pion,Cathode)

#include "StorageManagement/instantiate_latticehelper.h"
