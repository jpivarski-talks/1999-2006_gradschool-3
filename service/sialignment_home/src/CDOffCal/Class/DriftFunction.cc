// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      DriftFunction
// 
// Description: This is the root class for DriftFunction classes
//
// Author:      Martin Lohner, Ken McLean, Adam Lyon
// Created:     Wed Mar  4 14:16:52 EST 1998
// $Id: DriftFunction.cc,v 1.65 2002/02/17 17:26:56 wsun Exp $
//
// Revision history
//
// $Log: DriftFunction.cc,v $
// Revision 1.65  2002/02/17 17:26:56  wsun
// Protect against crossing array boundary in phi fitting weight interpolation.
//
// Revision 1.64  2001/10/06 18:49:35  urner
// changed sign of entrance angle correction
//
// Revision 1.63  2000/12/21 19:41:26  urner
// fixed all comparision to varibale magic(double!) which were set with memset by comparing to a double which was filled using memset too. If comparision is true (some mistake in the constants file occourd) I inserted an assert.
//
// Revision 1.62  2000/12/08 20:45:40  mclean
// Fixed scale problem in fitting weight / resolution drift extrapolation
//
// Revision 1.61  2000/12/03 02:50:38  cdj
// put back DriftFunction::time2tdc used by MCRawDataProd
//
// Revision 1.60  2000/12/01 19:56:54  mclean
// Changed Zcorr to be dependent on layer and drift ( to account for
// readout end )
//
// Revision 1.59  2000/12/01 14:40:30  urner
// included time2charge correction to take care of time slewing using T0global BDL at present as container for the correction
//
// Revision 1.58  2000/11/14 02:36:53  mclean
// Well, I was confused, the parity/symmetry/timelike/spacelike nature
// of the correction was fine, the theory now is the overall sign of
// the damned thing is reversed. Now I flee the country.
//
// Revision 1.57  2000/11/14 00:52:58  mclean
// Corrected entrance angle correction from timelike to spacelike (the
// Garfield data has a sign convention where the correction for ambiguity<0 is the
// opposite sign than the correcton for ambiguity >0, i.e. the correction
// is meant to be added to the unsigned drift, whereas DriftFunction.cc was
// adding it to the signed drift).
//
// Revision 1.56  2000/10/31 23:09:30  mclean
// *** empty log message ***
//
// Revision 1.55  2000/08/15 21:44:43  wsun
// Was using wrong pointer for ADRSenseWireStore.
//
// Revision 1.54  2000/08/14 23:39:52  wsun
// Shift drift time to z=0 before converting to drift distance.
//
// Revision 1.53  2000/08/04 22:34:10  mclean
// Fixed bad logic in timing-window hierarchy decision
//
// Revision 1.52  2000/07/27 16:25:09  lyon
// Fix early/late time determination -- programming error always caused lateGap == 0
//
// Revision 1.51  2000/07/03 02:31:27  lkg
// Fix type in protection against 0 gains: m_adcgain[i] == 1. -> m_adcgain[i] = 1.
//
// Revision 1.50  2000/05/17 21:58:58  urner
// supports new t0 structure
//
// Revision 1.49  2000/05/12 16:34:04  mclean
// Added t0global constant.
//
// Revision 1.48  2000/04/27 20:40:40  mclean
// The BDL of the drdriftdata object followed the CLEO II/II.V convention:
//    record#  timevalue driftvalue layer*10000000+type*1000+point
// however the last record was mistyped float instead of integer.
//
// One cannot conveniently change BDL.
//
// On writing data into the database rounding corrupts the "point" field
// since the value is never actually used we thought this was OK. However,
// on writing data from the database to ascii files and then reading it back
// in, different rounding occurs. I.E. if everything was float "type" would be
// 1+epsilon (normal drift function type), instead it is 1-epsilon which integer
// truncates to 0 (undefined drift function type, skipped in unpack).
//
// So this resulted in drift functions beyond layer 26 not being unpacked
// as they had type "0" which is not used.
//
// The fix is just not to test on the "type" field, we will probably
// not use the various subclasses of drift functions needed for DR II.
//
// Revision 1.47  2000/04/24 23:33:56  mclean
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
// Revision 1.46  2000/03/29 22:33:12  mclean
// Added hooks for drt0channelv2 and drt0hvcard
//
// Revision 1.45  2000/03/13 20:33:32  mclean
// Commented out all m_statistics->counter() quality statistics monitoring
// calls. I will enable these again when I have time to implement CDJ's
// suggestions about speeding up the use of string variables.
//
// Revision 1.44  2000/03/12 01:35:13  mclean
// Removed obsolete calls to m_statistics.counter() because
// of unreasonable CPU overhead.
//
// Revision 1.43  2000/03/07 22:27:23  mclean
// Added function adcwithped, just subtracts ped from adc, no
// gain correction (which is included properly in adc2Charge).
//
// Revision 1.42  2000/03/03 15:17:33  lyon
// Change lng to int for quality word
//
// Revision 1.41  2000/02/18 00:13:51  mclean
// I recently added many channel-referenced arrays for constants like
// t0 and tdcslope, these are initialized internally for DriftFunctionType 0/1
// and loaded from external constants for DriftFunctionType 2
//
// There was some ill thought out initialization that introduced a bug
// which set the tdcslope to 1 and prevented parameter override of this
// value thru TDCQuantum.
//
// Now the t0/tdcslope arrays are initialized to the parameter values
// of GlobalT0 and TDCQuantum for DriftFunctionTypes 0/1.
//
// For all DriftFunctionTypes the internal m_channelt0 array is now the sum of
// GlobalT0 and the information from the DRt0channel constant (rather than
// do the sum over for every hit).
//
// Revision 1.40  2000/02/15 21:32:26  mclean
// Added a parameter to set the resolution (used for the fitting weight)
// i.e.
// param CDOffCal Resolution 10.0
// sets the resolution to 10 meters.
//
// Revision 1.39  2000/02/15 17:30:33  mclean
// Added CellID2DaqID function and corresponding member data, introduced
// code in constructor to fill member data.
//
// Rearranged some "for" iterator definitions to work on SunOS and OSF1.
//
// Revision 1.38  2000/02/15 13:57:15  lyon
// Fixed multiple defines of for-loop variable
//
// Revision 1.37  2000/02/14 23:49:02  mclean
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
// Revision 1.36  2000/02/02 15:37:39  cdj
// -now dependent on DFDataHolder instead of DriftFunctionProxy
// -fixed compile problem on Solaris
//
// Revision 1.35  2000/02/01 23:03:17  mclean
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
//         DBDRadc2charge
// 	   DBDRtdc2time
// 	   DBDRDaqCellIDMap
// 	   DBDRGarfield_DistanceCorrection
// 	   DBDRGarfield_TimeCorrection
// 	   DBDRWireQuality
// 	   DBDRt0card
// 	   DBDRt0crate
//       **DBDRDriftData
// 	  *DBDRadcpedestal
//       **DBDRt0channel
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
// Revision 1.34  2000/01/25 18:50:05  mclean
// I added provisions for easy access to pedestal files meant for the
// use of constants-production experts/specialist. This is not intended for
// general use.
//
// If one creates a file:
//                    channelPeds.v01
// in the directory you are running suez from this will OVERRIDE any other source
// (database or ascii source format neither of which is implemented) of these
// constants. channelPeds.v01 is meant to represent the constants defined in
// DRadcpedestal.bdl and should be in the format of ordered triples
//             (channel#,ped,pedestal_rms)
// i.e. the same as the normal ascii format without headers (this can be
// changed if requested).
//
// The record ordering (i.e. channel#) is not checked and is assumed to be
// 1-1 increasing CELL#, i.e. that appropriate after the DRDaqCellIDMap
// mis-cabling map has been applied to the raw DAQID.
//
// Revision 1.33  1999/12/15 22:50:37  mclean
// Added HitQuality.h as an include
//
// Revision 1.32  1999/12/15 19:48:52  mclean
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
// Revision 1.31  1999/12/07 23:30:06  mclean
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
// Revision 1.30  1999/12/06 20:34:38  mclean
// StopGap Measure: delete lots of report(debug... lines to limit cpu consumption
//
// Revision 1.29  1999/11/26 22:14:50  mclean
// Added statistical counters for messages that were annoying everyone along
// with lots of new information.
// These print out at INFO level when DriftFunction's destructor is called
// (i.e. once per calibration set).
//
// Any hits with cellid > 9796 are now flagged as bad.
// Any hits with more than one TDC hit were allready flagged as bad.
// Streamlined makeCalibratedHIt.
//
// Revision 1.28  1999/11/10 20:48:23  mclean
// Removed extraneous debug messages associated with negative times from
// bunch finding.
//
// Revision 1.27  1999/11/09 20:43:34  mclean
// Deleted unused functions *_dbg
// NaiveDriftFunction: the Tseries object in CalibrateDRHit now contains
//                     only a DriftDistance, the first and second derivatives
//                     are now zero.
// DriftFunction: makeCalibratedDRHit is modified to deal with events
//                with no TDCs, these are flagged with a nonzero qualhit now.
//                distance,weight and time are set to zero.
//
// Revision 1.26  1999/10/26 16:26:54  mclean
// Modified definition of time in CalibratedDRHit.
// It now includes the bunch shift appropriately (i.e. it is a true
// Drift time).
//
// Revision 1.25  1999/06/24 20:10:43  lyon
// Add DriftFunction:: in appropriate places to get things to compile on OSF1
//
// Revision 1.24  1999/06/23 22:21:12  lyon
// DriftFunction.cc now has functionality common to all drift functions
//
// (all older messages only in HePrDriftFunction or NaiveDriftFunction
//  as appropriate)

// compiler bug flags
#include "Experiment/Experiment.h"

// system include files
#include <fstream.h>
#include <iostream.h>

// compiler bug fixes 
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
#include "CDOffCal/CDOffCalProducer.h"
#include "CDOffCal/DriftFunction.h"
#include "CDOffCal/DriftFunctionProxy.h"
#include "CDOffCal/HitQuality.h"
#include "CDOffCal/Statistics.h"
#include "CalibratedData/Tseries.h"
#include "CalibratedData/CalibratedDRHit.h"
#include "ADRGeom/ADRSenseWireStore.h"

#include <vector>
#include <map>
#include <set>

static const char* const kFacilityString   = "CDOffCal.DriftFunction" ;
const double DriftFunction::kdefault_MinTightTime =  -30000. ;
const double DriftFunction::kdefault_MaxTightTime =  350000. ;
const double DriftFunction::kdefault_MinLooseTime =  -50000. ;
const double DriftFunction::kdefault_MaxLooseTime =  600000. ;
const double DriftFunction::kdefault_GlobalT0     =       0. ;
const double DriftFunction::kdefault_TDCQuantum   =    -500. ;
const double DriftFunction::kdefault_Resolution   = 0.000123 ; // 1/123microns**2
const double DriftFunction::kdefault_SignalProp   = 0.5      ; 
const double DriftFunction::kclight        = 3.0e-4      ;
const UInt16 DriftFunction::kHighTDC       = 16384       ; 

//----------------------------------------------------------------------------
//
DriftFunction::DriftFunction(CDOffCalProducer* CDOffCal,
			     DFDataHolder* iDataHolder) :
   m_dataHolder( iDataHolder ),
   m_CDOffCal( CDOffCal )
{
  report( DEBUG, kFacilityString )  << "Constructor Starts " << endl;
  //-----------------------------------------------------------------------------------------------------------
  //
  // These are now used as function calls to the producer,
  // this allows parameters to be changed "on the fly"
  // 
  //m_Mintime    = CDOffCal->getMinimumTime();
  //m_Maxtime    = CDOffCal->getMaximumTime();
  //m_TDCQuantum = CDOffCal->getTDCQuantum();
  //m_GlobalT0   = CDOffCal->getGlobalT0();

  //-----------------------------------------------------------------------------------------------------------
  //
  // This enables the set of statistics counters
  // these have been mostly commented out because
  // the code is slow because of the way it 
  // handles the character arguments
  //
  m_statistics = new Statistics;
  m_statistics->counter(0,"CDOffCal Calibration Statistics"); 

  //-----------------------------------------------------------------------------------------------------------
  //
  // Oldstyle drift functions (NaiveDriftFunction and
  // HePrDriftFunction) are obsolete and deleted.
  //
  if ( m_CDOffCal->getType() < DriftFunction::kDFType_HePr_data1 ) 
    { 
      report( EMERGENCY, kFacilityString )  << "Invalid DriftFunctionType Parameter: " 
					    <<  m_CDOffCal->getType() 
					    << endl;
      assert( false );
      ::exit( 1 ); 
    }
  //-----------------------------------------------------------------------------------------------------------
  // rather than have many dual function calls, parse this
  // to local member data
  for(int level =  0 ;
          level < 20 ;
        ++level      )
    {
      debugLevel[level] = ( m_CDOffCal->getDebugLevel() == -level || 
	                    m_CDOffCal->getDebugLevel() >=  level  ) ;
    }
  //-----------------------------------------------------------------------------------------------------------
  // Unpack Fitting Weights 
  //
  report( DEBUG, kFacilityString )  << "Trying to get Fitting Weights  " << endl;
  report( INFO , kFacilityString )  << "Global scale (user parameter): " << m_CDOffCal->getResolution() << endl;
  //
  //-----------------------------------------------------------------------------------------------------------
  //
  //       Layer Correction
  //
  float sum   = 0. ;
  float count = 0. ;

  memset ( m_LayerWeight, 1., sizeof(m_LayerWeight) ) ; // set default value

  for(FAConstants<DBDRWeightLayer>::const_iterator 
	wloop1  = m_dataHolder->LayerWeight().begin() ;
        wloop1 != m_dataHolder->LayerWeight().end()   ; 
      ++wloop1 ) 
    {
      int  layer   = (*wloop1).get_layer()   ;
      float weight = (*wloop1).get_fittingweight() ;
      if ( weight <= 0 )
	{
	  report( ERROR , kFacilityString )  << " Layer " << layer 
					     << " Illegal Weight Found " << weight 
					     << " SET TO UNITY! " << endl;
	  weight = 1.0 ;
	}
      if ( layer > 0 && layer <= kmaxDRlayer )
	{
	  m_LayerWeight[layer-1] = weight ;
	  sum   += weight                 ;
	  count += 1.0                    ;
        }
      else
	{
	  report( ERROR , kFacilityString )  << " Illegal Layer Found " 
					     << layer 
					     << endl ;
	}
    }

  if ( count != 0 )
    {
      report( INFO , kFacilityString )  << count 
					<< " Layer Fitting Weights found, average= " 
					<< ( sum/count ) << endl;    
    }


  if ( count != kmaxDRlayer ) 
    {
      report( ERROR , kFacilityString )  << " Layer Fitting Weights Incomplete, #found= " 
					 << count << endl;
    }
  //-----------------------------------------------------------------------------------------------------------
  //
  //    Drift Distance Correction For Each Layer 
  //
  sum   = 0. ;
  count = 0. ;
  
  memset ( m_DriftWeight, 1., sizeof(m_DriftWeight) ) ; // set default value

  for(FAConstants<DBDRWeightDrift>::const_iterator 
	wloop2 =m_dataHolder->DriftWeight().begin();
        wloop2!=m_dataHolder->DriftWeight().end()  ; 
      ++wloop2 ) 
    {
      int layer = (*wloop2).get_layer() ;
      //
      // data for fake layer 0 is number of bins and maximum value of dependent variable
      //
      if ( layer == 0 ) 
	{
	  m_DriftWeight_Max   = (*wloop2).get_fittingweight() ;
	  m_DriftWeight_Nbins = (*wloop2).get_bin()           ;
	}
      else
	{
	  //
	  // data for fake layer 48 is minimum value of dependent variable
	  //
	  if ( layer > kmaxDRlayer ) 
	    {
	      m_DriftWeight_Min = (*wloop2).get_fittingweight() ;
	    }
	  else
	    {
	      //
	      // actual fitting weight unpack
	      //
	      int   bin    = (*wloop2).get_bin()           ;
	      float weight = (*wloop2).get_fittingweight() ;
	      if ( weight <= 0 )
		{
		  report( ERROR , kFacilityString )  << " Layer " << layer 
						     << " Drift Bin " << bin 
						     << " Illegal Weight Found " << weight 
						     << " SET TO UNITY! " << endl;
		  weight = 1.0 ;
		}
	      if ( bin <= kmaxWeightBins && bin > 0 ) 
		{
		  m_DriftWeight[layer-1][bin-1] = weight ;
		  sum   += weight                        ; 
		  count += 1.0                           ;
		}
	      else
		{
		  report( ERROR , kFacilityString )  << " Layer " << layer 
						     << " Drift Bin " << bin 
						     << " Illegal Bin Found " << endl;
		}
	    }
	}
    }

  m_DriftWeight_BinSize = ( m_DriftWeight_Max - m_DriftWeight_Min ) / 
                          ( (float) m_DriftWeight_Nbins )           ;
  if ( m_DriftWeight_BinSize <= 0 )
    {
      report( ERROR , kFacilityString )  << count 
					 << " Drift Fitting Weights Problem, BinSize= " 
					 << m_DriftWeight_BinSize  
					 << endl;
    } 

  if ( count != 0 )
    {
      report( INFO , kFacilityString )  << count 
					<< " Drift Fitting Weights found, average= " 
					<< ( sum/count ) 
					<< endl;
    }

  if ( count != ( kmaxDRlayer * m_DriftWeight_Nbins )  ) 
    {
      report( ERROR , kFacilityString )  << " Drift Fitting Weights Incomplete, #found= " \
					 << count 
					 << " #expected= "  << ( kmaxDRlayer * m_DriftWeight_Nbins )
					 << endl;
    }

  //-----------------------------------------------------------------------------------------------------------
  //
  //    cos(theta) Correction For Each Layer 
  //
  sum   = 0. ;
  count = 0. ;

  memset ( m_ThetaWeight, 1., sizeof(m_ThetaWeight) ) ; // set default value

  for(FAConstants<DBDRWeightTheta>::const_iterator 
	wloop3 =m_dataHolder->ThetaWeight().begin();
        wloop3!=m_dataHolder->ThetaWeight().end()  ; 
      ++wloop3 ) 
    {
      int layer = (*wloop3).get_layer() ;
      //
      // data for fake layer 0 is number of bins and maximum value of dependent variable
      //
      if ( layer == 0 ) 
	{
	  m_ThetaWeight_Max   = (*wloop3).get_fittingweight() ;
	  m_ThetaWeight_Nbins = (*wloop3).get_bin()           ;
	}
      else
	{
	  //
	  // data for fake layer 48 is minimum value of dependent variable
	  //
	  if ( layer > kmaxDRlayer ) 
	    {
	      m_ThetaWeight_Min = (*wloop3).get_fittingweight() ;
	    }
	  else
	    {
	      //
	      // actual fitting weight unpack
	      //
	      int   bin    = (*wloop3).get_bin()           ;
	      float weight = (*wloop3).get_fittingweight() ;
	      if ( weight <= 0 )
		{
		  report( ERROR , kFacilityString )  << " Layer " << layer 
						     << " cos(Theta) Bin " << bin 
						     << " Illegal Weight Found " << weight 
						     << " SET TO UNITY! " << endl;
		  weight = 1.0 ;
		}
	      if ( bin <= kmaxWeightBins && bin > 0 ) 
		{
		  m_ThetaWeight[layer-1][bin-1] = weight ;
		  sum   += weight                        ; 
		  count += 1.0                           ;
		}
	      else
		{
		  report( ERROR , kFacilityString )  << " Layer " << layer 
						     << " Time Bin " << bin 
						     << " Illegal Bin Found " << endl;
		}
	    }
	}
    }

  m_ThetaWeight_BinSize = ( m_ThetaWeight_Max - m_ThetaWeight_Min ) / 
                          ( (float) m_ThetaWeight_Nbins )           ;
  if ( m_ThetaWeight_BinSize <= 0 )
    {
      report( ERROR , kFacilityString )  << count 
					 << " Theta Fitting Weights Problem, BinSize= " 
					 << m_ThetaWeight_BinSize  
					 << endl;
    } 

  if ( count != 0 )
    {
      report( INFO , kFacilityString )  << count 
					<< " Theta Fitting Weights found, average= " 
					<< ( sum/count ) 
					<< endl;
    }

  if ( count != ( kmaxDRlayer * m_ThetaWeight_Nbins )  ) 
    {
      report( ERROR , kFacilityString )  << " Theta Fitting Weights Incomplete, #found= " 
					 << count
					 << " #expected= "  << ( kmaxDRlayer * m_ThetaWeight_Nbins )
					 << endl;
    }
  //-----------------------------------------------------------------------------------------------------------
  //
  //    Phi Correction For Each Layer 
  //
  sum   = 0. ;
  count = 0. ;

  memset ( m_PhiWeight, 1., sizeof(m_PhiWeight) ) ; // set default value

  for(FAConstants<DBDRWeightPhi>::const_iterator 
        wloop4 =m_dataHolder->PhiWeight().begin();
        wloop4!=m_dataHolder->PhiWeight().end()  ; 
      ++wloop4 ) 
    {
      int layer = (*wloop4).get_layer() ;
      //
      // data for fake layer 0 is number of bins and maximum value of dependent variable
      //
      if ( layer == 0 ) 
	{
	  m_PhiWeight_Max     = (*wloop4).get_fittingweight() ;
	  m_PhiWeight_Nbins   = (*wloop4).get_bin()           ;
	}
      else
	{
	  //
	  // data for fake layer 48 is minimum value of dependent variable
	  //
	  if ( layer > kmaxDRlayer ) 
	    {
	      m_PhiWeight_Min = (*wloop4).get_fittingweight() ;
	    }
	  else
	    {
	      //
	      // actual fitting weight unpack
	      //
	      int   bin    = (*wloop4).get_bin()           ;
	      float weight = (*wloop4).get_fittingweight() ;
	      if ( weight <= 0 )
		{
		  report( ERROR , kFacilityString )  << " Layer " << layer 
						     << " Phi Bin " << bin 
						     << " Illegal Weight Found " << weight 
						     << " SET TO UNITY! " << endl;
		  weight = 1.0 ;
		}
	      if ( bin <= kmaxWeightBins && bin > 0 ) 
		{
		  m_PhiWeight[layer-1][bin-1] = weight       ;
		  sum   += weight ; 
		  count += 1.0;
		}
	      else
		{
		  report( ERROR , kFacilityString )  << " Layer " << layer 
						     << " Phi Bin " << bin 
						     << " Illegal Bin Found " << endl;
		}
	    }
	}
    }

  m_PhiWeight_BinSize = ( m_PhiWeight_Max - m_PhiWeight_Min ) / m_PhiWeight_Nbins ;
  if ( m_PhiWeight_BinSize <= 0 )
    {
      report( ERROR , kFacilityString )  << count << " Phi Fitting Weights Problem, BinSize= " 
					 << m_PhiWeight_BinSize  
					 << endl;
    } 

  if ( count != 0 )
    {
      report( INFO , kFacilityString )  << count 
					<< " Phi Fitting Weights found, average= " 
					<< ( sum/count ) 
					<< endl ;
    }

  if ( count != ( kmaxDRlayer * m_PhiWeight_Nbins )  ) 
    {
      report( ERROR , kFacilityString )  << " Phi Fitting Weights Incomplete, #found= " 
					 << count 
					 << " #expected= "  << ( kmaxDRlayer * m_PhiWeight_Nbins )
					 << endl;
    }

  //-------------------------------------------------------------------------------------------------------------------------------------------
  //
  // Put dump option for fitting weights here?
  // 
  //-------------------------------------------------------------------------------------------------------------------------------------------
  // Unpack T0s 
  //
  int i;
  int imagic  = 123456789 ;
  double magic ;
  memset(&magic,imagic,sizeof(magic));  // unique number for initialization 
                                         // to test if constants unpack has
                                         // set all channels 

  report( DEBUG, kFacilityString )  << "Trying to get T0s" << endl;
  report( INFO , kFacilityString )  << "Global T0 offset 1 (user parameter): " 
				    << m_CDOffCal->getGlobalT0() << endl;
  report( INFO , kFacilityString )  << "Global T0 offset 2 (constants)     : " 
				    << m_dataHolder->T0Global()[1].get_tzero() << endl;

  memset ( m_channelT0, imagic, sizeof(m_channelT0) ) ;

  for(FAConstants<DBDRt0channelv2>::const_iterator 
	t0loop =m_dataHolder->T0Channel().begin();
        t0loop!=m_dataHolder->T0Channel().end()  ;
      ++t0loop ) 
    {
      int layer = m_dataHolder->Geometry()->layerWire( (*t0loop).get_indexNumber()).first ;

      m_channelT0[(*t0loop).get_indexNumber()-1] 
        = (*t0loop).get_t0CommonMode()
	+ (*t0loop).get_t0Channel   ()
	+ m_dataHolder->T0Crate ()[(*t0loop).get_crate ()].get_tzero()
	+ m_dataHolder->T0Card  ()[(*t0loop).get_card  ()].get_tzero()
	+ m_dataHolder->T0HVCard()[(*t0loop).get_hvcard()].get_tzero()
	+ m_dataHolder->T0Global()[1].get_tzero()
	+ m_CDOffCal->getGlobalT0() 
	+ ZShift ( layer ) 
	- ( m_dataHolder->Geometry()->radiusNominal( layer ) / kclight ) ;
    }

  int nt0err = 0 ;
  for ( i=0; i<kTotalDRWires; ++i ) 
    { 
      if ( m_channelT0[i] == magic )
	{ 
	  nt0err++ ; 
	  report(ERROR,kFacilityString) << " could not load wire " << (i+1) << " of DRt0channelv2" << endl;
	} 
    }

  if ( nt0err == 0 ) 
    { 
      report( INFO, kFacilityString )  << "t0s loaded OK" 
				       << endl; 
    }
  else
    {
      report( ERROR, kFacilityString )  << "t0s loaded, some channels missing " 
					<< nt0err << endl; 
      assert(nt0err==0);
    }

  
  
  //-------------------------------------------------------------------------------------------------------------------------------------------
  //
  // Unpack Charge Dependent T0 shifts
  //
  int qcount = 0 ;
  memset ( m_Qcorr, 0, sizeof(m_Qcorr) )  ;
  for(FAConstants<DBDRt0global>::const_iterator 
      t0_qcorr  = m_dataHolder->T0Global().begin() ;
      t0_qcorr != m_dataHolder->T0Global().end() ;
    ++t0_qcorr )
    {
      int qbin = (*t0_qcorr).get_index() - 2 ; 
      if ( qbin >= 0 ) 
	{ 
	  if ( qbin >= kmaxQcorrBins ) 
	    {
	      report( ERROR, kFacilityString )  << " overflow in pulseheight-dependent t0 shifts " 
						<< endl ;
	    }
	  else
	    {
	      m_Qcorr[qbin] = (*t0_qcorr).get_tzero();
              qcount++ ;
	    }
	}
    }
  report( INFO, kFacilityString )  << " Found " 
				   << qcount
				   << " Charge Dependent T0 Shifts " 
				   << endl; 
  

  //-------------------------------------------------------------------------------------------------------------------------------------------
  // Unpack ADC information (pedestals only for now)
  //
  report( DEBUG, kFacilityString )  << "Trying to get ADC pedestals" << endl;

  memset ( m_channelPed, imagic, sizeof(m_channelPed) ) ;

  for(FAConstants<DBDRadcpedestal>::const_iterator 
	adcloop =m_dataHolder->ADCped().begin();
        adcloop!=m_dataHolder->ADCped().end()  ;
      ++adcloop ) 
    {
      m_channelPed   [(*adcloop).get_CellID()-1]
	=(*adcloop).get_pedestal();
      m_channelPedRMS[(*adcloop).get_CellID()-1]
	=(*adcloop).get_pedestal_RMS();
    }

  int npederr = 0 ;
  for ( i=0; i<kTotalDRWires; ++i ) 
    { 
      if ( m_channelPed[i] == magic ) 
	{ 
	  npederr++ ; 
	  report(ERROR,kFacilityString) << " could not load wire " << (i+1) << " of DRadcpedestal" << endl;
	} 
    }

  if ( npederr == 0 ) 
    {
      report( INFO , kFacilityString )  << "Pedestals loaded OK" 
					<< endl;
    }
  else
    {
      report( ERROR, kFacilityString )  << "Pedestals loaded, some channels missing "
					<< npederr 
					<< endl;
      assert(npederr==0);
    }
  
  //-------------------------------------------------------------------------------------------------------------------------------------------
  // Unpack WireQuality information
  //
  report( DEBUG, kFacilityString )  << "Trying to get WireQuality from frame" << endl;
  
  memset ( m_quality, imagic, sizeof(m_quality) ) ;
  
  for(FAConstants<DBDRWireQuality>::const_iterator 
	qloop =m_dataHolder->WireQuality().begin();
        qloop!=m_dataHolder->WireQuality().end()  ;
      ++qloop ) 
    {	  
      m_quality[(*qloop).get_CellID()-1]=(*qloop).get_Quality();
    }

  int nqerr = 0 ;
  for ( i=0; i<kTotalDRWires; ++i ) 
    { 
      if ( m_quality[i] == imagic ) 
	{ 
	  nqerr++ ; 
	  report(ERROR,kFacilityString) << " could not load wire " << (i+1) << " of DRWireQuality" << endl;
	} 
    }

  if ( nqerr == 0 ) 
    {
      report( INFO , kFacilityString )  << "WireQuality Info loaded OK" 
					<< endl ; 
      assert(nqerr==0);
    }
  else
    {
      report( ERROR, kFacilityString )  << "WireQuality Info loaded, some channels missing  " 
					<< nqerr 
					<< endl;
    }
  //-------------------------------------------------------------------------------------------------------------------------------------------
  // Unpack MisCabling information
  //
  report( DEBUG, kFacilityString )  << "Trying to get Miscabling Information" << endl;
  for ( i=0; i<kTotalDRWires; ++i ) 
    { 
      m_daqmap[i]    = kTotalDRWires+1 ; 
      m_invdaqmap[i] = kTotalDRWires+1 ; 
    }

  for(FAConstants<DBDRDaqCellIDMap>::const_iterator 
	wloop =m_dataHolder->WireMap().begin();
        wloop!=m_dataHolder->WireMap().end();
      ++wloop ) 
    {
         m_daqmap[(*wloop).get_DaqId() -1]=(*wloop).get_CellId();
      m_invdaqmap[(*wloop).get_CellId()-1]=(*wloop).get_DaqId();
    }

  int nwerr = 0 ;
  for ( i=0; i<kTotalDRWires; ++i ) 
    { 
      if ( m_daqmap[i] == kTotalDRWires+1 ) 
	{ 
	  nwerr++ ; 
	} 
    }

  int nwerr2 = 0 ;
  for ( i=0; i<kTotalDRWires; ++i ) 
    { 
      if ( m_invdaqmap[i] == kTotalDRWires+1 ) 
	{ 
	  nwerr2++ ; 
	} 
    }

  if ( nwerr == 0 ) 
    { 
      report( INFO , kFacilityString )  << "WireMap loaded OK" 
					<< endl; 
    }
  else
    { 
      report( ERROR, kFacilityString )  << "WireMap loaded, some channels missing  " 
					<< nwerr << " " 
					<< nwerr2 
					<< endl; 
    }
  //-------------------------------------------------------------------------------------------------------------------------------------------
  // Unpack Wire ADC Gain Information 
  //
  report( DEBUG, kFacilityString )  << "Trying to get ADC Gains" << endl;
 
  memset ( m_adcgain, imagic, sizeof(m_adcgain) ) ;
 
  for(FAConstants<DBDRadc2charge>::const_iterator 
	gloop =m_dataHolder->ADCpar().begin();
        gloop!=m_dataHolder->ADCpar().end()  ;
      ++gloop ) 
    {
      m_adcgain   [(*gloop).get_CellID()-1]=(*gloop).get_slope();
      m_adcgainRMS[(*gloop).get_CellID()-1]=(*gloop).get_slope_error();
    }

  int ngerr = 0 ;
  for( i=0; i<kTotalDRWires; ++i ) 
    { 
      if ( m_adcgain[i] == magic ) 
	{ 
	  ngerr++ ;
 	  report(ERROR,kFacilityString) << " could not load wire " << (i+1) << " of DRadc2charge" << endl;
	} 
    } 

  if ( ngerr == 0 ) 
    { 
      report( INFO, kFacilityString )  << "ADC Gains loaded OK" 
				       << endl; 
    }
  else
    { 
      report( ERROR, kFacilityString )  << "adcgains, some channels missing> "
					<< ngerr 
					<< endl; 
      assert(ngerr==0);
    }

  int ngerr2 = 0 ;
  for( i=0; i<kTotalDRWires; ++i ) 
    { 
      if ( m_adcgain[i] == 0 ) 
	{ 
	  ngerr2++ ; 
	  m_adcgain[i] = 1. ; 
	} 
    } 

  if ( ngerr2 == 0 ) 
    { 
      report( INFO, kFacilityString )  << "No Zero ADC Gains" 
				       << endl; 
    }
  else
    { 
      report( ERROR, kFacilityString )  << "adcgains, some channels have zero gain, set to 1> "
					<< ngerr2 
					<< endl; 
    }

  //-------------------------------------------------------------------------------------------------------------------------------------------
  //
  // Unpack Wire TDC Slope Information 
  //
  report( DEBUG, kFacilityString )  << "Trying to get TDC Slopes" << endl;

  memset ( m_tdcslope, imagic, sizeof(m_tdcslope) ) ;

  for(FAConstants<DBDRtdc2time>::const_iterator 
	tloop =m_dataHolder->TDCpar().begin();
        tloop!=m_dataHolder->TDCpar().end()  ;
      ++tloop ) 
    {
      m_tdcslope   [(*tloop).get_CellID()-1]=(*tloop).get_slope();
      m_tdcslopeRMS[(*tloop).get_CellID()-1]=(*tloop).get_slope_error();
      m_tdcped     [(*tloop).get_CellID()-1]=(*tloop).get_offset();
      m_tdcpedRMS  [(*tloop).get_CellID()-1]=(*tloop).get_pedestal_noise();
    }

  int nterr = 0 ;
  for( i=0; i<kTotalDRWires; ++i ) 
    { 
      if ( m_tdcslope[i] == magic )
	{ 
	  ngerr++ ; 
	  report(ERROR,kFacilityString) << " could not load wire " << (i+1) << " of DRtdc2time" << endl;
	} 
    } 

  if ( nterr == 0 ) 
    { 
      report( INFO , kFacilityString )  << "TDC Slopes loaded OK" 
					<< endl; 
    }
  else
    { 
      report( ERROR, kFacilityString )  << "TDC Slopes, some channels missing> "
					<< nterr 
					<< endl; 
      assert(nterr==0);
    }

  int nterr2 = 0 ;
  for( i=0; i<kTotalDRWires; ++i ) 
    { 
      if ( m_tdcslope[i] == 0 ) 
	{ 
	  nterr2++ ; 
	  m_tdcslope[i] == 1. ; 
	}
    } 

  if ( nterr2 == 0 ) 
    { 
      report( INFO, kFacilityString )  << "ADC Gains loaded OK" 
				       << endl; 
    }
  else
    { 
      report( ERROR, kFacilityString )  << "tdcslopes, some channels have zero gain, set to 1> "
					<< nterr2 
					<< endl; 
    }
  //-------------------------------------------------------------------------------------------------------------------------------------------
  if ( debugLevel[5] )
    {
      for(int i=0;i<kTotalDRWires;++i)
	{
	  report(DEBUG,kFacilityString) << "Chan/T0/Gain/Slope/Ped " << i+1 << " " 
					<<  m_channelT0[i]       << " " 
					<<    m_adcgain[i]       << " " 
					<<   m_tdcslope[i]       << " " 
					<< m_channelPed[i]       << endl;
	} 
    }

  report( DEBUG, kFacilityString )  << "Constructor Ends " << endl;
}
//
//----------------------------------------------------------------------------
//
DriftFunction::~DriftFunction()
{
  report( DEBUG, kFacilityString )  << "Destructor Starts " << endl;
  delete m_statistics;
  report( DEBUG, kFacilityString )  << "Destructor Ends " << endl;
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Charge 
DriftFunction::adcpedestal(UInt16 wire) const 
{ 
  return m_channelPed[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Charge 
DriftFunction::adcpedestalRMS(UInt16 wire) const 
{ 
  return m_channelPedRMS[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Charge
DriftFunction::adcgain(UInt16 wire) const 
{ 
  return m_adcgain[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Charge
DriftFunction::adcgainRMS(UInt16 wire) const 
{ 
  return m_adcgainRMS[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Time
DriftFunction::tdcslope(UInt16 wire) const 
{ 
  return m_tdcslope[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Time
DriftFunction::tdcslopeRMS(UInt16 wire) const 
{ 
  return m_tdcslopeRMS[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Time
DriftFunction::tdcped(UInt16 wire) const 
{ 
  return m_tdcslope[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Time
DriftFunction::tdcpedRMS(UInt16 wire) const 
{ 
  return m_tdcpedRMS[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Time 
DriftFunction::tdctzero(UInt16 wire) const 
{ 
  return m_channelT0[wire-1];
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Drift 
DriftFunction::resolution(int layer, 
			  DriftFunction::Drift drift, 
			  float cTheta, 
			  float oPhi) const 
{ 
  float value = resolution(layer,drift,cTheta) ;

  const float twopi =  6.283185307 ;
  float x = fmod ( oPhi , twopi ) ;
  if ( x < 0 ) 
    { 
      x = twopi + x ; 
    }

  x = x / m_PhiWeight_BinSize ;

  //
  // Truncate Distributions, periodically
  //
  int bin2 = ( ( ( int ) x ) % m_PhiWeight_Nbins ) ;
  int bin1 = ( ( bin2 - 1  ) % m_PhiWeight_Nbins ) ;
  int bin3 = ( ( bin2 + 1  ) % m_PhiWeight_Nbins ) ;

  x = x - ( (float) bin2 ) ;

// ~~~~~~ THIS WAS THE CODE UNTIL 2/16/02.  IT READS BEYOND ARRAY BOUNDS ~~~~~
//   //
//   // Weight corrections are at bin centers, unlike
//   // entrance angle data which are at bin edges
//   //
//   // Value at X=0.5 should be [bin2]
//   //
//   if ( x < 0.5 ) 
//     { 
//       value *= (  ( m_PhiWeight[layer-1][bin1]*(   0.5-x ) ) 
//                +  ( m_PhiWeight[layer-1][bin2]*(   0.5+x ) ) ) ;
//     }
//   else
//     {
//       value *= (  ( m_PhiWeight[layer-1][bin2]*(  1.5-x ) ) 
//                +  ( m_PhiWeight[layer-1][bin3]*( -0.5+x ) ) ) ;
//     }
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  /*
     Weight corrections are at bin centers, unlike
     entrance angle data which are at bin edges
    
     Value at X=0.5 should be [bin]
    
     X<0.5 in bin 0   extrapolated from bin0 central value
                      and bin0/bin1 slope
     X>0.5 in bin Max extrapolated from bin(Max) central value
                      and bin(Max)-bin(Max-1) slope 
  */
  int bin = bin2 ;
  if ( ( x < 0.5 && bin > 0 ) || bin == m_PhiWeight_Nbins - 1 )
    {
      value *= ( ( m_PhiWeight[layer-1][bin-1]*( 0.5-x ) ) + 
                 ( m_PhiWeight[layer-1][bin  ]*( 0.5+x ) ) ) ;
    }
  else
    {
      value *= ( ( m_PhiWeight[layer-1][bin  ]*(  1.5-x ) ) + 
                 ( m_PhiWeight[layer-1][bin+1]*( -0.5+x ) ) ) ;
    }
  
  if ( value < 0 )
    {
      float newvalue = resolution(layer,drift,cTheta) ;
      report( ERROR, kFacilityString )  << "Negative Resoluton After Phi Correction "
					<< value 
					<< "replaced by " 
					<< newvalue 
					<< endl;
      value = newvalue;
    }

  return value ;
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Drift 
DriftFunction::resolution(int layer, DriftFunction::Drift drift, float cTheta) const 
{ 
  float value = resolution(layer,drift) ; 
  float x     = ( cTheta - m_ThetaWeight_Min ) /
                  m_ThetaWeight_BinSize        ;
  int   bin   = ( ( int ) x )                  ;
  x           = x - ( (float) bin )            ;

  //
  // Truncate Distributions
  //   Values are NOT extrapolated out of data region,
  //   they hit a brick wall.
  //
  if ( bin < 0 ) 
    {
      bin = 0  ;
      x   = 0. ;
    }
  else
    {
      if ( bin >= m_ThetaWeight_Nbins )
	{
	  bin = m_ThetaWeight_Nbins - 1 ; 
	  x   = 1.                      ;
	}
    }
 	
  /*
     Weight corrections are at bin centers, unlike
     entrance angle data which are at bin edges
    
     Value at X=0.5 should be [bin]
    
     X<0.5 in bin 0   extrapolated from bin0 central value
                      and bin0/bin1 slope
     X>0.5 in bin Max extrapolated from bin(Max) central value
                      and bin(Max)-bin(Max-1) slope 
  */
  if ( ( x < 0.5 && bin > 0 ) || bin == m_ThetaWeight_Nbins - 1 )
    {
      value *= ( ( m_ThetaWeight[layer-1][bin-1]*( 0.5-x ) ) + 
                 ( m_ThetaWeight[layer-1][bin  ]*( 0.5+x ) ) ) ;
    }
  else
    {
      value *= ( ( m_ThetaWeight[layer-1][bin  ]*(  1.5-x ) ) + 
                 ( m_ThetaWeight[layer-1][bin+1]*( -0.5+x ) ) ) ;
    }
  
  if ( value < 0 )
    {
      float newvalue = resolution(layer,drift) ;
      report( ERROR, kFacilityString )  << "Negative Resolution after CosTheta Correction "
					<< value 
					<< "replaced by " 
					<< newvalue 
					<< endl;
      value = newvalue;
    }
  return value ;
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Drift 
DriftFunction::resolution(int layer, DriftFunction::Drift driftinmeters) const 
{ 
  Drift x ;
  double drift = (2. * driftinmeters )/ 
    ( m_dataHolder->Geometry()->radiusNominal( layer ) *
      m_dataHolder->Geometry()->cellPhiWidth ( layer ) ) ;
  float value = resolution(layer) ;	


  //
  // Determine if resolution correction is to signed
  // drift or unsigned drift.
  //
  if ( m_DriftWeight_Min >= 0 ) 
    { 
      x = ( ( fabs ( drift ) ) - m_DriftWeight_Min ) / 
	    m_DriftWeight_BinSize                    ;
    }
  else
    { 
      x = ( drift - m_DriftWeight_Min ) / 
	    m_DriftWeight_BinSize       ; 
    }

  int bin = ( int ) x           ;          
  x       = x - ( (float) bin ) ;

  //
  // Truncate Distributions
  //   Values are NOT extrapolated out of data region,
  //   they hit a brick wall.
  //
  if ( bin < 0 ) 
    {
      bin = 0  ;
      x   = 0. ;
    }
  else
    {
      if ( bin >= m_DriftWeight_Nbins )
	{
	  bin = m_DriftWeight_Nbins - 1 ; 
	  x   = 1.                      ;
	}
    }
 	
  /*
     Weight corrections are at bin centers, unlike
     entrance angle data which are at bin edges
    
     Value at X=0.5 should be [bin]
    
     X<0.5 in bin 0   extrapolated from bin0 central value
                      and bin0/bin1 slope
     X>0.5 in bin Max extrapolated from bin(Max) central value
                      and bin(Max)-bin(Max-1) slope 
  */
  if ( ( x < 0.5 && bin > 0 ) || bin == m_DriftWeight_Nbins - 1 )
    {
      value *= ( ( m_DriftWeight[layer-1][bin-1]*( 0.5-x ) ) + 
                 ( m_DriftWeight[layer-1][bin  ]*( 0.5+x ) ) ) ;
    }
  else
    {
      value *= ( ( m_DriftWeight[layer-1][bin  ]*(  1.5-x ) ) + 
                 ( m_DriftWeight[layer-1][bin+1]*( -0.5+x ) ) ) ;
    }
  
  if ( value < 0 )
    {
      float newvalue = resolution(layer) ;
      report( ERROR, kFacilityString )  << "Negative resolution after drift correction " 
					<< value 
					<< "replaced by " 
					<< newvalue 
					<< endl;
      value = newvalue;
    }
  return value ;
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Drift 
DriftFunction::resolution(int layer) const 
{ 
  if ( layer < 1 || layer > kmaxDRlayer ) 
    {
      /* 
	 rather than check this at every layer of 
	 overloading-recursion, we should reach here
	 before layer is used for anything else, if it
	 is out of range a whole bunch of random is about to be
	 dragged out of memory... (but no overwrites though!)
      */    
      report( ERROR, kFacilityString )  << "bad layer value "
					<< layer  
					<< endl;
    }
   
  float value = m_LayerWeight[layer-1] * resolution() ;

  if ( value < 0 )
    {
      float newvalue = resolution();
      report( ERROR, kFacilityString )  << "Negative resolution after layer correction " 
					<< value
					<< "replaced by " 
					<< newvalue 
					<< endl;
      value = newvalue;
    }
  return value;
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Drift 
DriftFunction::resolution() const 
{ 
  return m_CDOffCal->getResolution();
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Time DriftFunction::badwiremap(UInt16 wire) const 
{ 
  return m_quality[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Charge DriftFunction::adc2Charge( UInt16 wire, 
						 DRDatum::DigiType adc) const 
{ 
  return Charge ( m_adcgain[wire-1] * ( adc - m_channelPed[wire-1] ) ); 
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Charge DriftFunction::adcwithped( UInt16 wire, 
						 DRDatum::DigiType adc) const 
{ 
  //just pedestal correction
  return Charge ( adc - m_channelPed[wire-1] ); 
}
//
//----------------------------------------------------------------------------
//
DriftFunction::Time DriftFunction::tdc2Time( UInt16 wire, 
					     DRDatum::DigiType tdc ) const
{  
  // verify TDC slope is OK at constants unpack
  return Time ( m_tdcslope[wire-1] * double ( tdc ) ) + m_channelT0[wire-1];
}
//
//----------------------------------------------------------------------------
//
DRDatum::DigiType DriftFunction::charge2adc( UInt16 wire, 
					     Charge PulseHeight ) const
{ 
  return DRDatum::DigiType ( ( PulseHeight / m_adcgain[wire-1] ) + m_channelPed[wire-1] ); 
}
//
//----------------------------------------------------------------------------
//
DRDatum::DigiType DriftFunction::time2tdc( UInt16 wire, 
					   Time time) const
{  
  // verify TDC slope is OK at constants unpack
  return  ( ( time - m_channelT0[wire-1] ) / m_tdcslope[wire-1] );
}
DRDatum::DigiType DriftFunction::time2tdc( UInt16 wire,
                                           Time time,
                                           double radiusOfLayer ) const
{ return time2tdc( wire, time ) ; }
//
//----------------------------------------------------------------------
//
// daqID2CellID( daqID )
//
//  Convert a daqId to a CellID (take care of miscablings, trivial right now)
//
UInt16 DriftFunction::daqID2CellID( UInt16 daqID ) const
{ 
  return m_daqmap[daqID-1]; 
}
//---------------------------------------------------------------------------
//
// CellID2daqID( CellID )
//
//  Convert a CellID2daqId (take care of miscablings, trivial right now)
//
UInt16 DriftFunction::CellID2daqID( UInt16 CellID ) const
{ 
  return m_invdaqmap[CellID-1]; 
}
//---------------------------------------------------------------------------
//
// ZCorr( )
//
DriftFunction::Time DriftFunction::Zcorr( int layer, float dz ) const
{
  float z = dz ;
  if ( m_dataHolder->Geometry()->layer( layer ).readoutEnd() == 
       AEWireLayer::kReadoutEast ) 
    { 
      z *= -1.;
    }
  return (z/(m_CDOffCal->signalPropBeta_Val()*kclight));
  //
  // If this was my world I would define the geometry so one could
  // simply do:
  /*
    return (dz*m_dataHolder->Geometry()->layer( layer ).readoutSign()/
            (m_CDOffCal->signalPropBeta_Val()*kclight));
  */
  // but nooooooooooooo
  //
}
//---------------------------------------------------------------------------
//
// ZShift( )
//
//  Check parameters, determine appropriate Z-shift
//
DriftFunction::Time DriftFunction::ZShift( int layer ) const
{
  // Shift time to z=0 AFTER the raw time has been checked above.
  if( m_CDOffCal->shiftTimeToCenter() )
    {
      // This is the time for a signal to travel from the endplate
      // at readout to the center of the chamber, z=0.  This time
      // is ADDED to a time at the endplate to shift it to z=0.
      
      // West is +z
      Meters dz ;
      if( m_dataHolder->Geometry()->layer( layer ).readoutEnd() ==
	  AEWireLayer::kReadoutEast )
	{
	  dz = m_dataHolder->Geometry()->layer( layer ).zMin() ;
	}
      else
	{
	  dz = m_dataHolder->Geometry()->layer( layer ).zMax()  ;
	}
      return Zcorr ( layer , dz ) ;
    }
  else
    {
      return 0 ;
    }
}
//---------------------------------------------------------------------------
// makeCalibratedHit - convert a raw hit into a calibrated hit
//
//        Note tOffset has default of 0.0 ps
//
//        See .h file for more info and warnings
//
CalibratedDRHit* DriftFunction::makeCalibratedHit( const CollatedRawDRHit& rawhit, 
						   int id, 
						   const ADRSenseWireStore* geom, 
						   double tOffset ) const
{      
   DABoolean singleTDC(true)   ;
   DRDatum::DigiType tdc(0)    ;
   Time         time(0)        ;
   UInt16       wTdc(0)        ;
   Charge       charge(-1)     ;
   HitQuality         qhit     ; // default is "bad" 
   double       weight(1.)     ;    
   Tseries      dva(0.,0.,0.)  ;   
   unsigned int  nTDCs(0)      ;
   Time earlyGap(-1.)          ;
   Time lateGap(-1.)           ;
   int    layer(0)             ;
   int    wireInLayer(0)       ;
   UInt16 wire(0)              ;
   int wintype(0)              ;

   //higher channels are electronic test channels (counts from 1)
   if ( rawhit.daqId() <= kTotalDRWires )
     {
       UInt16 cellID = daqID2CellID(rawhit.daqId()); 
       ADRSenseWireStore::LayerWire coord = geom->layerWire( cellID );
       layer       = coord.first       ;
       wireInLayer = coord.second      ;
       wire        = cellID            ;
       STL_VECTOR(DRDatum::DigiType)::const_iterator itdc = rawhit.digiTimes().begin();   
       nTDCs     = rawhit.digiTimes().size()  ;
       //m_statistics->counter(2,"normal hits"); 
       if (rawhit.isCharge()) 
	 {
	   charge = adc2Charge(wire,rawhit.digiCharge());
	 }
       else                   
	 { 
	   //m_statistics->counter(3,"no charge info"); 
	   qhit.set_nocharge()                    ;
	 }
       if ( nTDCs == 0 )  
	 { 
	   //m_statistics->counter(6,"no TDCS in hit"); 
	   qhit.set_notime(); 
	 } 
       else
	 {
           int i;

           UInt16 tdcTight,tdcLoose,tdcHigh,tdcLow;

	   int wTdcTight = -1;
           int wTdcLoose = -1;
           int wTdcHigh  = -1;
           int wTdcLow   = -1;

           Time timeTight = m_CDOffCal->getMaxTightTime();
           Time timeLoose = m_CDOffCal->getMaxLooseTime();
           Time timeHigh  = m_CDOffCal->getMaxLooseTime() + 1.e12;
           Time timeLow   = m_CDOffCal->getMinLooseTime() - 1.e12;

           for ( i=0 ; i<nTDCs ; ++i )
	     {
	       DRDatum::DigiType   newtdc  = rawhit.digiTimes()[i];
	       Time newtime = tdc2Time(wire,newtdc)-tOffset; 
	       if ( newtime > m_CDOffCal->getMinTightTime() && newtime < m_CDOffCal->getMaxTightTime() ) 
		 { if ( newtime < timeTight ) { timeTight = newtime ; tdcTight = newtdc ; wTdcTight = i; } }
               else 
		 {
		   if ( newtime > m_CDOffCal->getMinLooseTime() && newtime < m_CDOffCal->getMaxLooseTime() ) 
		     { if ( newtime < timeLoose ) { timeLoose = newtime ; tdcLoose = newtdc ; wTdcLoose = i; } } 
		   else
		     {
		       if ( newtime > m_CDOffCal->getMaxLooseTime() ) 
			 { if ( newtime < timeHigh ) { timeHigh  = newtime ; tdcHigh = newtdc ; wTdcHigh = i; } }
		       else
			 {
			   if ( newtime < m_CDOffCal->getMinLooseTime() ) 
			     { if ( newtime > timeLow ) { timeLow = newtime ; tdcLow = newtdc ; wTdcLow = i; } }
			 }
		     }
		 }
	     }
	   if ( wTdcTight > -1 ) 
	     { 
	       time = timeTight; 
	       tdc  = tdcTight; 
	       wTdc = wTdcTight; 
	       qhit.set_windowNarrow() ;
               wintype = 1 ; 
               // m_statistics->counter(7,"Narrow Time"); 
	     }
	   else
	     {
	       if ( wTdcLoose > -1 ) 
		 { 
		   time = timeLoose; 
		   tdc  = tdcLoose; 
		   wTdc = wTdcLoose; 
		   qhit.set_windowWide() ; 
                   wintype = 2 ; 
		   //m_statistics->counter(8,"Wide Time");
		 }
	       else
		 {
		   if ( wTdcHigh > -1 ) 
		     { 
		       time = timeHigh; 
		       tdc  = tdcHigh; 
		       wTdc = wTdcHigh;
		       qhit.set_windowHigh() ; 
                       wintype = 3 ; 
		       //m_statistics->counter(9,"High Time");
		     }
		   else
		     {
		       if ( wTdcLow > -1 ) 
			 { 
			   time = timeLow; 
			   tdc  = tdcLow; 
			   wTdc = wTdcLow;
			   qhit.set_windowLow() ; 
                           wintype = 4 ; 
			   //m_statistics->counter(10,"Low Time");
			 }
		       else
			 {			  
                           //m_statistics->counter(11,"Time Window Error");
			   report( ERROR, kFacilityString )  << "Error: no valid window decision!" << endl;
			 }
		     }
		 }
	     }
           if ( charge > 0 && nTDCs == 1 ) 
	     {
               time += charge2TimeCorr(time, charge) ;
	     }
	   dva  = time2DriftSeries(time, layer) ;
	   qhit.set_good() ;
	   weight   =  resolution(layer,dva.distance());
           weight   =  1. / (weight*weight);

	   // Determine the earlier and later time
 	   for( i=0 ; i<nTDCs ; ++i ) 
	   {  

	     // If this is the TDC we're using for the hit, skip
	     if ( i == wTdc ) continue;
	     
	     // Get the time of this TDC
	     DRDatum::DigiType   newtdc  = rawhit.digiTimes()[i];
	     Time newtime = tdc2Time(wire,newtdc)-tOffset;

	     // Compare to the time of the TDC we're using for the hit
	     float delta = time - newtime ;

	     // Is this hit earlier than the "standard" hit?
	     if ( delta > 0 )
	     { 
	       if ( delta < earlyGap || earlyGap < 0 ) 
	       { 
		 earlyGap = delta; 
	       } 
	     }
	     else 
	     { 
	       if ( -delta < lateGap || lateGap  < 0 ) 
	       { 
		 lateGap = -delta; 
	       } 
	     } // if ( delta > 0 )

	   }  // for ( over TDC's )

	 } // else from (if nTDC's == 0 )

     }  // if ( raw DAQId is ok )
   else
     {
       //m_statistics->counter(12,"test hits"); 
       qhit.set_testchannel();
     }
   if ( debugLevel[2] )
     {
       report( DEBUG, kFacilityString )  << "Wire#    " << wire           << endl
					 << "WireLyr  " << wireInLayer    << endl
					 << "Layer    " << layer          << endl   
					 << "Flag     " << qhit.value()   << endl
					 << "#TDC     " << nTDCs          << endl   
					 << "TDCused  " << tdc            << endl
					 << "charge   " << charge         << endl
					 << "time     " << time           << endl
					 << "drift    " << dva.distance() << endl
					 << "weight   " << weight         << endl
					 << "bunch    " << tOffset        << endl
					 << "wintype  " << wintype        << endl
					 << "earlyGap " << earlyGap       << endl
					 << "lateGap  " << lateGap        << endl
	                                 << "TDC#     " << wTdc           << endl;
     }
   return new CalibratedDRHit(wire, layer, wireInLayer, charge, time, dva, 
			      weight,qhit.value(), nTDCs, earlyGap, lateGap, wTdc, id);
}
//---------------------------------------------------------------------------
DriftFunction::Drift DriftFunction::time2Drift( Time time, 
						int layer ) const 
{
  return time2Drift ( time,  layer, kLRaveraged ) ;
}
//--------------------------------------------------------------------------
DriftFunction::Time DriftFunction::drift2Time( Drift drift, 
					       int layer ) const
{ 
  return drift2Time ( drift, layer, kLRaveraged ) ;
}
//---------------------------------------------------------------------------
Tseries DriftFunction::time2DriftSeries( Time time, 
					 int layer ) const
{
  //
  // For now drift velocity and curvature are not set
  //
  Tseries DVA(time2Drift(time,layer,kLRaveraged),0.0,0.0);
  return DVA;
}
//---------------------------------------------------------------------------
DriftFunction::Drift DriftFunction::time2SignedDrift( Time time, 
						      int layer, 
						      int ambiguity,
						      float eAngle,
						      float zHit ) const
{
  if ( ambiguity < 0 )
    { 
      return -time2Drift( time, layer, krightside )
             -time2DriftCorr  ( time, layer, 0, eAngle, zHit ) ;
    }
  else
    { 
      return +time2Drift( time, layer, kleftside  )
             -time2DriftCorr  ( time, layer, 1, eAngle, zHit ) ;
    }
}
//---------------------------------------------------------------------------
DriftFunction::Drift DriftFunction::time2SignedDrift( Time time, 
						      int layer, 
						      int ambiguity) const
{
  if ( ambiguity < 0 )
    { 
      return -time2Drift( time, layer, krightside ) ; 
    }
  else
    { 
      return  time2Drift( time, layer, kleftside  ) ; 
    }
}
//--------------------------------------------------------------------------
DriftFunction::Time DriftFunction::signeddrift2Time( Drift drift, 
						     int layer,
						     float eAngle,
						     float zHit ) const
{
  return signeddrift2Time( drift, layer ) - 
         drift2TimeCorr  ( drift, layer, eAngle, zHit ) ;
}
//--------------------------------------------------------------------------
DriftFunction::Time DriftFunction::signeddrift2Time( Drift drift, 
						     int layer ) const 
{
  if ( drift < 0 )
    { return drift2Time( -drift, layer, krightside ) ; }
  else
    { return drift2Time( drift, layer, kleftside  ) ; }
}
