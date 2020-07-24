// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      STL_vectorOfHitAndDistP
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Wed Sep  8 19:19:55 EDT 1999
// $Id: STL_vectorOfHitAndDistP.cc,v 1.2 2000/02/03 07:29:18 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfHitAndDistP.cc,v $
// Revision 1.2  2000/02/03 07:29:18  wsun
// HIMultiWireCylinder::DRHitAndDistUsed -> HIMultiWireCylinder::HitAndUsed.
//
// Revision 1.1  1999/09/10 21:21:11  wsun
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

typedef HIMultiWireCylinder::HitAndUsed* _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"

