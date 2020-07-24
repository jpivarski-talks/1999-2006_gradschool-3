// -*- C++ -*-
//
// Package:     <package>
// Module:      T_CFProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Sep 22 15:19:50 EDT 2003
// $Id: T_Binder.cc,v 1.0 2003/09/22 15:19:50 mccann Exp $
//
// Revision history
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "ZDWeight2LayerThetaFileSourceFormat/ZDWeight2LayerThetaInfo.h"
#include "ConstantsFileDelivery/Template/CFProxy.cc"

//
// instantiate
//
template class CFProxy< ZDWeight2LayerThetaInfo::ConstantType >;





