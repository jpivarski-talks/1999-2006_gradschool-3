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
// Created:     Sat Feb  5 16:32:03 EST 2000
// $Id: STL_vectorOfCalibratedCathodeHits.cc,v 1.1 2000/02/05 21:36:44 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfCalibratedCathodeHits.cc,v $
// Revision 1.1  2000/02/05 21:36:44  wsun
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


class CalibratedCathodeHit;
typedef const CalibratedCathodeHit* _vector_contents_;

#include "STLUtility/instantiate_vector.h"

