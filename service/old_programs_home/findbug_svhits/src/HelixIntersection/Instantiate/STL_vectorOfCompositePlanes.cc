// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      STL_vectorOfCompositePlanes
// 
// Description: Instantiate STL classes for Command
//
//
// Author:      Werner Sun
// Created:     Tue Nov  9 00:14:07 EST 1999
// $Id: STL_vectorOfCompositePlanes.cc,v 1.1 1999/11/21 22:23:03 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfCompositePlanes.cc,v $
// Revision 1.1  1999/11/21 22:23:03  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"
// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include <vector>

// user include files

class HICompositePlane ;
typedef HICompositePlane* _vector_contents_;

#include "STLUtility/instantiate_vector.h"


