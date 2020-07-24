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
// $Id: T_deque_TrackFitDRHitLink.cc,v 1.1 2002/04/27 17:22:57 cleo3 Exp $
//
// Revision history
//
// $Log: T_deque_TrackFitDRHitLink.cc,v $
// Revision 1.1  2002/04/27 17:22:57  cleo3
// replaced vectors with deques
//
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
#include <deque>

typedef TrackFitDRHitLink _deque_contents_;

#include "STLUtility/instantiate_deque.h"

