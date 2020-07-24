#if !defined(SINGLELAYER_DF_DATA_H)
#define      SINGLELAYER_DF_DATA_H
class Singlelayer_df_data
{
public:
      Singlelayer_df_data();
      ~Singlelayer_df_data();
      typedef double  Drift;       // Drift distance in meters
      typedef double  Time;        // Drift Time in picoseconds
      STL_MAP(DriftFunction::Time, Tseries) t2d_map;
      STL_MAP(DriftFunction::Drift, Tseries) d2t_map;
      Time  max_time  ; 
      Time  min_time  ; 
      Drift max_drift ; 
      int   side      ;   
};
#endif 
