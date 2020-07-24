// -*- C++ -*-
//
// Package:     ConstantsPhase2Delivery
// Module:      T_CP2SourceBinder
// 
// Description: Instantiate Binder template for CP2SourceBinder
//
//
// Author:      Valentine Kouznetsov
// Created:     Mon Jan 20 09:53:17 EST 2003
// $Id: T_CP2SourceBinder.cc,v 1.1.1.1 2003/01/28 21:49:54 vk Exp $
//
// Revision history
//
// $Log: T_CP2SourceBinder.cc,v $
// Revision 1.1.1.1  2003/01/28 21:49:54  vk
// imported ConstantsPhase2Delivery sources
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#include <map>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DataDelivery/DataSourceBinder.h"
#include "ConstantsPhase2Delivery/CP2SourceBinder.h"

// stl includes
#include <string>
#include <map>

typedef CP2SourceBinder _binder_;
#include "DataDelivery/instantiateNotChainableBinder.h"

