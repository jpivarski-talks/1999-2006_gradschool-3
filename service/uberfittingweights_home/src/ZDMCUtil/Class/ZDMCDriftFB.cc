// -*- C++ -*-
//
// Package:     ZDMCUtil
// Module:      ZDMCDriftFB
// 
// Description: Frame Broker object for ZD MC access to ZDDriftFunctionProd.
//              (drift2time, time2tdc, charge2adc relations)
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Guangshun Huang
// Created:     Tue Dec 17, 2002
//
// Revision history

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Experiment/fortran_types.h"
#include "FrameAccess/FATable.h"
#include "ZDMCUtil/ZDMCDriftFB.h"
#include "AZDGeom/AZDSenseWireStore.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ZDMCUtil.ZDMCDriftFB" ;

//
// static data member definitions
//
ZDMCDriftFB* ZDMCDriftFB::m_instance = 0;

//
// constructors and destructor
//
ZDMCDriftFB::ZDMCDriftFB(){}
ZDMCDriftFB::~ZDMCDriftFB(){}
//
// member functions
//
void ZDMCDriftFB::setDrift( FAItem< ZDDriftFunction     > calibration,
                            FAItem< AZDSenseWireStore > geometry     ) 
{
   m_calibration = calibration ;
   m_geometry    = geometry    ;
   return;
}
FAItem< ZDDriftFunction     >  ZDMCDriftFB::getCalibration()   
{  return m_calibration; }
FAItem< AZDSenseWireStore >  ZDMCDriftFB::getGeometry()      { return m_geometry;    }
//
// static member functions
//
ZDMCDriftFB*
ZDMCDriftFB::instance()
{
  //-> Creates instance of singleton object
  if ( 0 == m_instance )
    {
      report( INFO, kFacilityString ) << "Creating ZD Calibration Singleton" << endl;
      m_instance = new ZDMCDriftFB;
    }
  return m_instance;
}
void
ZDMCDriftFB::destroy()
{
  //-> Destroys instance of singleton object
  if ( 0 != m_instance )
    {
      delete m_instance;
      m_instance = 0;
    }
  return;
}
//
// F77 interface functions
//
//
// replaces fortran routine tmccl from old zdoffcal
//
extern "C" void  zdmc_drift2time_( FReal*    FlightTime,
                                   FReal*    SmearedDriftDistance,
 			    	   FInteger* ZDLayer,
                                   FReal*    ZonWire,
                                   FInteger* Ambiguity,
                                   FReal*    EntranceAngle,
                                   FReal*    Cotangent,
                                   FReal*    PulseHeight,
                                   FReal*    HitTime )
{
  ZDMCDriftFB* fb  =  ZDMCDriftFB::instance();
  double Radius    =  fb->getGeometry()->radiusNominal(*ZDLayer);
  //------------------------------------------------------------------------------
  //
  // Invert Drift Function
  //

  
  //double DriftTime =  fb->getCalibration()->signeddrift2Time( *SmearedDriftDistance, 
  //							      *ZDLayer, 
  //							      *EntranceAngle,
  //							      *ZonWire );

  // 2001/10/25 D.M. ^^^ Stupid! *SmearedDriftDistance is always > 0 !
  // The sign is transmitted through *Ambiguity !!! So far I have to add the next lines

         FReal SignedDriftDistance = (*SmearedDriftDistance) * (*Ambiguity);
  double DriftTime =  fb->getCalibration()->signeddrift2Time( SignedDriftDistance, 
  							      *ZDLayer, 
  							      *EntranceAngle,
  							      *ZonWire );
report(INFO, kFacilityString) << "hgs:DriftTime=" << DriftTime << endl;
  //
  //------------------------------------------------------------------------------
  //
  // Reverse Pulseheight Time Slewing Correction (makeCalibratedHit)
  // (some hysteresis...should be iterated...)  (Applied in HitCorrector.cc)
  //
  //
  //------------------------------------------------------------------------------
  //  //  2001/10/30 M.Dubrovin Test print-out for charge2TimeCorr function
  //  static DABoolean once=true;
  //  if ( once )
  //    {  once=false;
  //
  //    cout << "Drift time (ps), Charge, dt(ps) "<< endl;
  //
  //	cout << "Drift times for each row, ps = " << endl;
  //	    for(double DrTime=0.; DrTime <= 700000; DrTime += 100000 )
  //    	       {cout << DrTime << "  "; }
  //                cout << endl;
  //	
  //	for ( FReal Charge=0.; Charge != 2510.; Charge+=10.)
  //          {
  //	    cout << Charge << "  ";
  //	    for(double DrTime=0.; DrTime <= 700000; DrTime += 100000 )
  //	      {
  //        double dDriftTime = fb->getCalibration()->charge2TimeCorr(DrTime,Charge) ;
  //            cout << dDriftTime << "  ";
  //	      }
  //                cout << endl;
  //          }
  //    }
  //------------------------------------------------------------------------------


  DriftTime -= fb->getCalibration()->charge2TimeCorr(DriftTime,*PulseHeight) ;
report(INFO, kFacilityString) << "charge2TimeCorr:-"
<< fb->getCalibration()->charge2TimeCorr(DriftTime,*PulseHeight)
<< "hgs:DriftTime=" << DriftTime << endl;
  //
  //------------------------------------------------------------------------------
  //
  // Reverse Exact Flight Time Correction (Applied in HitCorrector.cc)
  //
  if ( FlightTime <= 0 )
    {
      DriftTime += sqrt((Radius*Radius)+(*ZonWire)*(*ZonWire))/ZDDriftFunction::kclight;
    }
  else
    {
      DriftTime += *FlightTime ;
report(INFO, kFacilityString) << "TOFGeantHit:+" << *FlightTime
<< "hgs:DriftTime=" << DriftTime << endl;
    } 
  //
  //------------------------------------------------------------------------------ 
  //
  // Add Straight-Line beta=1 Flight Time Correction (Subtracted in HitCorrector.cc)
  // 
  DriftTime -= (Radius/ZDDriftFunction::kclight) ;
report(INFO, kFacilityString) << "Radius/c:-"
<< (Radius/ZDDriftFunction::kclight)
<< "hgs:DriftTime=" << DriftTime << endl;
  //
  //------------------------------------------------------------------------------
  //  2001/10/30 M.Dubrovin Test print-out for Zcorr(Layer,ZWire) function
  //
  //  static DABoolean onceZ=true;
  //  if ( onceZ )
  //    {  onceZ=false;
  //
  //    cout << "Test of the correction sign at Z=0.1m"<< endl;
  //     for( int Layer=1; Layer != 48; Layer++ )
  //       { cout <<"  L:"<< Layer <<"  " ;
  //         FReal ZWire=0.1;
  //         double dt = fb->getCalibration()->Zcorr(Layer,ZWire); 
  //         cout << dt  << endl;
  //       }
  //
  //    cout << "Z(m), dt1(ps), dt2(ps), dt3(ps), ... "<< endl;
  //     for( int Layer=1; Layer != 10; Layer++ )
  //       { cout <<"  L:"<< Layer ;}
  //         cout << endl;
  //
  //    for( FReal ZWire=-1.2; ZWire <= 1.22; ZWire += 0.02)
  //     {
  //         cout << ZWire << " ";
  //     for( int Layer=1; Layer != 10; Layer++ )
  //       {
  //         double dt = fb->getCalibration()->Zcorr(Layer,ZWire);  
  //	 cout << dt << " ";
  //       }
  //         cout << endl;
  //     }
  //
  //    }
  //
  //------------------------------------------------------------------------------
  //
  // Subtract Z Propagation correction (added in HitCorrector.cc)
  // 
  DriftTime -= fb->getCalibration()->Zcorr(*ZDLayer,*ZonWire) ;
report(INFO, kFacilityString) << "Zcorr:-"
<< fb->getCalibration()->Zcorr(*ZDLayer,*ZonWire)
<< "hgs:DriftTime=" << DriftTime << endl;
  //

  *HitTime = DriftTime  ;

  return ; 
}
//
// replaces fortran dfofps from old zdoffcal
//
extern "C" void  zdmc_time2drift_( FReal*    DriftTime,
                                   FReal*    DriftDistance,
 			    	   FInteger* ZDLayer,
                                   FReal*    ZonWire,
                                   FInteger* Ambiguity,
                                   FReal*    EntranceAngle,
                                   FReal*    Cotangent,
                                   FReal*    PulseHeight)
{
  ZDMCDriftFB* fb  =  ZDMCDriftFB::instance();
  *DriftDistance =  fb->getCalibration()->time2SignedDrift( 
							   *DriftTime, 
							   *ZDLayer,
							   *Ambiguity,
							   *EntranceAngle,
							   *ZonWire) ;
    return ; 
}
//
// replaces fortran routine ktofps from old zdoffcal
//
extern "C" void zdmc_time2tdc_( FInteger* cellID,
                                FReal*    Time,
				FInteger* tdc)
{
  ZDMCDriftFB* fb = ZDMCDriftFB::instance();
  *tdc =  fb->getCalibration()->time2tdc(*cellID, *Time);  
  return;
}   
//
// replaces fortran routine ktofps from old zdoffcal
//
extern "C" void zdmc_tdc2time_( FInteger* cellID,
                                FReal*    Time,
				FInteger* tdc)
{
  ZDMCDriftFB* fb = ZDMCDriftFB::instance();
  *Time =  fb->getCalibration()->tdc2Time(*cellID, *tdc);  
  return;
}   
//
// replaces fortran routine kqofph from old zdoffcal
//
extern "C" void zdmc_charge2adc_(     FInteger* cellID,
                                      FReal*    Charge,
                                      FInteger* adc            )
{
  ZDMCDriftFB* fb = ZDMCDriftFB::instance();
  *adc =  fb->getCalibration()->charge2adc(*cellID, *Charge );  
  return;
}   
//
// replaces fortran routine phofkq from old zdoffcal
//
extern "C" void zdmc_adc2charge_(     FInteger* cellID,
                                      FReal*    Charge,
                                      FInteger* adc            )
{
  ZDMCDriftFB* fb = ZDMCDriftFB::instance();
  *Charge =  fb->getCalibration()->adc2Charge(*cellID, *adc );  
  return;
}   

//
// resolution from ZDDriftFunctionProd
//
// extern "C" void zdmc_resolution_(     FInteger* flayer,
//                                       FReal*    drift,
//                                       FReal*    eangle,
//                                       FReal*    costheta,
//                                       FReal*    phi,
//                                       FReal*    resolution)
// {
//   ZDMCDriftFB* fb = ZDMCDriftFB::instance();
//   int layer       = *flayer;
//   *resolution     =  fb->getCalibration()->resolution(layer, 
// 						      *drift, 
// 						      *costheta, 
// 						      *phi,
// 						      sin(*eangle));  
//   return;
// }   

//
// access to Constants unpacked in ZDDriftFunctionProd 
//
extern "C" void zdmc_adc_pedestal_( FInteger* cellID, FReal* adc_pedestal )
{
  ZDMCDriftFB* fb = ZDMCDriftFB::instance();
  *adc_pedestal=fb->getCalibration()->adcpedestal(*cellID);
  return;
}
extern "C" void zdmc_tdc_pedestal_( FInteger* cellID, FReal* tdc_pedestal )
{
  ZDMCDriftFB* fb = ZDMCDriftFB::instance();
  *tdc_pedestal=fb->getCalibration()->tdcped(*cellID);
  return;
}
extern "C" void zdmc_adc_width_( FInteger* cellID, FReal* adc_width )
{
  ZDMCDriftFB* fb = ZDMCDriftFB::instance();
  *adc_width=fb->getCalibration()->adcpedestalRMS(*cellID);
  return;
}
extern "C" void zdmc_tdc_width_( FInteger* cellID, FReal* tdc_width )
{
  ZDMCDriftFB* fb = ZDMCDriftFB::instance();
  *tdc_width=fb->getCalibration()->tdcpedRMS(*cellID);
  return;
}
extern "C" void zdmc_adc_slope_( FInteger* cellID, FReal* adc_slope )
{
  ZDMCDriftFB* fb = ZDMCDriftFB::instance();
  *adc_slope = fb->getCalibration()->adcgain(*cellID);
  return;
}
extern "C" void zdmc_tdc_slope_( FInteger* cellID, FReal* tdc_slope )
{
  ZDMCDriftFB* fb = ZDMCDriftFB::instance();
  *tdc_slope = fb->getCalibration()->tdcslope(*cellID);
  return;
}
extern "C" void zdmc_adc_slope_width_( FInteger* cellID, FReal* adc_slope_error )
{
  ZDMCDriftFB* fb = ZDMCDriftFB::instance();
  *adc_slope_error = fb->getCalibration()->adcgainRMS(*cellID);
  return;
}
extern "C" void zdmc_tdc_slope_width_( FInteger* cellID, FReal* tdc_slope_error )
{
  ZDMCDriftFB* fb = ZDMCDriftFB::instance();
  *tdc_slope_error = fb->getCalibration()->tdcslopeRMS(*cellID);
  return;
}
