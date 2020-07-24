// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      CDOffCalProducer
// 
// $Log: CDOffCalProducer.cc,v $
// Revision 1.17  2001/03/28 17:51:52  ajm36
// no longer register DriftFunctionProxy to drcalibration stream
//
// Revision 1.16  2000/10/31 23:09:28  mclean
// *** empty log message ***
//
// Revision 1.15  2000/08/14 23:40:50  wsun
// Added parameters for signal prop. speed and to disable time shift to z=0.
//
// Revision 1.14  2000/04/24 23:33:53  mclean
// In Singlelayer_df_data.cc
//   - Added a constructor and destructor, default values are set and a single
//     point is added to the drift function (0,0)
//   - DataDriftFunction.cc's constructor appends points, so this cannot be
//     initialized in one operation.
//   - Currently member data is accessed directly I will change this soon
// In DriftFunction.cc:
//    - default TDCQuantum -500ns/count
//    - default DriftFunctionType 2 (external constants)
//    - improved some error messages
//    - set tdc2time/time2tdc return value to zero (was undefined) if tdc slope
//      is zero and print error message.
// For DataDriftFunction.cc
//    - trivial change
//
// Revision 1.13  2000/02/15 21:32:25  mclean
// Added a parameter to set the resolution (used for the fitting weight)
// i.e.
// param CDOffCal Resolution 10.0
// sets the resolution to 10 meters.
//
// Revision 1.12  2000/02/14 23:49:02  mclean
// Added DPP Logic
// - take first hit from tight window, else
// - take first hit from wide window, else
// - take first hit from high window, else
// - take kast hit fron low window
// Where first,last are ordered as normal clock time...(increasing entropy).
//
// Added resolution(layer,drift) function for MC (just returns 123micron
// at present).
//
// Implemented ADCgain, TDCslope constants in code.
//
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
// Revision 1.11  2000/02/02 15:28:51  cdj
// modernized to use ProxyFactories
//
// Revision 1.10  1999/12/15 19:48:50  mclean
// Some bug fixes, made get-parameter functions in CDOffCalProducer
// inline functions, added channel t0s to DriftFunction.h,
// added DebugLevel parameter to CDOffCalProducer.
//
// DriftFunction Constructor reads in the channel t0s from a file
// channelT0s.v01 in the current (suez executing) directory (this is
// just a set of 9796 ordered pairs (wire#,T0).
// If this file is not found, then they are set t0 0. These add to the
// GlobalT0 that can be set by parameter (default 0). NOTE all
// times are in PICOSECONDS.
//
// There is now a DebugLevel Parameter.
//  0 means do nothing (currently this doesnt work, does same as 1)
//  1 means count various failure and success modes
//  2 means dump info from makeCalibratedDRHit once per hit
//  3 means dump raw DF info (only for HePrDriftFunction and DataDriftFunction)
//  4 means dump DF map info (only for HePrDriftFunction and DataDriftFunction)
// These decisions filter out most of the report(DEBUG messages, a few
// remain but should be executed at about the same rate that new calibration
// constants appear in the input streams.
//
// The parameters (even things needed per hit like GlobalT0 and TDCQuantum)
// are accessed by inline function calls to member data of CDOffCalProducer.
// In the future it might be better to force the DriftFunction (i.e. entire
// DR Calibration object) to be deleted and remade when a parameter changes,
// or to pass the data into the DriftFunction object from the
// init() function call in CDOffCalProducer.
//
// Revision 1.9  1999/12/15 08:07:07  mclean
// Additional debug trace statements in CDOffCalProducer/DriftFunctionProxy.
//
// DriftFunctionProxy also had one small bug fixed in the constructor,
// large swathes of code are also preprocessed out via the REALCONSTANTS
// flag on a temporary bases.
//
// In HePrDriftFunction.cc I massaged the report DEBUG statements to look
// a bit nicer, they trace the entire history of the DriftFunction constants
// objects construction, and then dump it again.. you will not enjoy
// "report level debug". I will AND this with an internal debug parameter
// flag soon, really, trust me.
//
// In Statistics.cc, instead of doing an "assert" when someone passes
// it an illegal index (i.e. books a counter out of the preallocated
// memory space ("just so Fortran-Passe' you say")) I just dont do
// anything. Seems civilized.
//
// Revision 1.8  1999/12/07 23:30:05  mclean
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
// Revision 1.7  1999/06/23 22:21:23  lyon
// Altered to handle drift function selection
//
// Revision 1.6  1998/12/15 21:03:45  mclean
// First working HePr DF code (hardwired, tcl parameters not enabled, not for
//                             general use)
//
// Revision 1.5  1998/12/10 21:43:49  mclean
// Updates for HePr drift functions (old analytic DFs are still default)
//
// Revision 1.4  1998/09/10 15:17:46  lyon
// DriftFunction goes in BeginRun stream, not Event stream
//
// Revision 1.1  1998/04/24 20:32:01  mclean
// First Real CDOffCal Code
//

#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif             
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "CDOffCal/CDOffCalProducer.h"
#include "CDOffCal/DriftFunctionProxy.h"
#include "DataHandler/ProxyFactoryFunction.h"

#include <vector>
#include <map>
#include <set>

static const char* const kFacilityString = "Processor.CDOffCalProducer" ;

//---------------------------------------------------------------------------
// CONSTRUCTOR
//
CDOffCalProducer::CDOffCalProducer( void ) : 
     Producer("CDOffCalProducer" ), 
     m_DriftFunctionType(
              "DriftFunctionType",
              this,
              DriftFunction::kDFType_HePr_data1),
     m_MinTightTime(
              "MinimumTime",
              this,
              DriftFunction::kdefault_MinTightTime),
     m_MaxTightTime(
              "MaximumTime",
              this,
              DriftFunction::kdefault_MaxTightTime),
     m_MinLooseTime(
              "MinimumLooseTime",
              this,
              DriftFunction::kdefault_MinLooseTime),
     m_MaxLooseTime(
              "MaximumLooseTime",
              this,
              DriftFunction::kdefault_MaxLooseTime),
     m_GlobalT0(
              "GlobalT0",
              this,
              DriftFunction::kdefault_GlobalT0),
     m_TDCQuantum(
              "TDCQuantum",
              this,
              DriftFunction::kdefault_TDCQuantum),
     m_Resolution(
              "Resolution",
              this,
              DriftFunction::kdefault_Resolution),
     m_DebugLevel(
              "DebugLevel",
              this,0),
     m_shiftTimeToCenter( "ShiftTimeToCenter", this, true ),
     m_signalPropBeta(    "SignalPropBeta", this, -0.5 )
{ 
  report( DEBUG, kFacilityString ) << "start constructor" << endl; 
  m_DriftFunctionType.setHelpString(
  "=0 (old Naive drift functions), =1 (He/Pr drift functions)\n");
  m_MinTightTime.setHelpString(
  "low edge of narrow time window (with normal time orientation, i.e negative TDC slope, high TDC");
  m_MaxTightTime.setHelpString(
  "=high edge of narrow time window (with normal time orientation, i.e negative TDC slope, low TDC");
  m_MinLooseTime.setHelpString(
  "low edge of wide time window (with normal time orientation, i.e negative TDC slope, high TDC");
  m_MaxLooseTime.setHelpString(
  "=high edge of wide time window (with normal time orientation, i.e negative TDC slope, low TDC");
  m_GlobalT0.setHelpString  ("Global T0   : TIME=(GLOBALT0+TDCQuantum*TDC) ");
  m_TDCQuantum.setHelpString("TDCQuantum  : TIME=(GLOBALT0+TDCQuantum*TDC) ");
  m_Resolution.setHelpString("Resolution  : in meters                      ");
  m_DebugLevel.setHelpString("DebugLevel  : 0-none, 1-counters, 2-hit level, 3-constuctor input data, 4-constructor map dump");
   m_shiftTimeToCenter.setHelpString(
      string("Set to true (default) to shift drift times to z=0 BEFORE\n") +
      string("converting it to a drift distance." ) ) ;
   m_signalPropBeta.setHelpString(
      string("Fraction of speed of light for signals in the DR wires\n") ) ;
  //
  // Connect to a stream to drive constructor - essential
  // 
//  registerProxyFactory( Stream::kDRCalibration,
//			new ProxyFactoryFunction<CDOffCalProducer,
//			                         DriftFunctionProxy>
//			( *this,
//			  &CDOffCalProducer::makeDriftFunctionProxy ),
//			UsageTag() );
  registerProxyFactory( Stream::kBeginRun,
			new ProxyFactoryFunction<CDOffCalProducer,
                                                 DriftFunctionProxy>
			( *this,
			  &CDOffCalProducer::makeDriftFunctionProxy ),
			UsageTag() );
  
  report( DEBUG, kFacilityString ) << "end constructor" << endl; 
}
#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction<CDOffCalProducer,DriftFunctionProxy>;

//-------------------------------------------------------------------------------------------
//
// DESTRUCTOR
//
CDOffCalProducer::~CDOffCalProducer()
  //deallocate resources, close files, write will...
{
  report( DEBUG, kFacilityString ) << "destructor" << endl; 
}
//-------------------------------------------------------------------------------------------
//
// INIT
//
void
CDOffCalProducer::init( void )
{
  report( DEBUG, kFacilityString ) << "init" << endl; 
}
//-------------------------------------------------------------------------------------------
//
// TERMINATE
//
void
CDOffCalProducer::terminate( void )
{
  report( DEBUG, kFacilityString ) << "terminate" << endl; 
}
//-------------------------------------------------------------------------------------------

ProxyBase*
CDOffCalProducer::makeDriftFunctionProxy() {
   return new DriftFunctionProxy(this);
}
