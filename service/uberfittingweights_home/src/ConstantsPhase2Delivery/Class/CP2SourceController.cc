#if !defined(CP2DELIVERY_CP2SOURCECONTROLLER_CC)
#define CP2DELIVERY_CP2SOURCECONTROLLER_CC
// -*- C++ -*-
//
// Library:     ConstantsPhase2Delivery
// Module:      CP2SourceController
// 
// Description: A DataSourceController for Constants access

// Usage:

// Invariants:
//
// Author:      Valentine Kouznetsov
// Created:     Fri Jan  3 14:06:16 EST 2003
// $Id: CP2SourceController.cc,v 1.2 2003/02/07 22:09:35 vk Exp $
//
// Revision history
//
// $Log: CP2SourceController.cc,v $
// Revision 1.2  2003/02/07 22:09:35  vk
// Add NextRecordSync functionality
//
// Revision 1.1.1.1  2003/01/28 21:49:54  vk
// imported ConstantsPhase2Delivery sources
//
// Revision 1.1.1.1  2003/01/28 20:40:24  vk
// imported ConstantsPhase2Delivery sources
//
//
// Wed Jan 15 10:53:08 EST 2003 vk
// First attempt to create working source controller.

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "Experiment/Experiment.h"
#include "Experiment/report.h"

// user include files
#include "ConstantsPhase2Delivery/CP2SourceController.h"
#include "DataHandler/Stream.h"
#include "DataHandler/StreamSet.h"

// stl include files

// forward declarations

static const char* const kFacilityString = "ConstantsPhase2Delivery.CP2SourceController";

CP2SourceController::CP2SourceController(
   const Stream::Set& iReadStreams,
   const string& dsID,
   const DBConstantsSelectionTag& iTags ) :
   DataSourceController(),
   m_sourceID( dsID ),
   m_selectionTags( iTags ),
   m_peekAheadRecord(Stream::kNone ),
   m_peekAheadStatus( DataSourceController::kSearchNoError ),
   m_havePeekedAhead(false),
   m_prioritizedPeekedRecords( * (new STL_MULTISET_COMP( Stream::Type, StreamPriorityGreater) ) )
{
   if( 0 == &m_prioritizedPeekedRecords ){
      report( ERROR, kFacilityString) << "unable to allocate memory" << endl;
      assert(false);
      exit( -1 );
   }
   //See if we can read these records

   DABoolean isOK( setRecordsToRetrieve( iReadStreams ) );
   if( ! isOK ){
      setInitializationStatus( kInitInvalidStopsForSource );
      return;
   }

   //need to create the Proxies since they hold the DBCP2
   // which we must use to do the access the Records
   CP2dbProxyDeliverer().makeProxies( iReadStreams ) ;

   if( ! CP2dbProxyDeliverer().sourceAccessible() ) {
      setInitializationStatus( kInitSourceInaccessible );
      return;
   }

   setIsSequentiallyAccessing( true );
   setInitializationStatus( kInitNoError );

}

CP2SourceController::~CP2SourceController() {
   delete &m_prioritizedPeekedRecords;
}

DataSourceController::SyncStatus 
CP2SourceController::synchronizeSource( const SyncValue& iSyncTo )
{

   SyncStatus retStatus = kSyncNoError;

   //NOTE: only need to resynchronize if run has changed
   if( currentSyncValue().runNumber() != iSyncTo.runNumber() ) {
      //Need to resynchronize all Records that we are supposed to retrieve

      SyncValue closestSyncValue = SyncValue::kBeginningOfTime ;

      //needed if two streams have the same syncValue
      StreamPriorityGreater streamPriorityGreater;

      Stream::Type synchedRecord;
      Stream::Set::const_iterator itLastStream = recordsToRetrieve().end();
      for( Stream::Set::const_iterator itStream = recordsToRetrieve().begin();
	   itStream != itLastStream;
	   ++itStream ) {
	 SyncValue testValue;
	 if( CP2dbProxyDeliverer().synchronize( *itStream, 
					     iSyncTo, 
					     testValue ) ) {
	    if( testValue > closestSyncValue ) {
	       closestSyncValue = testValue;
	       synchedRecord = *itStream;
	    } else if( testValue == closestSyncValue ) {
	       //need to choose this record if it has a higher 
               // priority then the
	       //  previous
	       if( streamPriorityGreater( *itStream, synchedRecord ) ) {
		  synchedRecord = *itStream;
	       }
	    }
	 } else {
	    //didn't synchronize properly
	    return DataSourceController::kSyncProblemWithSource;
	 }

      }
      //Must set current stop to the Record that has a syncValue closest to the
      //  value I am supposed to sync to.
      setCurrentStop( synchedRecord );

   }
   setSyncValue( iSyncTo );

   return retStatus;
}

DataSourceController::SearchStatus
CP2SourceController::findNextRecord(SyncValue& oSyncValueOfNextRecord,
				   Stream::Type& oStreamTypeOfNextRecord )
{

   // define max syncValue
   SyncValue maxSyncValue = SyncValue(SyncValue::kMaxRunEventValue,
				      SyncValue::kMaxRunEventValue );

   if( ! m_havePeekedAhead){
      // do we know if we have multiple records at this same syncValue?
      if( m_prioritizedPeekedRecords.empty() ) {
	 // Only need to peek at the active streams
	 const Stream::Set& tempActiveStreams( activeStreams() );
	 Stream::Set::const_iterator itLastStream = tempActiveStreams.end();
	 
	 // Need to find the 'next' record with the smallest sync value
	 SyncValue tempSyncValue = SyncValue(SyncValue::kMaxRunEventValue,
					     SyncValue::kMaxRunEventValue );

	 for( Stream::Set::const_iterator itStream = tempActiveStreams.begin();
	      itStream != itLastStream;
	      ++itStream ) {
	    SyncValue syncOfNextRecord;
	    if( CP2dbProxyDeliverer().syncValueOfNextRecord( *itStream,
							syncOfNextRecord ) ) {
	       if( syncOfNextRecord < tempSyncValue ) {
		  tempSyncValue = syncOfNextRecord;
		  m_prioritizedPeekedRecords.clear();
		  m_prioritizedPeekedRecords.insert( *itStream );
		  
	       } else if ( tempSyncValue == syncOfNextRecord &&
	                   tempSyncValue != maxSyncValue
	                 ){
		  m_prioritizedPeekedRecords.insert( *itStream );
	       }
	    } else {
	       //problem looking at next record
	       return ( m_peekAheadStatus = kSearchProblemWithSource);
	    }
            //report(DEBUG, kFacilityString) << "tempSyncValue " << tempSyncValue.runNumber() << endl;
	 }
	 m_peekAheadSyncValue = tempSyncValue;

      } 
	
      // if there is no more active streams we're done
      if(!m_prioritizedPeekedRecords.size()) return kSearchEndOfSource;

      // pick first record in our prioritized list
      STL_MULTISET_COMP(Stream::Type, StreamPriorityGreater)::iterator itFirstRecord = 
	 m_prioritizedPeekedRecords.begin();

      m_peekAheadRecord = *itFirstRecord;
      // now remove this record from our prioritized list
      m_prioritizedPeekedRecords.erase( itFirstRecord );

      // Tell object that we already cached our values
      m_havePeekedAhead = true;
   }

   oStreamTypeOfNextRecord = m_peekAheadRecord;
   oSyncValueOfNextRecord = m_peekAheadSyncValue;

   return m_peekAheadStatus;
}

DataSourceController::SearchStatus 
CP2SourceController::gotoNextRecord( void )
{

   //all of the hard work was done in findNextRecord()
   Stream::Type tempNextRecord;
   SyncValue tempSyncValue;
   SearchStatus returnStatus = findNextRecord(  tempSyncValue, tempNextRecord );
   
   setCurrentStop( tempNextRecord );
   setSyncValue( tempSyncValue );

   // now tell all records to synchronize
   Stream::Set::const_iterator itLastStream = recordsToRetrieve().end();
   for( Stream::Set::const_iterator itStream = recordsToRetrieve().begin();
	itStream != itLastStream;
	++itStream ) {
      SyncValue testValue;
      if( ! CP2dbProxyDeliverer().synchronize( *itStream, 
					    tempSyncValue, 
					    testValue ) ) {
	 //we had a problem
	 report(ERROR, kFacilityString) <<" gotoNextRecord, proxy's didn't sync"<<endl;
	 returnStatus = kSearchUnknownError;
	 break;
      }
   }
   
   // clear cache of CP2SourceController
   m_havePeekedAhead = false;

   if( m_peekAheadRecord != Stream::kNone && 
       tempSyncValue != SyncValue::kBeginningOfTime &&
       m_prioritizedPeekedRecords.size() > 1 ) {
   //
   // Tell explicitly to current stop to update its NextRecord only if
   // - it's active stream
   // - we're not at the beginning of time
   // - there are number of multiple active streams around 
   //   next syncValue for single active stream is already found by findNextRecord call

     SyncValue tSyncValue;
     DABoolean statusOfNextValuaOfCurrentStop = 
       CP2dbProxyDeliverer().syncValueOfNextRecord(m_peekAheadRecord,tSyncValue);
     if( !statusOfNextValuaOfCurrentStop ) {
       // we have a problem with next value of current stop
       report(ERROR, kFacilityString)
         << "We have a problem with next value of current record (stop)" << endl;
       returnStatus = kSearchUnknownError;
     }
   
   }

   report(DEBUG, kFacilityString) 
     << "gotoNextRecord, current syncValue " << currentSyncValue() << endl;

   return returnStatus;
}

CP2ProxyDeliverer& 
CP2SourceController::CP2dbProxyDeliverer()
{
   return (CP2ProxyDeliverer& ) (*proxyDeliverer() );
}

DABoolean CP2SourceController::canRandomAccess( void ) const
{
   return true;
}

string 
CP2SourceController::dataSourceID( void ) const
{
   return m_sourceID;
}

string 
CP2SourceController::parameters( void ) const
{
   return m_selectionTags.toString();
}

void 
CP2SourceController::implementSetForcedSequentialAccess( DABoolean iFlag )
{}

ProxyDeliverer* 
CP2SourceController::createProxyDeliverer( void ) const
{
   return new CP2ProxyDeliverer( m_sourceID, m_selectionTags );
}

#endif /* CP2DELIVERY_CP2SOURCECONTROLLER_CC */

