#if !defined(CP2DELIVERY_CP2PROXYDELIVERER_CC)
#define CP2DELIVERY_CP2PROXYDELIVERER_CC
// -*- C++ -*-
//
// Package:     ConstantsPhase2Delivery
// Module:      CP2ProxyDeliverer
// 
// Description: A ProxyDeliverer for Constants access

// Usage:
//        Create and control proxies for DataBase constants.
//
// Invariants:
//
// Author:      Valentine Kouznetsov
// Created:     Wed Jan 15 10:57:59 EST 2003
//
// $Id: CP2ProxyDeliverer.cc,v 1.4 2003/08/29 16:49:50 vk Exp $
//
// Revision history
//
// $Log: CP2ProxyDeliverer.cc,v $
// Revision 1.4  2003/08/29 16:49:50  vk
// Come back to original parser. Splitting constants.map now has been done in Makefile. Remove obsolete split_map_file.py. Declare registerProxyFactories1/registerProxyFactories2 in CP2ProxyDeliverer whose implementation auto generated from Template CP2ProxyDeliverer_registerFactories.cc
//
// Revision 1.3  2003/01/31 16:17:27  vk
//
// Fix Makefile to deal with private version of constants.map
// add cust_constant.map which doesn't have AZD constants
// Fix Deliverer to handle a case of gap in validityRange, invoke additional
// reset to undefined syncvalue and return true value. Suez later on will skip
// this frame.
//
// Revision 1.2  2003/01/29 20:56:57  vk
// Fix return status of CP2ProxyDeliverer::synchronize as well as
// reset call.
//
// Revision 1.1.1.1  2003/01/28 21:49:54  vk
// imported ConstantsPhase2Delivery sources
//
// Revision 1.1.1.1  2003/01/28 20:40:24  vk
// imported ConstantsPhase2Delivery sources
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#include <set>
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include <assert.h>

// user include files
#include "Experiment/report.h"
#include "DataDelivery/ProxyDeliverer.h"
#include "DataHandler/Stream.h"
#include "DataHandler/StreamSet.h"
#include "ConstantsDelivery/ConstantsSourceController.h"
#include "ConstantsDelivery/CDNoServerFoundException.h"

#include "ConstantsPhase2Delivery/CP2ProxyDeliverer.h"
#include "ConstantsPhase2Delivery/DBCP2ProxyFactoryBase.h"
#include "ConstantsPhase2Delivery/DBCP2RecordInfo.h"
#include "ConstantsPhase2Delivery/DBCP2Info.h"

//STL includes
#include <string>
#include <set>
#include <vector>
#include <map>

static const char* const kFacilityString = "ConstantsPhase2Delivery.CP2ProxyDeliverer";

typedef STL_MAP( Stream::Type, DBCP2RecordInfo* ) StreamToRecordInfoMap;
typedef STL_MAP( Stream::Type, STL_VECTOR( DBCP2ProxyFactoryBase* ) ) StreamToFactoryMap;

CP2ProxyDeliverer::CP2ProxyDeliverer(
   const string& iBootFilePath,
   const DBConstantsSelectionTag& iTags) 
   : ProxyDeliverer("ConstantsDatabasePhase2"),
     m_available( * ( new STL_VECTOR( DABoolean ) ) ),
     m_streamToRecordInfoMap( *( new StreamToRecordInfoMap ) ),
     m_streamToFactoryMap( *( new StreamToFactoryMap ) ),
     m_bootFilePath( iBootFilePath ),
     m_selectionTags( iTags )
{
   if( (0 == &m_streamToRecordInfoMap) ||
       (0 == &m_streamToFactoryMap) ||
       (0 == &m_available) ) {
      report( ERROR, kFacilityString ) << "Unable to allocate memory" << endl;
      assert(false);
      ::exit( -1 );
   }
   registerProxyFactories();
}


CP2ProxyDeliverer::~CP2ProxyDeliverer() 
{
   //delete all the factories
   StreamToFactoryMap::iterator itLastFactory = m_streamToFactoryMap.end();
   for( StreamToFactoryMap::iterator itFactory = m_streamToFactoryMap.begin();
	itFactory != itLastFactory ;
	++itFactory ) {
     if ( 0 < (*itFactory).second.size() ) {
       for ( int i = 0; i < (*itFactory).second.size(); i++ ) {
	 delete (*itFactory).second[i];
       }
     }
   }
   
   StreamToRecordInfoMap::iterator itLastInfo = m_streamToRecordInfoMap.end();
   for( StreamToRecordInfoMap::iterator itInfo = 
	   m_streamToRecordInfoMap.begin();
	itInfo != itLastInfo ;
	++itInfo ) {
      delete (*itInfo).second;
   }

   delete &m_available;
   delete &m_streamToRecordInfoMap;
   delete &m_streamToFactoryMap;
}

// enable, disable and resetCache do nothing because this class does
// not handle or process any data
void 
CP2ProxyDeliverer::disable( const Stream::Type& aStream ) 
{
   //Shouldn't read from this Stream
   m_available[ aStream.index() ] = false ;
}

void 
CP2ProxyDeliverer::enable( const Stream::Type& aStream ) 
{
   //Can now read from this Stream
   m_available[ aStream.index() ] = !false ;
}

void 
CP2ProxyDeliverer::resetCache( const Stream::Type& aStream ) 
{
   m_available[ aStream.index() ] = !false;
}

void 
CP2ProxyDeliverer::registerProxies( const Stream::Type& aStream ,
					  KeyedProxies& aProxyList ) 
{
   report(DEBUG, kFacilityString) <<"registering Proxies for constants in "<<aStream.value() <<endl;
   //Need to add an entry in m_available
   UInt32 max_size( aStream.index() );
   while( m_available.size() <= max_size ){
      m_available.push_back( false );
   }
   m_available[ max_size ] = false;

   DBCP2RecordInfo* pRecordInfo = new DBCP2RecordInfo(aStream);
   assert( 0 != pRecordInfo );
   //put this in our record info list
   m_streamToRecordInfoMap.insert( 
      StreamToRecordInfoMap::value_type( aStream,
					 pRecordInfo ) );

   //have the factories create the different proxies
   StreamToFactoryMap::iterator itFactoryList = m_streamToFactoryMap.find( aStream );

   assert( itFactoryList != m_streamToFactoryMap.end() );

   STL_VECTOR( DBCP2ProxyFactoryBase* )::iterator itFactoryEnd = (*itFactoryList).second.end();
   for( STL_VECTOR( DBCP2ProxyFactoryBase* )::iterator itFactory = (*itFactoryList).second.begin();
	itFactory != itFactoryEnd;
	++itFactory ) {
      DBCP2Info* pInfo;
      KeyedProxy keyedProxy;
      (*(*itFactory)).createProxy( m_bootFilePath, m_selectionTags, 
				   keyedProxy, pInfo ) ;

      //put this in the proxy list
      aProxyList.push_back( keyedProxy );

      pRecordInfo->addConstantsInfo( pInfo );

   }
   report(DEBUG, kFacilityString) <<"  done" << endl;

}

DABoolean
CP2ProxyDeliverer::synchronize( const Stream::Type& iStream,
				     const SyncValue& iSyncTo, 
				     SyncValue& oClosestSyncValue )
{
   DABoolean status = false;

   StreamToRecordInfoMap::const_iterator itRecordInfo = 
      m_streamToRecordInfoMap.find( iStream );

   assert( itRecordInfo != m_streamToRecordInfoMap.end() );

   try {
      status  = 
	 (*(*itRecordInfo).second).closestSyncValueTo( iSyncTo, oClosestSyncValue );
      
      
      if(status) {
      
        if( syncValue(iStream) != oClosestSyncValue ) {
        // only when our syncValue has changed we need to reset cache which
        // will cause later on faultHandler call to retrieve a new set of data
        // tell parent class that we have a new Record in this stream
          reset( iStream, oClosestSyncValue );
        }
      
      } else {
      
        // in the case of gap in validityRange reset to undefined syncvalue
        reset( iStream, SyncValue::kUndefined );
      
      }
      
   } catch (CDNoServerFoundException& iException ) {
      report( EMERGENCY, kFacilityString ) 
	 << "Constants source failed to synchronize because \n"
	 << iException.what() << endl;
      return false;
   }
   return true;
}

void
CP2ProxyDeliverer::makeProxies( const Stream::Set& iSet ) {
   Stream::Set::const_iterator itEnd = iSet.end();
   for( Stream::Set::const_iterator itStream = iSet.begin();
	itStream != itEnd;
	++itStream ) {
      makeProxies( *itStream );
   }
}

void
CP2ProxyDeliverer::makeProxies( const Stream::Type& iStream ) {
   //asking for keyedProxies will eventually call registerProxies
   keyedProxies( iStream );
}

void
CP2ProxyDeliverer::registerProxyFactory( const Stream::Type& iStream,
				      DBCP2ProxyFactoryBase* iFactory )
{
   if( 0 == iFactory ) {
      report( ERROR, kFacilityString) << "no factory created" << endl;
      assert( false );
      ::exit( -1 );
   }

   //see if this stream has been add to our list
   StreamToFactoryMap::iterator itFactoryList = m_streamToFactoryMap.find( iStream );
   
   if( itFactoryList == m_streamToFactoryMap.end() ){

      // list was not found
      STL_VECTOR( DBCP2ProxyFactoryBase* ) tempList;
      tempList.reserve( 1 ); //often there will only be one Proxy per stream
      tempList.push_back( iFactory );
      m_streamToFactoryMap.insert( StreamToFactoryMap::value_type( iStream,
								   tempList ) );
      
      registerStream( iStream );
   } else {
      //list already exists
      (*itFactoryList).second.push_back( iFactory );
   }
}

//
// const member functions
//

DABoolean 
CP2ProxyDeliverer::syncValueOfNextRecord( const Stream::Type& iStream,
					       SyncValue& oSyncValue ) const
{

   StreamToRecordInfoMap::const_iterator itRecordInfo = 
      m_streamToRecordInfoMap.find( iStream );

   assert( itRecordInfo != m_streamToRecordInfoMap.end() );
   return (*(*itRecordInfo).second).syncValueOfNextRecord( oSyncValue );
}

DABoolean
CP2ProxyDeliverer::available( const Stream::Type& aStream ) const
{
   return( m_available[ aStream.index() ] ) ;
}

DABoolean
CP2ProxyDeliverer::sourceAccessible() const
{
   DABoolean returnValue = true;
   StreamToRecordInfoMap::const_iterator itEnd= m_streamToRecordInfoMap.end();

   for( StreamToRecordInfoMap::const_iterator itRecordInfoPair = 
	   m_streamToRecordInfoMap.begin();
	itRecordInfoPair != itEnd;
	++itRecordInfoPair ) {
      if( ! (*itRecordInfoPair).second->foundServers() ) {
	 returnValue = false;
	 //keep looping to check all servers
      }
   }
   return returnValue;
}

void
CP2ProxyDeliverer::registerProxyFactories()
{
   registerProxyFactories1();
   registerProxyFactories2();
}
#endif /* CP2DELIVERY_CP2PROXYDELIVERER_CC */

