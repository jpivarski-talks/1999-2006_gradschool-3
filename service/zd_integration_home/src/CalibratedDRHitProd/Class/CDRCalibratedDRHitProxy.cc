// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      CDRCalibratedDRHitProxy
// 
// Description: Proxy for final CalibratedDRHit (bunch found)
//
// Implementation:
//    Based on work by Jon Thaler and Inga Karliner.
//
//     This is the proxy triggered when the user extracts CalibratedDRHits.
//
//      Note that this proxy triggers CalibratedDRHitPreliminaryProxy and
//       BunchFinderProxy.
//
//     The basic idea is to make a list of CalibratedDRHits with just
//     the nominal T0 (this is the "preliminary" list), do bunch
//     finding to find the adjustment to the T0, then redo the
//     calibration with the new T0. The CalibratedDRHits are able to
//     extrapolate their drift distances from a shifted time [the bunch
//     finder uses that], but we want the nominal drift distance to be
//     the CORRECT bunch corrected distance. That way the user doesn't
//     have to carry around the T0 and bunch decision to figure out
//     the correct drift distance. The preliminary list is discarded
//     in the end.
//
//
//
// Author:      Adam Lyon
// Created:     Fri Aug 21 15:13:54 EDT 1998
// $Id: CDRCalibratedDRHitProxy.cc,v 1.10 2001/12/03 22:24:56 bkh Exp $
//
// Revision history
//
// $Log: CDRCalibratedDRHitProxy.cc,v $
// Revision 1.10  2001/12/03 22:24:56  bkh
// Check upper limit of allowed range not lower
//
// Revision 1.9  2001/12/03 21:57:23  bkh
// Fix cut on whichTDC for taking tdc value
//
// Revision 1.8  2001/11/29 22:18:09  bkh
// Restored raw dr hit proxy back to RawData producer
// insert raw charge and time into hit object
//
// Revision 1.7  2001/11/28 15:54:06  bkh
// Get header from proper place
//
// Revision 1.6  2000/03/13 16:01:35  lyon
// Removed valid checks
//
// Revision 1.4  2000/02/08 18:24:32  lyon
// Moved preliminary usage tag string to CalibratedData
//
// Revision 1.3  1999/11/18 22:04:08  lyon
// CalibratedDRHit now gets the same identifier as the CollatedRawDRHit (explicity)
//
// Revision 1.2  1999/11/16 23:55:28  cdj
// now check to make sure we have a valid daqID before making a CalibratedDRHit
//
// Revision 1.1.1.1  1999/09/23 02:44:15  cdj
// imported CalibratedDRHitProd sources
//
// Revision 1.8  1999/06/23 22:23:48  lyon
// Now must pass CDOffCal the DR geometry
//
// Revision 1.7  1999/05/18 19:14:51  lyon
// Bunch finder no longer fails for no hit events
//
// Revision 1.6  1999/05/03 21:14:42  lyon
// die a horrible death if no bunch finder
//
// Revision 1.5  1999/03/19 17:41:19  jjt
// Implement operator<< for all devices.
//
// Revision 1.4  1998/09/10 15:32:29  lyon
// New proxy
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

#include "CalibratedDRHitProd/CDRCalibratedDRHitProxy.h"
#include "RawData/CollatedRawDRHit.h"
#include "CDOffCal/DriftFunction.h"
#include "CalibratedData/CDPreliminaryID.h"
#include "PreliminaryDRHitProd/PreliminaryDRHitProxy.h"
#include "BunchFinder/BunchDecision.h"
#include "ADRGeom/ADRSenseWireStore.h"

// STL classes
#include <map>
#include <vector>
//
// constants, enums and typedefs
//

static const char* const kFacilityString = "CalibratedData.CDRCalibratedDRHitProxy" ;

//
// static data member definitions
//

//
// constructors and destructor
//
CDRCalibratedDRHitProxy::CDRCalibratedDRHitProxy()
                    : m_ptrTable( new value_type )
{ if (0 == m_ptrTable)
  { report(EMERGENCY, kFacilityString) << "Unable to allocate memory" << endl;
    ::exit(1);
  }
}

// CDRCalibratedDRHitProxy::CDRCalibratedDRHitProxy( const CDRCalibratedDRHitProxy& iproxy )
// {
//   *this = iproxy;
// }

CDRCalibratedDRHitProxy::~CDRCalibratedDRHitProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const CDRCalibratedDRHitProxy& CDRCalibratedDRHitProxy::operator=( const CDRCalibratedDRHitProxy& iproxy )
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
CDRCalibratedDRHitProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseAll() ;
   }
}


//////////////////////////////////////////////////////////////////////
//  faultHandler - does all the work of making the final list of 
//                  CalibratedDRHits.
const CDRCalibratedDRHitProxy::value_type*
CDRCalibratedDRHitProxy::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   // Launch the bunch finder by asking for the bunch decision.
   // This triggers BunchFinderProxy.
   FAItem<BunchDecision> bunchDecision;
   extract( iRecord, bunchDecision );

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
     
     report(DEBUG, kFacilityString)
       << "Best bunch is #" << bunchDecision->bestBunchNumber()
       << " with time of "  << bunchDecision->bestBunchTime() 
       << ", there were "   << bunchDecision->numberOfTestedBunches()
       << " bunches tested" << endl;
   }

   // Construct the final list of CalibratedDRHits (the Preliminary list 
   //             will be discarded in the end)
   
   // If the tOffset is 0, then the job is really easy
   if (tOffset != 0.0)
   {
      // There's work to be done!
      CalibrateDRHits( iRecord, tOffset );   // Recalibrate the hits 
                                             // taking into account the
                                             // time offset of the picked bunch
   }
   else
   { report(DEBUG, kFacilityString) << "Time offset is zero, so copy hits" 
			            << endl;

     // Extract the preliminary list of CalibratedDRHits.
   
     FATable<CalibratedDRHit> prelimHitList;
     extract( iRecord, prelimHitList, 
	      CDPreliminaryID::kSecondaryLabel ); 
   
     // Since the tOffset is zero, there's no reason to recalibrate the hits.
     // Just copy the hits from the preliminary list to this final one.
     for (FAPtrTable<CalibratedDRHit>::const_iterator
          iter = prelimHitList.begin();  iter != prelimHitList.end();  ++iter)
     {
       // Copy the old hit to the new list (yes - we want to copy
       //  to avoid deletion problems) - this is called a deep copy
       m_ptrTable->insert( new CalibratedDRHit(*iter) );
     }
   }      // if ( tOffset ) 
   
   // We're done - whew!
   return (m_ptrTable) ;
}


//////////////////////////////////////////////////////////////////////
// CalibrateDRHits - Turn Raw DR Hits into Calibrated DR Hits.
//                   This fills the m_ptrTable data member
//
//        Note double tOffset has a default value of 0.0 ns
//
void CDRCalibratedDRHitProxy::CalibrateDRHits( const Record& iRecord, 
					    double tOffset )
{

   // Extract the Raw DR hits
   FATable<CollatedRawDRHit> rawHits;
   extract( iRecord, rawHits );

   // Get the DriftFunction object from the BeginRun record
   // (the record handed to this proxy is an event record, hence
   //      the record and frame gymnastics below)
   // 
   FAItem<DriftFunction> calibration;
   extract( (iRecord.frame()).record(Stream::kBeginRun), calibration);
   assert(calibration.valid());

   // Get the DR Active Geometry
   FAItem<ADRSenseWireStore> senseWireStore;
   extract( (iRecord.frame()).record(Stream::kBaseGeometry), senseWireStore);
   assert(senseWireStore.valid());


   // Loop over the raw hits to construct calibrated hits
   for ( FATable<CollatedRawDRHit>::const_iterator iter = rawHits.begin();
	 iter != rawHits.end(); ++iter )
   {
      //Check to make sure we were given a valid address
      unsigned int daqId = iter->daqId();
      if( ( 0 == daqId ) ||
	  ( daqId > ADRSenseWireStore::kNumberOfWiresTotal ) ) {
	 report( WARNING, kFacilityString) << "RawDRHit with invalid daqId "
					   << daqId << endl;
      } else {

	// Calibrate this hit (note tOffset is converted to pico-seconds)
	 
	// Determine the identifier of the CollatedRawDRHit - use the 
        // same for the CalibratedDRHit. This will make going from
        // calibrated to collated easy.
	int id = iter->identifier();
            
	// Make the hit
	CalibratedDRHit* aHit = calibration->makeCalibratedHit(
	    *iter, id, &(*senseWireStore), 
	    tOffset*1000.0 );

	aHit->setRawChargeTime( CalibratedDRHit::RawChargeTime( 
	   iter->digiCharge(), 
	   ( aHit->whichTDC() < iter->digiTimes().size() ? 
	     iter->digiTimes()[ aHit->whichTDC() ]
	     : 0 ) ) ) ;
	 
	// Insert the new CalibratedDRHit
	//   if the new fails, an exception is thrown and program dies
	m_ptrTable->insert(aHit);
	 
      }

   }  // for (over raw hits)
   
}
