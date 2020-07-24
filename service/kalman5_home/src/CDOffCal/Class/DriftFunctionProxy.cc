// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      DriftFunctionProxy
// 
// Author:      Ken McLean
// Created:     Wed Mar  4 14:15:57 EST 1998
// $Id: DriftFunctionProxy.cc,v 1.15 2000/11/19 03:38:30 cdj Exp $
//
// Revision history
//
// $Log: DriftFunctionProxy.cc,v $
// Revision 1.15  2000/11/19 03:38:30  cdj
// if can't find constants, now throws exception instead of aborting
//
// Revision 1.14  2000/10/31 23:09:32  mclean
// *** empty log message ***
//
// Revision 1.13  2000/05/12 16:34:05  mclean
// Added t0global constant.
//
// Revision 1.12  2000/03/29 22:33:12  mclean
// Added hooks for drt0channelv2 and drt0hvcard
//
// Revision 1.11  2000/02/02 15:33:49  cdj
// -modernized: now inherits from ProxyTemplate
// -moved data needed by DriftFunctions to DFDataHolder class so that
//   DriftFunctions are no longer dependent on DriftFunctionProxy
//
// Revision 1.10  2000/02/01 23:03:17  mclean
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
//       **DBDRDriftData
// 	 *DBDRadcpedestal
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
// Revision 1.9  1999/12/15 08:07:08  mclean
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
// Revision 1.8  1999/12/07 23:30:08  mclean
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
// Revision 1.7  1999/06/23 22:21:24  lyon
// Altered to handle drift function selection
//
// Revision 1.6  1998/12/15 21:03:47  mclean
// First working HePr DF code (hardwired, tcl parameters not enabled, not for
//                             general use)
//
// Revision 1.5  1998/12/10 21:43:50  mclean
// Updates for HePr drift functions (old analytic DFs are still default)
//
// Revision 1.3  1998/04/24 20:30:41  mclean
// First Real Code Installed
//
// Revision 1.1.1.1  1998/03/04 20:41:49  mkl
// imported CDOffCal sources
//
//

#include "Experiment/Experiment.h"
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>

// User include files
#include "Experiment/report.h"
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"
#include "C3dr/DrStream.h"
#include "CDOffCal/CDOffCalProducer.h"
#include "CDOffCal/DriftFunctionProxy.h"
#include "CDOffCal/DataDriftFunction.h" 
#include "DAException/DANoDataException.h"
#include "DataHandler/DHNoRecordException.h"

#include <vector>
#include <map>
#include <set>

//
//-----------------------------------------------------------------------------
//
static const char* const kFacilityString = "CDOffCal.DriftFunctionProxy" ;
//
//-----------------------------------------------------------------------------
//
DriftFunctionProxy::DriftFunctionProxy(CDOffCalProducer *CDOffCal): 
   m_DriftFunction(0),
   m_Producer(CDOffCal)
{
  report(DEBUG,kFacilityString) << "constructor starts" << endl;
  report(DEBUG,kFacilityString) << "constructor ends" << endl;
} 
//
//-----------------------------------------------------------------------------
//
DriftFunctionProxy::~DriftFunctionProxy()
{
  report(DEBUG,kFacilityString) << "destructor starts" << endl;
  delete m_DriftFunction ;
  report(DEBUG,kFacilityString) << "destructor ends" << endl;
}
//
//-----------------------------------------------------------------------------
//
void DriftFunctionProxy::invalidateCache()
{
  report(DEBUG,kFacilityString) << "destroying calibration" << endl;
  delete m_DriftFunction;
  m_DriftFunction = 0 ;
  report(DEBUG,kFacilityString) << "calibration destroyed" << endl;
}
//-----------------------------------------------------------------------------
//
//  Fault handler for the proxy 
//
const DriftFunctionProxy::value_type*
DriftFunctionProxy::faultHandler( const Record& iRecord,
				  const DataKey& iKey )
{
  // There should be no cached data at this point
  report(DEBUG,kFacilityString) << "faultHandler starts: No calibration...so make one!" << endl;
  assert(0 == m_DriftFunction);
  // Set frame pointers to well defined state 
  //
  // Make the drift function according to the parameter
  //
  if ( m_Producer->getType() ==  DriftFunction::kDFType_NAIVE ) 
    { 
        report(ERROR,kFacilityString) << "Naive DR Calibration Called, Disabled " << endl; 
        // m_DriftFunction = new NaiveDriftFunction(m_Producer,&m_dataHolder);
    }
  else
    if ( m_Producer->getType() ==  DriftFunction::kDFType_HePr ) 
      { 
	report(ERROR,kFacilityString) << "HePr Old MC DR Calibration Called, Disabled " << endl; 
        // m_DriftFunction = new HePrDriftFunction(m_Producer,&m_dataHolder);
      }
    else
      { 	 
        if ( m_Producer->getType() ==  DriftFunction::kDFType_HePr_data1 ) 
	  {
	    report(DEBUG,kFacilityString) << "HePr Data/New DR Calibration Called " << endl; 

	    report(DEBUG,kFacilityString) << "Extracting Geometry" << endl;
	    extract( (iRecord.frame()).record(Stream::kBaseGeometry  ), 
		     m_dataHolder.Geometry());

	    report(DEBUG,kFacilityString) << "Extracting Fitting Weight Corrections" << endl;
	    extract( (iRecord.frame()).record(DrStream::kWeightDrift ), 
		     m_dataHolder.DriftWeight()     );
	    extract( (iRecord.frame()).record(DrStream::kWeightLayer ), 
		     m_dataHolder.LayerWeight()     );
	    extract( (iRecord.frame()).record(DrStream::kWeightPhi   ), 
		     m_dataHolder.PhiWeight()       );
	    extract( (iRecord.frame()).record(DrStream::kWeightTheta ), 
		     m_dataHolder.ThetaWeight()     );

	    report(DEBUG,kFacilityString) << "Extracting Garfield Entrance Angle Corrections" << endl;
	    extract( (iRecord.frame()).record(DrStream::kGarfield_DistanceCorrection ), 
		     m_dataHolder.DriftCorr() );
	    extract( (iRecord.frame()).record(DrStream::kGarfield_TimeCorrection     ), 
		     m_dataHolder.TimeCorr()  );
	    
	    report(DEBUG,kFacilityString) << "Extracting DaqID to CellID miscabling map" << endl;
	    extract( (iRecord.frame()).record(DrStream::kDaqCellIDMap), 
		     m_dataHolder.WireMap()          );
	    
	    report(DEBUG,kFacilityString) << "Extracting Bad Wire Map" << endl;
	    extract( (iRecord.frame()).record(DrStream::kWireQuality ), 
		     m_dataHolder.WireQuality()         );
	    
	    report(DEBUG,kFacilityString) << "Extracting TDC to time constants" << endl;
	    extract( (iRecord.frame()).record(DrStream::ktdc2time    ), 
		     m_dataHolder.TDCpar()    );
	    
	    report(DEBUG,kFacilityString) << "Extracting ADC to charge constants" << endl;
	    extract( (iRecord.frame()).record(DrStream::kadc2charge  ), 
		     m_dataHolder.ADCpar()    );

            //
            // Unpack both DriftData and DriftDatav2 so member 
            // data in DFDataHolder is well defined and DataDriftFunction 
            // can test both with .valid()
            //
            //
            DABoolean driftdatav2_BAD(false);
	    try
	      {
		report(INFO,kFacilityString) << "Extracting DriftDatav2 Constant Object" << endl;
		extract( (iRecord.frame()).record(DrStream::kDriftDatav2   ), 
			 m_dataHolder.DriftDatav2()        );
	      }
	    catch(DHNoRecordException& aException) 
              //catch( DAExceptionBase& aException) 
	      //catch( NO_CONSTANTS_EXCEPTION( DBDRDriftDatav2 )& aException )
	      {
		driftdatav2_BAD = true ;
		report(WARNING,kFacilityString) << aException.what() << endl;
	        report(WARNING,kFacilityString) << "DriftDatav2 Constant Object Not Found" << endl;
	      }
	    //
            DABoolean driftdata_BAD(false);
	    try
	      {
		report(INFO,kFacilityString) << "Extracting DriftData Constant Object" << endl;
		extract( (iRecord.frame()).record(DrStream::kDriftData   ), 
			 m_dataHolder.DriftData()        );
	      }
	    catch(DHNoRecordException& aException) 
	      //catch( DAExceptionBase& aException) 
	      //catch( NO_CONSTANTS_EXCEPTION( DBDRDriftData )& aException )
	      {
		driftdata_BAD = true ;
		report(WARNING,kFacilityString) << aException.what() << endl;
	        report(WARNING,kFacilityString) << "DriftData Constant Object Not Found" << endl;
	      }

	    
            // 
            // The default parameter value is -0.5 
	    // If this is unchanged we look in the database
	    // If the database is empty we use the default
            //
            DABoolean signalpropagation_BAD(false);
            if ( m_Producer->signalPropBeta_Par() > 0. )
	      {
                if (  m_Producer->signalPropBeta_Par() > 1. )
		  {
                    signalpropagation_BAD = true ;
		    m_Producer->signalPropBeta_Set( DriftFunction::kdefault_SignalProp );
		    report(WARNING,kFacilityString) << "Invalid User Parameter: Signal Propagation set to default "
						    << m_Producer->signalPropBeta_Val()
						    << endl;
		  }
		else
		  {
		    m_Producer->signalPropBeta_Set( m_Producer->signalPropBeta_Par() );
		    report(INFO,kFacilityString) << "Signal Propagation User Parameter used " 
						 << m_Producer->signalPropBeta_Val()
						 << endl;
		  }
	      } 
	    else
	      {
		try
		  {
		    extract( (iRecord.frame()).record(DrStream::kvarious),m_dataHolder.various());
		    m_Producer->signalPropBeta_Set( m_dataHolder.various()[1].get_float1() ) ;
		    report(INFO,kFacilityString) << "Signal Propagation taken from Constants Database " 
						 << m_Producer->signalPropBeta_Val()
						 << endl;
		  }
		catch( DHNoRecordException& aException) 
		  {
                    signalpropagation_BAD = true ;
		    m_Producer->signalPropBeta_Set( DriftFunction::kdefault_SignalProp ) ;
		    report(WARNING,kFacilityString) << aException.what() << endl;
		    report(WARNING,kFacilityString) << "DataBase Extract Exception: Signal Propagation set to default "
						    << m_Producer->signalPropBeta_Val()
						    << endl;
		  }
	      }
	    
	    report(DEBUG,kFacilityString) << "Extracting ADC Pedestal Constants" << endl;
	    extract( (iRecord.frame()).record(DrStream::kadcpedestal ), 
		     m_dataHolder.ADCped()     );
	    
	    report(DEBUG,kFacilityString) << "Extracting T0s by channel" << endl;
	    extract( (iRecord.frame()).record(DrStream::kt0channelv2 ), 
		     m_dataHolder.T0Channel() );
	    
	    report(DEBUG,kFacilityString) << "Extracting T0s by card" << endl;
	    extract( (iRecord.frame()).record(DrStream::kt0card      ), 
		     m_dataHolder.T0Card()           );
	    
	    report(DEBUG,kFacilityString) << "Extracting T0s by card" << endl;
	    extract( (iRecord.frame()).record(DrStream::kt0hvcard    ), 
		     m_dataHolder.T0HVCard()         );
	    
	    report(DEBUG,kFacilityString) << "Extracting T0s by crate" << endl;
	    extract( (iRecord.frame()).record(DrStream::kt0crate     ), 
		     m_dataHolder.T0Crate()          );

	    report(DEBUG,kFacilityString) << "Extracting Global T0" << endl;
	    extract( (iRecord.frame()).record(DrStream::kt0global    ), 
		     m_dataHolder.T0Global()         );
	    
	    if ( ( driftdatav2_BAD && driftdata_BAD ) || signalpropagation_BAD )
	      {
		report(EMERGENCY,kFacilityString) << "Constants Missing" << endl; 
	      }
	    else
	      {
		report(DEBUG,kFacilityString) << "HePr Data/New DR Calibration OK " << endl; 
		m_DriftFunction = new DataDriftFunction(m_Producer,
							&m_dataHolder);
	      }
	  }
	else
	  {
	    report(ERROR,kFacilityString) << "Invalid Drift Function Parameter, option not defined" << endl;
	  }
      }
  report(DEBUG,kFacilityString) << "faulthandler ends" << endl;
  return m_DriftFunction;
}






