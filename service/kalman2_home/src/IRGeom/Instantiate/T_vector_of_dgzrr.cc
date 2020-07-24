// -*- C++ -*-
//
// Package:     <MCInfo>
// Module:      T_vector_of_dgzrr.cc
// 
// Description: Instantiate a vector holding MCParticleProperty::QQ_id
//
//
// Author:      Lawrence Gibbons
// Created:     Fri Jan  8 16:48:05 EST 1999
// $Id: T_vector_of_dgzrr.cc,v 1.1.1.1 1999/07/22 14:38:43 lkg Exp $
//
// Revision history
//
// $Log: T_vector_of_dgzrr.cc,v $
// Revision 1.1.1.1  1999/07/22 14:38:43  lkg
// First release
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DetectorGeometry/DGZrR.h"

// STL classes
#include <vector>

// forward declarations

typedef DGZrR _vector_contents_;

#include "STLUtility/instantiate_vector.h"
