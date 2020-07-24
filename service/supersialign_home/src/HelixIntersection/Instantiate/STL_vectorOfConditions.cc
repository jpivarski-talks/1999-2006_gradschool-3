// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      STL_vectorOfConditions
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Oct  8 22:47:39 EDT 1998
// $Id: STL_vectorOfConditions.cc,v 1.1.1.1 1998/12/07 04:58:33 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfConditions.cc,v $
// Revision 1.1.1.1  1998/12/07 04:58:33  wsun
// First submission.
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


class HICondition ;
typedef HICondition* _vector_contents_;

#include "STLUtility/instantiate_vector.h"

