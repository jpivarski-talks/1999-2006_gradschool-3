// -*- C++ -*-
//
// Package:     <ZDOffCal>
// Module:      ZDDriftFunctionProxy
// 
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDDriftFunctionProxy.cc,v 1.1 2003/04/24 18:08:10 huanggs Exp $
//
// Revision history
//
// waiting to see if necessary: ZDGarfield_DistanceCorrection 
//   ZDGarfield_TimeCorrection ZDWirePropagation
//
// $Log: ZDDriftFunctionProxy.cc,v $
// Revision 1.1  2003/04/24 18:08:10  huanggs
// add new files
//
// Revision 1.2  2003/04/24 14:51:51  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:57  huanggs
// imported source
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
#include "C3zd/ZdStream.h"
#include "ZDOffCal/ZDOffCalProducer.h"
#include "ZDOffCal/ZDDriftFunctionProxy.h"
#include "ZDOffCal/ZDDataDriftFunction.h" 
#include "DAException/DANoDataException.h"
#include "DataHandler/DHNoRecordException.h"

#include <vector>
#include <map>
#include <set>

//
//-----------------------------------------------------------------------------
//
static const char* const kFacilityString = "ZDOffCal.ZDDriftFunctionProxy" ;
//
//-----------------------------------------------------------------------------
//
ZDDriftFunctionProxy::ZDDriftFunctionProxy(ZDOffCalProducer *ZDOffCal): 
   m_DriftFunction(0),
   m_Producer(ZDOffCal)
{
  report(DEBUG,kFacilityString) << "constructor starts" << endl;
  report(DEBUG,kFacilityString) << "constructor ends" << endl;
} 
//
//-----------------------------------------------------------------------------
//
ZDDriftFunctionProxy::~ZDDriftFunctionProxy()
{
  report(DEBUG,kFacilityString) << "destructor starts" << endl;
  delete m_DriftFunction ;
  report(DEBUG,kFacilityString) << "destructor ends" << endl;
}
//
//-----------------------------------------------------------------------------
//
void ZDDriftFunctionProxy::invalidateCache()
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
const ZDDriftFunctionProxy::value_type*
ZDDriftFunctionProxy::faultHandler( const Record& iRecord,
				  const DataKey& iKey )
{
  // There should be no cached data at this point
  report(DEBUG,kFacilityString) << "faultHandler starts: No calibration...so make one!" << endl;
  assert(0 == m_DriftFunction);
  // Set frame pointers to well defined state 
  //
  // Make the drift function according to the parameter
  //
  if ( m_Producer->getType() ==  ZDDriftFunction::kDFType_NAIVE ) 
    { 
        report(ERROR,kFacilityString) << "Naive ZD Calibration Called, Disabled " << endl; 
        // m_DriftFunction = new NaiveDriftFunction(m_Producer,&m_dataHolder);
    }
  else
    if ( m_Producer->getType() ==  ZDDriftFunction::kDFType_HePr ) 
      { 
	report(ERROR,kFacilityString) << "HePr Old MC ZD Calibration Called, Disabled " << endl; 
        // m_DriftFunction = new HePrDriftFunction(m_Producer,&m_dataHolder);
      }
    else
      { 	 
        if ( m_Producer->getType() ==  ZDDriftFunction::kDFType_HePr_data1 ) 
	  {
	    report(DEBUG,kFacilityString) << "HePr Data/New ZD Calibration Called " << endl; 

	    report(DEBUG,kFacilityString) << "Extracting Geometry" << endl;
	    extract( (iRecord.frame()).record(Stream::kBaseGeometry  ), 
		     m_dataHolder.Geometry());

	    report(DEBUG,kFacilityString) << "Extracting Fitting Weight Corrections" << endl;
	    extract( (iRecord.frame()).record(ZdStream::kWeightDrift ), 
		     m_dataHolder.DriftWeight()     );
	    extract( (iRecord.frame()).record(ZdStream::kWeightLayer ), 
		     m_dataHolder.LayerWeight()     );
	    extract( (iRecord.frame()).record(ZdStream::kWeightPhi   ), 
		     m_dataHolder.PhiWeight()       );
	    extract( (iRecord.frame()).record(ZdStream::kWeightTheta ), 
		     m_dataHolder.ThetaWeight()     );

/*	    report(DEBUG,kFacilityString) << "Extracting Garfield Entrance Angle Corrections" << endl;
	    extract( (iRecord.frame()).record(ZdStream::kGarfield_DistanceCorrection ), 
		     m_dataHolder.DriftCorr() );
	    extract( (iRecord.frame()).record(ZdStream::kGarfield_TimeCorrection     ), 
		     m_dataHolder.TimeCorr()  );
hgs: waiting*/	    
	    report(DEBUG,kFacilityString) << "Extracting DaqID to CellID miscabling map" << endl;
	    extract( (iRecord.frame()).record(ZdStream::kDaqCellIDMap), 
		     m_dataHolder.WireMap()          );
	    
	    report(DEBUG,kFacilityString) << "Extracting Bad Wire Map" << endl;
	    extract( (iRecord.frame()).record(ZdStream::kWireQuality ), 
		     m_dataHolder.WireQuality()         );
	    
	    report(DEBUG,kFacilityString) << "Extracting ADC to charge constants" << endl;
	    extract( (iRecord.frame()).record(ZdStream::kadc2charge  ), 
		     m_dataHolder.ADCpar()    );

            //
            // Unpack ZDDriftData so member data in ZDDFDataHolder is well
            // defined and ZDDataDriftFunction can test it with .valid()
            //
            //
            DABoolean driftdata_BAD(false);
	    try
	      {
		report(INFO,kFacilityString) << "Extracting DriftData Constant Object" << endl;
		extract( (iRecord.frame()).record(ZdStream::kDriftData   ), 
			 m_dataHolder.DriftData()        );
	      }
	    catch(DHNoRecordException& aException) 
              //catch( DAExceptionBase& aException) 
	      //catch( NO_CONSTANTS_EXCEPTION( DBZDDriftData )& aException )
	      {
		driftdata_BAD = true ;
		report(WARNING,kFacilityString) << aException.what() << endl;
	        report(WARNING,kFacilityString) << "DriftData Constant Object Not Found" << endl;
	      }
	    //
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
		    m_Producer->signalPropBeta_Set( ZDDriftFunction::kdefault_SignalProp );
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

            /*hgs: waiting
		try
		  {
		    extract( (iRecord.frame()).record(ZdStream::kWirePropagation),m_dataHolder.WirePropagation());
		    m_Producer->signalPropBeta_Set( m_dataHolder.WirePropagation()[1].get_float1() ) ;
		    report(INFO,kFacilityString) << "Signal Propagation taken from Constants Database " 
						 << m_Producer->signalPropBeta_Val()
						 << endl;
		  }

		catch( DHNoRecordException& aException) 
		  {
                    signalpropagation_BAD = true ;
		    m_Producer->signalPropBeta_Set( ZDDriftFunction::kdefault_SignalProp ) ;
		    report(WARNING,kFacilityString) << aException.what() << endl;
		    report(WARNING,kFacilityString) << "DataBase Extract Exception: Signal Propagation set to default "
						    << m_Producer->signalPropBeta_Val()
						    << endl;
		  }
            hgs: waiting*/

	      }
//hgs: use default
	    m_Producer->signalPropBeta_Set( ZDDriftFunction::kdefault_SignalProp );
	    
	    report(DEBUG,kFacilityString) << "Extracting ADC Pedestal Constants" << endl;
	    extract( (iRecord.frame()).record(ZdStream::kadcpedestal ), 
		     m_dataHolder.ADCped()     );
	    
	    report(DEBUG,kFacilityString) << "Extracting T0s by channel" << endl;
	    extract( (iRecord.frame()).record(ZdStream::kt0channel ), 
		     m_dataHolder.T0Channel() );
	    
	    report(DEBUG,kFacilityString) << "Extracting T0s by card" << endl;
	    extract( (iRecord.frame()).record(ZdStream::kt0card      ), 
		     m_dataHolder.T0Card()           );
	    
	    report(DEBUG,kFacilityString) << "Extracting T0s by card" << endl;
	    extract( (iRecord.frame()).record(ZdStream::kt0hvcard    ), 
		     m_dataHolder.T0HVCard()         );
	    
	    report(DEBUG,kFacilityString) << "Extracting T0s by crate" << endl;
	    extract( (iRecord.frame()).record(ZdStream::kt0crate     ), 
		     m_dataHolder.T0Crate()          );

	    report(DEBUG,kFacilityString) << "Extracting Global T0" << endl;
	    extract( (iRecord.frame()).record(ZdStream::kt0global    ), 
		     m_dataHolder.T0Global()         );

//hgs:waiting	    if ( driftdata_BAD || signalpropagation_BAD )
	    if ( driftdata_BAD )
	      {
		report(EMERGENCY,kFacilityString) << "Constants Missing" << endl; 
	      }
	    else
	      {
		report(DEBUG,kFacilityString) << "HePr Data/New ZD Calibration OK " << endl; 
		m_DriftFunction = new ZDDataDriftFunction(m_Producer,
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
