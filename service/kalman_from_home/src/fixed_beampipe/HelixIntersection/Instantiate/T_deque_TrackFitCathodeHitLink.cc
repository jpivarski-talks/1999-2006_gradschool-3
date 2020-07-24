// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      STL_vectorOfTrackFitCathodeHitLink
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Jun  9 17:18:53 EDT 2000
// $Id: T_deque_TrackFitCathodeHitLink.cc,v 1.1 2002/04/27 17:22:56 cleo3 Exp $
//
// Revision history
//
// $Log: T_deque_TrackFitCathodeHitLink.cc,v $
// Revision 1.1  2002/04/27 17:22:56  cleo3
// replaced vectors with deques
//
// Revision 1.1  2000/06/09 23:43:42  wsun
// First submission.
//
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <deque>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "TrackFitter/TrackFitCathodeHitLink.h"

// STL classes
#include <deque>

typedef TrackFitCathodeHitLink _deque_contents_;

#include "STLUtility/instantiate_deque.h"

