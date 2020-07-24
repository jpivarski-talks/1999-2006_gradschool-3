// -*- C++ -*-
//
// Package:     DRAlignmentMod
// Module:      STL_vectorOfvectorOfCalibratedDRHit
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Dec  8 11:57:02 EST 2003
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
#include "CalibratedData/CalibratedDRHit.h"

// STL classes
#include <vector>


typedef vector<CalibratedDRHit*>* _vector_contents_;

#include "STLUtility/instantiate_vector.h"

