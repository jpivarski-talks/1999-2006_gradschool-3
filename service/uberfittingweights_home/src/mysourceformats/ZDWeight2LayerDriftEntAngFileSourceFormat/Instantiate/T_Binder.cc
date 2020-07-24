// -*- C++ -*-
//
// Package:     <ZDWeight2LayerDriftEntAngFileSourceFormat>
// Module:      T_Binder
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
#include <string>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "ZDWeight2LayerDriftEntAngFileSourceFormat/ZDWeight2LayerDriftEntAngInfo.h"
#include "ConstantsFileDelivery/CFSourceBinder.h"

// STL classes
#include <string>
#include <map>

//
// instantiate
//
typedef CFSourceBinder<ZDWeight2LayerDriftEntAngInfo> _binder_;
#include "DataDelivery/instantiateNotChainableBinder.h"

