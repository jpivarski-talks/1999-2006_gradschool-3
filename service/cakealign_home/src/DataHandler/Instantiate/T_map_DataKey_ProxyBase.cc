// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      T_map_DataKey_ProxyBase
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D. Jones
// Created:     Thu Sep 24 17:05:32 EDT 1998
// $Id: T_map_DataKey_ProxyBase.cc,v 1.1 1998/09/25 19:32:11 cdj Exp $
//
// Revision history
//
// $Log: T_map_DataKey_ProxyBase.cc,v $
// Revision 1.1  1998/09/25 19:32:11  cdj
// no longer use ProxyDict
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DataHandler/DataKey.h"

// STL classes
#include <map>

//
// instantiate
//
class ProxyBase;

typedef DataKey _map_key_;
typedef const ProxyBase* _map_contents_ ;
typedef less< DataKey > _map_compare_ ;

#include "STLUtility/instantiate_map.h"
