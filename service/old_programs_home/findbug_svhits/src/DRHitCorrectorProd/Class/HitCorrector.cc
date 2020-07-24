// -*- C++ -*-
//
// Package:     DRHitCorrectorProd
// Module:      HitCorrector
// 
// Description: Applies Corrections to DR Hits when assigned to tracks
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Ken McLean
// Created:     Wed Jul 28 16:54:57 EDT 1999
// $Id: HitCorrector.cc,v 1.19 2001/11/02 20:27:17 dubrovin Exp $
//
// Revision history
//
// $Log: HitCorrector.cc,v $
// Revision 1.19  2001/11/02 20:27:17  dubrovin
// Remove aCorrectedTime=hit.time() initialization at internal call
//
// Revision 1.18  2001/10/05 19:49:56  urner
// fixed entrance angle input
//
// Revision 1.17  2000/12/02 02:24:23  urner
// added functionality to correct the drift time of hits with no charge measurement for the average charge fo the hits on track with valid charge
//
// Revision 1.16  2000/12/01 19:58:15  mclean
// Updated to match CDOffCal and DRMCDriftFB.cc
//
// Revision 1.15  2000/11/18 04:57:30  wsun
// If swim to layer fails, swim to wire instead.
//
// Revision 1.14  2000/10/31 22:56:27  mclean
// CorrectedDRHit* used to "make" CorrectedDRHit's, we never actually used
// these, relying on storing the corrected values in link data, so now
// its gone forever!
//
// Deleted sigPropBeta parameter from DRHitCorrectorProd,HitCorrector
// and HitCorrectorProxy, this now comes from CDOffCal via the call to
// ZCorr added to HitCorrector.
//
// In HitCorrector.cc, correctedDriftTimeAndDist is overloaded with
// one additional argument the resolution, if this form of the
// function is called by the track fitter then the corrected resolution
// will be returned (fitting weight implimentation).
//
// The code is slightly reorganized and the user is blessed with rare
// informational messages about  which corrections are being applied.
//
// Additional error checking for failed track swims is included.
// The old drift function call is replaced by time2SignedDrift which
// will, if available return values for a two sided drift function.
// Less arguments are required since CDOffCal access the geometry from
// the frame internally.
//
// If, as when entrance angle corrections are enabled,  a 4th and 5th argument
// is added (entrance angle and zhit) then entrance angle corrections are added
// inside CDOffCal by overloading time2SignedDrift.
//
// Revision 1.13  2000/08/14 22:41:14  wsun
// Shifted drift time back to z=0, which is where CDOffCal will put it.
//
// Revision 1.12  2000/06/29 21:33:39  wsun
// Shifted drift time from z=0 to endplate.
//
// Revision 1.11  2000/06/29 00:05:34  wsun
// STL includes for OSF1 compilation, new data member m_signalPropBeta.
//
// Revision 1.10  2000/06/27 03:16:08  wsun
// Added parameters for controlling each correction individually.
//
// Revision 1.9  2000/06/22 14:27:42  cdj
// removed TBThreeVector/Point, made constant variables nanames begin with k and removed unnecessary includes
//
// Revision 1.8  2000/06/09 00:16:06  wsun
// Fixed bug in track swim and added makeCorrectedDRHit().
//
// Revision 1.7  1999/11/05 19:45:57  wsun
// Changed swim to radius zero to swim to origin.
//
// Revision 1.6  1999/10/26 16:53:31  mclean
// Bugfix: change track.z0() to track.position().z()
//
// Revision 1.5  1999/09/30 22:37:45  mclean
// Small bug fix in z correction: +-z/c instead of (maxZ+-Z/c)
//
// Revision 1.4  1999/08/26 21:19:44  mclean
// Add signeddriftdistance to argument list. This should be passed in
// to give CorrectedDRHit the hit ambiguity as determined by the Finder.
//
// Revision 1.3  1999/08/22 15:12:41  mclean
// Use ReadOut End information from ActiveElement's instead of hardcoding
//
// Revision 1.2  1999/08/21 14:37:05  mclean
// Add corrections for flight time and signal propagation over sense wires.
//
//
#include "Experiment/Experiment.h"
//
// system include files
//
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
//#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
//
// user include files
//
#include "Experiment/report.h"
#include "MagField/MagneticField.h"
#include "CDOffCal/DriftFunction.h"
#include "ADRGeom/ADRSenseWireStore.h"
#include "DRHitCorrectorProd/HitCorrector.h"
#include "KinematicTrajectory/KTHelix.h"
//#include "TrackRoot/TRSeedTrack.h"
#include "CalibratedData/CalibratedDRHit.h"
//
// constants, enums and typedefs
//
static const char* const kFacilityString = "DRHitCorrectorProd.HitCorrector" ;
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HitCorrector.cc,v 1.19 2001/11/02 20:27:17 dubrovin Exp $";
static const char* const kTagString = "$Name:  $";
//
//
//
// const vector<int> HitCorrector::NoCorrections(1,0);
// const vector<int> HitCorrector::StandardCorrections(1,0);
// const vector<int> HitCorrector::AllCorrections(1,1);
//
// DR Hit Corrector
//
HitCorrector::HitCorrector(){}
HitCorrector::HitCorrector( const DriftFunction* calibration,
			    const ADRSenseWireStore* geometry,
			    const MagneticField* field )
   :  m_calibration( calibration ),
      m_geometry( geometry ),
      m_field( field ),
      m_disableAllCorrections( false ),
      m_applySignalPropCorr( true ),
      m_applyTimeOfFlightCorr( true ),
      m_applyPulseHeightCorr( true ),
      m_applyEntranceAngleCorr( true ){}
HitCorrector::~HitCorrector(){}
//
// member functions 
//
//
// Entrance Angle, Z corrections on top of
// Mass, Arc, Propagation corrections
//
void HitCorrector::correctDriftTimeAndDist(
					   const KTHelix& track,           // input
					   const CalibratedDRHit& hit,     // input
					   GeV     mass,                   // input
					   Meters  signeddrift,            // input
					   double averagedCharge,          // input
					   CalibratedTimeHit::PicoSec& aCorrectedTime, // OUTPUT
					   Meters&  aCorrectedDriftDist,               // OUTPUT
					   Meters&  resValue                           // OUTPUT
					   ) const 
{
  aCorrectedTime      = hit.time()  ;
  double charge = hit.charge();
  aCorrectedDriftDist = signeddrift ;
  resValue            = m_calibration->
                        resolution(m_geometry->layerWire(hit.wire()).first);

  static DABoolean once(true);
  if ( once ) 
    {
      once = false ;
      if( m_disableAllCorrections ) 
	{ 
	  report( INFO, kFacilityString ) << "All Corrections Disabled" << endl; 
	} 
      else
	{
	  report( INFO, kFacilityString ) << "Weight Connection Enabled" << endl;
	}
    }
  if( m_disableAllCorrections ) { return ; } 

  //-------------------------------------------------------------------------
  // 2001/10/31 M.Dubrovin. Happy Hallowe'en! 
  // This is an extraordinary case for charge2TimeCorr. 
  // See the normal correction in CDOffCal/DataDriftFunction

  if(!(charge>0 && hit.nTDC()==1)) 
    {

      //  aCorrectedTime += m_calibration->charge2TimeCorr(aCorrectedTime, charge);
          aCorrectedTime += m_calibration->charge2TimeCorr(aCorrectedTime, averagedCharge);
    }


  KTHelix xtrack = track; 
  correctDriftTimeAndDist(xtrack,
			  hit,
			  mass,
			  signeddrift,
			  aCorrectedTime,
			  aCorrectedDriftDist) ; 


  // Swim to origin 
  ADRSenseWireStore::LayerWire coord=m_geometry->layerWire(hit.wire());
  UInt16 lyr=coord.first;
  HepVector3D Xaxis(1.,0.,0.);
  HepVector3D Yaxis(0.,1.,0.);
  KTHelix::MoveStatus status;
  KTHelix::ValueType  distance = 0 ;
  KTHelix::ValueType  radius   = 0 ;
  
  // Transport the track to the layer radius.
  radius = m_geometry->radiusNominal(lyr);
  status = xtrack.moveToRadius( radius,
	    	       	        distance,
			        KTMoveControl::kDirectionForward );

  // If swim to radius failed, try swimming to wire.  This can happen at the
  // top of a curler when charge is collected in a layer outside the track
  // circle.
  if ( status != KTMoveControl::kMoveOK )
    { 
      const AEWireInfo& wireInfo = m_geometry->wire( hit.wire() ) ;

      // Wire position and direction at the helix's z position.
      AEWireInfo::WireDirZ wirePosDir =
	 wireInfo.zPoint( xtrack.referencePoint().z() + xtrack.z0() ) ;

      Meters distance ;
      status = xtrack.moveToLine( wirePosDir.first,        // wire pos
				  wirePosDir.second.unit(),// wire dir
				  distance,
				  KTMoveControl::kDirectionForward ) ;

      if ( status != KTMoveControl::kMoveOK )
      {
	 report( WARNING, kFacilityString ) << "Swim-To-Wire Failed" << endl;
	 return ;
      }
    } 
  
  resValue  = m_calibration->
              resolution(m_geometry->layerWire(hit.wire()).first,
          	         aCorrectedDriftDist,
	                 xtrack.momentum((*m_field).BField()).cosTheta(), 
                         xtrack.position().phi() ); 
}
//
// Mass, Arc, Propagation corrections
//
void HitCorrector::correctDriftTimeAndDist(
					   const KTHelix& xtrack,          // input
					   const CalibratedDRHit& hit,     // input
					   GeV     mass,                   // input
					   Meters  signeddrift,            // input
					   CalibratedTimeHit::PicoSec& aCorrectedTime,    // OUTPUT
					   Meters&  aCorrectedDriftDist                   // OUTPUT
					   ) const
{
  //  aCorrectedTime      = hit.time()  ; // This should be taken from input parameter !
  aCorrectedDriftDist = signeddrift ;
  int mesflag(0);
  static DABoolean once(true);
  if ( once ) 
    {
      once = false ;
      if( m_disableAllCorrections )
	{ 
	  report( INFO, kFacilityString ) << "All Corrections Disabled" << endl;
	}
      else
	{
	  report( INFO, kFacilityString ) << "Corrections Enabled:" << endl;
	  if( m_applyTimeOfFlightCorr  )
	    {  
	      report( INFO, kFacilityString ) 
		<< "                     Time Of Flight" << endl;
	    }
	  if( m_applySignalPropCorr    )
	    {  
	      report( INFO, kFacilityString ) 
		<< "                     Signal Propagation" << endl;
	    }
	  if( m_applyEntranceAngleCorr )
	    {  
	      report( INFO, kFacilityString ) 
		<< "                     Entrance Angle" << endl;
	    }
	}
    }
  if( m_disableAllCorrections ) { return ; }
  
  
  //~~~~~~~~~ Apply corrections ~~~~~~~~~~
  KTHelix track = xtrack; 
  ADRSenseWireStore::LayerWire coord=m_geometry->layerWire(hit.wire());
  UInt16 lyr=coord.first;
  HepVector3D Xaxis(1.,0.,0.);
  HepVector3D Yaxis(0.,1.,0.);
  KTHelix::MoveStatus status;
  KTHelix::ValueType  radius   = 0 ;
  KTHelix::ValueType  distance = 0 ; 
  
  status = track.moveToReferencePoint( HepPoint3D( 0., 0., 0. ),
				       distance,
				       KTMoveControl::kDirectionEither) ;
  if ( status != KTMoveControl::kMoveOK )
    { 
      report( WARNING, kFacilityString ) << "Swim-To-Origin Failed" << endl;
      return ; 
    } 
  
  // Transport the track to the layer radius.
  radius = m_geometry->radiusNominal(lyr);
  status = track.moveToRadius(radius,distance,
			      KTMoveControl::kDirectionForward );
  if ( status != KTMoveControl::kMoveOK )
    { 
      const AEWireInfo& wireInfo = m_geometry->wire( hit.wire() ) ;

      // Wire position and direction at the helix's z position.
      AEWireInfo::WireDirZ wirePosDir =
	 wireInfo.zPoint( track.referencePoint().z() + track.z0() ) ;

      status = track.moveToLine( wirePosDir.first,        // wire pos
				  wirePosDir.second.unit(),// wire dir
				  distance,
				  KTMoveControl::kDirectionForward ) ;

      if ( status != KTMoveControl::kMoveOK )
      {
	 report( WARNING, kFacilityString ) << "Swim-To-Wire Failed" << endl;
	 return ;
      }
    } 
  
  // Now apply corrections.
  if( m_applyTimeOfFlightCorr )
    {
      float p    = track.momentum((*m_field).BField()).mag();
      float beta = p/sqrt((p*p)+(mass*mass));
      
      aCorrectedTime +=
	( radius - distance/beta ) / DriftFunction::kclight ;
    }
  
  if( m_applySignalPropCorr )
    {
      aCorrectedTime += m_calibration->Zcorr(lyr,track.position().z());
    }
  
  int ambiguity = 1 ;
  if ( signeddrift < 0 ) { ambiguity = -1 ; }
  
  if( m_applyEntranceAngleCorr ) 
   {
      const float twopi =  6.283185307 ;
      const float pi = 3.1415926536;
      HepVector3D mom = track.momentum((*m_field).BField());
      float phi         = mom.phi();
      HepNormal3D norm = track.position();
      norm.setZ(0);
      norm = norm.unit();

      double sinTrackToNormal =
	( norm.x() * mom.y() - norm.y() * mom.x() ) /
	( norm.perp() * mom.perp() ) ;
      double entranceAngle=180. * sinTrackToNormal / pi;
      if(fabs(sinTrackToNormal) > 0.5)
	{
	  entranceAngle=180*asin(sinTrackToNormal)/pi;
	}

      aCorrectedDriftDist = m_calibration->
      time2SignedDrift(
                       aCorrectedTime,
		       lyr,
		       ambiguity,
		       //                       fmod ( ( phi - ( twopi / 2. ) ) , twopi ) ,
		       entranceAngle,
                       track.position().z() ) ;
    }
  else 
    {
      aCorrectedDriftDist = m_calibration->
      time2SignedDrift(
		       aCorrectedTime,
		       lyr,
		       ambiguity) ;
    }
  return;
}
