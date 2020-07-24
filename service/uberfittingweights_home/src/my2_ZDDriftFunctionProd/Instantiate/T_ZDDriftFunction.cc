// -*- C++ -*-
//
// Package:     ZDDriftFunctionProd
// Module:      T_ZDDriftFunction
// 
// Description: Instantiate Classes for ZDDriftFunction
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: T_ZDDriftFunction.cc,v 1.1.1.1 2003/06/26 19:02:51 huanggs Exp $
//
// Revision history
//
// $Log: T_ZDDriftFunction.cc,v $
// Revision 1.1.1.1  2003/06/26 19:02:51  huanggs
// imported ZDDriftFunctionProd source
//
// Revision 1.3  2003/04/24 17:35:33  huanggs
// file names match class names
//
// Revision 1.2  2003/04/24 14:51:55  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:57  huanggs
// imported source
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ZDDriftFunctionProd/ZDDriftFunction.h"

#include "FrameAccess/instantiate_macros.h" 
INSTANTIATE_ONE( ZDDriftFunction ) 

#include "FrameAccess/instantiate_one.h"
