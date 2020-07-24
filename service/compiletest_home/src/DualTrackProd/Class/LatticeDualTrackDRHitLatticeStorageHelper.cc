// -*- C++ -*-
//
// Package:     <package>
// Module:      LatticeDualTrackDRHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:38:22 EDT 1999
// $Id: LatticeDualTrackDRHitLatticeStorageHelper.cc,v 1.1 2003/12/15 17:53:14 mccann Exp $
//
// Revision history
//
// $Log: LatticeDualTrackDRHitLatticeStorageHelper.cc,v $
// Revision 1.1  2003/12/15 17:53:14  mccann
// adding hit lattices for dual-constrained residuals
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
#include "DualTrackProd/DualTrackDRHitLink.h"

typedef DualTrackDRHitLattice _lattice_element_ ;
#include "StorageManagement/instantiate_latticehelper.h"

