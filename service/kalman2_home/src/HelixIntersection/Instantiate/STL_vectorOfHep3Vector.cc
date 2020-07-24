// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      STL_vectorOfSurfaces
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Oct  8 22:47:39 EDT 1998
// $Id: STL_vectorOfHep3Vector.cc,v 1.1 1999/04/27 22:10:57 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfHep3Vector.cc,v $
// Revision 1.1  1999/04/27 22:10:57  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:34  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "CLHEP/Vector/ThreeVector.h"

// STL classes
#include <vector>

typedef Hep3Vector _vector_contents_;

#include "STLUtility/instantiate_vector.h"

