#if !defined(CONSTANTSDELIVERY_CONSTANTSCONTROLLER_CC)
#define CONSTANTSDELIVERY_CONSTANTSCONTROLLER_CC
// -*- C++ -*-
//
// Library:     ConstantsDelivery
// Module:      ConstantsSourceController
// 
// Description: A DataSourceController for Constants access

// Usage:

// Invariants:
//
// Author:      Rich Baker
// Created:     Tues Nov 19 1997
//
// $Id: ConstantsSourceController.cc,v 1.7 2000/04/27 21:14:01 cdj Exp $
//
// Revision history
//
// $Log: ConstantsSourceController.cc,v $
// Revision 1.7  2000/04/27 21:14:01  cdj
// now works on OSF using MICO ORB
//
// Revision 1.6  2000/02/24 20:12:02  cdj
// speed improvement: only resynchronize DBConstants if run number changes
//
// Revision 1.5  1999/07/14 21:22:27  cdj
// now uses bootfile path to find servers
//
// Revision 1.4  1999/07/10 21:03:21  cdj
// now inform user if server is unavailable
//
// Revision 1.3  1999/04/29 18:45:35  cdj
// updated to use new constants implementation
//
// Revision 1.2  1998/11/24 20:12:44  baker
// Change normal constructor for ConstantsSourceBinder
// to allow specification of the desired Federated
// Database.
// Added a Constructor for DBConstantsRecordInfo
// that takes a single string variable.
//
// Revision 1.1  1998/11/16 22:03:33  mkl
// renamed ConstantsBinder/Controller ConstantsSourceBinder/Controller
//
// Revision 1.6  1998/11/10 22:12:08  baker
// Several Fixes.
// Constants Delivery Tested and Working!
// Some kludges still need to be worked on though.
//
// Revision 1.5  1998/11/07 19:12:46  baker
// Added Data Source ID information to Controller
//
// Revision 1.4  1998/11/04 18:55:37  baker
// Fix stupid errors so that this library actually compiles (finally!)
//
// Revision 1.3  1998/11/04 15:39:10  baker
// Continuing updates to get this library compiled
//
// Revision 1.2  1998/09/30 20:39:58  baker
// Changed class names and include file names as
// necessary for migration from old DataBaseDelivery
// to new ConstantsDelivery packages.
//
// Revision 1.1  1998/09/30 18:22:59  baker
// // Renamed DataBaseDelivery files as ConstantsDelivery
//
// Revision 1.3  1998/08/19 14:23:42  cdj
// forgot to synchronize passive Records when call gotoNextRecord
//
// Revision 1.2  1998/08/19 03:14:21  cdj
// re-engineered.  This probably won't compile.
//
// Revision 1.1.1.2  1998/01/14 20:56:53  baker
// make DataBaseDelivery library compileable
//
// Revision 1.1.1.1  1997/12/17 20:25:11  cleo3
// imported DataBaseDelivery sources
//


#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include "Experiment/Experiment.h"
#include "Experiment/report.h"
#include "Suez/externConst.h"

// user include files
#include "ConstantsDelivery/ConstantsSourceController.h"
#include "DataHandler/Stream.h"
#include "DataHandler/StreamSet.h"

// stl include files

// forward declarations

// Jan 14, 1998 Rich Baker
// Corrected errors in initialization of status variables
// 11/7/98 Rich Baker
// Added another parameter to this constructor to allow the 
// binder to pass the data source ID which is stored in a
// new m_sourceID member data item.
ConstantsSourceController::ConstantsSourceController(
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
   //need to create the Proxies since they hold the DBConstants
   // which we must use to do the access the Records
   dbProxyDeliverer().makeProxies( iReadStreams ) ;

   if( ! dbProxyDeliverer().sourceAccessible() ) {
      setInitializationStatus( kInitSourceInaccessible );
      return;
   }

   setIsSequentiallyAccessing( true );
   setInitializationStatus( kInitNoError );
}

ConstantsSourceController::~ConstantsSourceController() {
   delete &m_prioritizedPeekedRecords;
}

DataSourceController::SyncStatus 
ConstantsSourceController::synchronizeSource( const SyncValue& iSyncTo )
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
	 if( dbProxyDeliverer().synchronize( *itStream, 
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
ConstantsSourceController::findNextRecord(SyncValue& oSyncValueOfNextRecord,
				   Stream::Type& oRecordTypeOfNextRecord )
{
   //==============================================
   // DBConstants do not yet allow synchronization on run number
   //  Therefore we can not use this as an active source

   report(EMERGENCY, kFacilityString ) <<"source "<< dataSourceID() <<" can not be used as an active source" << endl;
   return kSearchUnknownError;
   //==============================================

   if( ! m_havePeekedAhead){
      //do we know if we have multiple records at this same syncValue?
      if( m_prioritizedPeekedRecords.empty() ) {
	 //Only need to peek at the active streams
	 const Stream::Set& tempActiveStreams( activeStreams() );
	 Stream::Set::const_iterator itLastStream = tempActiveStreams.end();
	 
	 //Need to find the 'next' record with the smallest sync value
	 SyncValue tempSyncValue = SyncValue(SyncValue::kMaxRunEventValue,
					     SyncValue::kMaxRunEventValue );

	 for( Stream::Set::const_iterator itStream = tempActiveStreams.begin();
	      itStream != itLastStream;
	      ++itStream ) {
	    SyncValue syncOfNextRecord;
	    if( dbProxyDeliverer().syncValueOfNextRecord( *itStream,
							syncOfNextRecord ) ) {
	       if( syncOfNextRecord < tempSyncValue ) {
		  tempSyncValue = syncOfNextRecord;
		  m_prioritizedPeekedRecords.clear();
		  m_prioritizedPeekedRecords.insert( *itStream );
		  
	       } else if ( tempSyncValue == syncOfNextRecord ){
		  //add this Record to our list of Records at this syncValue
		 //		  if( m_prioritizedPeekedRecords.empty() ) {
		 //  m_prioritizedPeekedRecords.insert( tempPrevRecord );
	         //}
		  m_prioritizedPeekedRecords.insert( *itStream );
	       }
	    } else {
	       //problem looking at next record
	       return ( m_peekAheadStatus = kSearchProblemWithSource);
	    }
	 }
	 m_peekAheadSyncValue = tempSyncValue;

      } 
	
      //pick first record in our prioritized list
      STL_MULTISET_COMP(Stream::Type, StreamPriorityGreater)::iterator itFirstRecord = 
	 m_prioritizedPeekedRecords.begin();
      
      m_peekAheadRecord = *itFirstRecord;
      //now remove this record from our prioritized list
      m_prioritizedPeekedRecords.erase( itFirstRecord );
   }

   oRecordTypeOfNextRecord = m_peekAheadRecord;
   oSyncValueOfNextRecord = m_peekAheadSyncValue;

   return m_peekAheadStatus;
}

DataSourceController::SearchStatus 
ConstantsSourceController::gotoNextRecord( void )
{
   //all of the hard work was done in findNextRecord()
   Stream::Type tempNextRecord;
   SyncValue tempSyncValue;
   SearchStatus returnStatus = findNextRecord(  tempSyncValue, tempNextRecord );

   setCurrentStop( tempNextRecord );
   setSyncValue( tempSyncValue );


   //now tell allrecords to synchronize
   Stream::Set::const_iterator itLastStream = recordsToRetrieve().end();
   for( Stream::Set::const_iterator itStream = recordsToRetrieve().begin();
	itStream != itLastStream;
	++itStream ) {
      SyncValue testValue;
      if( ! dbProxyDeliverer().synchronize( *itStream, 
					    tempSyncValue, 
					    testValue ) ) {
	 //we had a problem
	 report(ERROR, kFacilityString) <<" gotoNextRecord, proxy's didn't sync"<<endl;
	 returnStatus = kSearchUnknownError;
	 break;
      }
   }
   m_havePeekedAhead = false;

   return returnStatus;
}

ConstantsProxyDeliverer& 
ConstantsSourceController::dbProxyDeliverer()
{
   return (ConstantsProxyDeliverer& ) (*proxyDeliverer() );
}


DABoolean ConstantsSourceController::canRandomAccess( void ) const
{
   return true;
}
// 11/7/98 Rich Baker
// Updated this function so that it returns the
// source ID that was passed from the ConstantsSourceBinder class.
string 
ConstantsSourceController::dataSourceID( void ) const
{
   return m_sourceID;
}

string 
ConstantsSourceController::parameters( void ) const
{
   //need to create a string out of the data in DBConstantsSelectionTag
   // 11/9/98 Rich Baker
   // This function has to return the same parameters that
   // were passed to the base class of the ConstantsSourceBinder.
   return m_selectionTags.toString();
}

void 
ConstantsSourceController::implementSetForcedSequentialAccess( DABoolean iFlag )
{}

ProxyDeliverer* 
ConstantsSourceController::createProxyDeliverer( void ) const
{
   return new ConstantsProxyDeliverer( m_sourceID, m_selectionTags );
}

#endif /* CONSTANTSDELIVERY_CONSTANTSCONTROLLER_CC */

