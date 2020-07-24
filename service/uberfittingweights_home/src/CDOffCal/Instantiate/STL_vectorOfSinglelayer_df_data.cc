// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      STL_vectorOfSinglelayer_df_data
// 
// Author:      Ken McLean 
//
#include "Experiment/Experiment.h"
#include "Experiment/types.h"

#include <stdlib.h>
#include <assert.h>
// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "CDOffCal/DataDriftFunction.h"

// STL classes
#include <vector>

typedef Singlelayer_df_data _vector_contents_;

#include "STLUtility/instantiate_vector.h"
