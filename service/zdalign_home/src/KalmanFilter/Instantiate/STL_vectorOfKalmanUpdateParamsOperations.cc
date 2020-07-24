// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      STL_vectorOfKalmanUpdateParamsOperations
// 
// Description: Instantiate STL classes for Command
//
//
// Author:      Werner Sun
// Created:     Tue Mar 27 10:22:06 EST 2001
// $Id: STL_vectorOfKalmanUpdateParamsOperations.cc,v 1.1 2001/03/27 17:34:26 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfKalmanUpdateParamsOperations.cc,v $
// Revision 1.1  2001/03/27 17:34:26  wsun
// First submission.
//
//
//

#include "Experiment/Experiment.h"
// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include <vector>

// user include files

class KalmanUpdateParamsOperation ;
typedef KalmanUpdateParamsOperation* _vector_contents_;

#include "STLUtility/instantiate_vector.h"


