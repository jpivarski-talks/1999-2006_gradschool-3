// -*- C++ -*-
//
// Package:     Navigation
// Module:      STL_vector_of_CalibratedSVRphiHit
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Tue Dec 29 14:10:30 EST 1998
// $Id: STL_vector_of_CalibratedSVRphiHit.cc,v 1.3 1999/06/25 17:28:50 pg Exp $
//
// Revision history
//
// $Log: STL_vector_of_CalibratedSVRphiHit.cc,v $
// Revision 1.3  1999/06/25 17:28:50  pg
// Added map.h to couple of instantiation files.
//
// Revision 1.2  1999/06/25 12:08:45  marsh
// Fixed include path for calibrated silicon hits.
//
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
#include "SiHits/CalibratedSVRphiHit.h"

// STL classes
#include <vector>

typedef const CalibratedSVRphiHit* _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"


