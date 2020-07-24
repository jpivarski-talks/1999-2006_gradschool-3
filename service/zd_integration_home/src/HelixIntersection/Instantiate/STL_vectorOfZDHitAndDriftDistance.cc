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
// Author:      Jim Pivarski
// Created:     Mon Jul 28 10:40:26 EDT 2003
// $Id: STL_vectorOfZDHitAndDriftDistance.cc,v 1.1 2003/07/28 14:49:57 mccann Exp $
//
// Revision history
//
// Revision 1.1.1.1  2003/07/28 10:40:26 mccann
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "HelixIntersection/HIZDSurfaceFactory.h"

// STL classes
#include <vector>


//class CalibratedZDHit ;
//template class pair< const CalibratedZDHit*, double > ;

//typedef pair< const CalibratedZDHit*, double >
typedef HIZDSurfaceFactory::ZDHitAndDriftDistance _vector_contents_;

#include "STLUtility/instantiate_vector.h"

