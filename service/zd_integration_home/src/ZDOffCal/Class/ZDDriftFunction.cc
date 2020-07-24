// -*- C++ -*-
//
// Package:     <ZDOffCal>
// Module:      ZDDriftFunction
// 
// Description: This is the root class for ZDDriftFunction classes
//
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDDriftFunction.cc,v 1.1 2003/04/24 18:08:10 huanggs Exp $
//
// Revision history
//
// For the ZD, we have only one version ZDt0channel, and this is an
// equivalent of DRt0channelv2. We don't need ZDtdc2time, since the
// slope is same for all TDC channels.
//
// $Log: ZDDriftFunction.cc,v $
// Revision 1.1  2003/04/24 18:08:10  huanggs
// add new files
//
// Revision 1.2  2003/04/24 14:51:50  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:57  huanggs
// imported source
//

// compiler bug flags
#include "Experiment/Experiment.h"

// system include files
#include "C++Std/fstream.h"
#include "C++Std/iostream.h"

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
#include "ZDOffCal/ZDOffCalProducer.h"
#include "ZDOffCal/ZDDriftFunction.h"
#include "ZDOffCal/ZDDriftFunctionProxy.h"
#include "ZDOffCal/ZDHitQuality.h"
#include "ZDOffCal/ZDStatistics.h"
#include "CalibratedData/Tseries.h"
#include "CalibratedData/CalibratedZDHit.h"
#include "AZDGeom/AZDSenseWireStore.h"

#include <vector>
#include <map>
#include <set>

static const char* const kFacilityString   = "ZDOffCal.ZDDriftFunction" ;
const double ZDDriftFunction::kdefault_MinTightTime =  -30000. ;
const double ZDDriftFunction::kdefault_MaxTightTime =  350000. ;
const double ZDDriftFunction::kdefault_MinLooseTime =  -50000. ;
const double ZDDriftFunction::kdefault_MaxLooseTime =  600000. ;
const double ZDDriftFunction::kdefault_GlobalT0     =       0. ;
const double ZDDriftFunction::kdefault_TDCQuantum   =    -500. ;
const double ZDDriftFunction::kdefault_Resolution   = 0.000123 ; // 1/123microns**2
const double ZDDriftFunction::kdefault_SignalProp   = 0.5      ; 
const double ZDDriftFunction::kclight        = 3.0e-4      ;
const UInt16 ZDDriftFunction::kHighTDC       = 16384       ; 

//----------------------------------------------------------------------------
//
ZDDriftFunction::ZDDriftFunction(ZDOffCalProducer* ZDOffCal,
			     ZDDFDataHolder* iDataHolder) :
   m_dataHolder( iDataHolder ),
   m_ZDOffCal( ZDOffCal )
{
  report( DEBUG, kFacilityString )  << "Constructor Starts " << endl;
  //-----------------------------------------------------------------------------------------------------------
  //
  // These are now used as function calls to the producer,
  // this allows parameters to be changed "on the fly"
  // 
  //m_Mintime    = ZDOffCal->getMinimumTime();
  //m_Maxtime    = ZDOffCal->getMaximumTime();
  //m_TDCQuantum = ZDOffCal->getTDCQuantum();
  //m_GlobalT0   = ZDOffCal->getGlobalT0();

  //-----------------------------------------------------------------------------------------------------------
  //
  // This enables the set of statistics counters
  // these have been mostly commented out because
  // the code is slow because of the way it 
  // handles the character arguments
  //
  m_statistics = new ZDStatistics;
  m_statistics->counter(0,"ZDOffCal Calibration Statistics"); 

  //-----------------------------------------------------------------------------------------------------------
  //
  // Oldstyle drift functions (NaiveDriftFunction and
  // HePrDriftFunction) are obsolete and deleted.
  //
  if ( m_ZDOffCal->getType() < ZDDriftFunction::kDFType_HePr_data1 ) 
    { 
      report( EMERGENCY, kFacilityString )  << "Invalid DriftFunctionType Parameter: " 
					    <<  m_ZDOffCal->getType() 
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
      debugLevel[level] = ( m_ZDOffCal->getDebugLevel() == -level || 
	                    m_ZDOffCal->getDebugLevel() >=  level  ) ;
    }
  //-----------------------------------------------------------------------------------------------------------
  // Unpack Fitting Weights 
  //
  report( DEBUG, kFacilityString )  << "Trying to get Fitting Weights  " << endl;
  report( INFO , kFacilityString )  << "Global scale (user parameter): " << m_ZDOffCal->getResolution() << endl;
  //
  //-----------------------------------------------------------------------------------------------------------
  //
  //       Layer Correction
  //
  float sum   = 0. ;
  float count = 0. ;

  memset ( m_LayerWeight, 1., sizeof(m_LayerWeight) ) ; // set default value

  for(FAConstants<DBZDWeightLayer>::const_iterator 
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
      if ( layer > 0 && layer <= kmaxZDlayer )
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


  if ( count != kmaxZDlayer ) 
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

  for(FAConstants<DBZDWeightDrift>::const_iterator 
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
	  if ( layer > kmaxZDlayer ) 
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

  if ( count != ( kmaxZDlayer * m_DriftWeight_Nbins )  ) 
    {
      report( ERROR , kFacilityString )  << " Drift Fitting Weights Incomplete, #found= " \
					 << count 
					 << " #expected= "  << ( kmaxZDlayer * m_DriftWeight_Nbins )
					 << endl;
    }

  //-----------------------------------------------------------------------------------------------------------
  //
  //    cos(theta) Correction For Each Layer 
  //
  sum   = 0. ;
  count = 0. ;

  memset ( m_ThetaWeight, 1., sizeof(m_ThetaWeight) ) ; // set default value

  for(FAConstants<DBZDWeightTheta>::const_iterator 
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
	  if ( layer > kmaxZDlayer ) 
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

  if ( count != ( kmaxZDlayer * m_ThetaWeight_Nbins )  ) 
    {
      report( ERROR , kFacilityString )  << " Theta Fitting Weights Incomplete, #found= " 
					 << count
					 << " #expected= "  << ( kmaxZDlayer * m_ThetaWeight_Nbins )
					 << endl;
    }
  //-----------------------------------------------------------------------------------------------------------
  //
  //    Phi Correction For Each Layer 
  //
  sum   = 0. ;
  count = 0. ;

  memset ( m_PhiWeight, 1., sizeof(m_PhiWeight) ) ; // set default value

  for(FAConstants<DBZDWeightPhi>::const_iterator 
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
	  if ( layer > kmaxZDlayer ) 
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

  if ( count != ( kmaxZDlayer * m_PhiWeight_Nbins )  ) 
    {
      report( ERROR , kFacilityString )  << " Phi Fitting Weights Incomplete, #found= " 
					 << count 
					 << " #expected= "  << ( kmaxZDlayer * m_PhiWeight_Nbins )
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
				    << m_ZDOffCal->getGlobalT0() << endl;
  report( INFO , kFacilityString )  << "Global T0 offset 2 (constants)     : " 
				    << m_dataHolder->T0Global()[1].get_tzero() << endl;

//  report( INFO , kFacilityString )  << "hgs: sizeof(m_channelT0)=" 
//                                    << sizeof(m_channelT0) << endl;

  memset ( m_channelT0, imagic, sizeof(m_channelT0) ) ;

//  report( INFO , kFacilityString )  << "hgs: memset finished" << endl;

  for(FAConstants<DBZDt0channel>::const_iterator 
	t0loop =m_dataHolder->T0Channel().begin();
        t0loop!=m_dataHolder->T0Channel().end()  ;
      ++t0loop ) 
    {

//  report( INFO , kFacilityString )  << "hgs: inside loop:" << t0loop << endl;
//  report( INFO , kFacilityString )  << "hgs: wire# ="
//                                    << (*t0loop).get_indexNumber() << endl;

      int layer = m_dataHolder->Geometry()->layerWire( (*t0loop).get_indexNumber()).first ;

//  report( INFO , kFacilityString )  << "hgs: layer=" << layer << endl;

/*report(INFO,kFacilityString )<< "hgs,a:"<< (*t0loop).get_t0CommonMode()<<endl;
report(INFO,kFacilityString )<< "hgs,b:"<< (*t0loop).get_t0Channel()<<endl;
report(INFO,kFacilityString )<< "hgs,c:"<<
       m_dataHolder->T0Crate()[(*t0loop).get_crate ()].get_tzero() <<endl;
report(INFO,kFacilityString )<< "hgs,d:"<<
       m_dataHolder->T0Card  ()[(*t0loop).get_card  ()].get_tzero() << endl;
report(INFO,kFacilityString )<< "hgs,e:"<<
       m_dataHolder->T0HVCard()[(*t0loop).get_hvcard()].get_tzero() << endl;
report(INFO,kFacilityString )<< "hgs,f:"<<
       m_dataHolder->T0Global()[1].get_tzero() << endl;
report(INFO,kFacilityString )<< "hgs,g:"<<m_ZDOffCal->getGlobalT0() << endl;
report(INFO,kFacilityString )<< "hgs,h:"<<ZShift ( layer ) << endl;
report(INFO,kFacilityString )<< "hgs,i:"<<
       ( m_dataHolder->Geometry()->radiusNominal( layer ) / kclight ) 
       << "hgs#" <<endl;
*/

      m_channelT0[(*t0loop).get_indexNumber()-1] 
        = (*t0loop).get_t0CommonMode()
	+ (*t0loop).get_t0Channel   ()
	+ m_dataHolder->T0Crate ()[(*t0loop).get_crate ()].get_tzero()
	+ m_dataHolder->T0Card  ()[(*t0loop).get_card  ()].get_tzero()
	+ m_dataHolder->T0HVCard()[(*t0loop).get_hvcard()].get_tzero()
	+ m_dataHolder->T0Global()[1].get_tzero()
	+ m_ZDOffCal->getGlobalT0() 
	+ ZShift ( layer ) 
	- ( m_dataHolder->Geometry()->radiusNominal( layer ) / kclight ) ;
    }

  int nt0err = 0 ;
  for ( i=0; i<kTotalZDWires; ++i ) 
    { 
      if ( m_channelT0[i] == magic )
	{ 
	  nt0err++ ; 
	  report(ERROR,kFacilityString) << " could not load wire " << (i+1) << " of ZDt0channel" << endl;
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
  for(FAConstants<DBZDt0global>::const_iterator 
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

  for(FAConstants<DBZDadcpedestal>::const_iterator 
	adcloop =m_dataHolder->ADCped().begin();
        adcloop!=m_dataHolder->ADCped().end()  ;
      ++adcloop ) 
    {
//report(INFO, kFacilityString) << "hgs:" << (*adcloop).get_CellID()-1 << " "
//                              << (*adcloop).get_pedestal() << endl;

      m_channelPed   [(*adcloop).get_CellID()-1]
	=(*adcloop).get_pedestal();
      m_channelPedRMS[(*adcloop).get_CellID()-1]
	=(*adcloop).get_pedestal_RMS();
    }

  int npederr = 0 ;
  for ( i=0; i<kTotalZDWires; ++i ) 
    { 
      if ( m_channelPed[i] == magic ) 
	{ 
	  npederr++ ; 
	  report(ERROR,kFacilityString) << " could not load wire " << (i+1) << " of ZDadcpedestal" << endl;
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
  
  for(FAConstants<DBZDWireQuality>::const_iterator 
	qloop =m_dataHolder->WireQuality().begin();
        qloop!=m_dataHolder->WireQuality().end()  ;
      ++qloop ) 
    {	  
      m_quality[(*qloop).get_CellID()-1]=(*qloop).get_Quality();
    }

  int nqerr = 0 ;
  for ( i=0; i<kTotalZDWires; ++i ) 
    { 
      if ( m_quality[i] == imagic ) 
	{ 
	  nqerr++ ; 
	  report(ERROR,kFacilityString) << " could not load wire " << (i+1) << " of ZDWireQuality" << endl;
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
  for ( i=0; i<kTotalZDWires; ++i ) 
    { 
      m_daqmap[i]    = kTotalZDWires+1 ; 
      m_invdaqmap[i] = kTotalZDWires+1 ; 
    }

  for(FAConstants<DBZDDaqCellIDMap>::const_iterator 
	wloop =m_dataHolder->WireMap().begin();
        wloop!=m_dataHolder->WireMap().end();
      ++wloop ) 
    {
         m_daqmap[(*wloop).get_DaqId() -1]=(*wloop).get_CellId();
      m_invdaqmap[(*wloop).get_CellId()-1]=(*wloop).get_DaqId();
    }

  int nwerr = 0 ;
  for ( i=0; i<kTotalZDWires; ++i ) 
    { 
      if ( m_daqmap[i] == kTotalZDWires+1 ) 
	{ 
	  nwerr++ ; 
	} 
    }

  int nwerr2 = 0 ;
  for ( i=0; i<kTotalZDWires; ++i ) 
    { 
      if ( m_invdaqmap[i] == kTotalZDWires+1 ) 
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
 
  for(FAConstants<DBZDadc2charge>::const_iterator 
	gloop =m_dataHolder->ADCpar().begin();
        gloop!=m_dataHolder->ADCpar().end()  ;
      ++gloop ) 
    {
      m_adcgain   [(*gloop).get_CellID()-1]=(*gloop).get_slope();
      m_adcgainRMS[(*gloop).get_CellID()-1]=(*gloop).get_slope_error();
    }

  int ngerr = 0 ;
  for( i=0; i<kTotalZDWires; ++i ) 
    { 
      if ( m_adcgain[i] == magic ) 
	{ 
	  ngerr++ ;
 	  report(ERROR,kFacilityString) << " could not load wire " << (i+1) << " of ZDadc2charge" << endl;
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
  for( i=0; i<kTotalZDWires; ++i ) 
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
  // Set Wire TDC Slope Information rather than unpack from database
  //
  report( INFO, kFacilityString )  << "Trying to set TDC Slopes" << endl;

  for( i=0; i<kTotalZDWires; ++i ) 
    { 
       m_tdcslope[i] = 500. ; //hgs: naive, in ps
       m_tdcslopeRMS[i] = 50. ; //hgs: naive
       m_tdcped[i] = 500. ; //hgs: naive
       m_tdcpedRMS[i] =50. ; //hgs: naive
    } 

  report( INFO, kFacilityString )  << "Constructor Ends " << endl;
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::~ZDDriftFunction()
{
  report( DEBUG, kFacilityString )  << "Destructor Starts " << endl;
  delete m_statistics;
  report( DEBUG, kFacilityString )  << "Destructor Ends " << endl;
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Charge 
ZDDriftFunction::adcpedestal(UInt16 wire) const 
{ 
//report(INFO, kFacilityString ) << "hgs: wire=" << wire << endl;
//report(INFO, kFacilityString ) << "hgs: m_channelPed=" << m_channelPed << endl;
  return m_channelPed[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Charge 
ZDDriftFunction::adcpedestalRMS(UInt16 wire) const 
{ 
  return m_channelPedRMS[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Charge
ZDDriftFunction::adcgain(UInt16 wire) const 
{ 
  return m_adcgain[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Charge
ZDDriftFunction::adcgainRMS(UInt16 wire) const 
{ 
  return m_adcgainRMS[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Time
ZDDriftFunction::tdcslope(UInt16 wire) const 
{ 
  return m_tdcslope[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Time
ZDDriftFunction::tdcslopeRMS(UInt16 wire) const 
{ 
  return m_tdcslopeRMS[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Time
ZDDriftFunction::tdcped(UInt16 wire) const 
{ 
  return m_tdcped[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Time
ZDDriftFunction::tdcpedRMS(UInt16 wire) const 
{ 
  return m_tdcpedRMS[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Time 
ZDDriftFunction::tdctzero(UInt16 wire) const 
{ 
  return m_channelT0[wire-1];
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Drift 
ZDDriftFunction::resolution(int layer, 
			  ZDDriftFunction::Drift drift, 
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
ZDDriftFunction::Drift 
ZDDriftFunction::resolution(int layer, ZDDriftFunction::Drift drift, float cTheta) const 
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
ZDDriftFunction::Drift 
ZDDriftFunction::resolution(int layer, ZDDriftFunction::Drift driftinmeters) const 
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
ZDDriftFunction::Drift 
ZDDriftFunction::resolution(int layer) const 
{ 
  if ( layer < 1 || layer > kmaxZDlayer ) 
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
ZDDriftFunction::Drift 
ZDDriftFunction::resolution() const 
{ 
  return m_ZDOffCal->getResolution();
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Time ZDDriftFunction::badwiremap(UInt16 wire) const 
{ 
  return m_quality[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Charge ZDDriftFunction::adc2Charge( UInt16 wire, 
						 ZDDatum::DigiType adc) const 
{ 
  return Charge ( m_adcgain[wire-1] * ( adc - m_channelPed[wire-1] ) ); 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Charge ZDDriftFunction::adcwithped( UInt16 wire, 
						 ZDDatum::DigiType adc) const 
{ 
  //just pedestal correction
  return Charge ( adc - m_channelPed[wire-1] ); 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Time ZDDriftFunction::tdc2Time( UInt16 wire, 
					     ZDDatum::DigiType tdc ) const
{  
  // verify TDC slope is OK at constants unpack
  return Time ( m_tdcslope[wire-1] * double ( tdc ) ) + m_channelT0[wire-1];
}
//
//----------------------------------------------------------------------------
//
ZDDatum::DigiType ZDDriftFunction::charge2adc( UInt16 wire, 
					     Charge PulseHeight ) const
{ 
  return ZDDatum::DigiType ( ( PulseHeight / m_adcgain[wire-1] ) + m_channelPed[wire-1] ); 
}
//
//----------------------------------------------------------------------------
//
ZDDatum::DigiType ZDDriftFunction::time2tdc( UInt16 wire, 
					   Time time) const
{  
  // verify TDC slope is OK at constants unpack
  return  ( ( time - m_channelT0[wire-1] ) / m_tdcslope[wire-1] );
}
ZDDatum::DigiType ZDDriftFunction::time2tdc( UInt16 wire,
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
UInt16 ZDDriftFunction::daqID2CellID( UInt16 daqID ) const
{ 
  return m_daqmap[daqID-1]; 
}
//---------------------------------------------------------------------------
//
// CellID2daqID( CellID )
//
//  Convert a CellID2daqId (take care of miscablings, trivial right now)
//
UInt16 ZDDriftFunction::CellID2daqID( UInt16 CellID ) const
{ 
  return m_invdaqmap[CellID-1]; 
}
//---------------------------------------------------------------------------
//
// ZCorr( )
//
ZDDriftFunction::Time ZDDriftFunction::Zcorr( int layer, float dz ) const
{
  float z = dz ;
  if ( m_dataHolder->Geometry()->layer( layer ).readoutEnd() == 
       AEWireLayer::kReadoutEast ) 
    { 
      z *= -1.;
    }
//report(INFO,kFacilityString )<< "hgs: in Zcorr" << endl;
//report(INFO,kFacilityString )<< "hgs: in Zcorr "
//  << m_ZDOffCal->signalPropBeta_Val() << " " << kclight << endl;

  return (z/(m_ZDOffCal->signalPropBeta_Val()*kclight));
  //
  // If this was my world I would define the geometry so one could
  // simply do:
  /*
    return (dz*m_dataHolder->Geometry()->layer( layer ).readoutSign()/
            (m_ZDOffCal->signalPropBeta_Val()*kclight));
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
ZDDriftFunction::Time ZDDriftFunction::ZShift( int layer ) const
{
//report(INFO,kFacilityString )<< "hgs: in ZShift:a, layer=" << layer << endl;
  // Shift time to z=0 AFTER the raw time has been checked above.
  if( m_ZDOffCal->shiftTimeToCenter() )
    {
//report(INFO,kFacilityString )<< "hgs: in ZShift:b" << endl;
      // This is the time for a signal to travel from the endplate
      // at readout to the center of the chamber, z=0.  This time
      // is ADDED to a time at the endplate to shift it to z=0.
      
      // West is +z
      Meters dz ;
      if( m_dataHolder->Geometry()->layer( layer ).readoutEnd() ==
	  AEWireLayer::kReadoutEast )
	{
//report(INFO,kFacilityString )<< "hgs: in ZShift:c" << endl;
	  dz = m_dataHolder->Geometry()->layer( layer ).zMin() ;
//report(INFO,kFacilityString )<< "hgs: in ZShift:c2" << endl;
	}
      else
	{
//report(INFO,kFacilityString )<< "hgs: in ZShift:d" << endl;
	  dz = m_dataHolder->Geometry()->layer( layer ).zMax()  ;
	}
//report(INFO,kFacilityString )<< "hgs: in ZShift:e" << endl;
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
CalibratedZDHit* ZDDriftFunction::makeCalibratedHit( const CollatedRawZDHit& rawhit, 
						   int id, 
						   const AZDSenseWireStore* geom, 
						   double tOffset ) const
{      
   DABoolean singleTDC(true)   ;
   ZDDatum::DigiType tdc(0)    ;
   Time         time(0)        ;
   UInt16       wTdc(0)        ;
   Charge       charge(-1)     ;
   ZDHitQuality       qhit     ; // default is "bad" 
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
   if ( rawhit.daqId() <= kTotalZDWires )
     {
       UInt16 cellID = daqID2CellID(rawhit.daqId()); 
       AZDSenseWireStore::LayerWire coord = geom->layerWire( cellID );
       layer       = coord.first       ;
       wireInLayer = coord.second      ;
       wire        = cellID            ;
       STL_VECTOR(ZDDatum::DigiType)::const_iterator itdc = rawhit.digiTimes().begin();   
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

           Time timeTight = m_ZDOffCal->getMaxTightTime();
           Time timeLoose = m_ZDOffCal->getMaxLooseTime();
           Time timeHigh  = m_ZDOffCal->getMaxLooseTime() + 1.e12;
           Time timeLow   = m_ZDOffCal->getMinLooseTime() - 1.e12;

           for ( i=0 ; i<nTDCs ; ++i )
	     {
	       ZDDatum::DigiType   newtdc  = rawhit.digiTimes()[i];
	       Time newtime = tdc2Time(wire,newtdc)-tOffset; 
	       if ( newtime > m_ZDOffCal->getMinTightTime() && newtime < m_ZDOffCal->getMaxTightTime() ) 
		 { if ( newtime < timeTight ) { timeTight = newtime ; tdcTight = newtdc ; wTdcTight = i; } }
               else 
		 {
		   if ( newtime > m_ZDOffCal->getMinLooseTime() && newtime < m_ZDOffCal->getMaxLooseTime() ) 
		     { if ( newtime < timeLoose ) { timeLoose = newtime ; tdcLoose = newtdc ; wTdcLoose = i; } } 
		   else
		     {
		       if ( newtime > m_ZDOffCal->getMaxLooseTime() ) 
			 { if ( newtime < timeHigh ) { timeHigh  = newtime ; tdcHigh = newtdc ; wTdcHigh = i; } }
		       else
			 {
			   if ( newtime < m_ZDOffCal->getMinLooseTime() ) 
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
	     ZDDatum::DigiType   newtdc  = rawhit.digiTimes()[i];
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
   return new CalibratedZDHit(wire, layer, wireInLayer, charge, time, dva, 
			      weight,qhit.value(), nTDCs, earlyGap, lateGap, wTdc, id);
}
//---------------------------------------------------------------------------
ZDDriftFunction::Drift ZDDriftFunction::time2Drift( Time time, 
						int layer ) const 
{
  return time2Drift ( time,  layer, kLRaveraged ) ;
}
//--------------------------------------------------------------------------
ZDDriftFunction::Time ZDDriftFunction::drift2Time( Drift drift, 
					       int layer ) const
{ 
  return drift2Time ( drift, layer, kLRaveraged ) ;
}
//---------------------------------------------------------------------------
Tseries ZDDriftFunction::time2DriftSeries( Time time, 
					 int layer ) const
{
  //
  // For now drift velocity and curvature are not set
  //
  Tseries DVA(time2Drift(time,layer,kLRaveraged),0.0,0.0);
  return DVA;
}
//---------------------------------------------------------------------------
ZDDriftFunction::Drift ZDDriftFunction::time2SignedDrift( Time time, 
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
ZDDriftFunction::Drift ZDDriftFunction::time2SignedDrift( Time time, 
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
ZDDriftFunction::Time ZDDriftFunction::signeddrift2Time( Drift drift, 
						     int layer,
						     float eAngle,
						     float zHit ) const
{
  return signeddrift2Time( drift, layer ) - 
         drift2TimeCorr  ( drift, layer, eAngle, zHit ) ;
}
//--------------------------------------------------------------------------
ZDDriftFunction::Time ZDDriftFunction::signeddrift2Time( Drift drift, 
						     int layer ) const 
{
  if ( drift < 0 )
    { return drift2Time( -drift, layer, krightside ) ; }
  else
    { return drift2Time( drift, layer, kleftside  ) ; }
}
