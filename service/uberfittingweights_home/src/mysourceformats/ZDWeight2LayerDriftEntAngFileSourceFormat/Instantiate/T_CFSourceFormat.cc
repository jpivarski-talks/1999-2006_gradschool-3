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
// Author:      Jim Pivarski
// Created:     Mon Sep 22 15:19:44 EDT 2003
// $Id: T_Binder.cc,v 1.0 2003/09/22 15:19:44 mccann Exp $
//
// Revision history
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

