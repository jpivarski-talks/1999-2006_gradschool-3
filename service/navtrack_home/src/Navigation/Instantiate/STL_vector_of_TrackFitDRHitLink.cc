// -*- C++ -*-
//
// Package:     Navigation
// Module:      STL_vector_of_TrackFitDRHitLink
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Tue Dec 29 14:10:30 EST 1998
// $Id: STL_vector_of_TrackFitDRHitLink.cc,v 1.2 1999/04/23 21:35:42 marsh Exp $
//
// Revision history
//
// $Log: STL_vector_of_TrackFitDRHitLink.cc,v $
// Revision 1.2  1999/04/23 21:35:42  marsh
// Changed NavTrack to deliver links, not link data.
//
// Revision 1.1.1.1  1998/12/31 18:18:32  marsh
// New library for navigation objects.
//
// Revision 1.1  1998/12/29 22:20:16  marsh
// Mods and new instantiation files for providing Lattice info.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackFitter/TrackFitDRHitLink.h"
#include "Navigation/NavTrack.h"

// STL classes
#include <vector>

typedef const NavTrack::DRHitLink* _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"
