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
#include "CDOffCal/DataDriftFunction.h"
#include "CDOffCal/Singlelayer_df_data.h"

// STL classes
#include <vector>
#include <map>

static const char* const kFacilityString   = "CDOffCal.Singlelayer_df_data" ;

Singlelayer_df_data::Singlelayer_df_data()
{		 
  Tseries New_DF_Point                                       ;
  New_DF_Point.setD(0.0)                                     ;
  New_DF_Point.setV(0.0)                                     ;
  New_DF_Point.setA(0.0)                                     ;
  t2d_map[0.0] = New_DF_Point                                ; 
  Tseries New_InverseDF_Point                                ;
  New_InverseDF_Point.setD(0.0)                              ;
  New_InverseDF_Point.setV(DataDriftFunction::kIllegalValue) ;
  New_InverseDF_Point.setA(0.0)                              ;
  d2t_map[0.0] = New_InverseDF_Point                         ;
  min_time  =  DataDriftFunction::kminimum_time  ;
  max_time  =  DataDriftFunction::kmaximum_time  ;
  max_drift =  DataDriftFunction::kmaximum_drift ;
  side      =  DataDriftFunction::kLRaveraged    ;
}
Singlelayer_df_data::~Singlelayer_df_data()
{}




