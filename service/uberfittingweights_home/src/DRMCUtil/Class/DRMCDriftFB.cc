// -*- C++ -*-
//
// Package:     DRMCUtil
// Module:      DRMCDriftFB
// 
// Description: Frame Broker object for DR MC access to CDOffCal.
//              (drift2time, time2tdc, charge2adc relations)
//              Based on Mike Marsh's K3FFrameBroker object.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      K W McLean
// Created:     Thu Sep 30 15:11:32 EDT 1999
// $Id: DRMCDriftFB.cc,v 1.14 2003/10/13 19:52:59 mccann Exp $
//
// Revision history
// $Log: DRMCDriftFB.cc,v $
// Revision 1.14  2003/10/13 19:52:59  mccann
// removing drmc_resolution_ (temporarily?) to see if anything depends on it
//
// Revision 1.13  2003/10/13 14:55:27  mccann
// updated drmc_resolution_ call to use new DriftFunction::resolution(-, -, -, -, -) interface
//
// Revision 1.12  2001/11/02 22:56:09  dubrovin
// Save commented stuff for test Zcorr and andcharge2TimeCorr functions
//
// Revision 1.11  2001/10/30 18:38:57  dubrovin
// Correct the sign of SmearedDriftDistance at call to signeddrift2Time
//
// Revision 1.10  2000/12/01 19:57:30  mclean
// Updated drift function inversion to match CDOffCal
//
// Revision 1.9  2000/04/23 21:01:02  mclean
// For the following functions the old hardcoded return values are now replaced
// by function calls to CDOffCal so their values now come properly from the
// appropriate constants entities.
// DRMCDriftFB Function:          CDOffCal:DriftFunction() function:
//     drmc_adc_pedestal_      >> 0        - now from adcpedestal()
//     drmc_tdc_pedestal_      >> 0        -  "   "   tdcped()
//     drmc_adc_width_         >> 10.0     -  "   "   adcpedestalRMS()
//     drmc_tdc_width_         >> 1.2      -  "   "   tdcpedRMS()
//     drmc_tdc_slope_         >> 250.     -  "   "   tdcslope()
//     drmc_adc_slope_         >> 1.       -  "   "   adcgain()
//     drmc_adc_slope_error_   >> 0.       -  "   "   adcgainRMS()
//     drmc_tdc_slope_error_   >> 0.       -  "   "   tdcslopeRMS()
// Note that, during the time these were hardcoded, the actual digitzation
// was done by calls to other CDOffCal routines like tdc2time etc. which
// used whatever slopes were set inside CDOffCal.
//
// Revision 1.8  2000/02/14 23:52:35  mclean
// Added hook to get resolution from CDOffCal
//
// Revision 1.7  1999/12/17 21:39:12  jjo
// Dereference cellID in drmc_time<->tdc, drmc_charge<->adc
//
// Revision 1.6  1999/12/17 20:27:43  mclean
// Added new wire/cellID argument to tdc2time,time2tdc,charge2adc,adc2charge
// to match modifications to CDOffCal.
//
// Revision 1.5  1999/12/09 21:25:20  mclean
// Added functions drmc_adc2charge_
//                 drmc_tdc2time_
//
// Revision 1.4  1999/10/28 19:41:56  mclean
// Added access to CDOffCal tdc and adc constants (now dummy values consistent
// with existing code).
// Added time2drift function.
//
// Revision 1.3  1999/10/20 21:32:56  jjo
// Change name of Fortran callable routines to have all lower case
// which is required.
//
//
#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Experiment/fortran_types.h"
#include "FrameAccess/FATable.h"
#include "DRMCUtil/DRMCDriftFB.h"
#include "ADRGeom/ADRSenseWireStore.h"
#include "FrameAccess/FAConstants.h"

#include <string>

// STL classes
//#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRMCUtil.DRMCDriftFB" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRMCDriftFB.cc,v 1.14 2003/10/13 19:52:59 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//
DRMCDriftFB* DRMCDriftFB::m_instance = 0;

//
// constructors and destructor
//
DRMCDriftFB::DRMCDriftFB(){}
DRMCDriftFB::~DRMCDriftFB(){}
//
// member functions
//
void DRMCDriftFB::setDrift( FAItem< DriftFunction     > calibration,
                            FAItem< ADRSenseWireStore > geometry     ) 
{
   m_calibration = calibration ;
   m_geometry    = geometry    ;
   return;
}
FAItem< DriftFunction     >  DRMCDriftFB::getCalibration()   { return m_calibration; }
FAItem< ADRSenseWireStore >  DRMCDriftFB::getGeometry()      { return m_geometry;    }
//
// static member functions
//
DRMCDriftFB*
DRMCDriftFB::instance()
{
  //-> Creates instance of singleton object
  if ( 0 == m_instance )
    {
      report( INFO, kFacilityString ) << "Creating DR Calibration Singleton" << endl;
      m_instance = new DRMCDriftFB;
    }
  return m_instance;
}
void
DRMCDriftFB::destroy()
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
// replaces fortran routine tmccl from old cdoffcal
//
extern "C" void  drmc_drift2time_( FReal*    FlightTime,
                                   FReal*    SmearedDriftDistance,
 			    	   FInteger* DRLayer,
                                   FReal*    ZonWire,
                                   FInteger* Ambiguity,
                                   FReal*    EntranceAngle,
                                   FReal*    Cotangent,
                                   FReal*    PulseHeight,
                                   FReal*    HitTime )
{
  DRMCDriftFB* fb  =  DRMCDriftFB::instance();
  double Radius    =  fb->getGeometry()->radiusNominal(*DRLayer);
  //------------------------------------------------------------------------------
  //
  // Invert Drift Function
  //

  
  //double DriftTime =  fb->getCalibration()->signeddrift2Time( *SmearedDriftDistance, 
  //							      *DRLayer, 
  //							      *EntranceAngle,
  //							      *ZonWire );

  // 2001/10/25 D.M. ^^^ Stupid! *SmearedDriftDistance is always > 0 !
  // The sign is transmitted through *Ambiguity !!! So far I have to add the next lines

         FReal SignedDriftDistance = (*SmearedDriftDistance) * (*Ambiguity);
  double DriftTime =  fb->getCalibration()->signeddrift2Time( SignedDriftDistance, 
  							      *DRLayer, 
  							      *EntranceAngle,
  							      *ZonWire );

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
  //
  //------------------------------------------------------------------------------
  //
  // Reverse Exact Flight Time Correction (Applied in HitCorrector.cc)
  //
  if ( FlightTime <= 0 )
    {
      DriftTime += sqrt((Radius*Radius)+(*ZonWire)*(*ZonWire))/DriftFunction::kclight;
    }
  else
    {
      DriftTime += *FlightTime ;
    } 
  //
  //------------------------------------------------------------------------------ 
  //
  // Add Straight-Line beta=1 Flight Time Correction (Subtracted in HitCorrector.cc)
  // 
  DriftTime -= (Radius/DriftFunction::kclight) ;
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
  DriftTime -= fb->getCalibration()->Zcorr(*DRLayer,*ZonWire) ;
  //

  *HitTime = DriftTime  ;

  return ; 
}
//
// replaces fortran dfofps from old cdoffcal
//
extern "C" void  drmc_time2drift_( FReal*    DriftTime,
                                   FReal*    DriftDistance,
 			    	   FInteger* DRLayer,
                                   FReal*    ZonWire,
                                   FInteger* Ambiguity,
                                   FReal*    EntranceAngle,
                                   FReal*    Cotangent,
                                   FReal*    PulseHeight)
{
  DRMCDriftFB* fb  =  DRMCDriftFB::instance();
  *DriftDistance =  fb->getCalibration()->time2SignedDrift( 
							   *DriftTime, 
							   *DRLayer,
							   *Ambiguity,
							   *EntranceAngle,
							   *ZonWire) ;
    return ; 
}
//
// replaces fortran routine ktofps from old cdoffcal
//
extern "C" void drmc_time2tdc_( FInteger* cellID,
                                FReal*    Time,
				FInteger* tdc)
{
  DRMCDriftFB* fb = DRMCDriftFB::instance();
  *tdc =  fb->getCalibration()->time2tdc(*cellID, *Time);  
  return;
}   
//
// replaces fortran routine ktofps from old cdoffcal
//
extern "C" void drmc_tdc2time_( FInteger* cellID,
                                FReal*    Time,
				FInteger* tdc)
{
  DRMCDriftFB* fb = DRMCDriftFB::instance();
  *Time =  fb->getCalibration()->tdc2Time(*cellID, *tdc);  
  return;
}   
//
// replaces fortran routine kqofph from old cdoffcal
//
extern "C" void drmc_charge2adc_(     FInteger* cellID,
                                      FReal*    Charge,
                                      FInteger* adc            )
{
  DRMCDriftFB* fb = DRMCDriftFB::instance();
  *adc =  fb->getCalibration()->charge2adc(*cellID, *Charge );  
  return;
}   
//
// replaces fortran routine phofkq from old cdoffcal
//
extern "C" void drmc_adc2charge_(     FInteger* cellID,
                                      FReal*    Charge,
                                      FInteger* adc            )
{
  DRMCDriftFB* fb = DRMCDriftFB::instance();
  *Charge =  fb->getCalibration()->adc2Charge(*cellID, *adc );  
  return;
}   

//
// resolution from CDOffCal
//
// extern "C" void drmc_resolution_(     FInteger* flayer,
//                                       FReal*    drift,
//                                       FReal*    eangle,
//                                       FReal*    costheta,
//                                       FReal*    phi,
//                                       FReal*    resolution)
// {
//   DRMCDriftFB* fb = DRMCDriftFB::instance();
//   int layer       = *flayer;
//   *resolution     =  fb->getCalibration()->resolution(layer, 
// 						      *drift, 
// 						      *costheta, 
// 						      *phi,
// 						      sin(*eangle));  
//   return;
// }   


//
// access to Constants unpacked in CDOffCal 
//
extern "C" void drmc_adc_pedestal_( FInteger* cellID, FReal* adc_pedestal )
{
  DRMCDriftFB* fb = DRMCDriftFB::instance();
  *adc_pedestal=fb->getCalibration()->adcpedestal(*cellID);
  return;
}
extern "C" void drmc_tdc_pedestal_( FInteger* cellID, FReal* tdc_pedestal )
{
  DRMCDriftFB* fb = DRMCDriftFB::instance();
  *tdc_pedestal=fb->getCalibration()->tdcped(*cellID);
  return;
}
extern "C" void drmc_adc_width_( FInteger* cellID, FReal* adc_width )
{
  DRMCDriftFB* fb = DRMCDriftFB::instance();
  *adc_width=fb->getCalibration()->adcpedestalRMS(*cellID);
  return;
}
extern "C" void drmc_tdc_width_( FInteger* cellID, FReal* tdc_width )
{
  DRMCDriftFB* fb = DRMCDriftFB::instance();
  *tdc_width=fb->getCalibration()->tdcpedRMS(*cellID);
  return;
}
extern "C" void drmc_adc_slope_( FInteger* cellID, FReal* adc_slope )
{
  DRMCDriftFB* fb = DRMCDriftFB::instance();
  *adc_slope = fb->getCalibration()->adcgain(*cellID);
  return;
}
extern "C" void drmc_tdc_slope_( FInteger* cellID, FReal* tdc_slope )
{
  DRMCDriftFB* fb = DRMCDriftFB::instance();
  *tdc_slope = fb->getCalibration()->tdcslope(*cellID);
  return;
}
extern "C" void drmc_adc_slope_width_( FInteger* cellID, FReal* adc_slope_error )
{
  DRMCDriftFB* fb = DRMCDriftFB::instance();
  *adc_slope_error = fb->getCalibration()->adcgainRMS(*cellID);
  return;
}
extern "C" void drmc_tdc_slope_width_( FInteger* cellID, FReal* tdc_slope_error )
{
  DRMCDriftFB* fb = DRMCDriftFB::instance();
  *tdc_slope_error = fb->getCalibration()->tdcslopeRMS(*cellID);
  return;
}




