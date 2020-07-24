// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      STL_vectorOfConstraints
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Jul  4 15:48:58 EDT 1999
// $Id: STL_vectorOfConstraints.cc,v 1.1 1999/07/07 16:41:53 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfConstraints.cc,v $
// Revision 1.1  1999/07/07 16:41:53  wsun
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


class HIFitConstraint ;
typedef HIFitConstraint* _vector_contents_;

#include "STLUtility/instantiate_vector.h"
