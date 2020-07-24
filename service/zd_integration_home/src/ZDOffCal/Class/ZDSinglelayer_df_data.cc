// -*- C++ -*-
//
// Package:     <ZDOffCal>
// Module:      ZDSinglelayer_df_data
//
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDSinglelayer_df_data.cc,v 1.1 2003/04/24 18:08:11 huanggs Exp $
//
// Revision history
//
// $Log: ZDSinglelayer_df_data.cc,v $
// Revision 1.1  2003/04/24 18:08:11  huanggs
// add new files
//
// Revision 1.2  2003/04/24 14:51:52  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:57  huanggs
// imported source
//

#include "Experiment/Experiment.h"
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>

// user include files
#include "Experiment/report.h"
#include "ZDOffCal/ZDDataDriftFunction.h"
#include "ZDOffCal/ZDSinglelayer_df_data.h"

// STL classes
#include <vector>
#include <map>

static const char* const kFacilityString   = "ZDOffCal.ZDSinglelayer_df_data" ;

ZDSinglelayer_df_data::ZDSinglelayer_df_data()
{		 
  Tseries New_DF_Point                                       ;
  New_DF_Point.setD(0.0)                                     ;
  New_DF_Point.setV(0.0)                                     ;
  New_DF_Point.setA(0.0)                                     ;
  t2d_map[0.0] = New_DF_Point                                ; 
  Tseries New_InverseDF_Point                                ;
  New_InverseDF_Point.setD(0.0)                              ;
  New_InverseDF_Point.setV(ZDDataDriftFunction::kIllegalValue) ;
  New_InverseDF_Point.setA(0.0)                              ;
  d2t_map[0.0] = New_InverseDF_Point                         ;
  min_time  =  ZDDataDriftFunction::kminimum_time  ;
  max_time  =  ZDDataDriftFunction::kmaximum_time  ;
  max_drift =  ZDDataDriftFunction::kmaximum_drift ;
  side      =  ZDDataDriftFunction::kLRaveraged    ;
}
ZDSinglelayer_df_data::~ZDSinglelayer_df_data()
{}
