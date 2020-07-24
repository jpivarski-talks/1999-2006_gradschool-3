// -*- C++ -*-
//
// Package:     Navigation
// Module:      STL_vector_of_CalibratedDRHit
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Tue Dec 29 14:10:30 EST 1998
// $Id: STL_vector_of_CalibratedDRHit.cc,v 1.1.1.1 1998/12/31 18:18:31 marsh Exp $
//
// Revision history
//
// $Log: STL_vector_of_CalibratedDRHit.cc,v $
// Revision 1.1.1.1  1998/12/31 18:18:31  marsh
// New library for navigation objects.
//
// Revision 1.1  1998/12/29 22:20:13  marsh
// Mods and new instantiation files for providing Lattice info.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "CalibratedData/CalibratedDRHit.h"

// STL classes
#include <vector>

typedef const CalibratedDRHit* _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"
