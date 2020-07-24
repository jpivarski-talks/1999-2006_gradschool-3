#if !defined(DRMCUTIL_DRMCDriftFB_H)
#define DRMCUTIL_DRMCDriftFB_H
// -*- C++ -*-
//
// Package:     <DRMCUtil>
// Module:      DRMCDriftFB
// 
// Description: Frame Broker object for DR MC access to CDOffCal.
//              (drift2time, time2tdc, charge2adc relations)
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Usage:
//  This is a Singleton pattern (_Design_Patterns_, p.127).
//  The unique instance holds smart pointer to DBDRMCDrift m_calibration
//
//  Need to initialize object with object extracted from the Frame
//  via setXXX( XXX ) function.
//
//  The functions drmc_Drift2Time_ 
//                drmc_Time2tdc_  
//                drmc_Charge2adc_ 
//  provide Fortran access to member functions of CDOffCal::DriftFunction
//
//  The Fortran functions
//         TestDRMCConstantsProc/Fortran/test_Drift.F 
//         TestDRMCConstantsProc/Fortran/test_Time.F 
//         TestDRMCConstantsProc/Fortran/test_Charge.F 
//  prints out sample points on the drift function
//
//  The TestDRMCConstantsProc Suez processor initializes the DRMCxxxxFB
//  objects and then calls Fortran subroutines to test all of the constants
//
// Author:      K W McLean
// Created:     Thu Sep 30 15:11:32 EDT 1999
// $Id: DRMCDriftFB.h,v 1.1 1999/09/30 22:36:14 mclean Exp $
//
// Revision history
//

// system include files

// user include files
#include "CDOffCal/DriftFunction.h"
#include "FrameAccess/FAConstants.h"

class DRMCDriftFB
{
   public:

      // member functions
      //     Initialize the object
      void setDrift( FAItem< DriftFunction     > calibration ,
                     FAItem< ADRSenseWireStore > geometry    );
      
      FAItem< DriftFunction     > getCalibration ();
      FAItem< ADRSenseWireStore > getGeometry    ();
      
      // static member functions
      static DRMCDriftFB* instance();
      static void destroy();

   protected:
      // Constructors and destructor
      // protected member functions
      DRMCDriftFB();
      virtual ~DRMCDriftFB();

   private:
      // Constructors and destructor
      DRMCDriftFB( const DRMCDriftFB& );  // stop default

      // assignment operator(s)
      const DRMCDriftFB& operator=( const DRMCDriftFB& );  // stop default

      // data members
      FAItem< DriftFunction     > m_calibration ;
      FAItem< ADRSenseWireStore > m_geometry    ;
      
      // static data members
      static DRMCDriftFB* m_instance;
};
#endif /* DRMCUTIL_DRMCDriftFB_H */
