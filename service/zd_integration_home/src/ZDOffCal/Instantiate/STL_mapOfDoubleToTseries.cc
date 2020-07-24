// -*- C++ -*-
//
// Package:     <ZDOffCal>
// Module:      STL_mapOfDriftTseries
// 
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: STL_mapOfDoubleToTseries.cc,v 1.2 2003/04/24 17:35:33 huanggs Exp $
//
// Revision history
//
// $Log: STL_mapOfDoubleToTseries.cc,v $
// Revision 1.2  2003/04/24 17:35:33  huanggs
// file names match class names
//
// Revision 1.1.1.1  2003/02/03 20:11:57  huanggs
// imported source
//
//
#include "Experiment/Experiment.h"
#include "Experiment/types.h"

#include <stdlib.h>
#include <assert.h>
// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ZDOffCal/ZDDriftFunction.h"

// STL classes
#include <map>

typedef double               _map_key_       ;
typedef Tseries              _map_contents_  ;
typedef less< _map_key_ >    _map_compare_   ;

#include "STLUtility/instantiate_map.h"
