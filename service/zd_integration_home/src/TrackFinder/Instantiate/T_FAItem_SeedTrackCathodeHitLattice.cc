// -*- C++ -*-
//
// Package:     TrackFinder
// Module:      T_FAItem_SeedTrackCathodeHitLattice
// 
// Description: Instantiate FAItem< SeedTrackCathodeHitLattice >
//
// Author:      Werner Sun
// Created:     Sun Sep  5 22:17:35 EDT 1999
// $Id: T_FAItem_SeedTrackCathodeHitLattice.cc,v 1.2 2000/02/08 18:30:34 lyon Exp $
//
// Revision history
// 
// $Log: T_FAItem_SeedTrackCathodeHitLattice.cc,v $
// Revision 1.2  2000/02/08 18:30:34  lyon
// Add Storage helper instantiations
//
// Revision 1.1  1999/09/10 20:21:33  wsun
// First submission.
//
//


#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackFinder/SeedTrackCathodeHitLattice.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>


#include "FrameAccess/instantiate_macros.h" 

INSTANTIATE_ONE( SeedTrackCathodeHitLattice )

#include "FrameAccess/instantiate_one.h"

// Instantiate the storage helper
#include "StorageManagement/SMLatticeStorageHelper.h"
typedef SMLatticeStorageHelper<SeedTrackCathodeHitLattice>
        SeedTrackCathodeHitLatticeStorageHelper;
SM_REGISTER_STORAGE( SeedTrackCathodeHitLatticeStorageHelper );
