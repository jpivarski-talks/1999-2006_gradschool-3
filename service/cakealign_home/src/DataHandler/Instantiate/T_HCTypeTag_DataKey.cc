// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      T_HCTypeTag_DataKey
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D. Jones
// Created:     Thu Sep 24 17:13:38 EDT 1998
// $Id: T_HCTypeTag_DataKey.cc,v 1.1 1998/09/25 19:32:10 cdj Exp $
//
// Revision history
//
// $Log: T_HCTypeTag_DataKey.cc,v $
// Revision 1.1  1998/09/25 19:32:10  cdj
// no longer use ProxyDict
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DataHandler/DataKey.h"
#include "HeteroContainer/Template/HCTypeTag.cc"
// STL classes

//
// instantiate
//
template class HCTypeTag< DataKey > ;
