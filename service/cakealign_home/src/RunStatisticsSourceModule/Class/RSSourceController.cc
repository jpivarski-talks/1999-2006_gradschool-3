// -*- C++ -*-
//
// Package:     RunStatisticsSourceModule
// Module:      RSSourceController
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Tue Jan 25 13:49:17 EST 2000
// $Id: RSSourceController.cc,v 1.16 2003/09/15 18:32:08 cdj Exp $
//
// Revision history
//
// $Log: RSSourceController.cc,v $
// Revision 1.16  2003/09/15 18:32:08  cdj
// only do DEBUG output loop if DEBUG report level is set
//
// Revision 1.15  2003/03/04 00:07:15  cleo3
// fixed bug under mico since I assumed CORBA _var variables used reference counting
//
// Revision 1.14  2003/02/25 18:54:21  cleo3
// removed problematic spaces in CLEORB macro call
//
// Revision 1.13  2003/01/30 15:36:13  cleo3
// changed loop index to unsigned long to avoid function ambiguity
//
// Revision 1.12  2002/02/15 17:53:48  hubert
// Use methods instead of static variables for type codes to avoid dependency on static constructor run order
//
// Revision 1.11  2001/11/29 19:40:38  cdj
// now properly handles multiple start and begin run records
//
// Revision 1.10  2001/11/29 17:43:04  cdj
// can now handle multiple begin and start run records
//
// Revision 1.9  2001/11/16 16:05:07  cdj
// recent change now compiles on OSF and made new reports DEBUG
//
// Revision 1.8  2001/11/15 18:29:48  hubert
// Adjusted output
//
// Revision 1.7  2001/10/26 00:53:36  cdj
// should no longer talk with database every other stop
//
// Revision 1.6  2001/10/25 19:46:29  cdj
// if don't get startrun the first time, try again (needed for Online)
//
// Revision 1.5  2000/11/09 03:52:01  lkg
// In getDataForRun, fill member item m_rsList using m_rsListList rather
// than the local variable "data".  On OSF, the pointers in rsList must
// have pointed to garbage once "data" was deleted.  Were we lucky on solaris,
// or is this a visibroker/mico difference?
//
// Revision 1.4  2000/11/08 03:04:58  lkg
// tweaked scale factors for some of the modified CESR parameters
//
// Revision 1.3  2000/05/08 19:52:07  cdj
// now compiles on OSF with MICO
//
// Revision 1.2  2000/02/29 22:40:36  cdj
// added DEBUG output when no Run Statistics found
//
// Revision 1.1.1.1  2000/01/29 20:47:37  cdj
// imported RunStatisticsSourceModule source
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
#include <map>
#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
//#if defined(SUNCC)
//#include "/usr/local/vb3.2/include/viostrm.h"
//#endif

// user include files
#include "Experiment/report.h"
#include "RunStatisticsSourceModule/RSSourceController.h"
#include "RunStatisticsSourceModule/RSProxyDeliverer.h"
#include "RunStatisticsSourceModule/infoXandStream.h"

#include "CLEORB.hxx"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
typedef STL_VECTOR(InfoX::Type) UsedStatesList;

static const char* const kFacilityString = "RunStatisticsSourceModule.RSSourceController" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: RSSourceController.cc,v 1.16 2003/09/15 18:32:08 cdj Exp $";
static const char* const kTagString = "$Name:  $";

typedef STL_VECTOR(const DBRunStatistics::RunStatistics* ) RSList;
//
// static data member definitions
//
const char* const 
RSSourceController::kDataSourceID = "RunStatistics";
//
// constructors and destructor
//
RSSourceController::RSSourceController(const Stream::Set& iStreams ) :
   m_runNumber(0),
   m_numberOfRetrievedRecords(0)
{
   //Try to attach to the RunStatistics server
   PMCTRY 
      {
	 CLEORB_BIND2_ANON(m_rsdb,DBRunStatistics,DataBase)
	    //m_rsdb = DBRunStatistics::DataBase::_bind();
      }
   PMCCATCH(CORBA::SystemException,exc) 
      {
	 report( ERROR, kFacilityString ) << "Error binding to RS server.\n"
					  << exc << endl;
	 setInitializationStatus( kInitSourceInaccessible );
	 return;
      }
   PMCEND_CATCH
      
   if( ! setRecordsToRetrieve( iStreams ) ) {
      setInitializationStatus( kInitInvalidStopsForSource );
      return;
   }

   //set which states to use
   Stream::Set::const_iterator itEnd = iStreams.end();
   for( Stream::Set::const_iterator itStream = iStreams.begin();
	itStream != itEnd;
	++itStream ) {
      m_usedStatesList.push_back( stream2InfoXType( *itStream ) );
   }

   //we do not necessarily need to read data in time order
   setIsSequentiallyAccessing( false );
}

// RSSourceController::RSSourceController( const RSSourceController& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

RSSourceController::~RSSourceController()
{
   for(STL_VECTOR(DBRunStatistics::RunStatisticsList*)::iterator itList =
	  m_rsListList.begin();
       itList != m_rsListList.end();
       ++itList) {
      delete *itList;
   }
}

//
// assignment operators
//
// const RSSourceController& RSSourceController::operator=( const RSSourceController& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//
static
UInt32
findEventNumber(const DBRunStatistics::RunStatistics* iRS )
{
   UInt32 returnValue = 0;
   for( int index=0; index < iRS->rsdata.length(); ++index ){
    //this is useful for debugging
    //report(INFO, "RSProxySingleValue") << runStatistics()->rsdata[index].name << endl;
    //first check to see if it is the correct type
    CORBA::TypeCode_var typeCode = 
       iRS->rsdata[index].value.type();
    if( typeCode->equal( CORBA::_tc_long ) ) {
       //now see if the name is correct
       // skip the component and path and go straight to field name
       const char * fieldName = iRS->rsdata[index].name;
       if( 0 == strcmp(fieldName, "EventNumber") ) {
	  CORBA::Long temp;
	  iRS->rsdata[index].value >>= temp;
	  returnValue =temp;
	  break;
       }
    }
   }
   return returnValue;
}

DataSourceController::SyncStatus 
RSSourceController::synchronizeSource( const SyncValue& iSyncTo )
{
   SyncStatus returnValue = kSyncNoError;
   if( m_runNumber != iSyncTo.runNumber() ||
       m_usedStatesList.size() != m_numberOfRetrievedRecords) {
      //try to get new info
      returnValue = getDataForRun( iSyncTo.runNumber() );
   }
   
   if( kSyncNoError == returnValue ) {
      m_numberOfRetrievedRecords = 0;
      m_runNumber = iSyncTo.runNumber();
      //KLUDGE: unfortunately at the moment, the time given for a State
      //   transition recorded by RunStatistics is NOT the same as the
      //   time used in the data stream.  This means I can not use time
      //   to synchronize.  
      //
      //   It is trivial to deliver BeginRun and EndRun because I know
      //    apriori what their 'event numbers' should be.  For StartRuns
      //    I can look in the RunStatistics record itself for 'EventNumber'.

      
      pair<UInt32, const DBRunStatistics::RunStatistics* > 
         nearestStartRun(UInt32(0), static_cast<const DBRunStatistics::RunStatistics*>(0));
      DABoolean foundStartRun = false;
      DABoolean foundBeginRun = false;
      RSList::iterator itEnd = m_rsList.end();
      for( RSList::iterator itRS = m_rsList.begin();
	   itRS != itEnd;
	   ++itRS ) {
	 if( (*itRS)->rsType == InfoX::BeginRun ) {
	    if( !foundBeginRun ) {
	       //NOTE: some Runs have multiple BeginRuns because of problems
	       // with the DAQ
	       rsProxyDeliverer().use( *itRS,
				       SyncValue( m_runNumber, 0, 0 ) );
	       ++m_numberOfRetrievedRecords;
	       foundBeginRun = true;
	    }
	    continue;
	 }
	 if( (*itRS)->rsType == InfoX::StartRun ) {
	    UInt32 eventNumber = findEventNumber( *itRS );
	    if( eventNumber <= iSyncTo.eventNumber() &&
	        eventNumber >= nearestStartRun.first ) {
	       nearestStartRun.first = eventNumber;
	       nearestStartRun.second = *itRS;
	       if( !foundStartRun ) {
	          foundStartRun = true;
	          ++m_numberOfRetrievedRecords;
	       }
	    }
	    continue;
	 }
	 
	 //EndRuns are flagged by the eventNumber being at it's max value
	 if( ( iSyncTo.eventNumber() == SyncValue::kMaxRunEventValue ) &&
	     (*itRS)->rsType == InfoX::EndRun ) {
	    rsProxyDeliverer().use( *itRS,
				    iSyncTo );
	    continue;
	 }
      }
      if(foundStartRun) {
         rsProxyDeliverer().use( nearestStartRun.second,
				SyncValue( m_runNumber, nearestStartRun.first, 0) );
      }

   }
   setSyncValue( iSyncTo );
   return returnValue;
}

DataSourceController::SearchStatus
RSSourceController::findNextRecord(SyncValue& oSyncValueOfNextRecord,
				   Stream::Type& oRecordTypeOfNextRecord ) 
{
   report( ERROR, kFacilityString  ) <<" RunStatistics can not be used as an active source" << endl;
   return kSearchUnknownError;
}

DataSourceController::SearchStatus 
RSSourceController::gotoNextRecord() 
{
   report( ERROR, kFacilityString  ) <<" RunStatistics can not be used as an active source" << endl;
   return kSearchUnknownError;
}



static 
DABoolean 
rsTimeLess( const DBRunStatistics::RunStatistics* iRHS,
	    const DBRunStatistics::RunStatistics* iLHS)
{
   return ( iRHS->sequenceNumber < iLHS->sequenceNumber );
}

DataSourceController::SyncStatus 
RSSourceController::getDataForRun( unsigned int iRunNumber )
{
   report(DEBUG,kFacilityString) 
     <<"getting RunStatistics info for run " << iRunNumber << endl;
   //CCO_info(__FILE__,__LINE__,"getting RunStatistics info for run %d.",
   //    iRunNumber);
   m_rsList.clear();
   for(STL_VECTOR(DBRunStatistics::RunStatisticsList*)::iterator itList =
	  m_rsListList.begin();
       itList != m_rsListList.end();
       ++itList) {
      delete *itList;
   }
   m_rsListList.clear();
   DABoolean status;
   DBRunStatistics::RunStatisticsList* data;

   UsedStatesList::iterator itEnd = m_usedStatesList.end();
   for( UsedStatesList::iterator itStateTrans = m_usedStatesList.begin();
	itStateTrans != itEnd;
	++itStateTrans ) {
     report( DEBUG, kFacilityString )
       << "Retrieve RS for transition " << *itStateTrans << endl;
      PMCTRY
	 {
	    status = m_rsdb->retrieve(iRunNumber, *itStateTrans,
				      data);
	 }
      PMCCATCH(CORBA::SystemException,exc)
	 {
	    report( ERROR, kFacilityString )
	       << "Cannot talk with RunStatistics server. \n"
	       << exc << endl;
	    return kSyncUnknownError;
	 }
      PMCEND_CATCH

      if( status ) {
	report( DEBUG, kFacilityString )
	  << "RS for transition " << *itStateTrans
	  << " has length " << data->length() << endl;
// useful for debugging 
//	report( DEBUG, kFacilityString )
//	  << "Found this run statistics information:" << endl;
	//#if defined(SUNCC)
	//VISostream crap = report( NOTICE, kFacilityString );
	//crap
	//<< data << endl;
	//#endif
//#else
	//	ostream& crap = report( DEBUG, kFacilityString );
	//only do the loop if want debug output
	if( DEBUG <= reportstream().leastSeverity() ) {
	   for ( unsigned long listCount=0; listCount<data->length(); listCount++ ) {
	      DBRunStatistics::RunStatistics& temp = (*data)[listCount];
	      for ( unsigned long dataCount=0; dataCount<temp.rsdata.length(); dataCount++ ){
		 report( DEBUG, kFacilityString )
		    << "type " << temp.rsType << ": "
		    << temp.rsdata[dataCount].name << endl;
	      }
	   }
	}
	//	crap << endl;
//#endif
	m_rsListList.push_back( data );
	for( unsigned long index=0; index<data->length(); ++index ) {
	   m_rsList.push_back( & ((*data)[index]) );

	}
      }
      else {
	report( WARNING, kFacilityString )
	  << "Retrieve RS for transition " << *itStateTrans
	  << " returned false." << endl;
      }
   }

   //now order the list
   //  first define the function used to sort
   if( m_rsList.empty() ) {
      report(WARNING,kFacilityString) 
	 <<"no Run Statistics found for run " << iRunNumber << endl;
      //CCO_info(__FILE__,__LINE__,"no RunStatistics found for run %d.",
      //       iRunNumber);
   }

   sort( m_rsList.begin(), m_rsList.end(), rsTimeLess );
   report(DEBUG,kFacilityString) <<" finished" << endl;
   return kSyncNoError;
}


void 
RSSourceController::implementSetForcedSequentialAccess( DABoolean )
{
   //ignore this for now
}

RSProxyDeliverer& 
RSSourceController::rsProxyDeliverer() const
{
   return *static_cast<RSProxyDeliverer*>( proxyDeliverer() );
}

//
// const member functions
//
DABoolean 
RSSourceController::canRandomAccess() const 
{
   return true;
}

string 
RSSourceController::dataSourceID() const 
{
   return string(kDataSourceID);
}

string 
RSSourceController::parameters() const
{
   return string("");
}


ProxyDeliverer* 
RSSourceController::createProxyDeliverer() const
{
   return new RSProxyDeliverer;
}

//
// static member functions
//

