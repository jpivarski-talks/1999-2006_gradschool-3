#if !defined(ZDDRIFTFUNCTION_DATADRIFTFUNCTION_H)
#define ZDDRIFTFUNCTION_DATADRIFTFUNCTION_H
// -*- C++ -*-
//
// Package:     <ZDDriftFunctionProd>
// Module:      ZDDataDriftFunction
// 
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDDataDriftFunction.h,v 1.1.1.1 2003/06/26 19:02:49 huanggs Exp $
//
// Revision history
//
// $Log: ZDDataDriftFunction.h,v $
// Revision 1.1.1.1  2003/06/26 19:02:49  huanggs
// imported ZDDriftFunctionProd source
//
// Revision 1.1  2003/04/24 18:08:16  huanggs
// add new files
//
// Revision 1.2  2003/04/24 14:51:57  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:58  huanggs
// imported source
//
//

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ZDDriftFunctionProd/ZDDriftFunction.h"

// STL classes
#include <vector>
#include <map>
#include "STLUtility/fwd_vector.h"
#include "STLUtility/fwd_map.h"


#include "ZDDriftFunctionProd/ZDSinglelayer_df_data.h"

class ZDDataDriftFunction : public ZDDriftFunction
{
public:

  // ---------- constants, enums and typedefs --------------
  static const double kIllegalValue, kminimum_time, kmaximum_time, kmaximum_drift;

  static const float kCorr_DriftMax ,
                     kCorr_TimeMax  ,
                     kCorr_EAngleMax,
                     kCorr_QchargeMax,
                     kCorr_Qquantum ;
                     
  /*
     kCorr_EAngleBins    # of 5o Entrange angle bins and extreme e-angles +-45o
     kCorr_DriftCorrBins # of >Time< Bins in >Drift< Correction (0-265ns)/5+1
     kCorr_TimeCorrBins  # of >Drift< Bins in >Time< Correction (0-7mm)/0.2+1
     kCorr_QCorrBins    
  */
  enum ArrayLengths 
  { kCorr_EAngleBins   =  19, 
    kCorr_TimeBins     =  54,
    kCorr_DriftBins    =  36,
    kCorr_QBins        =  80 } ;

  // ---------- Constructors and destructor ----------------
  ZDDataDriftFunction(ZDDriftFunctionProd* Producer, ZDDFDataHolder* iDataHolder);
  virtual ~ZDDataDriftFunction();
  
  // ---------- const member functions ---------------------
  
  ZDDriftFunction::Drift  time2Drift( 
                                     ZDDriftFunction::Time  time, 
				     int                  layer, 
				     int                  side ) const;
  
  ZDDriftFunction::Time   drift2Time( 
                                     ZDDriftFunction::Drift drift, 
                                     int                  layer, 
                                     int                  side ) const;

  ZDDriftFunction::Drift  time2DriftCorr( 
                                        ZDDriftFunction::Time  time, 
                                        int                  layer, 
                                        int              ambiguity,
                                        float               eAngle, 
                                        float                 Zhit ) const;
  
  ZDDriftFunction::Time   drift2TimeCorr( 
                                        ZDDriftFunction::Drift drift, 
                                        int                  layer, 
                                        float               eAngle, 
                                        float                 Zhit ) const;

  ZDDriftFunction::Time   charge2TimeCorr(
                                        ZDDriftFunction::Time time, 
                                        ZDDriftFunction::Charge Qcharge) const;

private:

  STL_VECTOR( STL_VECTOR ( ZDSinglelayer_df_data ) ) zd_df_data; 
  float kCorr_EAngleBinSize,kCorr_DriftBinSize,kCorr_TimeBinSize;  
  float corrDrift[kmaxZDlayer][kCorr_EAngleBins][kAmbiguities][kCorr_TimeBins] ;
  float corrTime [kmaxZDlayer][kCorr_EAngleBins][kAmbiguities][kCorr_DriftBins] ;

  // ---------- Constructors and destructor ----------------
  ZDDataDriftFunction( const ZDDataDriftFunction& );
  const ZDDataDriftFunction& operator=( const ZDDataDriftFunction& );

};

#endif /* ZDDRIFTFUNCTION_DATADRIFTFUNCTION_H */
