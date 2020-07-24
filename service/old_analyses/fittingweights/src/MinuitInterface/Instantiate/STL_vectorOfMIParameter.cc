// -*- C++ -*-
//
// Package:     MinuitInterface
// 
// Description: Instantiate STL classes for MIParameter
//
//
// Author:      Adam Lyon
// Created:     22 Oct 99
// $Id: STL_vectorOfMIParameter.cc,v 1.1.1.1 1999/12/09 15:34:51 lyon Exp $
//
// Revision history
//
// $Log: STL_vectorOfMIParameter.cc,v $
// Revision 1.1.1.1  1999/12/09 15:34:51  lyon
// Imported MinuitInterface sources
//
//

#include "Experiment/Experiment.h"
// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "MinuitInterface/MIParameter.h"

typedef  MIParameter _vector_contents_;

#include "STLUtility/instantiate_vector.h"
