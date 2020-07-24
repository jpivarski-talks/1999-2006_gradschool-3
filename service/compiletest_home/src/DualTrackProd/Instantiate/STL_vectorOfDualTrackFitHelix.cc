// -*- C++ -*-
//
// Package:     DualTrackProd
// Module:      STL_vectorOfDualTrackFitHelix
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 09:49:22 EST 2003
// $Id: 
//
// Revision history

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DualTrackProd/DualTrackFitHelix.h"

// STL classes
#include <vector>


typedef DualTrackFitHelix* _vector_contents_;

#include "STLUtility/instantiate_vector.h"

