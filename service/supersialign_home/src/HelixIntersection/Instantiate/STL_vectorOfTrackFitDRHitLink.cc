// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      STL_vectorOfTrackFitDRHitLink
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Jun  9 17:18:53 EDT 2000
// $Id: STL_vectorOfTrackFitDRHitLink.cc,v 1.1 2000/06/09 23:43:43 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfTrackFitDRHitLink.cc,v $
// Revision 1.1  2000/06/09 23:43:43  wsun
// First submission.
//
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "TrackFitter/TrackFitDRHitLink.h"

// STL classes
#include <vector>

typedef TrackFitDRHitLink _vector_contents_;

#include "STLUtility/instantiate_vector.h"

