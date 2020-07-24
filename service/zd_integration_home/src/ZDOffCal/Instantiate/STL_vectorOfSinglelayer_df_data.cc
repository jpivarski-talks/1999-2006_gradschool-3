// -*- C++ -*-
//
// Package:     <ZDOffCal>
// Module:      STL_vectorOfSinglelayer_df_data
// 
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: STL_vectorOfSinglelayer_df_data.cc,v 1.3 2003/04/24 17:35:33 huanggs Exp $
//
// Revision history
//
// $Log: STL_vectorOfSinglelayer_df_data.cc,v $
// Revision 1.3  2003/04/24 17:35:33  huanggs
// file names match class names
//
// Revision 1.2  2003/04/24 14:51:55  huanggs
// ZD calsses
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
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ZDOffCal/ZDDataDriftFunction.h"

// STL classes
#include <vector>

typedef ZDSinglelayer_df_data _vector_contents_;

#include "STLUtility/instantiate_vector.h"
