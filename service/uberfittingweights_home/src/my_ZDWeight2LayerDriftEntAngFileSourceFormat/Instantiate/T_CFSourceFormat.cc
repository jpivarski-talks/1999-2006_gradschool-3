// -*- C++ -*-
//
// Package:     ZDWeight2LayerDriftEntAngFileSourceFormat
// Module:      T_CFSourceFormat
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Sat Mar 13 15:41:22 EST 1999
// $Id: T_CFSourceFormat.cc,v 1.1 1999/03/13 22:44:27 cdj Exp $
//
// Revision history
//
// $Log: T_CFSourceFormat.cc,v $
// Revision 1.1  1999/03/13 22:44:27  cdj
// First submission
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "ConstantsFileDelivery/Template/CFSourceFormat.cc"
#include "ZDWeight2LayerDriftEntAngFileSourceFormat/ZDWeight2LayerDriftEntAngInfo.h"

// STL classes
// You may have to uncomment some of these or other stl headers
#include <set>

//
// instantiate
//
template class CFSourceFormat< ZDWeight2LayerDriftEntAngInfo >;

