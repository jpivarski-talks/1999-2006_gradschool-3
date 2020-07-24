// -*- C++ -*-
//
// Package:     PivarskiFilter
// Module:      STL_vectorOfPivarskiHelix
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Oct  8 22:47:39 EDT 1998
// $Id: STL_vectorOfPivarskiHelix.cc,v 1.1 1999/05/14 01:13:54 wsun Exp $
//
// Revision history
//
// $Log: STL_vectorOfPivarskiHelix.cc,v $
// Revision 1.1  1999/05/14 01:13:54  wsun
// First submission of producer.
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

class PivarskiHelix ;
typedef PivarskiHelix* _vector_contents_ ;
#include "STLUtility/instantiate_vector.h"

