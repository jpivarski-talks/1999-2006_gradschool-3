// -*- C++ -*-
//
// Package:     CommandPattern
// Module:      STL_Command
// 
// Description: Instantiate STL classes for Command
//
//
// Author:      Martin Lohner
// Created:     Fri July 11 12:00:00 EDT 1997
// $Id: STL_vectorOfCalibratedZDHits.cc,v 1.1 2002/09/27 21:02:51 bkh Exp $
//
// Revision history
//
// $Log: STL_vectorOfCalibratedZDHits.cc,v $
// Revision 1.1  2002/09/27 21:02:51  bkh
// Add ZD capability
//
//

#include "Experiment/Experiment.h"
// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include <vector>

// user include files

class CalibratedZDHit;
typedef const CalibratedZDHit* _vector_contents_;

#include "STLUtility/instantiate_vector.h"


