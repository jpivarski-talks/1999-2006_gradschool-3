// -*- C++ -*-
//
// Package:     Navigation
// Module:      STL_vector_of_CalibratedSVZHit
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Tue Dec 29 14:10:30 EST 1998
// $Id: STL_vector_of_CalibratedSVZHit.cc,v 1.2 1999/06/25 12:08:45 marsh Exp $
//
// Revision history
//
// $Log: STL_vector_of_CalibratedSVZHit.cc,v $
// Revision 1.2  1999/06/25 12:08:45  marsh
// Fixed include path for calibrated silicon hits.
//
// Revision 1.1.1.1  1998/12/31 18:18:31  marsh
// New library for navigation objects.
//
// Revision 1.1  1998/12/29 22:20:14  marsh
// Mods and new instantiation files for providing Lattice info.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "SiHits/CalibratedSVZHit.h"

// STL classes
#include <vector>

typedef const CalibratedSVZHit* _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"
