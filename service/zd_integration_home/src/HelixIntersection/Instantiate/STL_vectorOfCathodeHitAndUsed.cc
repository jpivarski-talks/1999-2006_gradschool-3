// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      STL_vectorOfCathodeHitAndUsed
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Wed Sep  8 00:28:52 EDT 1999
// $Id: STL_vectorOfCathodeHitAndUsed.cc,v 1.2 2000/02/05 21:39:28 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfCathodeHitAndUsed.cc,v $
// Revision 1.2  2000/02/05 21:39:28  wsun
// Instantiation types changed.
//
// Revision 1.1  1999/09/10 21:21:09  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"

#include "HelixIntersection/HICalibratedCathodeHit.h"

// STL classes
#include <vector>

template class pair< HICalibratedCathodeHit, DABoolean > ;

typedef pair< HICalibratedCathodeHit, DABoolean >
_vector_contents_;

#include "STLUtility/instantiate_vector.h"

