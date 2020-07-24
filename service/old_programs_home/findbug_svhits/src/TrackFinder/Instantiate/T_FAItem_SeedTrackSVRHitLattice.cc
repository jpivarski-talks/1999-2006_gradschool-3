// -*- C++ -*-
//
// Package:     TrackFinder
// Module:      T_FAItem_Lattice_track-SVRHit
// 
// Description: Instantiate FAItem< SeedTrackSVRHitLattice >
//
// Author:      Merlin Meyer-Mitchell
// Created:     Thu Oct 1 1998
// $Id: T_FAItem_SeedTrackSVRHitLattice.cc,v 1.2 2000/02/08 18:30:35 lyon Exp $
//
// Revision history
// 
// $Log: T_FAItem_SeedTrackSVRHitLattice.cc,v $
// Revision 1.2  2000/02/08 18:30:35  lyon
// Add Storage helper instantiations
//
// Revision 1.1  1998/12/17 00:33:25  meyerm
// moving SeedTrackSV*Lattice stuff to TrackFinder
//
// Revision 1.2  1998/12/10 22:26:29  meyerm
// added some set functions and fixed a few Sun bugs
//
// Revision 1.1  1998/10/08 21:18:32  meyerm
// added SeedTrackSVZHitLattice and renamed .SVRh. to .SVRH.
//
// Revision 1.5  1998/10/02 18:22:34  meyerm
// updated all to work with SeedTrackSVRHitLattice.h
//
// Revision 1.3  1998/10/01 23:18:40  meyerm
// added log
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
#include "TrackFinder/SeedTrackSVRHitLattice.h"

//-------------------//
// STL include files //
//-------------------//
#include <vector>
#include <set>

//-----------------------------------//
// instantiate FrameAccess templates //
//-----------------------------------//
#include "FrameAccess/instantiate_macros.h" 

INSTANTIATE_ONE( SeedTrackSVRHitLattice )

#include "FrameAccess/instantiate_one.h"

// Instantiate the storage helper
#include "StorageManagement/SMLatticeStorageHelper.h"
typedef SMLatticeStorageHelper<SeedTrackSVRHitLattice>
        SeedTrackSVRHitLatticeStorageHelper;
SM_REGISTER_STORAGE( SeedTrackSVRHitLatticeStorageHelper );
