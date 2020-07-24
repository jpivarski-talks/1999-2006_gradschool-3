// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      CalibratedZDHitProxy
// 
// Description: Proxy for final CalibratedZDHit (bunch found)
//
// Implementation:
//    Based on work by Jon Thaler and Inga Karliner.
//
//     This is the proxy triggered when the user extracts CalibratedZDHits.
//
//
//
//
// Author:      Brian Heltsley
// Created:     Fri Aug 21 15:13:54 EDT 1998
// $Id: CalibratedZDHitProxy.cc,v 1.7 2003/06/26 18:10:11 huanggs Exp $
//
// Revision history
//
// $Log: CalibratedZDHitProxy.cc,v $
// Revision 1.7  2003/06/26 18:10:11  huanggs
// fix
//
// Revision 1.6  2003/04/24 17:37:09  huanggs
// changed with ZDDriftFunctionProd
//
// Revision 1.5  2003/04/24 15:09:31  huanggs
// make CalibratedZDHit
//
// Revision 1.4  2003/04/15 22:04:48  bkh
// Throw no data exception for incorrect config
//
// Revision 1.3  2003/03/20 15:27:29  bkh
// Make dummy version work for now
//
// Revision 1.2  2003/02/04 15:31:26  huanggs
// make ZD code effective
//
// Revision 1.1.1.1  2002/09/27 20:34:03  bkh
// imported CalibratedZDHitProd sources
//
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAPtrTable.h"

#include "CalibratedZDHitProd/CalibratedZDHitProxy.h"
#include "RawData/CollatedRawZDHit.h"
#include "ZDDriftFunctionProd/ZDDriftFunction.h"
#include "CalibratedData/CDPreliminaryID.h"
#include "BunchFinder/BunchDecision.h"
#include "AZDGeom/AZDSenseWireStore.h"

#include "DetectorConfiguration/DetectorConfiguration.h"
#include "DAException/DANoDataException.h"

// STL classes
#include <map>
#include <vector>
//
// constants, enums and typedefs
//

static const char* const kFacilityString = "CalibratedData.CalibratedZDHitProxy" ;

//
// static data member definitions
//

//
// constructors and destructor
//
CalibratedZDHitProxy::CalibratedZDHitProxy()
   : m_ptrTable( new value_type )
{ 
   if (0 == m_ptrTable)
   { report(EMERGENCY, kFacilityString) << "Unable to allocate memory" << endl;
   ::exit(1);
   }
}

// CalibratedZDHitProxy::CalibratedZDHitProxy( const CalibratedZDHitProxy& iproxy )
// {
//   *this = iproxy;
// }

CalibratedZDHitProxy::~CalibratedZDHitProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const CalibratedZDHitProxy& CalibratedZDHitProxy::operator=( const CalibratedZDHitProxy& iproxy )
// {
//   if( this != &iproxy ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iproxy );
//   }
//
//   return *this;
// }

//
// member functions
//
void
CalibratedZDHitProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseAll() ;
   }
}


//////////////////////////////////////////////////////////////////////
//  faultHandler - does all the work of making the final list of 
//                  CalibratedZDHits.
const CalibratedZDHitProxy::value_type*
CalibratedZDHitProxy::faultHandler( const Record& iRecord,
				    const DataKey& iKey )
{
   FAItem<DetectorConfiguration>                              iConfig   ;
   extract( iRecord.frame().record( Stream::kBaseGeometry ) , iConfig ) ;
   if( iConfig->useSInotZD() )
   {
      throw DANoDataException< value_type >( iRecord.stream(),iKey ) ;
   }
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   // Launch the bunch finder by asking for the bunch decision.
   // This triggers BunchFinderProxy.
   FAItem<BunchDecision> bunchDecision   ;
   extract( iRecord,     bunchDecision ) ;

   double tOffset;  // The time offset to be determined by the bunch finder
   
   // Did we test any bunches?
   if ( bunchDecision->numberOfTestedBunches() == 0 )
   {
     report(WARNING, kFacilityString) << "No bunches were tested!!"
				      << "  Using 0ns offset."
				      << endl;
     tOffset = 0.0;
   }
   else
   {
     // use the time of the best bunch
     tOffset = bunchDecision->bestBunchTime();
     
     report(INFO, kFacilityString)
       << "Best bunch is #" << bunchDecision->bestBunchNumber()
       << " with time of "  << bunchDecision->bestBunchTime() 
       << ", there were "   << bunchDecision->numberOfTestedBunches()
       << " bunches tested" << endl;
   }

   // Construct the final list of CalibratedZDHits (the Preliminary list 
   //             will be discarded in the end)

   CalibrateZDHits( iRecord, tOffset );   
   
   return m_ptrTable ;
}


//////////////////////////////////////////////////////////////////////
// CalibrateZDHits - Turn Raw ZD Hits into Calibrated ZD Hits.
//                   This fills the m_ptrTable data member
//
//        Note double tOffset has a default value of 0.0 ns
//
void CalibratedZDHitProxy::CalibrateZDHits( const Record& iRecord, 
					    double tOffset )
{
   // Extract the Raw ZD hits
   FATable<CollatedRawZDHit> rawHits   ;
   extract( iRecord,         rawHits ) ;

   // Get the DriftFunction object from the BeginRun record
   // (the record handed to this proxy is an event record, hence
   //      the record and frame gymnastics below)
   // 
   FAItem<ZDDriftFunction>                                 calibration;
   extract( (iRecord.frame()).record(Stream::kBeginRun), calibration);
   assert(calibration.valid());

   // Get the ZD Active Geometry

   FAItem<AZDSenseWireStore>                                 senseWireStore;
   extract( (iRecord.frame()).record(Stream::kBaseGeometry), senseWireStore);
   assert(senseWireStore.valid());

   // Loop over the raw hits to construct calibrated hits
   for ( FATable<CollatedRawZDHit>::const_iterator iter = rawHits.begin();
	 iter != rawHits.end(); ++iter )
   {
      //Check to make sure we were given a valid address
      unsigned int daqId = iter->daqId();
      if( ( 0 == daqId )
	  || ( daqId > AZDSenseWireStore::kNumberOfSenseWires )
	 )
      {
	 report( WARNING, kFacilityString) << "RawZDHit with invalid daqId "
					   << daqId << endl;
      } else 
      {
	 // Calibrate this hit (note tOffset is converted to pico-seconds)
	 
	 // Determine the identifier of the CollatedRawZDHit - use the 
	 // same for the CalibratedZDHit. This will make going from
	 // calibrated to collated easy.
	 int id = iter->identifier();

	 // Make the hit
	 CalibratedZDHit* aHit = calibration->makeCalibratedHit(
	    *iter, id, &(*senseWireStore), 
	    tOffset*1000.0 );

	 aHit->setRawChargeTime( CalibratedZDHit::RawChargeTime( 
	    iter->digiCharge(), 
	    ( aHit->whichTDC() < iter->digiTimes().size() ? 
	      iter->digiTimes()[ aHit->whichTDC() ]
	      : 0 ) ) ) ;
	 
	 // Insert the new CalibratedZDHit
	 //   if the new fails, an exception is thrown and program dies
	 m_ptrTable->insert(aHit);

      }
   }
}
