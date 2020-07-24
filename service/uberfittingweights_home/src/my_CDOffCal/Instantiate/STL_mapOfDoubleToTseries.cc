// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      STL_mapOfDriftTseries
// 
// Author:      Ken McLean 
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
#include "CDOffCal/DriftFunction.h"

// STL classes
#include <map>

typedef double               _map_key_       ;
typedef Tseries              _map_contents_  ;
typedef less< _map_key_ >    _map_compare_   ;

#include "STLUtility/instantiate_map.h"
