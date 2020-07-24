#if !defined(ZDMCUTIL_ZDMCDriftFB_H)
#define ZDMCUTIL_ZDMCDriftFB_H
// -*- C++ -*-
//
// Package:     <ZDMCUtil>
// Module:      ZDMCDriftFB
// 
// Description: Frame Broker object for ZD MC access to ZDDriftFunctionProd.
//              (drift2time, time2tdc, charge2adc relations)
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBZDMCDrift m_calibration
//
//  Need to initialize object with object extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The functions zdmc_Drift2Time_ 
//                zdmc_Time2tdc_  
//                zdmc_Charge2adc_ 
//  provide Fortran access to member functions of ZDDriftFunctionProd::DriftFunction
//
//  The Fortran functions
//         TestZDMCConstantsProc/Fortran/test_Drift.F 
//         TestZDMCConstantsProc/Fortran/test_Time.F 
//         TestZDMCConstantsProc/Fortran/test_Charge.F 
//  prints out sample points on the drift function
//
//  The TestZDMCConstantsProc Suez processor initializes the ZDMCxxxxFB
//  objects and then calls Fortran subroutines to test all of the constants
//
// Author:      Guangshun Huang
// Created:     Tue Dec 17, 2002
//
// Revision history
//

// system include files

// user include files
#include "ZDDriftFunctionProd/ZDDriftFunction.h"
#include "FrameAccess/FAConstants.h"

class ZDMCDriftFB
{
   public:

      // member functions
      //     Initialize the object
      void setDrift( FAItem< ZDDriftFunction     > calibration ,
                     FAItem< AZDSenseWireStore > geometry    );
      
      FAItem< ZDDriftFunction     > getCalibration ();
      FAItem< AZDSenseWireStore > getGeometry    ();
      
      // static member functions
      static ZDMCDriftFB* instance();
      static void destroy();

   protected:
      // Constructors and destructor
      // protected member functions
      ZDMCDriftFB();
      virtual ~ZDMCDriftFB();

   private:
      // Constructors and destructor
      ZDMCDriftFB( const ZDMCDriftFB& );  // stop default

      // assignment operator(s)
      const ZDMCDriftFB& operator=( const ZDMCDriftFB& );  // stop default

      // data members
      FAItem< ZDDriftFunction     > m_calibration ;
      FAItem< AZDSenseWireStore > m_geometry    ;
      
      // static data members
      static ZDMCDriftFB* m_instance;
};
#endif /* ZDMCUTIL_ZDMCDriftFB_H */
