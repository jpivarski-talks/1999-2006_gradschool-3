#if !defined(CDOFFCAL_DATADRIFTFUNCTION_H)
#define CDOFFCAL_DATADRIFTFUNCTION_H
// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      DataDriftFunction
// 
// Author:      Martin Lohner, Ken McLean
// Created:     Wed Mar  4 14:16:47 EST 1998
// $Id: DataDriftFunction.h,v 1.5 2000/12/01 14:40:27 urner Exp $
//
// Revision history
//
// $Log: DataDriftFunction.h,v $
// Revision 1.5  2000/12/01 14:40:27  urner
// included time2charge correction to take care of time slewing using T0global BDL at present as container for the correction
//
// Revision 1.4  2000/10/31 23:09:19  mclean
// *** empty log message ***
//
// Revision 1.3  2000/02/02 15:36:16  cdj
// now dependent on DFDataHolder instead of DriftFunctionProxy
//
// Revision 1.2  1999/12/15 19:41:45  mclean
// Some bug fixes, made get-parameter functions in CDOffCalProducer
// inline functions, added channel t0s to DriftFunction.h,
// added DebugLevel parameter to CDOffCalProducer.
//
// Revision 1.3  1999/12/07 23:29:39  mclean
// Many changes:
//    (1) Parameter data is stored locally to the Producer and accessible
//        via member functions.
//    (2) Frame data is stored locally to the Proxy and accessible via
//        member functions (i.e. Geometry, Constants)
//    (3) Includes have been streamlined to speed up makedepend
//    (4) New parameters have been instituted for
//                 GlobalT0     (Global T0 shift)
//                 TDCQuantum   (TDC slope, 250. MC, -500. data)
//                 Mintime      (in normal time sense, earliest allowed hit)
//                 Maxtime      (in normal time sense, latest allowed hit)
//    (5) The following masks are overlayed in the "goodness"
//        parameter returned to makeCalibratedHit:
//                Flag_Bad         =  1;
//                Flag_SingleTDC   =  2;
//                Flag_TestChannel =  4;
//                Flag_NoCharge    =  8;
//                Flag_NoTime      = 16;
//                Flag_NotInWindow = 32;
//    (6) Class HitQuality has its own file and additional functionality
//        to set_ and test_ all the above qualities.
//    (7) Provision for access to all current DR constant entities
//        has been made for in the Proxy.
//    (8) There is an additional Class inheriting from DriftFunction:
//        DataDriftFunction. This will use real Frame-based constants
//        eventually.
//
// Revision 1.2  1999/11/26 22:11:44  mclean
// Moved constants from parent DriftFunction Class to derived class
// they are actually used in.
//
// Revision 1.1  1999/06/23 22:16:54  lyon
// Specific drift functions
//
// Revision 1.21  1999/03/05 20:57:37  pg
// Switched to using STL_XXX macros (was crashing on Solaris). Got rid of delete calls in DriftFunction.cc (same reason)
//
// Revision 1.20  1998/12/15 21:03:30  mclean
// First working Data DF code (hardwired, tcl parameters not enabled, not for
//                             general use)
//
// Revision 1.19  1998/12/10 21:43:28  mclean
// Updates for Data drift functions (old analytic DFs are still default)
//
// Revision 1.18  1998/11/27 20:23:51  mclean
// Introduce STL definition conventions.
//
// Revision 1.17  1998/11/26 14:04:50  mclean
// Explicitly set CPP preprocessor flag to disable "Data" drift functions
// (these can not be default until we modify cg3/cdoffcal, I will install
// a tcl toggle by that time)
//
// Revision 1.16  1998/11/25 23:07:14  lyon
// Use STL macros
//
// Revision 1.15  1998/11/19 19:16:15  mclean
// Preliminary changes for Data drift functions...dummied out in this
// file with C-preprocessor statements.
//
// Revision 1.12  1998/10/07 21:28:46  mclean
// Modified wire number to remove non-sense wires :)
//
// Revision 1.11  1998/09/21 00:57:04  lyon
// Changed location of Tseries
//
// Revision 1.10  1998/09/10 15:12:19  lyon
// Added getCalibratedDRSeries
//
// Revision 1.24  1999/11/26 22:11:43  mclean
// Moved constants from parent DriftFunction Class to derived class
// they are actually used in.
//
// Revision 1.22  1999/06/23 22:17:15  lyon
// DriftFunction is now a base class
//

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "CDOffCal/DriftFunction.h"

// STL classes
#include <vector>
#include <map>
#include "STLUtility/fwd_vector.h"
#include "STLUtility/fwd_map.h"


#include "CDOffCal/Singlelayer_df_data.h"

class DataDriftFunction : public DriftFunction
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
  DataDriftFunction(CDOffCalProducer* Producer, DFDataHolder* iDataHolder);
  virtual ~DataDriftFunction();
  
  // ---------- const member functions ---------------------
  
  DriftFunction::Drift  time2Drift( 
                                     DriftFunction::Time  time, 
				     int                  layer, 
				     int                  side ) const;
  
  DriftFunction::Time   drift2Time( 
                                     DriftFunction::Drift drift, 
                                     int                  layer, 
                                     int                  side ) const;

  DriftFunction::Drift  time2DriftCorr( 
                                        DriftFunction::Time  time, 
                                        int                  layer, 
                                        int              ambiguity,
                                        float               eAngle, 
                                        float                 Zhit ) const;
  
  DriftFunction::Time   drift2TimeCorr( 
                                        DriftFunction::Drift drift, 
                                        int                  layer, 
                                        float               eAngle, 
                                        float                 Zhit ) const;

  DriftFunction::Time   charge2TimeCorr(
                                        DriftFunction::Time time, 
                                        DriftFunction::Charge Qcharge) const;

private:

  STL_VECTOR( STL_VECTOR ( Singlelayer_df_data ) ) drIII_df_data; 
  float kCorr_EAngleBinSize,kCorr_DriftBinSize,kCorr_TimeBinSize;  
  float corrDrift[kmaxDRlayer][kCorr_EAngleBins][kAmbiguities][kCorr_TimeBins] ;
  float corrTime [kmaxDRlayer][kCorr_EAngleBins][kAmbiguities][kCorr_DriftBins] ;

  // ---------- Constructors and destructor ----------------
  DataDriftFunction( const DataDriftFunction& );
  const DataDriftFunction& operator=( const DataDriftFunction& );

};

#endif /* CDOFFCAL_DATADRIFTFUNCTION_H */





