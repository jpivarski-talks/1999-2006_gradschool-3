// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDSinglelayer_df_data.h,v 1.1 2003/04/24 18:08:18 huanggs Exp $
//
// Revision history
//
// $Log: ZDSinglelayer_df_data.h,v $
// Revision 1.1  2003/04/24 18:08:18  huanggs
// add new files
//
// Revision 1.2  2003/04/24 14:51:58  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:58  huanggs
// imported source
//
#if !defined(ZDSINGLELAYER_DF_DATA_H)
#define      ZDSINGLELAYER_DF_DATA_H
class ZDSinglelayer_df_data
{
public:
      ZDSinglelayer_df_data();
      ~ZDSinglelayer_df_data();
      typedef double  Drift;       // Drift distance in meters
      typedef double  Time;        // Drift Time in picoseconds
      STL_MAP(ZDDriftFunction::Time, Tseries) t2d_map;
      STL_MAP(ZDDriftFunction::Drift, Tseries) d2t_map;
      Time  max_time  ; 
      Time  min_time  ; 
      Drift max_drift ; 
      int   side      ;   
};
#endif 
