// -*- C++ -*-
//
// Package:     ConstantsModule
// Module:      T_DataSourceBinder
// 
// Description: Instantiate Binder template for DataSourceBinder
//
//
// Author:      Valentine Kouznetsov
// Created:     Tue Feb  4 14:09:10 EST 2003
// $Id: T_DataSourceBinder.cc,v 1.1 2003/03/04 18:50:42 vk Exp $
//
// Revision history
//
// $Log: T_DataSourceBinder.cc,v $
// Revision 1.1  2003/03/04 18:50:42  vk
// Add instantiateNotChainableBinder
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

// stl includes
#include <string>
#include <map>

typedef DataSourceBinder _binder_;
#include "DataDelivery/instantiateNotChainableBinder.h"

