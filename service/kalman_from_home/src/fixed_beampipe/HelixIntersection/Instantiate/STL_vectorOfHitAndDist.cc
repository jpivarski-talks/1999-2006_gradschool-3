// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      STL_vectorOfHitAndDist
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Oct  8 22:47:39 EDT 1998
// $Id: STL_vectorOfHitAndDist.cc,v 1.2 2000/02/03 07:29:17 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfHitAndDist.cc,v $
// Revision 1.2  2000/02/03 07:29:17  wsun
// HIMultiWireCylinder::DRHitAndDistUsed -> HIMultiWireCylinder::HitAndUsed.
//
// Revision 1.1  1999/08/03 21:41:54  wsun
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
#include "HelixIntersection/HIMultiWireCylinder.h"

// STL classes
#include <vector>

typedef HIMultiWireCylinder::HitAndUsed _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"

