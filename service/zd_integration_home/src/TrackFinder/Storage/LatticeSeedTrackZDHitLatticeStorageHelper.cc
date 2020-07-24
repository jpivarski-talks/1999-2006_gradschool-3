// -*- C++ -*-
//
// Package:     <package>
// Module:      LatticeSeedTrackZDHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:38:22 EDT 1999
// $Id: LatticeSeedTrackZDHitLatticeStorageHelper.cc,v 1.1 2002/09/27 21:02:59 bkh Exp $
//
// Revision history
//
// $Log: LatticeSeedTrackZDHitLatticeStorageHelper.cc,v $
// Revision 1.1  2002/09/27 21:02:59  bkh
// Add ZD capability
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "TrackFinder/SeedZDHitLink.h"

typedef SeedTrackZDHitLattice _lattice_element_ ;
#include "StorageManagement/instantiate_latticehelper.h"

