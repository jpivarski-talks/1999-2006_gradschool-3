// -*- C++ -*-
//
// Package:     ChsiqFitter
// Module:      STL_vectorOfHitAndLinkData
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Oct  8 22:47:39 EDT 1998
// $Id: STL_vectorOfHitAndLinkData.cc,v 1.1.1.1 1999/06/16 02:46:48 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfHitAndLinkData.cc,v $
// Revision 1.1.1.1  1999/06/16 02:46:48  wsun
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

#include "ChisqFitter/ChisqHelix.h"
typedef ChisqHelix::HitAndLinkData _vector_contents_ ;
#include "STLUtility/instantiate_vector.h"
