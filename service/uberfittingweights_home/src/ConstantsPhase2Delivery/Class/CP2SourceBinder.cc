#if !defined(CP2DELIVERY_CP2SOURCEBINDER_CC)
#define CP2DELIVERY_CP2SOURCEBINDER_CC
// -*- C++ -*-
//
// Library:     ConstantsPhase2Delivery
// Module:      CP2SourceBinder
// 
// Description: 

// Usage:

// Invariants:
//
// Author:      Valentine Kouznetsov
// Created:     Fri Jan 17 2003
//
// $Id: CP2SourceBinder.cc,v 1.4 2003/06/20 13:46:34 vk Exp $
//
// Revision history
//
// $Log: CP2SourceBinder.cc,v $
// Revision 1.4  2003/06/20 13:46:34  vk
// CDBinderException call accepts char* not a string
//
// Revision 1.3  2003/06/19 20:01:58  vk
// Throw CDBinderException exception when CORBA call error occurs
//
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
// Mon Jan 20 21:36:42 EST 2003
// first attempt for SourceBinder, work on SunOS, vk

// system include files
#include "Experiment/Experiment.h"
#include "Experiment/report.h"
#include <vector>
#include <map>

#include "ConstantsDelivery/CDBinderException.h"
#include "ConstantsDelivery/ConstantsStreamMap.h"
#include "DBMetaVersion.hh"
#include "DBMetaVersionFactory.hh"
#include "DBConstants.hxx"

// Include OS/C Standards
#if defined(__unix)
#include <unistd.h>
#endif

// user include files
#include "ConstantsPhase2Delivery/CP2SourceBinder.h"
#include "DataHandler/Stream.h"
#include "DataHandler/StreamSet.h"
#include "ConstantsDelivery/CDNoServerFoundException.h"
#include "ConstantsDelivery/CDBinderException.h"
#include "ConstantsPhase2Delivery/CP2SourceController.h"

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "ConstantsPhase2Delivery.CP2SourceBinder" ;


CP2SourceBinder::CP2SourceBinder( 
   const string& fdbName,
   const DBConstantsSelectionTag& tag,
   const Stream::Set& iBindStreams ) :

   DataSourceBinder(  fdbName, tag.toString(), iBindStreams ), mSelectTag( tag )
{
   report(DEBUG,kFacilityString) << "fdbName " << fdbName << " Tag " << tag.toString()<< endl;

   // Check if we bound to any streams
   if( 0 == iBindStreams.size() ) {
     //use defaults
     report(DEBUG,kFacilityString) << "is not bound to any streams, use defaults" << endl;

     // Try to access DB and retrieve typeList of available constants
     bindStreams( retrieveStreams(retrieveTypeList(tag.toString().c_str())) );
   }

}

CP2SourceBinder::~CP2SourceBinder() {}

DataSourceController* 
CP2SourceBinder::createDataSourceController( void ) const {

   return new CP2SourceController( boundStreams(), dataSourceID(), selTag() );

}


const DBConstantsSelectionTag& CP2SourceBinder::selTag( void ) const {
   return mSelectTag;
}

DBVersionManager::StringList*
CP2SourceBinder::retrieveTypeList(const char* tag) {

   // Bind to VersionManager server, without specifying object name
   DBVersionManager::Manager* thisVMServer;
//   const char* bindName = "Cleo3RunStatisticsDB";
   PMCTRY {
     CLEORB_BIND2_ANON(thisVMServer,DBVersionManager,Manager);
   }
   PMCCATCH(CORBA::SystemException, excep) {
     thisVMServer = NULL;
   }
   PMCEND_CATCH

   if ( thisVMServer == NULL ) {
      string error_condition = 
        "CP2SourceBinder fails to access the VersionManager server.";
      throw CDBinderException(error_condition.c_str());
   }

   // Data structures
   CORBA::Boolean retval = FALSE;
   DBVersionManager::StringList* typeList = NULL;
   char* resultString = NULL;
   // Load list from server
   PMCTRY {
     // I need to specify as char* tag and hostname
     // const char* tag = "CLEO3default";
     // Get hostname
     char* hostname = new char[50];
     if ( gethostname(hostname,50) != 0 ) {
        report(EMERGENCY, kFacilityString) << "Could not get hostname. Will stop here." << endl;
        assert(false);
        ::exit(1);
     }
     retval = thisVMServer->getTypesInValidityGraph( tag,
                                                     hostname,
                                                     typeList,
                                                     resultString );
   }
   PMCCATCH(CORBA::SystemException, excep) {
     cerr << "Could not get type list from VM server." << excep<< endl;
     //signal error condition
   }
   PMCEND_CATCH

   if ( ! retval ) {
      //cerr << "Could not get type list from VM server." << endl;
      //cerr << "Reason: " << resultString << endl;
      string errorResult = resultString;
      string error_condition = "Could not get type list from VM server.\n";
      error_condition += string("Reason")+errorResult+string("\n");
      //signal error condition
      throw CDBinderException(error_condition.c_str());
   }
   if ( typeList->length() == 0 ) {
      //cerr << "Found no types for this tag." << endl;
      //signal error condition
      throw CDBinderException("Found no types for this tag.");
   }
   // Loop over list and do the magic
   for ( int loopCount=0; loopCount<typeList->length(); loopCount++ ) {
     report( DEBUG, kFacilityString )
       << "Found type " << (*typeList)[loopCount] << endl;
   }
   return typeList;
}

Stream::Set
CP2SourceBinder::retrieveStreams(DBVersionManager::StringList* typeList) const {

   // bind stream(s)
   Stream::Set defaults;
   for ( int i = 0; i < typeList->length(); i++ ) {
     string thisType = string( (*typeList)[i] );
     Stream::Type stream = 
       ConstantsStreamMap::streamWithConstant( thisType );
       
     if ( stream != Stream::kNone ) {
       defaults.add( stream );
     } else {
       report( DEBUG, kFacilityString ) << "could not find a Stream for Constant type " << thisType <<endl;
     }
   }
   return defaults;
}

#endif /* CP2DELIVERY_CP2SOURCEBINDER_CC */
