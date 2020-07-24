#if !defined(CONSTANTSDELIVERY_CONSTANTSPROXYDELIVERER_CC)
#define CONSTANTSDELIVERY_CONSTANTSPROXYDELIVERER_CC
// -*- C++ -*-
//
// Package:     ConstantsDelivery
// Module:      ConstantsProxyDeliverer
// 
// Description: A ProxyDeliverer for Constants access

// Usage:
//        Create and control proxies for DataBase constants.
//
// Invariants:
//
// Author:      Rich Baker
// Created:     Tues Dec 16 1997
//
// $Id: ConstantsProxyDeliverer.cc,v 1.13 2003/09/05 15:13:42 vk Exp $
//
// Revision history
//
// $Log: ConstantsProxyDeliverer.cc,v $
// Revision 1.13  2003/09/05 15:13:42  vk
// Split contants.map in two halfs and compile them separetely.
//
// Revision 1.12  2000/06/17 19:21:08  cdj
// properly handle case where can not find server or no constant for this run
//
// Revision 1.11  1999/12/06 20:35:23  cdj
// updated to use new DBConstants code & improved error reports
//
// Revision 1.10  1999/11/03 03:33:19  cdj
// now handles multiple constants per stream
//
// Revision 1.9  1999/07/14 21:22:26  cdj
// now uses bootfile path to find servers
//
// Revision 1.8  1999/07/10 21:03:19  cdj
// now inform user if server is unavailable
//
// Revision 1.7  1999/04/29 18:45:33  cdj
// updated to use new constants implementation
//
// Revision 1.6  1998/11/16 22:03:31  mkl
// renamed ConstantsBinder/Controller ConstantsSourceBinder/Controller
//
// Revision 1.5  1998/11/10 22:12:09  baker
// Several Fixes.
// Constants Delivery Tested and Working!
// Some kludges still need to be worked on though.
//
// Revision 1.4  1998/11/04 18:55:38  baker
// Fix stupid errors so that this library actually compiles (finally!)
//
// Revision 1.3  1998/11/04 15:39:11  baker
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
// Revision 1.3  1998/08/19 14:24:34  cdj
// forgot to call reset(...) in synchronize()
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
#include "ConstantsDelivery/ConstantsProxyDeliverer.h"
#include "ConstantsDelivery/CDNoServerFoundException.h"

// 11/4/98 Rich Baker
// Added the following 2 include statements.
// It's surprizing how much better the compiler
// likes it when you actually tell it about the
// classes you use.
#include "ConstantsDelivery/DBConstantsProxyFactoryBase.h"
#include "ConstantsDelivery/DBConstantsRecordInfo.h"
#include "ConstantsDelivery/DBConstantsInfo.h"

//STL includes
#include <string>
#include <set>
#include <vector>
#include <map>

static const char* const kFacilityString = "ConstantsDelivery.ConstantsProxydeliverer";

typedef STL_MAP( Stream::Type, DBConstantsRecordInfo* ) StreamToRecordInfoMap;
typedef STL_MAP( Stream::Type, STL_VECTOR( DBConstantsProxyFactoryBase* ) ) StreamToFactoryMap;

ConstantsProxyDeliverer::ConstantsProxyDeliverer(
   const string& iBootFilePath,
   const DBConstantsSelectionTag& iTags) 
   : ProxyDeliverer("ConstantsDatabase"),
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


ConstantsProxyDeliverer::~ConstantsProxyDeliverer() 
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
ConstantsProxyDeliverer::disable( const Stream::Type& aStream ) 
{
   //Shouldn't read from this Stream
   m_available[ aStream.index() ] = false ;
}

void 
ConstantsProxyDeliverer::enable( const Stream::Type& aStream ) 
{
   //Can now read from this Stream
   m_available[ aStream.index() ] = !false ;
}

void 
ConstantsProxyDeliverer::resetCache( const Stream::Type& aStream ) 
{
   m_available[ aStream.index() ] = !false;
}

void 
ConstantsProxyDeliverer::registerProxies( const Stream::Type& aStream ,
					  KeyedProxies& aProxyList ) 
{
   report(DEBUG, kFacilityString) <<"registering Proxies for constants in "<<aStream.value() <<endl;
   //Need to add an entry in m_available
   UInt32 max_size( aStream.index() );
   while( m_available.size() <= max_size ){
      m_available.push_back( false );
   }
   m_available[ max_size ] = false;

   DBConstantsRecordInfo* pRecordInfo = new DBConstantsRecordInfo(aStream);
   assert( 0 != pRecordInfo );
   //put this in our record info list
   m_streamToRecordInfoMap.insert( 
      StreamToRecordInfoMap::value_type( aStream,
					 pRecordInfo ) );

   //have the factories create the different proxies
   StreamToFactoryMap::iterator itFactoryList = m_streamToFactoryMap.find( aStream );

   assert( itFactoryList != m_streamToFactoryMap.end() );

   STL_VECTOR( DBConstantsProxyFactoryBase* )::iterator itFactoryEnd = (*itFactoryList).second.end();
   for( STL_VECTOR( DBConstantsProxyFactoryBase* )::iterator itFactory = (*itFactoryList).second.begin();
	itFactory != itFactoryEnd;
	++itFactory ) {
      DBConstantsInfo* pInfo;
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
ConstantsProxyDeliverer::synchronize( const Stream::Type& iStream,
				     const SyncValue& iSyncTo, 
				     SyncValue& oClosestSyncValue )
{

   StreamToRecordInfoMap::const_iterator itRecordInfo = 
      m_streamToRecordInfoMap.find( iStream );

   assert( itRecordInfo != m_streamToRecordInfoMap.end() );

   try {
      DABoolean status  = 
	 (*(*itRecordInfo).second).closestSyncValueTo( iSyncTo, oClosestSyncValue );
      
      //tell parent class that we have a new Record in this stream
      reset( iStream, oClosestSyncValue );

   } catch (CDNoServerFoundException& iException ) {
      report( EMERGENCY, kFacilityString ) 
	 << "Constants source failed to synchronize because \n"
	 << iException.what() << endl;
      return false;
   }
   return true;
}

void
ConstantsProxyDeliverer::makeProxies( const Stream::Set& iSet ) {
   Stream::Set::const_iterator itEnd = iSet.end();
   for( Stream::Set::const_iterator itStream = iSet.begin();
	itStream != itEnd;
	++itStream ) {
      makeProxies( *itStream );
   }
}

void
ConstantsProxyDeliverer::makeProxies( const Stream::Type& iStream ) {
   //asking for keyedProxies will eventually call registerProxies
   keyedProxies( iStream );
}

void
ConstantsProxyDeliverer::registerProxyFactory( const Stream::Type& iStream,
				      DBConstantsProxyFactoryBase* iFactory )
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
      STL_VECTOR( DBConstantsProxyFactoryBase* ) tempList;
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
ConstantsProxyDeliverer::syncValueOfNextRecord( const Stream::Type& iStream,
					       SyncValue& oSyncValue ) const
{

   StreamToRecordInfoMap::const_iterator itRecordInfo = 
      m_streamToRecordInfoMap.find( iStream );

   assert( itRecordInfo != m_streamToRecordInfoMap.end() );
   return (*(*itRecordInfo).second).syncValueOfNextRecord( oSyncValue );
}

DABoolean
ConstantsProxyDeliverer::available( const Stream::Type& aStream ) const
{
   return( m_available[ aStream.index() ] ) ;
}

DABoolean
ConstantsProxyDeliverer::sourceAccessible() const
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
ConstantsProxyDeliverer::registerProxyFactories()
{
   registerProxyFactories1();
   registerProxyFactories2();
}
#endif /* CONSTANTSDELIVERY_CONSTANTSPROXYDELIVERER_CC */

