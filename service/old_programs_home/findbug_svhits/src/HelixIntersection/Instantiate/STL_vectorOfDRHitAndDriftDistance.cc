// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      STL_vectorOfCalibratedHits
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Oct  8 22:47:39 EDT 1998
// $Id: STL_vectorOfDRHitAndDriftDistance.cc,v 1.2 1999/06/02 23:05:38 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfDRHitAndDriftDistance.cc,v $
// Revision 1.2  1999/06/02 23:05:38  wsun
// Added features for low-momentum fitting.
//
// Revision 1.1  1999/04/27 22:10:56  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:29  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "HelixIntersection/HIDRSurfaceFactory.h"

// STL classes
#include <vector>


//class CalibratedDRHit ;
//template class pair< const CalibratedDRHit*, double > ;

//typedef pair< const CalibratedDRHit*, double >
typedef HIDRSurfaceFactory::DRHitAndDriftDistance _vector_contents_;

#include "STLUtility/instantiate_vector.h"

