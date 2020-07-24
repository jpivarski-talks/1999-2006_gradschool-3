// -*- C++ -*-
//
// Package:     <ZDOffCal>
// Module:      ZDOffCalProducer
// 
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDOffCalProducer.cc,v 1.3 2003/04/24 17:35:29 huanggs Exp $
//
// Revision history
//
// $Log: ZDOffCalProducer.cc,v $
// Revision 1.3  2003/04/24 17:35:29  huanggs
// file names match class names
//
// Revision 1.2  2003/04/24 14:51:52  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:57  huanggs
// imported source
//
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
#include "ZDOffCal/ZDOffCalProducer.h"
#include "ZDOffCal/ZDDriftFunctionProxy.h"
#include "DataHandler/ProxyFactoryFunction.h"

#include <vector>
#include <map>
#include <set>

static const char* const kFacilityString = "Processor.ZDOffCalProducer" ;

//---------------------------------------------------------------------------
// CONSTRUCTOR
//
ZDOffCalProducer::ZDOffCalProducer( void ) : 
     Producer("ZDOffCalProducer" ), 
     m_DriftFunctionType(
              "DriftFunctionType",
              this,
              ZDDriftFunction::kDFType_HePr_data1),
     m_MinTightTime(
              "MinimumTime",
              this,
              ZDDriftFunction::kdefault_MinTightTime),
     m_MaxTightTime(
              "MaximumTime",
              this,
              ZDDriftFunction::kdefault_MaxTightTime),
     m_MinLooseTime(
              "MinimumLooseTime",
              this,
              ZDDriftFunction::kdefault_MinLooseTime),
     m_MaxLooseTime(
              "MaximumLooseTime",
              this,
              ZDDriftFunction::kdefault_MaxLooseTime),
     m_GlobalT0(
              "GlobalT0",
              this,
              ZDDriftFunction::kdefault_GlobalT0),
     m_TDCQuantum(
              "TDCQuantum",
              this,
              ZDDriftFunction::kdefault_TDCQuantum),
     m_Resolution(
              "Resolution",
              this,
              ZDDriftFunction::kdefault_Resolution),
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
      string("Fraction of speed of light for signals in the ZD wires\n") ) ;
  //
  // Connect to a stream to drive constructor - essential
  // 
//  registerProxyFactory( Stream::kZDCalibration,
//			new ProxyFactoryFunction<ZDOffCalProducer,
//			                         DriftFunctionProxy>
//			( *this,
//			  &ZDOffCalProducer::makeDriftFunctionProxy ),
//			UsageTag() );
  registerProxyFactory( Stream::kBeginRun,
			new ProxyFactoryFunction<ZDOffCalProducer,
                                                 ZDDriftFunctionProxy>
			( *this,
			  &ZDOffCalProducer::makeDriftFunctionProxy ),
			UsageTag() );
  
  report( DEBUG, kFacilityString ) << "end constructor" << endl; 
}
#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction<ZDOffCalProducer,ZDDriftFunctionProxy>;

//-------------------------------------------------------------------------------------------
//
// DESTRUCTOR
//
ZDOffCalProducer::~ZDOffCalProducer()
  //deallocate resources, close files, write will...
{
  report( DEBUG, kFacilityString ) << "destructor" << endl; 
}
//-------------------------------------------------------------------------------------------
//
// INIT
//
void
ZDOffCalProducer::init( void )
{
  report( DEBUG, kFacilityString ) << "init" << endl; 
}
//-------------------------------------------------------------------------------------------
//
// TERMINATE
//
void
ZDOffCalProducer::terminate( void )
{
  report( DEBUG, kFacilityString ) << "terminate" << endl; 
}
//-------------------------------------------------------------------------------------------

ProxyBase*
ZDOffCalProducer::makeDriftFunctionProxy() {
   return new ZDDriftFunctionProxy(this);
}
