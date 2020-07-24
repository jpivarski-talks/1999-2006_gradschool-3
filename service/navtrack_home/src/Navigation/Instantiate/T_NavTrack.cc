// -*- C++ -*-
//
// Package:     Navigation
// Module:      T_NavTrack
// 
// Description: Instantiate Classes for NavTrack
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Wed Jul  8 15:36:16 EDT 1998
// $Id: T_NavTrack.cc,v 1.2 2003/08/31 23:25:33 cdj Exp $
//
// Revision history
//
// $Log: T_NavTrack.cc,v $
// Revision 1.2  2003/08/31 23:25:33  cdj
// added the ability to store NavTracks
//
// Revision 1.1.1.1  1998/12/31 18:18:32  marsh
// New library for navigation objects.
//
// Revision 1.3  1998/12/29 22:20:20  marsh
// Mods and new instantiation files for providing Lattice info.
//
// Revision 1.2  1998/09/30 17:02:59  cdj
// use new instantiate macro
//
// Revision 1.1.1.1  1998/07/13 23:26:21  marsh
// New library for user-level analysis objects
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Navigation/NavTrack.h"

#include <map>
#include <vector>

#include "FrameAccess/instantiate_macros.h" 
INSTANTIATE_MANY( NavTrack ) 

#include "FrameAccess/instantiate_many.h"

#include "Navigation/Storage/ManyNavTrackStorageHelper.h"
SM_REGISTER_STORAGE(ManyNavTrackStorageHelper)
