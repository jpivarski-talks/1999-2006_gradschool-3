// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      T_ExitProtonFitSVZHitLattice
// 
// Description: Template instantiation of FAItem< ExitProtonFitSVZHitLattice >
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Tue Dec 15 14:10:23 EST 1998
// $Id: T_ExitProtonFitSVZHitLattice.cc,v 1.3 2000/02/24 17:38:27 wsun Exp $
//
// Revision history
//
// $Log: T_ExitProtonFitSVZHitLattice.cc,v $
// Revision 1.3  2000/02/24 17:38:27  wsun
// Updated for storage helpers.
//
// Revision 1.2  1999/06/21 22:53:20  pg
//  Modifications for new Si hits.
//
// Revision 1.1  1999/05/08 01:01:10  marsh
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
#include "TrackFitter/TrackFitSVZHitLink.h"

#include <vector>
#include <set>
#include <map>

#include "TrackFitter/Storage/instantiatestore_trackhitlattice.h"
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( ExitProtonFitSVZHitLattice )
#include "FrameAccess/instantiate_one.h"

#include "StorageManagement/SMLatticeStorageHelper.h"
#include "TrackFitter/Storage/LatticeLinkTrackFitSVZHitLatticeStorageHelper.h"

INSTANTIATESTORE_TRACKHITLATTICE(ExitProton,SVZ)

#include "StorageManagement/instantiate_latticehelper.h"
