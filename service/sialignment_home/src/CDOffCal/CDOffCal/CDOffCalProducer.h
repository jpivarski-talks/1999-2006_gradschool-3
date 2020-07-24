#if !defined(PACKAGE_CDOFFCALPRODUCER_H)
#define PACKAGE_CDOFFCALPRODUCER_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      CDOffCalProducer
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Adam Lyon
// Created:     Thu Apr 16 13:04:56 EDT 1998
// $Id: CDOffCalProducer.h,v 1.11 2000/10/31 23:09:18 mclean Exp $
//
// Revision history
//
// $Log: CDOffCalProducer.h,v $
// Revision 1.11  2000/10/31 23:09:18  mclean
// *** empty log message ***
//
// Revision 1.10  2000/08/14 23:40:54  wsun
// Added parameters for signal prop. speed and to disable time shift to z=0.
//
// Revision 1.9  2000/02/15 21:32:09  mclean
// Added a parameter to set the resolution (used for the fitting weight)
// i.e.
// param CDOffCal Resolution 10.0
// sets the resolution to 10 meters.
//
// Revision 1.8  2000/02/14 23:37:13  mclean
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
// Revision 1.7  2000/02/02 15:28:23  cdj
// modernized to use ProxyFactories
//
// Revision 1.6  1999/12/15 19:41:44  mclean
// Some bug fixes, made get-parameter functions in CDOffCalProducer
// inline functions, added channel t0s to DriftFunction.h,
// added DebugLevel parameter to CDOffCalProducer.
//
// Revision 1.5  1999/12/07 23:29:33  mclean
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
// Revision 1.4  1999/06/24 20:10:24  lyon
// Change Parameter<int> -> Parameter<long> to avoid confusion with bool on Solaris
//
// Revision 1.3  1999/06/23 22:17:35  lyon
// Added parameters to choose drift functions
//
// Revision 1.2  1998/12/10 21:43:27  mclean
// Updates for HePr drift functions (old analytic DFs are still default)
//
// Revision 1.1  1998/04/24 20:34:23  mclean
// First Real Code
//
//

// user include files
#include "Processor/Processor.h"
#include "CommandPattern/Parameter.h"

// forward declarations

class CDOffCalProducer : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      CDOffCalProducer( void );                      // anal1 
      virtual ~CDOffCalProducer();                   // anal5 

      // ------------ member functions ----------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // ------------ const member functions ----------------------

      inline double getMinTightTime() const {return m_MinTightTime.value()    ;};
      inline double getMaxTightTime() const {return m_MaxTightTime.value()    ;};
      inline double getMinLooseTime() const {return m_MinLooseTime.value()    ;};
      inline double getMaxLooseTime() const {return m_MaxLooseTime.value()    ;};
      inline double getGlobalT0()    const {return m_GlobalT0.value()         ;};
      inline double getTDCQuantum()  const {return m_TDCQuantum.value()       ;};
      inline double getResolution()  const {return m_Resolution.value()       ;};
      inline double getDebugLevel()  const {return m_DebugLevel.value()       ;};
      inline long   getType()        const {return m_DriftFunctionType.value();};

      DABoolean shiftTimeToCenter() const {return m_shiftTimeToCenter.value();};
      //
      //
      // 
      void signalPropBeta_Set(double signalPropBeta) { m_signalPropBeta_RealValue = signalPropBeta; };
      double signalPropBeta_Par() const { return m_signalPropBeta.value(); };
      double signalPropBeta_Val() const { return m_signalPropBeta_RealValue; };
      
      // ------------ static member functions ---------------------

   protected:
      // ------------ protected member functions ------------------

      // ------------ protected const member functions ------------

   private:
      // ------------ Constructors and destructor -----------------
      CDOffCalProducer( const CDOffCalProducer& );

      // ------------ assignment operator(s) ----------------------
      const CDOffCalProducer& operator=( const CDOffCalProducer& );

      // ------------ private member functions --------------------
      ProxyBase* makeDriftFunctionProxy() ;

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------
      Parameter<long>      m_DriftFunctionType  ; 
      Parameter<long>      m_DebugLevel         ; 
      Parameter<double>    m_MinTightTime       ;
      Parameter<double>    m_MaxTightTime       ;
      Parameter<double>    m_MinLooseTime       ;
      Parameter<double>    m_MaxLooseTime       ;
      Parameter<double>    m_GlobalT0           ;
      Parameter<double>    m_TDCQuantum         ;
      Parameter<double>    m_Resolution         ;

      Parameter< DABoolean > m_shiftTimeToCenter ;
      Parameter< double > m_signalPropBeta ;
      double m_signalPropBeta_RealValue    ;

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* PACKAGE_CDOFFCALPRODUCER_H */
