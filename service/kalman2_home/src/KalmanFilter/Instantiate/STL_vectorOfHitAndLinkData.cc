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
// $Id: STL_vectorOfHitAndLinkData.cc,v 1.2 1999/05/14 01:13:53 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfHitAndLinkData.cc,v $
// Revision 1.2  1999/05/14 01:13:53  wsun
// First submission of producer.
//
// Revision 1.1  1999/04/27 22:21:29  wsun
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

// STL classes
#include <vector>


// class CalibratedHit ;
// class TrackFitDRHitLink ;
// template class pair< const CalibratedHit*, TrackFitDRHitLink* > ;

// typedef pair< const CalibratedHit*, TrackFitDRHitLink* >
// _vector_contents_;

#include "KalmanFilter/KalmanHelix.h"
typedef KalmanHelix::HitAndLinkData _vector_contents_ ;
#include "STLUtility/instantiate_vector.h"

