// -*- C++ -*-
//
// Package:     <ZDOffCal>
// Module:      ZDDataDriftFunction
// 
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDDataDriftFunction.cc,v 1.1 2003/04/24 18:08:09 huanggs Exp $
//
// Revision history
//
// For the ZD, ZDDriftData is the only version used. This is an 
// equivalent of DRDriftDatav2.
//
// We are waiting to see if we need ZDGarfield_DistanceCorrection
// and ZDGarfield_TimeCorrection. The codes for getting constants
// from database are commented out. Anyway, these corrections are 
// very small, and set 0 for the moment.
//
// $Log: ZDDataDriftFunction.cc,v $
// Revision 1.1  2003/04/24 18:08:09  huanggs
// add new files
//
// Revision 1.2  2003/04/24 14:51:50  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:56  huanggs
// imported source
//
//
#include "Experiment/Experiment.h"

// system include files
#include "C++Std/fstream.h"
#include "C++Std/iostream.h"
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ZDOffCal/ZDOffCalProducer.h"
#include "ZDOffCal/ZDDataDriftFunction.h"
#include "ZDOffCal/ZDDFDataHolder.h"
#include "ZDOffCal/ZDStatistics.h"
#include "CalibratedData/Tseries.h"
#include "CalibratedData/CalibratedZDHit.h"
#include "AZDGeom/AZDSenseWireStore.h"

// STL include files
#include <vector>
#include <map>
#include <set>

static const char* const kFacilityString   = "ZDOffCal.ZDDataDriftFunction" ;
const double ZDDataDriftFunction::kIllegalValue    =  19610912.0 ;
const double ZDDataDriftFunction::kminimum_time    = -16384000.0 ;
const double ZDDataDriftFunction::kmaximum_time    =  16384000.0 ;
const double ZDDataDriftFunction::kmaximum_drift   =         0.1 ;
const float  ZDDataDriftFunction::kCorr_DriftMax   =       0.007 ; 
const float  ZDDataDriftFunction::kCorr_TimeMax    =    265000.0 ;
const float  ZDDataDriftFunction::kCorr_EAngleMax  =        45.0 ; 
const float  ZDDataDriftFunction::kCorr_QchargeMax =      8000.0 ; 
const float  ZDDataDriftFunction::kCorr_Qquantum   =       100.0 ; 


//----------------------------------------------------------------------------
ZDDataDriftFunction::ZDDataDriftFunction(ZDOffCalProducer* ZDOffCal,
				     ZDDFDataHolder* iDataHolder): 
          ZDDriftFunction(ZDOffCal, iDataHolder),
	  zd_df_data(kmaxDFside,STL_VECTOR(ZDSinglelayer_df_data)(kmaxZDlayer))
{
  //
  //---------------------------------------------------------------------------
  // OPEN CONSTANTS FROM FRAME
  //
  report( DEBUG, kFacilityString )  << "DataDriftFunction Constructor Starts" << endl;
  //
  //-------------------------------------------------------------------------------
  // DIGEST DRIFT DATA 
  //
  report ( INFO, kFacilityString )  << "Reading Drift Functions " << endl;  
  int    input_Record, input_Code, input_DFType ;
  int    input_ZDlayer, input_ZDlayer_Previous  ;
  double input_Time,    input_Time_Previous     ;
  double input_Dist,    input_Dist_Previous     ;
  double input_Velo    ;
  double input_InvVelo ;
  
  // 
  // layerread/layerused verifies that all input points are used properly
  // (local to constructor, constants verification)
  //
  int layerloop , layerread[kmaxZDlayer] , layerused[kmaxZDlayer] ; 
  for (layerloop = 0;layerloop<kmaxZDlayer;++layerloop) 
    { 
      layerread[layerloop]=0 ; 
      layerused[layerloop]=0 ;
    }
  //
  // zd_df_data is an STL_VECTOR of drift function objects
  // indexed by side (L+R,R,L) and layer
  // we dont initialize every layer and side, but the internal
  // variable "side" is set to kNODF and should remain that
  // way if no data for this layer/side is found. 
  // 
  for (layerloop = 0;layerloop<kmaxZDlayer;++layerloop) 
    { 
      int sideloop ;
      for (sideloop = 0;sideloop<kmaxDFside;++sideloop) 
	{
	  zd_df_data[sideloop][layerloop].side = kNODF ; 
	}
    }

  input_Record    = 0             ;
  input_ZDlayer   = 0             ;
  input_Time      = 0             ;
  input_Dist      = 0             ;
  input_Velo      = 0             ;
  input_InvVelo   = 0             ;
  input_ZDlayer   = 0             ;
  
  //-------------------------------------------------------------------------------------------------
  //
  // Unpack constants object if valid
  //
  DABoolean threesided = m_dataHolder->DriftData().valid() ;
  if ( threesided )
    {
      report( INFO, kFacilityString ) << "DriftData Object Used For DF Constants" << endl;
      int DF_filesize = m_dataHolder->DriftData().size();
      report(DEBUG, kFacilityString )  << "Drift Function File Size: " << DF_filesize << endl;
 
      for(FAConstants<DBZDDriftData>::const_iterator
	  loop1=m_dataHolder->DriftData().begin()   ;
	  loop1!=m_dataHolder->DriftData().end()    ;
	  ++loop1                                     ) 
	{
	  input_Time_Previous    = input_Time    ;
	  input_Dist_Previous    = input_Dist    ;
	  input_ZDlayer_Previous = input_ZDlayer ;
	  
	  input_Record++;
	  // Convert to PicoSeconds
	  input_Time = ( 1000 * (*loop1).get_Time() ) ;
	  input_Dist = (*loop1).get_Dist() ;
	  input_Code = (*loop1).get_Indx() ;
	  
	  //m_statistics->counter(51,"drift data records"); 
	  
	  // parse DF point code 
	  input_DFType    =    ( ( input_Code / 10000 ) % 1000 ) ;
	  input_ZDlayer   =    ( input_Code / 10000000 )   ;
	  layerread[input_ZDlayer-1]++;
	  
	  if  ( debugLevel[3] ) 
	    {
	      if ( input_ZDlayer_Previous != input_ZDlayer )
		{
		  report( INFO , kFacilityString )  << "New DF Layer Unpack" << endl;
		}
	      report( INFO , kFacilityString ) << "ZDDMP Starting record,  # " << input_Record << endl
					       << "ZDDMP                Time " << input_Time   << endl
					       << "ZDDMP            Distance " << input_Dist   << endl
					       << "ZDDMP                Type " << input_DFType << endl 
					       << "ZDDMP               Layer " << input_ZDlayer<< endl ;
	    }
	  if ( input_DFType == kLRaveraged || input_DFType == kleftside || input_DFType == krightside ) 
	    {
	      if ( input_ZDlayer <= kmaxZDlayer ) 
		{     
		  ZDSinglelayer_df_data& df = zd_df_data[input_DFType-1][input_ZDlayer-1];
		  if ( input_ZDlayer_Previous != input_ZDlayer )
		    {
		      //m_statistics->counter(52,"layers in drift records"); 
		      input_Time_Previous      = kminimum_time ;
		      input_Dist_Previous      = 0             ;
		      input_Velo               = 0             ;
		      input_InvVelo            = 0             ;
		      df.max_time              = kminimum_time ;
		      df.min_time              = kmaximum_time ;
		      df.max_drift             = 0             ;
		    }
		  if ( input_Time == input_Time_Previous ) 
		    {
		      report( WARNING, kFacilityString )  << "ZDDMP repeated time, skipping point " << input_Time << endl;
		      //m_statistics->counter(53,"repeated times in drift records"); 
		      if ( input_Dist == input_Dist_Previous )
			{
			  //m_statistics->counter(54,"repeated drift points, skipped"); 
			}
		    }
		  else
		    {
		      input_Velo      = ( input_Dist - input_Dist_Previous  ) /
			( input_Time - input_Time_Previous  ) ;
		      if ( input_Dist == input_Dist_Previous ) 
			{
			  //report( WARNING, kFacilityString )  << "ZDDMP repeated distance " << input_Dist << endl;
			  //m_statistics->counter(55,"repeated drift distance in drift records"); 
			  input_InvVelo      = kIllegalValue;
			}
		      else
			{
			  input_InvVelo      = ( input_Time    - input_Time_Previous     ) /
			    ( input_Dist    - input_Dist_Previous     ) ;
			}
		      //
		      // Should replace by an "addpoint" function in singledflayer object...
		      //
		      Tseries New_DF_Point(input_Dist,input_Velo,0.0)            ;
		      df.t2d_map[input_Time] = New_DF_Point                      ;
		      Tseries New_InverseDF_Point(input_Time,input_InvVelo)      ;
		      df.d2t_map[input_Dist] = New_InverseDF_Point               ;
		      if ( input_Time > df.max_time  ) df.max_time  = input_Time ;
		      if ( input_Time < df.min_time  ) df.min_time  = input_Time ;
		      if ( input_Dist > df.max_drift ) df.max_drift = input_Dist ;
		      df.side = input_DFType                                     ;
		      
		      layerused[input_ZDlayer-1]++                               ;
		      //m_statistics->counter(56,"drift points used")            ;
		      
		      if  ( debugLevel[3] )
			{
			  report( INFO , kFacilityString ) << "ZDDMP Velocity " 
							   << input_Velo  
							   << " Inverse " 
							   << input_InvVelo 
							   << " record# " 
							   << input_Record  
							   << endl ;
			}
		    }
		} 
	      else
		{
		  report( WARNING, kFacilityString )  << "Drift Function Layer has Illegal Value: " << input_ZDlayer << endl;
		}
	    }
	  else
	    {
	      report( WARNING, kFacilityString )  << "Illegal Drift Function Type: " << input_DFType << endl;
	    }
	}
    }
  if ( !threesided )
    {
      report( WARNING, kFacilityString )  << "NO DriftData Constants Found! \n" 
					  << "The Proxy should have thrown an exception! "
					  << endl;
    }
  //-------------------------------------------------------------------------------------------------
  //
  // Check all layers are complete
  //
  for (layerloop = 0;layerloop<kmaxZDlayer;++layerloop) 
    { 
      if( layerread[layerloop] != layerused[layerloop] )
	{
	  report( WARNING, kFacilityString )  
	    << "Layer " << layerloop+1 << " inconsistent "
	    << layerread[layerloop]  << " "
	    << layerused[layerloop]  << endl;
	}
      if( layerread[layerloop] == 0 )
	{
	  report( WARNING, kFacilityString )  << "Layer "<<layerloop+1<<" has no points "<< endl;
	}
    }
  //-------------------------------------------------------------------------------------------------
  //
  // Debug Dump
  //
  int ilayer ;
  if  ( debugLevel[4] )
    { 
      if ( threesided ) 
	{
	  for ( ilayer = 1; ilayer<=kmaxZDlayer; ++ilayer )
	    {
	      const ZDSinglelayer_df_data& ndf = zd_df_data[kLRaveraged][ilayer-1] ;   
	      report( INFO , kFacilityString )  << "DFMAP LR Average " 
						<< " Layer "            
						<< ilayer
						<< " min_time "         
						<< ndf.min_time 
						<< " max_time "         
						<< ndf.max_time 
						<< endl;   

	      for (STL_MAP(double,Tseries)::const_iterator 
                   iter  = (ndf.t2d_map).begin(); 
                   iter != (ndf.t2d_map).end()  ; 
                 ++iter )
		{
		  report( INFO , kFacilityString ) << " Distance: " 
						   << (*iter).second.distance()
						   << " Time: "     
						   << (*iter).first 
						   << endl;
		}
	    }
	}
      if ( threesided )
	{
	  for ( ilayer = 1; ilayer<=kmaxZDlayer; ++ilayer )
	    {
	      const ZDSinglelayer_df_data& ndf = zd_df_data[kleftside][ilayer-1] ;   
	      report( INFO , kFacilityString )  << "DFMAP Left " 
						<< " Layer "            
						<< ilayer
						<< " min_time "         
						<< ndf.min_time 
						<< " max_time "         
						<< ndf.max_time 
						<< endl;   

	      for (STL_MAP(double,Tseries)::const_iterator 
                   iter  = (ndf.t2d_map).begin(); 
                   iter != (ndf.t2d_map).end()  ; 
                 ++iter )
		{
		  report( INFO , kFacilityString ) << " Distance: " 
						   << (*iter).second.distance()
						   << " Time: "     
						   << (*iter).first << endl;
		}
	    }
	  for ( ilayer = 1; ilayer<=kmaxZDlayer; ++ilayer )
	    {
	      const ZDSinglelayer_df_data& ndf = zd_df_data[krightside][ilayer-1] ;   
	      report( INFO , kFacilityString )  << "DFMAP Right " 
						<< " Layer "            
						<< ilayer
						<< " min_time "         
						<< ndf.min_time 
						<< " max_time "         
						<< ndf.max_time 
						<< endl;   

	      for (STL_MAP(double,Tseries)::const_iterator 
                   iter  = (ndf.t2d_map).begin(); 
                   iter != (ndf.t2d_map).end()  ; 
                 ++iter )
		{
		  report( INFO , kFacilityString ) << " Distance: " 
						   << (*iter).second.distance()
						   << " Time: "     
						   << (*iter).first << endl;
		}
	    }
	}
    }
  //-------------------------------------------------------------------------------------
  //
  // Unpack Garfield Drift Distance Corrections from Lauren Hsu (llh14)
  //
  //-------------------------------------------------------------------------------------
  //
  // the "Bins" numbers are actually points to be interpolated between...hence the - 1
  kCorr_EAngleBinSize  = ( 2.0 * kCorr_EAngleMax) / ( (float) ( kCorr_EAngleBins   - 1 ) ) ; 
  kCorr_DriftBinSize   = kCorr_DriftMax           / ( (float) ( kCorr_DriftBins    - 1 ) ) ;
  kCorr_TimeBinSize    = kCorr_TimeMax            / ( (float) ( kCorr_TimeBins     - 1 ) ) ;
  /*-------------------------------------------------------------------------------------
     Initialize corrections to zero 
     so any missing data does not yield a zero or corrupt weight
     we DO NOT verify that data is complete, indeed currently there
     no correction in the BDL object for layers 1 and 47

     Zero "set" array to check for repeated attempt to set a bin
  */  
  int setDrift[kmaxZDlayer][kCorr_EAngleBins][kAmbiguities][kCorr_TimeBins] ;
  for ( int loop_layr1=0 ; loop_layr1<kmaxZDlayer ; ++loop_layr1 )
    {
      for ( int loop_eangl=0 ; loop_eangl<kCorr_EAngleBins ; ++loop_eangl )
	{
	  for ( int loop_timeb=0 ; loop_timeb<kCorr_TimeBins ; ++loop_timeb )
	    {
	      int loop_ambig = 0 ;
	      corrDrift[ loop_layr1 ]
   		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_timeb ] = 0.0 ;
	       setDrift[ loop_layr1 ]
		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_timeb ] = 0   ;
	      loop_ambig = 1 ;
	      corrDrift[ loop_layr1 ]
   		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_timeb ] = 0.0 ;
	       setDrift[ loop_layr1 ]
		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_timeb ] = 0   ;
	    }
	}
    }
  /*-------------------------------------------------------------------------------------
     BDL definition of DBZDGarfield_DistanceCorrection
     UInt32 entry
     float  drifttime (ps, 5000ps increments)
     float  z_position (actually layer*10000+2+ambiguity)  (last minute kluge, sorry)
     float  entrance_angle (degrees, 5 degree increments from -45o thru 45o)
     float  driftdistance_correction (distance, meters) 
  */
/*hgs: waiting
  report(DEBUG, kFacilityString )  << "Garfield Drift Correction File Size: " 
		 		   << m_dataHolder->DriftCorr().size()
			 	   << endl;
  for(FAConstants<DBZDGarfield_DistanceCorrection>::const_iterator 
	loopD =m_dataHolder->DriftCorr().begin() ;
        loopD!=m_dataHolder->DriftCorr().end()   ; ++loopD ) 
    {
        int error = 0 ;
        float angle = (*loopD).get_entrance_angle() ;
        if ( angle < -kCorr_EAngleMax )         { error +=  8 ; }
        if ( angle >  kCorr_EAngleMax )         { error += 16 ; }

	int anglebin  = ( (int) ( ( (angle + kCorr_EAngleMax) / kCorr_EAngleBinSize ) + 0.5 ) ) ;
        int timebin   = ( (int) ( ( (*loopD).get_drifttime()  / kCorr_TimeBinSize   ) + 0.5 ) ) ;
        int layer     = ( (int) ( ( (*loopD).get_z_position() / 10000.              ) + 0.5 ) ) ;

        int   ambiguity = 0 ; 
        if ( ( (*loopD).get_z_position() - ( ( (float) layer ) * 10000. ) - 2.0 ) > 0 )
	  { ambiguity = 1 ; } 
        //
        // Range and Error Checking
        // Dont store data from out of range points!
        //
        if ( ambiguity != 0 && ambiguity != 1 )  { error +=  1 ; }
        if ( layer    <             1  )         { error +=  2 ; }
        if ( layer    >   kmaxZDlayer  )         { error +=  4 ; }
        if ( timebin  <             0  )         { error += 32 ; }
        if ( timebin  > kCorr_TimeBins  )        { error += 64 ; }
	if ( error != 0 )
	  {
	    report( WARNING, kFacilityString )  << " DistanceCorrection, corrupt record "
						<< " Entry " << (*loopD).get_entry() 
						<< " Time  " << (*loopD).get_drifttime()
						<< " Code  " << (*loopD).get_z_position()
						<< " Angle " << (*loopD).get_entrance_angle()
						<< " CorrV " << (*loopD).get_driftdistance_correction()
						<< " Ambig " << ambiguity
						<< " Layer " << layer
						<< " Error " << error
						<< endl;	    
	  }
	else
	  {
	    if ( setDrift[layer-1  ]
	                 [anglebin ]
	                 [ambiguity]
	                 [timebin  ] == 0 )
	      {
		corrDrift[layer-1  ]
		         [anglebin ]
		         [ambiguity]
		         [timebin  ] = (*loopD).get_driftdistance_correction();
	      }
	    else
	      {
		report( WARNING, kFacilityString )  << " Repeated Record "
						    << " Entry " << (*loopD).get_entry() 
						    << " Time  " << (*loopD).get_drifttime()
						    << " Code  " << (*loopD).get_z_position()
						    << " Angle " << (*loopD).get_entrance_angle()
						    << " CorrV " << (*loopD).get_driftdistance_correction()
						    << " Ambig " << ambiguity
						    << " Tbin  " << timebin
						    << " Abin  " << anglebin
						    << " Layer " << layer
						    << " Error " << error
						    << endl;	    
		corrDrift[layer-1  ]
		         [anglebin ]
		         [ambiguity]
		         [timebin  ] = 0.0;
	      }
	    setDrift[layer-1  ]
	            [anglebin ]
	            [ambiguity]
	            [timebin  ]++;
	  }
    }
hgs: waiting*/

  /*-------------------------------------------------------------------------------------
     Initialize corrections to zero 
     so any missing data does not yield an undefined correction
     we DO NOT verify that data is complete, indeed currently there
     no correction in the BDL object for layers 1 and 47

     Zero "set" array to check for repeated attempt to set a bin
  */
  int setTime[kmaxZDlayer][kCorr_EAngleBins][kAmbiguities][kCorr_DriftBins];
  for ( int loop_layr2=0 ; loop_layr2<kmaxZDlayer ; ++loop_layr2 )
    {
      for ( int loop_eangl=0 ; loop_eangl<kCorr_EAngleBins ; ++loop_eangl )
	{
	  for ( int loop_drift=0 ; loop_drift<kCorr_DriftBins ; ++loop_drift)
	    {
	       int loop_ambig = 0 ;
	       corrTime[ loop_layr2 ]
   		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_drift ] = 0.0 ;
	        setTime[ loop_layr2 ]
		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_drift ] = 0   ;
	       loop_ambig = 1 ;
	       corrTime[ loop_layr2 ]
   		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_drift ] = 0.0 ;
	        setTime[ loop_layr2 ]
		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_drift ] = 0   ;
	    }
	}
    }
  /*----------------------------------------------------------------------------------- 
     BDL definition of DBZDGarfield_TimeCorrection
     UInt32 entry
     float  driftdistance (meters in 200 micron increments)
     float  z_position (actually layer*10000+1+ambiguity)
     float  entrance_angle (degrees, 5 degree increments from -45o thru 45o)
     float  drifttime_correction (time, ps)
  */
/*hgs: waiting
  report(DEBUG, kFacilityString )  << "Garfield Time  Correction File Size: " 
				   <<  m_dataHolder->TimeCorr().size()
				   << endl;

  for(FAConstants<DBZDGarfield_TimeCorrection>::const_iterator 
	loopT =m_dataHolder->TimeCorr().begin()  ;
        loopT!=m_dataHolder->TimeCorr().end()    ; ++loopT ) 
    {
        int error = 0 ;
        float angle = (*loopT).get_entrance_angle() ;
        if ( angle < -kCorr_EAngleMax )         { error +=  8 ; }
        if ( angle >  kCorr_EAngleMax )         { error += 16 ; }

	int anglebin  = ( (int) ( ( (angle + kCorr_EAngleMax)    / kCorr_EAngleBinSize ) + 0.5 ) ) ;
        int layer     = ( (int) ( ( (*loopT).get_z_position()    / 10000.              ) + 0.5 ) ) ;
        int driftbin  = ( (int) ( ( (*loopT).get_driftdistance() /  kCorr_DriftBinSize ) + 0.5 ) ) ;

        int   ambiguity = 0 ; 
        if ( ( (*loopT).get_z_position() - ( ( (float) layer ) * 10000. ) - 2.0 ) > 0 )
	  { ambiguity = 1 ; } 
        //
        // Range and Error Checking
        // Dont store data from out of range points!
        //
        if ( ambiguity != 0 && ambiguity != 1 )  { error +=  1 ; }
        if ( layer    <              1 )         { error +=  2 ; }
        if ( layer    >    kmaxZDlayer )         { error +=  4 ; }
        if ( driftbin <              0 )         { error += 32 ; }
        if ( driftbin > kCorr_DriftBins )        { error += 64 ; }
	if ( error != 0 )
	  {
	    report( WARNING, kFacilityString )  << " TimeCorrection, corrupt record "
						<< " Entry " << (*loopT).get_entry() 
						<< " Dist  " << (*loopT).get_driftdistance()
						<< " Code  " << (*loopT).get_z_position()
						<< " Angle " << (*loopT).get_entrance_angle()
						<< " CorrV " << (*loopT).get_drifttime_correction()
						<< " Ambig " << ambiguity
						<< " Dbin  " << driftbin
						<< " Abin  " << anglebin
						<< " Layer " << layer
						<< " Error " << error
						<< endl;	    
	  }
	else
	  {
            if ( setTime[layer-1  ]
                        [anglebin ]
                        [ambiguity]
                        [driftbin ] == 0 )
	      {
		corrTime[layer-1  ]
		        [anglebin ]
                        [ambiguity]
                        [driftbin ] = (*loopT).get_drifttime_correction();
	      }
	    else
	      {
		report( WARNING, kFacilityString )  << " TimeCorrection, repeated record "
						    << " Entry " << (*loopT).get_entry() 
						    << " Dist  " << (*loopT).get_driftdistance()
						    << " Code  " << (*loopT).get_z_position()
						    << " Angle " << (*loopT).get_entrance_angle()
						    << " CorrV " << (*loopT).get_drifttime_correction()
						    << " Ambig " << ambiguity
						    << " Layer " << layer
						    << " Error " << error
						    << endl;	    
		corrTime[layer-1  ]
		        [anglebin ]
                        [ambiguity]
                        [driftbin ] = 0 ;
	      }
	    setTime[layer-1  ]
	           [anglebin ]
	           [ambiguity]
	           [driftbin ]++;
	  }
    }
hgs: waiting*/

  //------------------------------------------------------------------------
  //
  // Distance Correction vs. Time
  //
  // A little bit of data quality verification 
  //
/*hgs: waiting
  float rms_dcorr         = 0. ;
  float count_dcorr       = 0. ;
  float count_all_dcorr   = 0. ;
  float count_dcorr_zeros = 0. ;
  for ( int loop_layr3=0 ; loop_layr3<kmaxZDlayer ; ++loop_layr3 )
    {
      for ( int loop_eangl=0 ; loop_eangl<kCorr_EAngleBins ; ++loop_eangl )
	{
	  for ( int loop_timeb=0 ; loop_timeb<kCorr_TimeBins ; ++loop_timeb )
	    {
              count_all_dcorr += 2. ;
              float X = 
	      corrDrift[ loop_layr3 ]
   		       [ loop_eangl ]
		       [          0 ]
		       [ loop_timeb ] ;
              if ( X != 0.0 ) 
		{
		  rms_dcorr += X*X ; 
		  count_dcorr++    ;
		}
              else
		{
		  count_dcorr_zeros++ ;
		}
              X = 
	      corrDrift[ loop_layr3 ]
   		       [ loop_eangl ]
		       [          1 ]
		       [ loop_timeb ] ;
              if ( X != 0.0 ) 
		{
		  rms_dcorr += X*X ; 
		  count_dcorr++    ;
		}
              else
		{
		  count_dcorr_zeros++ ;
		}
	    }
	}
    }
  if ( count_dcorr > 0 )
    {
      rms_dcorr = sqrt ( rms_dcorr / count_dcorr ) ;
      int nonzero_corrs = (int) 
	( ( ( count_dcorr                      / count_all_dcorr ) * 100. ) + 0.5 ) ;
      int    zero_corrs = (int) 
	( ( ( count_dcorr_zeros                / count_all_dcorr ) * 100. ) + 0.5 ) ;
      int  filled_corrs = (int) 
	( ( ( m_dataHolder->DriftCorr().size() / count_all_dcorr ) * 100. ) + 0.5 ) ;
      report( INFO, kFacilityString )  << "Entrance Angle Statistics \n"
                                       << "                                     Distance Correction vs. Time \n" 
                                       << "                                     Nonzero Correction Bins: " << nonzero_corrs << "%, \n" 
				       << "                                     RMS correction value   : " << rms_dcorr     << "(m), \n"
				       << "                                     Zero Correction Bins   : " << zero_corrs    << "%, \n"
				       << "                                     Filled Correction Bins : " << filled_corrs  << "%  \n"
				       << endl ;
   }  
  else 
    {
      int    zero_corrs = (int)
	( ( ( count_dcorr_zeros / m_dataHolder->DriftCorr().size() ) * 100. ) + 0.5 ) ;
      report( ERROR, kFacilityString ) << " Entrance Angle Statistics: Distance Correction vs. Time \n" 
				       << " Zero Correction Bins: "    << zero_corrs
				       << endl ;
    }  
hgs: waiting*/
  //------------------------------------------------------------------------
  //
  // Time Correction vs. Distance
  //
  // A little bit of data quality verification 
  //
/*hgs: waiting
  float rms_tcorr         = 0. ;
  float count_tcorr       = 0. ;
  float count_all_tcorr   = 0. ;
  float count_tcorr_zeros = 0. ;
  for ( int loop_layr4=0 ; loop_layr4<kmaxZDlayer ; ++loop_layr4 )
    {
      for ( int loop_eangl=0 ; loop_eangl<kCorr_EAngleBins ; ++loop_eangl )
	{
	  for ( int loop_drift=0 ; loop_drift<kCorr_DriftBins ; ++loop_drift)
	    {
               count_all_tcorr += 2. ;
               float X = 
	       corrTime[ loop_layr4 ]
   		       [ loop_eangl ]
		       [          0 ]
		       [ loop_drift ] ;
              if ( X != 0.0 ) 
		{
		  rms_tcorr += X*X ; 
		  count_tcorr++    ;
		}
              else
		{
		  count_tcorr_zeros++ ;
		}
               X = 
	       corrTime[ loop_layr4 ]
   		       [ loop_eangl ]
		       [          1 ]
		       [ loop_drift ] ;
              if ( X != 0.0 ) 
		{
		  rms_tcorr += X*X ; 
		  count_tcorr++    ;
		}
              else
		{
		  count_tcorr_zeros++ ;
		}
	    }
	}
    }
  if ( count_tcorr > 0 )
    {
      rms_tcorr = sqrt ( rms_tcorr / count_tcorr ) ;
      int nonzero_corrs = (int) 
	( ( ( count_tcorr                      / count_all_tcorr ) * 100. ) + 0.5 ) ;
      int    zero_corrs = (int) 
	( ( ( count_tcorr_zeros                / count_all_tcorr ) * 100. ) + 0.5 ) ;
      int  filled_corrs = (int) 
	( ( ( m_dataHolder->TimeCorr().size()  / count_all_tcorr ) * 100. ) + 0.5 ) ;
      report( INFO, kFacilityString )  << "Entrance Angle Statistics \n"
                                       << "                                     Time Correction vs. Distance \n" 
                                       << "                                     Nonzero Correction Bins: " << nonzero_corrs << "%, \n" 
				       << "                                     RMS correction value   : " << rms_tcorr     << "(ps),  \n"
				       << "                                     Zero Correction Bins   : " << zero_corrs    << "%, \n"
				       << "                                     Filled Correction Bins : " << filled_corrs  << "%  \n"
				       << endl ;
    }  
  else 
    {
      int    zero_corrs = (int) 
	( ( ( count_tcorr_zeros / m_dataHolder->TimeCorr().size() ) * 100. ) + 0.5 ) ;
      report( ERROR, kFacilityString )  << " Entrance Angle Statistics: Time Correction vs. Distance \n" 
					<< " Zero Correction Bins: "    << zero_corrs
					<< endl ;
    }  
hgs: waiting*/
  //----------------------------------------------------------------------------------- 
  report( DEBUG, kFacilityString )  << "DataDriftFunction Constructor Ends" << endl;
}
//---------------------------------------------------------------------------
ZDDataDriftFunction::~ZDDataDriftFunction()
{
  report( DEBUG, kFacilityString )  << "DataDriftFunction Destructor" << endl;
}
// const member functions
//---------------------------------------------------------------------------
ZDDriftFunction::Drift ZDDataDriftFunction::time2Drift( Time time, 
						    int layer, 
						    int outside) const
{
  // CHECK LAYER IN VALID RANGE
  if ( layer < 1 || layer > kmaxZDlayer )
    {
      report( ERROR, kFacilityString )  << "Illegal Layer # "
				        << layer 
				        << endl;
      return 0.0 ;
    }
  
  // CHECK AMBIGUITY IN VALID RANGE
  if ( outside != kLRaveraged && outside != kleftside && outside != krightside )
    {
      report( ERROR, kFacilityString )  << "Illegal Side Key "
			         	<< outside 
				        << endl;
      return 0.0 ;
    }
  double Distance(0.0);
  DABoolean once ( true ) ;
  //
  // If two-sided drift functions are not available 
  // use LR averaged or panic
  //
  int inside ;
  if ( zd_df_data[outside-1][layer-1].side == kNODF ) 
    { 
      if ( once ) 
	{
	  report( WARNING, kFacilityString )  << "DF type unavailable: "
					      << zd_df_data[outside-1][layer-1].side
					      << " trying to use " 
					      << kLRaveraged 
					      << endl ;
          once = false ; 
	}
      inside = kLRaveraged ;
      if ( zd_df_data[inside-1][layer-1].side == kNODF ) 
	{
	  report( ERROR  , kFacilityString )  << "No DF types available "
					      << zd_df_data[inside-1][layer-1].side 
					      << " "
					      << zd_df_data[outside-1][layer-1].side
					      << endl ;
	  return 0.0 ;
	}
    }        
  else
    {
      inside = outside ;
    }
  const ZDSinglelayer_df_data& df  = zd_df_data[inside-1][layer-1] ;   
  //
  // Evaluate
  // 
  //
  // Find first entry in map with time component greater than current time
  // (note that velocity stored in map is difference with previous point in map...)
  //   Note:
  //            (lowpoint,highpoint) upper_bound(lowpoint) returns highpoint
  //            (lowpoint,highpoint) lower_bound(lowpoint) returns lowpoint
  //            out of range points returns zero values for functions! but
  //            can test on begin and end to see out of range behaviour
  //            this should be caught by outer two levels of ifs...but...
  //
  STL_MAP(double,Tseries )::const_iterator it = (df.t2d_map).upper_bound(time);
  if ( ( time <= df.min_time ) ||  ( it == (df.t2d_map).begin() ) )
    {
      if ( time <= df.min_time )
	{
	  //m_statistics->counter(57,"time out of range, low");
	  if ( debugLevel[1] )
	    {
	      report( INFO , kFacilityString )  << "undertime " << time << " " << df.min_time << endl;
	    }
	}
      else 
	{
	  //m_statistics->counter(58,"time under range, iterator not found");
	  if ( debugLevel[1] )
	    {
	      report( INFO , kFacilityString )  << "time under range, iterator not found"<< endl;
	    }
	}
      return 0.0;
    }
  //
  // Check if time is out of range of drift function, high
  //
  if ( ( time >= df.max_time ) || ( it == df.t2d_map.end() ) ) 
    {
      if ( time >= df.max_time ) 
	{
	  //m_statistics->counter(59,"time out of range, high");
	  if ( debugLevel[1] )
	    {
	      report( INFO , kFacilityString )  << "overtime " << time << " " << df.max_time << endl;
	    }
	}
      else 
	{
	  //m_statistics->counter(60,"time over range, iterator not found");
	  if ( debugLevel[1] )
	    {
	      report( INFO , kFacilityString )  << "time over range, iterator not found"<< endl;
	    }
	}
      //
      // Extrapolate past measured time range using average drift velocity and 
      // last measured point on drift function 
      //
      Distance = ( ( ( time - df.max_time ) * ( df.max_drift / df.max_time ) ) + df.max_drift ) ;
      return Distance;
    }
  // 
  // Since we are extrapolating from the upper edge of the bin 
  // this DeltaT should be negative!
  //
  Distance = (*it).second.distance();
  Time DeltaT = time - (*it).first;  
  if ( DeltaT > 0 ) 
    {
      //m_statistics->counter(61,"time out of drift bin, problem");		  
      if ( debugLevel[1] )
	{
	  report( INFO , kFacilityString )  << "time out of drift bin, problem" << endl;
	}
    }
  else
    {
      Distance = Distance + (*it).second.velocity()*DeltaT;
      //m_statistics->counter(62,"time to drift OK");
    }
  if ( Distance < 0 )
    {
      //m_statistics->counter(63,"negative unsigned drift distance (zeroed)");
      if ( debugLevel[1] )
	{
	  report( INFO , kFacilityString )  << "negative unsigned drift distance (set 0)" << Distance << endl;
	}
      Distance = 0 ;
    }
  if ( debugLevel[1] )
    {
      report( INFO , kFacilityString )  << time << " " << Distance << endl;
    }
  return Distance;
}
//--------------------------------------------------------------------------
ZDDriftFunction::Time ZDDataDriftFunction::drift2Time( Drift drift, 
						   int layer, 
						   int outside ) const
{ 
  // CHECK LAYER IN VALID RANGE
  if ( layer < 1 || layer > kmaxZDlayer )
    {
      report( WARNING, kFacilityString )  << "Illegal Layer # "
					  << layer 
					  << endl;
      return 0.0 ;
    }
  
  // CHECK AMBIGUITY IN VALID RANGE
  if ( outside != kLRaveraged && outside != kleftside && outside != krightside )
    {
      report( WARNING, kFacilityString )  << "Illegal Side Key "
					  << outside 
					  << endl;
      return 0.0 ;
    }
  
  Time drifttime;
  //
  // Use absolute value internally, side defines "L,R,L+R"
  //
  double zdrift = fabs ( drift );
  DABoolean once ( true ) ;
  //
  // If two-sided drift functions are not available 
  // use LR averaged or panic
  //
  int inside ;
  if ( zd_df_data[outside-1][layer-1].side == kNODF ) 
    { 
      if ( once ) 
	{
	  report( WARNING, kFacilityString )  << "DF type unavailable: "
					      << zd_df_data[outside-1][layer-1].side
					      << " trying to use " 
					      << kLRaveraged 
					      << endl ;
          once = false ; 
	}
      inside = kLRaveraged ;
      if ( zd_df_data[inside-1][layer-1].side == kNODF ) 
	{
	  report( ERROR  , kFacilityString )  << "No DF types available "
					      << zd_df_data[inside-1][layer-1].side 
					      << " "
					      << zd_df_data[outside-1][layer-1].side
					      << endl ;
	  return 0.0 ;
	}
    }        
  else
    {
      inside = outside ;
    }
  const ZDSinglelayer_df_data& df  = zd_df_data[inside-1][layer-1] ;   
  //
  // Evaluate
  // 
  STL_MAP(double, Tseries)::const_iterator dfbin = (df.d2t_map).upper_bound(zdrift);
  
  if ( ( zdrift > df.max_drift ) || ( dfbin == df.d2t_map.end() ) ) 
    { 
      if ( zdrift > df.max_drift )
	{
	  //m_statistics->counter(64,"drift distance out of range");
	  if ( debugLevel[1] )
	    {
	      report( INFO , kFacilityString )  << " drift distance out of range " << zdrift << " " << df.max_time << endl;
	    }
	}
      else 
	{
	  //m_statistics->counter(65,"drift distance over range, iterator not found");
	  if ( debugLevel[1] )
	    {	 
	      report( INFO , kFacilityString )  << " drift distance over range, iterator not found  " 
						<< zdrift << " " << df.max_drift << endl;
	    }
	}
      drifttime = ( zdrift - df.max_drift ) / ( df.max_drift / df.max_time ) + df.max_time;
      return drifttime ;
    } 
  if ( dfbin == df.d2t_map.end() ) 
    { 
      //m_statistics->counter(66,"drift distance over range, iterator not found");
      if ( debugLevel[1] )
	{
	  report( INFO , kFacilityString )  << " drift distance under range, iterator not found  " 
					    << zdrift << " " << endl;
	}
      drifttime = 0.;
      return drifttime ;
    } 
  // the member data functions for Tseries are distance,velocity should really be
  // called "firstderivative" "secondderivative" :) in this context, confusing...
  drifttime =  (*dfbin).second.distance() ;
  if ( (*dfbin).second.velocity() != kIllegalValue )
    {
      //m_statistics->counter(67,"drift to time OK");
      Drift DeltaD = drift-(*dfbin).first;
      drifttime = drifttime + (*dfbin).second.velocity()*DeltaD;
    }
  else
    {
      //m_statistics->counter(68,"inverse velocity has illegal value");
      report( WARNING, kFacilityString )  << "inverse velocity has illegal value"<< endl;
    }
  return drifttime;
}
//--------------------------------------------------------------------------
ZDDriftFunction::Time ZDDataDriftFunction::drift2TimeCorr( Drift drift, 
				   		       int   layer,
						       float eAngle,
						       float zHit ) const
{
  // CHECK LAYER IN VALID RANGE
  if ( layer < 1 || layer > kmaxZDlayer )
    {
      report( WARNING, kFacilityString )  << "Illegal Layer # "
					  << layer 
					  << endl;
      return 0.0 ;
    }
  
  // CHECK AMBIGUITY IN VALID RANGE
  int ambiguity ;
  float ldrift ;
  if ( drift > 0 )
    {
      ambiguity = 1 ;
      ldrift = drift ;
    }
  else
    {
      ambiguity = 0 ;
      ldrift = -drift ;
    }
  
  // CHECK ENTRANCE ANGLE LIMITS 
  int anglebin  ; 
  float dangle  ;
  if ( eAngle < -kCorr_EAngleMax ) 
    { 
      anglebin = 0   ;  
      dangle   = 0.0 ;
    }
  else
    {
      if ( eAngle >= kCorr_EAngleMax )
	{ 
	  anglebin = kCorr_EAngleBins - 2 ;  // count from 0!
	  dangle   = 1.0                  ;  // interpolating between [-2] and [-1] (last two points)
	}
      else
	{ // assumes truncation runs 0.9999>0 
	  dangle   = (  ( eAngle + kCorr_EAngleMax ) / kCorr_EAngleBinSize ) ;
	  anglebin = (  (int) dangle                                       ) ;
	  dangle   = (  dangle - ( (float) anglebin )                      ) ;
	}
    }

  // CHECK ZDIFT LIMITS
  int driftbin ; 
  float ddrift ;
  if ( ldrift >= kCorr_DriftMax )
    { 
      driftbin = kCorr_DriftBins - 2 ; // count from 0!
      ddrift   = 1.0                 ; // interpolating between [-2] and [-1] (last two points)
    }
  else
    { // assumes truncation runs 0.9999>0 
      ddrift   = (  ldrift / kCorr_DriftBinSize    ) ;
      driftbin = (  (int) ddrift                   ) ;
      ddrift   = (  ddrift - ( (float) driftbin )  ) ;
    }
  //
  // BiLinear Interpolation 
  //
  float result = (corrTime[layer-1][anglebin  ][ambiguity][driftbin  ]*( (1.0-ddrift)  * (1.0-dangle) )) +
                 (corrTime[layer-1][anglebin+1][ambiguity][driftbin  ]*( (1.0-ddrift)  *    dangle    )) +
                 (corrTime[layer-1][anglebin  ][ambiguity][driftbin+1]*(   ddrift      * (1.0-dangle) )) +
                 (corrTime[layer-1][anglebin+1][ambiguity][driftbin+1]*(   ddrift      *    dangle    )) ;

  if  ( debugLevel[6] )
    {
      report( INFO , kFacilityString )  << " result   " << result   << endl
					<< " driftbin " << driftbin << endl
					<< " layer    " << layer    << endl
					<< " anglebin " << anglebin << endl
					<< " ddrift   " << ddrift   << endl
					<< " dangle   " << dangle   << endl
					<< " drift    " << drift    << endl
					<< " angle    " << eAngle   << endl ;
    }
  //
  // sign convention of correctiondata appears to be flipped...
  // 
  return -result; 
}
//---------------------------------------------------------------------------
ZDDriftFunction::Drift ZDDataDriftFunction::time2DriftCorr( Time  time, 
  						        int   layer, 
						        int   ambiguity,
						        float eAngle,
						        float zHit ) const
{
  //
  // CHECK LAYER IN VALID RANGE
  // 
  if ( layer < 1 || layer > kmaxZDlayer )
    {
      report( WARNING, kFacilityString )  << "Illegal Layer # "
					  << layer 
					  << endl;
      return 0.0 ;
    }
  
  // CHECK AMBIGUITY IN VALID RANGE
  if ( ambiguity != 0 && ambiguity != 1 )
    {
      report( WARNING, kFacilityString )  << "Illegal Ambiguity "
					  << ambiguity
					  << endl;
      return 0.0 ;
    }
  
  // CHECK eAngle (ENTRANCE ANGLE) LIMITS 
  int anglebin ; 
  float dangle ;
  if ( eAngle < -kCorr_EAngleMax ) 
    { 
      anglebin = 0.0 ;  
      dangle   = 0.0 ;
    }
  else
    {
      if ( eAngle > kCorr_EAngleMax )
	{ 
	  anglebin = kCorr_EAngleBins - 2 ; // counts from 0!
	  dangle   = 1.0                  ;
	}
      else
	{ // assumes truncation runs 0.9999>0 
          dangle   = ( ( eAngle + kCorr_EAngleMax ) / kCorr_EAngleBinSize ) ;
	  anglebin = ( (int) dangle )                                       ;
	  dangle   = ( dangle - ( (float) anglebin ) )                        ; 
	}
    }

  // CHECK TIME LIMITS
  int timebin ; 
  float dtime ;
  if ( time < 0 ) 
    { 
      return 0.0 ;  
    }
  else
    {
      if ( time > kCorr_TimeMax )
	{ 
	  timebin = kCorr_TimeBins - 2 ; // counts from 0!
          dtime   = 1.0                ; 
	}
      else
	{ // assumes truncation runs 0.9999>0 
          dtime   = ( time / kCorr_TimeBinSize );
          timebin = ( (int) dtime )             ;
	  dtime   = dtime - ( (int) timebin )   ; 
	}
    }
  //
  // BiLinear Interpolation 
  //
  float result = (corrDrift[layer-1][anglebin  ][ambiguity][timebin  ]*( (1.0-dtime)  * (1.0-dangle) )) +
                 (corrDrift[layer-1][anglebin+1][ambiguity][timebin  ]*( (1.0-dtime)  *    dangle    )) +
                 (corrDrift[layer-1][anglebin  ][ambiguity][timebin+1]*(   dtime      * (1.0-dangle) )) +
                 (corrDrift[layer-1][anglebin+1][ambiguity][timebin+1]*(   dtime      *    dangle    )) ;

  if  ( debugLevel[6] )
    {
      report( INFO , kFacilityString )  << " result   " << result   << endl
					<< " timebin  " << timebin  << endl
					<< " layer    " << layer    << endl
					<< " anglebin " << anglebin << endl
					<< " dtime    " << dtime    << endl
					<< " dangle   " << dangle   << endl
					<< " time     " << time     << endl
					<< " angle    " << eAngle   << endl ;
    }
  //
  // sign convention of correctiondata appears to be flipped...
  // 
  return -result ;
}
//---------------------------------------------------------------------------
ZDDriftFunction::Time ZDDataDriftFunction::charge2TimeCorr( Time   time, 
							Charge Qcharge ) const
{
  int Qbin ; 
  if ( Qcharge < 0 ) 
    {
      return 0 ;
    }
  else
    {
      if ( Qcharge > kCorr_QchargeMax ) 
	{
          Qbin = kCorr_QBins - 1 ; 
	}
      else
	{
	  Qbin = int( Qcharge / kCorr_Qquantum ) ;
	}
    }
  return m_Qcorr[Qbin];
}
