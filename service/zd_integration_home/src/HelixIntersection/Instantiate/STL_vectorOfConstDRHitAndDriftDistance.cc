// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      STL_vectorOfConstDRHitAndDriftDistance.cc
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Oct  8 22:47:39 EDT 1998
// $Id: STL_vectorOfConstDRHitAndDriftDistance.cc,v 1.1 1999/08/03 21:41:55 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfConstDRHitAndDriftDistance.cc,v $
// Revision 1.1  1999/08/03 21:41:55  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "HelixIntersection/HIDRSurfaceFactory.h"

// STL classes
#include <vector>
typedef const HIDRSurfaceFactory::DRHitAndDriftDistance* _vector_contents_;

#include "STLUtility/instantiate_vector.h"

