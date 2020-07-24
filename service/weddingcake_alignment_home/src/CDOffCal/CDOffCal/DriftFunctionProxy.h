#if !defined(CDOFFCAL_DRIFTFUNCTIONPROXY_H)
#define CDOFFCAL_DRIFTFUNCTIONPROXY_H
// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      DriftFunctionProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner, Ken McLean
// Created:     Wed Mar  4 14:15:56 EST 1998
// $Id: DriftFunctionProxy.h,v 1.9 2000/02/02 15:33:30 cdj Exp $
//
// Revision history
//
// $Log: DriftFunctionProxy.h,v $
// Revision 1.9  2000/02/02 15:33:30  cdj
// -modernized: now inherits from ProxyTemplate
// -moved data needed by DriftFunctions to DFDataHolder class so that
//   DriftFunctions are no longer dependent on DriftFunctionProxy
//
// Revision 1.8  2000/02/01 22:51:03  mclean
// This update introduces constants from the frame, i.e. from the official
// database or from CDJ FileSourceFormat ascii files as the user chooses.
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
//   DriftFunction.h - added const functions
//        adcpedetal(CellID), adcpedestalRMS(CellID),tdctzero(CellID)
//   DriftFunctionProxy.h - added member data and data-access functions
//        for all the BDL/constants objects.
//
// Revision 1.7  1999/12/15 08:02:40  mclean
// The references to FAConstants entities are temporarily preprocessed out
// via flag REALCONSTANTS in DriftFunctionProxy.h
//
// Statistics.h configured for up to 200 counters.
//
// Revision 1.6  1999/12/07 23:29:38  mclean
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
// Revision 1.5  1999/06/23 22:17:57  lyon
// Chooses drift function base on parameters
//
//

// user include files
#include "CommandPattern/Parameter.h"
#include "DataHandler/ProxyTemplate.h"
#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h" 
#include "CDOffCal/DriftFunction.h"
#include "CDOffCal/DFDataHolder.h"

class CDOffCalProducer ;

class DriftFunctionProxy : public ProxyTemplate< DriftFunction >
{
  // friend classes and functions
  
public:
  // constants, enums and typedefs
  typedef DriftFunction value_type;
  
  // Constructors and destructor
  DriftFunctionProxy(CDOffCalProducer* );
  virtual ~DriftFunctionProxy();
  
  // member functions
  virtual void invalidateCache() ;
  virtual const value_type* faultHandler( const Record& aRecord,
					  const DataKey& aKey );
  

  // const member functions
  
  // static member functions
  
protected:
  // protected member functions
  
  // protected const member functions
  
private:
  // Constructors and destructor
  DriftFunctionProxy( const DriftFunctionProxy& );
  
  // assignment operator(s)
  const DriftFunctionProxy& operator=( const DriftFunctionProxy& );
  
  // private member functions
  
  // private const member functions
  
  // data members
  value_type*  m_DriftFunction ;
  CDOffCalProducer*  m_Producer;
  DFDataHolder m_dataHolder;
  // static data members
  
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "CDOffCal/Template/DriftFunctionProxy.cc"
//#endif

#endif /* CDOFFCAL_DRIFTFUNCTIONPROXY_H */





