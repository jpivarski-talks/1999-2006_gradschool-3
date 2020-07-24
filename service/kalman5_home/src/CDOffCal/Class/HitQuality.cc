// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      DriftFunction
// 
// Description: This is the root class for DriftFunction classes
//
// Author:      Ken McLean
// Created:     Tue Dec  7 14:16:52 EST 1998
// $Id: HitQuality.cc,v 1.5 2000/03/03 15:17:38 lyon Exp $
//
// Revision history
//
// $Log: HitQuality.cc,v $
// Revision 1.5  2000/03/03 15:17:38  lyon
// Change lng to int for quality word
//
// Revision 1.4  2000/02/14 23:49:04  mclean
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
// Revision 1.3  2000/02/01 23:03:18  mclean
// This update introduces constants from the frame, i.e. from the official
// database or from CDJ FileSourceFormat ascii files as the user chooses.
//
// Guang Pei Chen also found a bug I wrote into HitQuality.cc, oops.
//
// I will add tcl scripts to the SuezScripts area as an example of each
// type of access, CDOffCal_dbase_v00.tcl CDOffCal_file_v00.tcl.
//
// I will not make loading constants from the frame default until there
// exists valid data-derived entities for all constants extracted from
// the database.
//
// The user can control where the constants come from (and other CDOffCal
// parameters) via the suez command, as reviewed below:
//   param CDOffCal DriftFunctionLevel 2
//      - This extracts constants from the frame
//      - The following BDL is expected to have existing constants,
//        either from the  data base or from ascii/FSF files :
//          DBDRadc2charge
// 	 DBDRtdc2time
// 	 DBDRDaqCellIDMap
// 	 DBDRGarfield_DistanceCorrection
// 	 DBDRGarfield_TimeCorrection
// 	 DBDRWireQuality
// 	 DBDRt0card
// 	 DBDRt0crate
//        **DBDRDriftData
// 	*DBDRadcpedestal
//        **DBDRt0channel
//        FileSourceFormat files exist for all the above in $C3_LIB/data/
//        and are loaded in /nfs/c3db/Offline/constants/Codi
//
//        Only those with (*,**) are used by this release. The others will
//        be implemented when nontrivial values are ready (this way people
//        will have to change their tcl scripts less often). Those with
//        ** are automatically used by "suez -cleog3", the adcpedestals require
//        an additional change to DRMCUtil.
//
//        Note that the crate/card t0s are summed into the channel value
//        currently.
//
//        The current default constants in FSF ascii format are in:
//                   $C3_LIB/data/test.drdriftdata
//                   $C3_LIB/data/test.drt0channel
//                   $C3_LIB/data/test.dradcpedestal
//        these correspond to that hard coded for DriftFunctionType 0 (see below)
//        and cleog3 old MC - this will change, old cleog3 MC will only
//        be supported for DriftFunctionLevel 0.
//      - Currently TDCQuantum, GlobalT0 parameters are still used.
//        TDCQuantum will become invalid when DBDRtdc2time constants are
//        available.
//
//   param CDOffCal DriftFunctionLevel 0
//      - use this for old cleog3 MC, otherwise obsolete
//      - default, this is the calibration corresponding to the old MonteCarlo
//                 TDC Quantum   +250ns/count
//                 Tzero         0ns
//                 ADC Quantum   1/count
//                 ADC pedestal  0
//                 DriftFunction hard coded (LKG)
//      - the following command overrides the TDC Quantum:
//           param CDOffCal TDCQuantum -500
//      - the following command -adds- a global T0 (picoseconds) to all channels:
//           param CDOffCal GlobalT0 5000.
//      - for real data with no available calibration one might
//        want to set TDCQuantum 0 and GlobalT0 to the middle
//        of the valid range for times (see other parameters below).
//      - if one wants to use a different t0 for individual channels
//        supply a file channelT0s.v01 in the same directory as you
//        run suez from, it is a list of ordered pairs (channel,tzero)
//        if this does not exist the t0s are set to 0+GlobalT0
//      - note that none of these options affect the old MC simulation
//        but they will affect suez -cleog.
//      - ADC pedestals for individual channels are read from channelPeds.v01
//        if the file does not exist they are set to zero
//   param CDOffCal DriftFunctionLevel 1
//      - this option should be used only for people making drift functions
//        and is essentially obsolete with this release
//      - same as level 0 except it uses the real HePr drift function
//        code from file hep_drift.v01 that must exist in the directory
//        you run suez from
//
// Details of changes:
//   DataDriftFunction.cc
//     - first nontrivial version, replaced file access by access
//       to constants from the frame as member functions of DriftFunctionProxy,
//       extracted before the DriftFunction is instantiated by the
//       faultHandler.
//     - Changed behaviour of DebugLevel parameter:
//          1   - nothing happens
//          >1  - information on every hit
//          =3  - dumps drift function information during constants unpack
//          =4  - dumps drift map information after constants unpack
//          =5  - dumps tzero,adc constants after constants unpack
//   DriftFunction.cc
//     - added adcpedestal(),adcpedestalRMS(),tdctzero() mainly
//       for use of constants external to CDOffCal (e.g. DRMCUtil)
//     - added unpack of t0s and adcpedestals for DriftFunctionType
//       kDFType_HePr_data1. Warnings are printed if constants
//       are not available for every channel.
//   DriftFunctionProxy.cc
//     All the BDL/constants objects described above are extracted
//     and stored as member data for access by the calibration,
//     but only for DriftFunctionType kDFType_HePr_data1 (behaviour
//     is not changed for other settings)
//   HePrDriftFunction.cc
//     Changes to DebugLevel parameter behaviour for DataDriftFunction.cc
//     are mirroed here.
//   HitQuality.cc
//     Guang Pei Chen corrected some of my miscoding, on evaluating expressions
//     of the type a & b == c I needed to add some brackets ( a & b ) == c
//
// Revision 1.2  1999/12/15 19:48:54  mclean
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
// Revision 1.1  1999/12/07 23:36:54  mclean
// Split off HitQuality.cc from DriftFunction.cc
// Added DataDriftFunction.cc (Derived from HePrDriftFunction.cc, major
// change is that the constructor will attempt to get all data from the
// frame instead of a private format ascii file. Since all other code overlaps
// with HePrDriftFunction, it will be better to rearrange the inheritance:
//
// DriftFunction ----> NaiveDriftFunction
//               |
//               ----> HePrDriftFunction -> FrameDriftFunction
//                                       |
//                                       -> FileDriftFunction
// Someday soon.
//

// compiler bug flags
#include "Experiment/Experiment.h"

// compiler bug fixes 
#include "CDOffCal/HitQuality.h"

static const char* const kFacilityString   = "CDOffCal.HitQuality" ;

const int   HitQuality::Flag_Bad          =   1;
const int   HitQuality::Flag_SingleTDC    =   2;
const int   HitQuality::Flag_TestChannel  =   4;
const int   HitQuality::Flag_NoCharge     =   8;
const int   HitQuality::Flag_NoTime       =  16;
const int   HitQuality::Flag_WindowNarrow =  32;
const int   HitQuality::Flag_WindowWide   =  64;
const int   HitQuality::Flag_WindowHigh   = 128;
const int   HitQuality::Flag_WindowLow    = 256;

HitQuality::HitQuality(){m_quality = Flag_Bad;}
HitQuality::HitQuality(int value): m_quality(value){}
HitQuality::~HitQuality(){}

DABoolean HitQuality::test_all() const
{
  if ( m_quality != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean HitQuality::test_good() const
{
  if ( ( m_quality & Flag_Bad ) == 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean HitQuality::test_bad() const
{
  if ( ( m_quality & Flag_Bad ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean HitQuality::test_singletdc() const
{
  if ( ( m_quality & Flag_SingleTDC ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean HitQuality::test_testchannel() const
{
  if ( ( m_quality & Flag_TestChannel ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean HitQuality::test_nocharge() const
{
  if ( ( m_quality & Flag_NoCharge ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean HitQuality::test_notime() const
{
  if ( ( m_quality & Flag_NoTime ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean HitQuality::test_windowNarrow() const
{
  if ( ( m_quality & Flag_WindowNarrow ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean HitQuality::test_windowWide() const
{
  if ( ( m_quality & Flag_WindowWide ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean HitQuality::test_windowHigh() const
{
  if ( ( m_quality & Flag_WindowHigh ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean HitQuality::test_windowLow() const
{
  if ( ( m_quality & Flag_WindowLow ) != 0 ) 
    { return true; }
  else
    { return false; }
}

void HitQuality::set_good()
{
  // clear 1st bit "and (compliment 0 exclusive or 1)"
  m_quality = m_quality & ( (~0) ^ 1 ) ;
}
void HitQuality::set_bad()
{
  m_quality = m_quality | Flag_Bad ;
}
void HitQuality::set_singletdc()
{
  m_quality = m_quality | Flag_SingleTDC ;
}
void HitQuality::set_testchannel()
{
  m_quality = m_quality | Flag_TestChannel ;
}
void HitQuality::set_nocharge()
{
  m_quality = m_quality | Flag_NoCharge ;
}
void HitQuality::set_notime()
{
  m_quality = m_quality | Flag_NoTime ;
}
void HitQuality::set_windowNarrow()
{
  m_quality = m_quality | Flag_WindowNarrow ;
}
void HitQuality::set_windowWide()
{
  m_quality = m_quality | Flag_WindowWide ;
}
void HitQuality::set_windowHigh()
{
  m_quality = m_quality | Flag_WindowHigh ;
}
void HitQuality::set_windowLow()
{
  m_quality = m_quality | Flag_WindowLow ;
}
int HitQuality::value()
{
  return m_quality ;
}





