// -*- C++ -*-
//
// Package:     Navigation
// Module:      STL_vector_of_SeedZDHitLink
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Oct  8 13:20:33 EDT 2003
// $Id: STL_vector_of_SeedZDHitLink.cc,v 1.1 2003/10/16 18:43:11 mccann Exp $
//
// Revision history
//
// $Log: STL_vector_of_SeedZDHitLink.cc,v $
// Revision 1.1  2003/10/16 18:43:11  mccann
// adding ZD files
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackFinder/SeedZDHitLink.h"

// STL classes
#include <vector>

typedef const SeedTrackZDHitLattice::Link* _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"
