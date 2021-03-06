// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      STL_vectorOfHepVector
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Oct  8 22:47:39 EDT 1998
// $Id: STL_vectorOfHepSymMatrix.cc,v 1.1 1999/06/02 23:08:43 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfHepSymMatrix.cc,v $
// Revision 1.1  1999/06/02 23:08:43  wsun
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
#include "CLHEP/Matrix/SymMatrix.h"

// STL classes
#include <vector>

typedef HepSymMatrix* _vector_contents_;

#include "STLUtility/instantiate_vector.h"

