// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      STL_vectorOfTrackFitZDHitLink
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Jul 28 10:40:26 EDT 2003
// $Id: T_deque_TrackFitZDHitLink.cc,v 1.1 2003/07/28 14:50:20 mccann Exp $
//
// Revision history
//
// Revision 1.1  2003/07/28 10:40:26 mccann
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
#include "TrackFitter/TrackFitZDHitLink.h"

// STL classes
#include <deque>

typedef TrackFitZDHitLink _deque_contents_;

#include "STLUtility/instantiate_deque.h"

