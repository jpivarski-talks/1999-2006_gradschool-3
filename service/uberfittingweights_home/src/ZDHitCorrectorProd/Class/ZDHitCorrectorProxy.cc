// -*- C++ -*-
//
// Package:     ZDHitCorrectorProd
// Module:      ZDHitCorrectorProxy
// 
// Author:      Guangshun Huang
// Created:     Thu Feb 27 2003
// $Id: ZDHitCorrectorProxy.cc,v 1.1.1.1 2003/08/30 00:17:09 wsun Exp $
//
// Revision history
//
// $Log: ZDHitCorrectorProxy.cc,v $
// Revision 1.1.1.1  2003/08/30 00:17:09  wsun
// Imported ZDHitCorrectorProd sources.
//
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
#include "ZDHitCorrectorProd/ZDHitCorrectorProd.h"
#include "ZDHitCorrectorProd/ZDHitCorrector.h"
#include "ZDHitCorrectorProd/ZDHitCorrectorProxy.h"
#include "ZDDriftFunctionProd/ZDDriftFunction.h"
#include "MagField/MagneticField.h"
#include "AZDGeom/AZDSenseWireStore.h"
//
// STL classes
//
//#include <vector>
//
// constants, enums and typedefs
//
static const char* const kFacilityString = "ZDHitCorrectorProd.HitCorrectorProxy" ;
//
// CONSTRUCTOR
//
// ZDHitCorrectorProxy::ZDHitCorrectorProxy(vector<int> corrlist) : 
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

ZDHitCorrectorProxy::ZDHitCorrectorProxy( ZDHitCorrectorProd* aProd )
   : m_prod( aProd ), m_ZDHitCorrector( 0 )
{
}

//-----------------------------------------------------------------------------
//
// CONSTRUCTOR
//
ZDHitCorrectorProxy::~ZDHitCorrectorProxy()
{ delete m_ZDHitCorrector ; }
//-----------------------------------------------------------------------------
//
// MEMBER FUNCTIONS 
//
void ZDHitCorrectorProxy::invalidateCache()
{ delete m_ZDHitCorrector ; m_ZDHitCorrector = 0 ; }
//-----------------------------------------------------------------------------
//
//  Fault handler for the proxy 
//
const ZDHitCorrectorProxy::value_type*
ZDHitCorrectorProxy::faultHandler( const Record& iRecord,
				 const IfdKey& iKey )
{
  // There should be no cached data at this point
  assert(0 == m_ZDHitCorrector);
  //
  // Get the ZDDriftFunction object from the default stream
  //
  FAItem<ZDDriftFunction> calibration;
  extract( iRecord.frame().record(Stream::kBeginRun), calibration);
  assert(calibration.valid());
  //
  // Get the Magnetic Field
  //
  FAItem<MagneticField> field;
  extract((iRecord.frame()).record(Stream::kStartRun), field);
  assert(field.valid());
  //
  // Get the ZD Active Geometry
  //
  FAItem<AZDSenseWireStore> geometry;
  extract((iRecord.frame()).record(Stream::kBaseGeometry), geometry);
  assert(geometry.valid());
  //    
  // Make the hit correction object, passing the parameters
  //
  m_ZDHitCorrector = new ZDHitCorrector( &(*calibration),
				     &(*geometry),
				     &(*field));

  m_ZDHitCorrector->setDisableAllCorrections(
     m_prod->disableAllCorrections() ) ;

  m_ZDHitCorrector->setApplySignalPropCorr(
     m_prod->applySignalPropCorr() ) ;
  m_ZDHitCorrector->setApplyTimeOfFlightCorr(
     m_prod->applyTimeOfFlightCorr() ) ;
  m_ZDHitCorrector->setApplyPulseHeightCorr(
     m_prod->applyPulseHeightCorr() ) ;
  m_ZDHitCorrector->setApplyEntranceAngleCorr(
     m_prod->applyEntranceAngleCorr() ) ;

  // Do this last because it should override the previous settings.
  if( m_prod->applyStandardCorrections() )
  {
     m_ZDHitCorrector->setApplyStandardCorrections() ;
  }
  if( 0 == m_ZDHitCorrector )
    {
      report( EMERGENCY, kFacilityString ) 
	<< "can't allocate memory; aborting" << endl;
      assert( false );
      ::exit( 1 );
    }  
  return m_ZDHitCorrector;
}



