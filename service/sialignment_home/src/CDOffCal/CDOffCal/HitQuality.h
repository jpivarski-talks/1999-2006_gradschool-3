#if !defined(CDOFFCAL_HITQUALITY_H)
#define CDOFFCAL_HITQUALITY_H
// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      HitQuality
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Ken McLean
// Created:     Tue DEC  7 14:16:47 EST 1998
// $Id: HitQuality.h,v 1.3 2000/03/03 15:17:23 lyon Exp $
//
// Revision history
//
// $Log: HitQuality.h,v $
// Revision 1.3  2000/03/03 15:17:23  lyon
// Change long to int for quality word
//
// Revision 1.2  2000/02/14 23:37:14  mclean
// Added parameters and access functions, default constants
// external name      internal name      default value (ps) access function
// MinimumTime        m_MinTightTime       -30000           getMinTightTime
// MaximumTime        m_MaxTightTime       350000           getMaxTightTime
// MinimumLooseTime   m_MinLooseTime       -50000           getMinLooseTime
// MaximumLooseTime   m_MaxLooseTime       600000           getMaxLooseTime
//
// Added new default resolution parameter (rather than weight=1/resolution**2)
// for convenience of MC.
//
// Added DriftFunction access functions for adcgain,
// adcgainRMS,tdcped,tdcpedRMS,tdcslope,tdcslopeRMS,resolution,badwiremap
//
// Added DriftFunction member data from constants:
// m_daqmap,m_quality,m_adcgain,m_adcgainRMS,m_tdcslope,m_tdcslopeRMS,m_tdcped,
// m_tdcpedRMS.
//
// HitQuality
// Added functions
//      set/test_windowNarrow,
//      set/test_windowWide,
//      set/test_windowHigh,
//      set/test_windowLow
// delete function
//      test_notinwindow
// and added/deleted corresponding static const "Flag*" bitmap variables.
//
// Revision 1.1  1999/12/07 23:33:58  mclean
// Split off HitQuality.h from DriftFunction.h
// Split off Singlelayer_df_data.h from HePrDriftFunction.h
// Added DataDriftFunction.h
//
//
class HitQuality {

public:

      // ---------- Constructors and destructor ----------------
      HitQuality();
      HitQuality(int quality);
      virtual ~HitQuality();

      // ---------- constants, enums and typedefs --------------
      static const int Flag_Bad,Flag_SingleTDC,Flag_TestChannel;
      static const int Flag_NoCharge,Flag_NoTime;
      static const int Flag_WindowNarrow,Flag_WindowWide,Flag_WindowHigh,Flag_WindowLow;

      // ---------- const member functions ---------------------
      // Hitquality test logicals
      // test if any error bit is set
      DABoolean test_all               () const;
      // test if hit is good for tracking
      DABoolean test_good              () const;
      // test if hit quality is bad (default initialization) 
      DABoolean test_bad               () const;
      // test if single tdc
      DABoolean test_singletdc         () const;
      // test if signal is from a testchannel
      DABoolean test_testchannel       () const;
      // test if there is no ADC
      DABoolean test_nocharge          () const;
      // test if there is no TDC
      DABoolean test_notime            () const;
      // test window
      DABoolean test_windowNarrow      () const;
      DABoolean test_windowWide        () const;
      DABoolean test_windowHigh        () const;
      DABoolean test_windowLow         () const;

      // set wire as good                          "bit 0"
      void set_good              () ;
      // set wire as bad (initialization default)  "bit 0"
      void set_bad               () ;
      // set singletdc                             "bit 1"
      void set_singletdc         () ;
      // set testchannel                           "bit 2"
      void set_testchannel       () ;
      // set no charge available                   "bit 3"
      void set_nocharge          () ;
      // set no time available                     "bit 4"
      void set_notime            () ;
      // set not in window                
      void set_windowNarrow      () ;//              "bit 5"
      void set_windowWide        () ;//              "bit 6"
      void set_windowHigh        () ;//              "bit 7"
      void set_windowLow         () ;//              "bit 8"

      int value() ;
   private:
   // ---------- Constructors and destructor ----------------
      HitQuality( HitQuality& );
      HitQuality& operator=( HitQuality& );
      int m_quality;
};

#endif /* CDOFFCAL_HITQUALITY_H */





