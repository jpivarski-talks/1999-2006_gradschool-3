// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      STL_vectorOfCompositeSurfaces
// 
// Description: Instantiate STL classes for Command
//
//
// Author:      Werner Sun
// Created:     Fri Nov 20 15:38:00 EDT 1998
// $Id: STL_vectorOfCompositeSurfaces.cc,v 1.1 1999/04/27 22:10:55 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfCompositeSurfaces.cc,v $
// Revision 1.1  1999/04/27 22:10:55  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:59:01  wsun
// First submission.
//

#include "Experiment/Experiment.h"
// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include <vector>

// user include files

class HICompositeSurface ;
typedef HICompositeSurface* _vector_contents_;

#include "STLUtility/instantiate_vector.h"


