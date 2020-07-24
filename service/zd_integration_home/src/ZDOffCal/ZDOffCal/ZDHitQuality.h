#if !defined(ZDOFFCAL_HITQUALITY_H)
#define ZDOFFCAL_HITQUALITY_H
// -*- C++ -*-
//
// Package:     <ZDOffCal>
// Module:      ZDHitQuality
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDHitQuality.h,v 1.1 2003/04/24 18:08:18 huanggs Exp $
//
// Revision history
//
// $Log: ZDHitQuality.h,v $
// Revision 1.1  2003/04/24 18:08:18  huanggs
// add new files
//
// Revision 1.2  2003/04/24 14:51:58  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:58  huanggs
// imported source
//
//
class ZDHitQuality {

public:

      // ---------- Constructors and destructor ----------------
      ZDHitQuality();
      ZDHitQuality(int quality);
      virtual ~ZDHitQuality();

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
      ZDHitQuality( ZDHitQuality& );
      ZDHitQuality& operator=( ZDHitQuality& );
      int m_quality;
};

#endif /* ZDOFFCAL_HITQUALITY_H */
