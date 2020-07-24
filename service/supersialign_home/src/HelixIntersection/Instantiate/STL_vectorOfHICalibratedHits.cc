// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      STL_vectorOfHICalibratedHits
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sat Mar  4 22:24:30 EST 2000
// $Id: STL_vectorOfHICalibratedHits.cc,v 1.1 2000/03/05 03:29:42 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfHICalibratedHits.cc,v $
// Revision 1.1  2000/03/05 03:29:42  wsun
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

// STL classes
#include <vector>


class HICalibratedHit;
typedef HICalibratedHit* _vector_contents_;

#include "STLUtility/instantiate_vector.h"

