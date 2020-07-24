// -*- C++ -*-
//
// Package:     <package>
// Module:      LatticeSeedTrackSVZHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:38:22 EDT 1999
// $Id: LatticeSeedTrackSVZHitLatticeStorageHelper.cc,v 1.1 2000/02/23 16:11:02 lyon Exp $
//
// Revision history
//
// $Log: LatticeSeedTrackSVZHitLatticeStorageHelper.cc,v $
// Revision 1.1  2000/02/23 16:11:02  lyon
// Added SVZ lattice storage helper
//
// Revision 1.1  1999/10/18 21:30:20  cdj
// renamed files
//
// Revision 1.2  1999/10/18 18:16:08  cdj
// no longer instantiates a SMLatticeStorageHelper in this file
//
// Revision 1.1  1999/10/11 22:00:37  cdj
// added Lattice storage code
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "TrackFinder/SeedTrackSVZHitLattice.h"

typedef SeedTrackSVZHitLattice _lattice_element_ ;
#include "StorageManagement/instantiate_latticehelper.h"

