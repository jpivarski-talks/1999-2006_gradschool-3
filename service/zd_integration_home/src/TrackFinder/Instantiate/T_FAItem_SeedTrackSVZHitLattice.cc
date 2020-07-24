// -*- C++ -*-
//
// Package:     TrackFinder
// Module:      T_FAItem_Lattice_track-SVZHit
// 
// Description: Instantiate FAItem< SeedTrackSVZHitLattice >
//
// Author:      Merlin Meyer-Mitchell
// Created:     Thu Oct 8 1998
// $Id: T_FAItem_SeedTrackSVZHitLattice.cc,v 1.3 2000/02/23 16:48:46 lyon Exp $
//
// Revision history
// 
// $Log: T_FAItem_SeedTrackSVZHitLattice.cc,v $
// Revision 1.3  2000/02/23 16:48:46  lyon
// Added #include<set>
//
// Revision 1.2  2000/02/23 16:10:58  lyon
// Added SVZ lattice storage helper
//
// Revision 1.1  1998/12/17 00:33:26  meyerm
// moving SeedTrackSV*Lattice stuff to TrackFinder
//
// Revision 1.2  1998/12/10 22:26:30  meyerm
// added some set functions and fixed a few Sun bugs
//
// Revision 1.1  1998/10/08 21:18:33  meyerm
// added SeedTrackSVZHitLattice and renamed .SVRh. to .SVRH.
//
//

//--------------------------//
// Experiment include files //
//--------------------------//
#include "Experiment/Experiment.h"

//----------------------//
// system include files //
//----------------------//
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

//--------------------//
// user include files //
//--------------------//
#include "TrackFinder/SeedTrackSVZHitLattice.h"

//-------------------//
// STL include files //
//-------------------//
#include <vector>
#include <set>

//-----------------------------------//
// instantiate FrameAccess templates //
//-----------------------------------//
#include "FrameAccess/instantiate_macros.h" 

INSTANTIATE_ONE( SeedTrackSVZHitLattice )

#include "FrameAccess/instantiate_one.h"

// Instantiate the storage helper
#include "StorageManagement/SMLatticeStorageHelper.h"
typedef SMLatticeStorageHelper<SeedTrackSVZHitLattice>
        SeedTrackSVZHitLatticeStorageHelper;
SM_REGISTER_STORAGE( SeedTrackSVZHitLatticeStorageHelper );
