// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      STL_vectorOfSeedTracks
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Oct  8 22:47:39 EDT 1998
// $Id: STL_vectorOfSeedTracks.cc,v 1.1 1999/05/28 22:50:14 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfSeedTracks.cc,v $
// Revision 1.1  1999/05/28 22:50:14  wsun
// Copied file from KalmanFilter.
//
// Revision 1.1  1999/05/14 01:13:55  wsun
// First submission of producer.
//
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


class TRSeedTrack;
typedef const TRSeedTrack* _vector_contents_;

#include "STLUtility/instantiate_vector.h"

