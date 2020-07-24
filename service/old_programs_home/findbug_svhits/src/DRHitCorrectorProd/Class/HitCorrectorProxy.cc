// -*- C++ -*-
//
// Package:     DRHitCorrectorProd
// Module:      HitCorrectorProxy
// 
// Author:      Ken McLean
// Created:     Thu Aug 19 16:20:57 EST 1999
// $Id: HitCorrectorProxy.cc,v 1.7 2001/02/26 00:57:19 cdj Exp $
//
// Revision history
//
// $Log: HitCorrectorProxy.cc,v $
// Revision 1.7  2001/02/26 00:57:19  cdj
// extract MagneticField from StartRun so move Proxy to StartRun as well
//
// Revision 1.6  2000/10/31 22:56:27  mclean
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
// Revision 1.5  2000/06/29 00:05:34  wsun
// STL includes for OSF1 compilation, new data member m_signalPropBeta.
//
// Revision 1.4  2000/06/27 03:16:08  wsun
// Added parameters for controlling each correction individually.
//
// Revision 1.3  1999/10/04 18:20:29  wsun
// Extract DriftFunction from beginrun stream, not drcalibration.
//
// Revision 1.2  1999/08/21 14:37:06  mclean
// Add corrections for flight time and signal propagation over sense wires.
//
//
#include "Experiment/Experiment.h"
//
// system include files
//
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include <stdlib.h>
#include <assert.h>
//
// user include files
//
#include "Experiment/report.h"
#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"
#include "DRHitCorrectorProd/DRHitCorrectorProd.h"
#include "DRHitCorrectorProd/HitCorrector.h"
#include "DRHitCorrectorProd/HitCorrectorProxy.h"
#include "CDOffCal/DriftFunction.h"
#include "MagField/MagneticField.h"
#include "ADRGeom/ADRSenseWireStore.h"
//
// STL classes
//
//#include <vector>
//
// constants, enums and typedefs
//
static const char* const kFacilityString = "DRHitCorrectorProd.HitCorrectorProxy" ;
//
// CONSTRUCTOR
//
// HitCorrectorProxy::HitCorrectorProxy(vector<int> corrlist) : 
//   m_HitCorrector( 0 ),
//   m_corrlist(corrlist)
//   //
//   // should parse corrlist and list corrections to be applied here
//   //
//   {
//   if ( corrlist[0] == 0 ) 
//     report(NOTICE,kFacilityString) << "No Corrections Applied" << endl;
//   else
//     report(NOTICE,kFacilityString) << "All Corrections Applied" << endl;
//   }

HitCorrectorProxy::HitCorrectorProxy( DRHitCorrectorProd* aProd )
   : m_prod( aProd ), m_HitCorrector( 0 )
{
}

//-----------------------------------------------------------------------------
//
// CONSTRUCTOR
//
HitCorrectorProxy::~HitCorrectorProxy()
{ delete m_HitCorrector ; }
//-----------------------------------------------------------------------------
//
// MEMBER FUNCTIONS 
//
void HitCorrectorProxy::invalidateCache()
{ delete m_HitCorrector ; m_HitCorrector = 0 ; }
//-----------------------------------------------------------------------------
//
//  Fault handler for the proxy 
//
const HitCorrectorProxy::value_type*
HitCorrectorProxy::faultHandler( const Record& iRecord,
				 const IfdKey& iKey )
{
  // There should be no cached data at this point
  assert(0 == m_HitCorrector);
  //
  // Get the DriftFunction object from the default stream
  //
  FAItem<DriftFunction> calibration;
  extract( iRecord.frame().record(Stream::kBeginRun), calibration);
  assert(calibration.valid());
  //
  // Get the Magnetic Field
  //
  FAItem<MagneticField> field;
  extract((iRecord.frame()).record(Stream::kStartRun), field);
  assert(field.valid());
  //
  // Get the DR Active Geometry
  //
  FAItem<ADRSenseWireStore> geometry;
  extract((iRecord.frame()).record(Stream::kBaseGeometry), geometry);
  assert(geometry.valid());
  //    
  // Make the hit correction object, passing the parameters
  //
  m_HitCorrector = new HitCorrector( &(*calibration),
				     &(*geometry),
				     &(*field));

  m_HitCorrector->setDisableAllCorrections(
     m_prod->disableAllCorrections() ) ;

  m_HitCorrector->setApplySignalPropCorr(
     m_prod->applySignalPropCorr() ) ;
  m_HitCorrector->setApplyTimeOfFlightCorr(
     m_prod->applyTimeOfFlightCorr() ) ;
  m_HitCorrector->setApplyPulseHeightCorr(
     m_prod->applyPulseHeightCorr() ) ;
  m_HitCorrector->setApplyEntranceAngleCorr(
     m_prod->applyEntranceAngleCorr() ) ;

  // Do this last because it should override the previous settings.
  if( m_prod->applyStandardCorrections() )
  {
     m_HitCorrector->setApplyStandardCorrections() ;
  }
  if( 0 == m_HitCorrector )
    {
      report( EMERGENCY, kFacilityString ) 
	<< "can't allocate memory; aborting" << endl;
      assert( false );
      ::exit( 1 );
    }  
  return m_HitCorrector;
}



