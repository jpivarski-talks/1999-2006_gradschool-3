// -*- C++ -*-
//
// Package:     CDOffCal
// Module:      T_DriftFunction
// 
// Description: Instantiate Classes for DriftFunction
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Martin Lohner, Ken McLean
// Created:     Wed Mar  4 14:41:37 EST 1998
// $Id: T_DriftFunction.cc,v 1.3 1998/09/30 17:03:28 cdj Exp $
//
// Revision history
//
// $Log: T_DriftFunction.cc,v $
// Revision 1.3  1998/09/30 17:03:28  cdj
// use new instantiate macro
//
// Revision 1.2  1998/05/06 21:40:28  mclean
// Updates to Match Modifications to RawData
//
// Revision 1.1.1.1  1998/03/04 20:41:49  mkl
// imported CDOffCal sources
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "CDOffCal/DriftFunction.h"

#include "FrameAccess/instantiate_macros.h" 
INSTANTIATE_ONE( DriftFunction ) 

#include "FrameAccess/instantiate_one.h"
