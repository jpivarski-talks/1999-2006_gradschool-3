// -*- C++ -*-
//
// Package:     KalmanProd
// Module:      STL_vectorOfHypos
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Jun  9 17:11:17 EDT 2000
// $Id: STL_vectorOfHypos.cc,v 1.1 2000/06/10 00:15:17 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfHypos.cc,v $
// Revision 1.1  2000/06/10 00:15:17  wsun
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
#include "CleoDB/DBCandidate.h"

// STL classes
#include <vector>


typedef DBCandidate::Hypo _vector_contents_;

#include "STLUtility/instantiate_vector.h"

