// -*- C++ -*-
//
// Package:     DualTrackProd
// Module:      STL_vectorOfHitAndLinkData
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 10:20:02 EST 2003
// $Id: 
//
// Revision history
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"

// STL classes
#include <vector>


// class CalibratedHit ;
// class TrackFitDRHitLink ;
// template class pair< const CalibratedHit*, TrackFitDRHitLink* > ;

// typedef pair< const CalibratedHit*, TrackFitDRHitLink* >
// _vector_contents_;

#include "DualTrackProd/DualTrackFitHelix.h"
typedef HIFitHelix::HitAndLinkData _vector_contents_ ;
#include "STLUtility/instantiate_vector.h"

