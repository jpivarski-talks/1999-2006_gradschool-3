// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      T_set_DataKey
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D. Jones
// Created:     Thu Sep 24 17:05:40 EDT 1998
// $Id: T_set_DataKey.cc,v 1.1 1998/09/25 19:32:13 cdj Exp $
//
// Revision history
//
// $Log: T_set_DataKey.cc,v $
// Revision 1.1  1998/09/25 19:32:13  cdj
// no longer use ProxyDict
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DataHandler/DataKey.h"

// STL classes
#include <set>

//
// constants, enums and typedefs
//
typedef DataKey _set_contents_ ;
typedef less< DataKey > _set_compare_ ;

#include "STLUtility/instantiate_set.h"
