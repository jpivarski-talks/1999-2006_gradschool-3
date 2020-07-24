// -*- C++ -*-
//
// Package:     <package>
// Module:      T_Lattice_TRSeedTrack_CalibratedZDHit
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Sun Jul 12 19:25:25 EDT 1998
// $Id: T_FAItem_SeedTrackZDHitLattice.cc,v 1.1 2002/09/27 21:02:52 bkh Exp $
//
// Revision history
//
// $Log: T_FAItem_SeedTrackZDHitLattice.cc,v $
// Revision 1.1  2002/09/27 21:02:52  bkh
// Add ZD capability
//
// Revision 1.3  2000/02/08 18:30:34  lyon
// Add Storage helper instantiations
//
// Revision 1.2  1999/05/23 21:10:18  meyerm
// simplify Instantiate directory-remove *_include.h files
//
// Revision 1.1  1998/12/17 00:33:24  meyerm
// moving SeedTrackSV*Lattice stuff to TrackFinder
//
// Revision 1.5  1998/12/03 19:50:57  cdj
// added #include "FrameAccess/instantiate_one.h" needed to do the actual
//  instantiation
//
// Revision 1.4  1998/12/01 03:50:33  lyon
// Fixes for Solaris
//
// Revision 1.3  1998/11/09 16:58:00  lyon
// Updated for new Lattice specification
//
// Revision 1.2  1998/09/30 17:06:12  cdj
// use new instantiate macro
//
// Revision 1.1  1998/08/10 19:03:41  lyon
// SeedTrack -> TRSeedTrack
//
// Revision 1.1  1998/07/26 19:24:09  lyon
// Working version with lattice for seed tracks
//

#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <set>
#include <list>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include <vector>
#include <set>
#include <list>
#include <map>


// user include files
//#include "Experiment/report.h"
#include "TrackFinder/SeedZDHitLink.h"

// This is for FAItem
#include "FrameAccess/instantiate_macros.h"

INSTANTIATE_ONE( SeedTrackZDHitLattice )

#include "FrameAccess/instantiate_one.h"

// Instantiate the storage helper
#include "StorageManagement/SMLatticeStorageHelper.h"
typedef SMLatticeStorageHelper<SeedTrackZDHitLattice>
        SeedTrackZDHitLatticeStorageHelper;
SM_REGISTER_STORAGE( SeedTrackZDHitLatticeStorageHelper );
