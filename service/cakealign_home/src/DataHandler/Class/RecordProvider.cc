// -*- C++ -*-
//
// Package:     DataDelivery
// Module:      RecordProvider
// 
// Description: Creates and Manages Records
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Simon Patton
// Created:     Thu Oct 30 16:41:02 EST 1997
// $Id: RecordProvider.cc,v 1.7 2003/10/16 21:18:41 vk Exp $
//
// Revision history
//
// $Log: RecordProvider.cc,v $
// Revision 1.7  2003/10/16 21:18:41  vk
// Modify RecordProvider to carry most recent time stamp for the record.
//
// Revision 1.6  2003/10/07 22:21:10  cdj
// in RecordProvider::reconfigure now reset Record's syncvalue to fix caching problem when using FrameIterate on only one run
//
// Revision 1.5  2002/03/08 20:24:57  cdj
// speed improvement: under perverse conditions, was throwing many exceptions per event
//
// Revision 1.4  2000/06/03 21:20:52  cdj
// now issue warning and continue if a Record that is supposed to be in the Frame fails to synchronize
//
// Revision 1.3  2000/03/17 15:32:38  cdj
// Added resetAllCachesOfProxies and unsynchronizedRecord. Needed for FrameIterate
//
// Revision 1.2  2000/01/22 02:34:21  cdj
// added contains method to RecordProvider
//
// Revision 1.1  1997/11/19 15:23:18  sjp
// Class to replace RecordDeliverer in DataDelivery
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>  // For 'exit'
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "DataHandler/SyncValue.h"
#include "DataHandler/Record.h"
#include "DataHandler/ProxyProvider.h"
#include "DataHandler/RecordProvider.h"
#include "DataHandler/DHStreamDidNotSyncException.h"

// STL classes

#include <algorithm>
#include <vector>

//
// constants, enums and typedefs
//

const string kFacilityString = "DataHandler.RecordProvider" ;

//
// static data member definitions
//

//
// local functions
//

//
// This function allows creations of subclasses of Record.
//

//
// constructors and destructor
//

RecordProvider::RecordProvider( const Stream::Type& aStream ) :
   m_stream( aStream ) ,
   m_providers( *( new Providers ) ) ,
   m_record( *( RecordProvider::createRecord( aStream ) ) ) ,
   m_needReconfigure( !false )
{
   if ( ( 0 == &m_record ) ||
	( 0 == &m_providers ) ) {
      delete &m_record ;
      delete &m_providers ;
      report( EMERGENCY ,
	      kFacilityString )
		 << "Unable to allocate memory"
		 << endl ;
      exit( 1 ) ;
   }
}

// RecordProvider::RecordProvider( const RecordProvider& )
// {
// }

RecordProvider::~RecordProvider()
{
   delete &m_record ;
   delete &m_providers ;
}

//
// assignment operators
//
// const RecordProvider& RecordProvider::operator=( const RecordProvider& )
// {
// }

//
// member functions
//

void
RecordProvider::add( ProxyProvider& aProvider )
{
//
// set flag to cause record to be reloaded from all ProxyProviders
//
   m_needReconfigure = !false ;
//
// if ProxyProvider can not supply Records for this stream issue warning
//   and do nothing else.
//
   if ( ! aProvider.canSupply( m_stream ) ) {
      report( WARNING ,
	      kFacilityString )
		 << "\nProxyProvider can not supply "
		 << m_stream
		 << " stream! "
		 << "Will ignore it."
		 << endl ;
      return ;
   }
//
// check the ProxyProvider is not already in the list.  If it is then
//    this is an error, as each ProxyProvider should only appear once.
//
   if ( m_providers.end() != find( m_providers.begin(),
				   m_providers.end(),
				   &aProvider) ) {
      report( ERROR ,
	      kFacilityString )
		 << "\nTrying to `add' ProxyProvider already being used."
		 << "\nAs order may be important you need to `remove'"
		 << " it first."
		 << endl ;
      return ;
   }
//
// add ProxyProvider to list and add new DataFields to Record
//
   m_providers.push_back( &aProvider ) ;
}

void
RecordProvider::remove( ProxyProvider& aProvider )
{
//
// set flag to cause record to be reloaded from all ProxyProviders
//
   m_needReconfigure = !false ;
//
// find the ProxyProvider and erase it from the list if it exists
//
   Providers::iterator oldProvider = find( m_providers.begin(),
					   m_providers.end(),
					   &aProvider) ;
   if( oldProvider != m_providers.end() ) {
      m_providers.erase( oldProvider ) ;
//
// clear basic structure of the Record and rebuilt it
//
   }
   else {
      report( WARNING ,
	      kFacilityString )
		 << "\nTrying to `remove' a ProxyProvider not used by "
		 << "this RecordProvider."
		 << endl ;
   }
}

void
RecordProvider::reconfigure()
{
   typedef ProxyProvider::KeyedProxies ProxyList ;
//
// until 'mutable' is widely implemented need to cast away 'const'
//
   m_record.removeAll() ;
   Providers::iterator finished( m_providers.end() ) ;
   for ( Providers::iterator provider( m_providers.begin() ) ;
	 provider != finished ;
	 ++provider ) {
      const ProxyList& keyedProxies( (*(*provider)).keyedProxies( m_stream ) ) ;
      ProxyList::const_iterator finishedProxyList( keyedProxies.end() ) ;
      for ( ProxyList::const_iterator keyedProxy( keyedProxies.begin() ) ;
	    keyedProxy != finishedProxyList ;
	    ++keyedProxy ) {
//
// until 'mutable' is widely implemented need to cast away 'const'
//
	 m_record.add( (*keyedProxy).first , (*keyedProxy).second ) ;
      }
   }
   //make sure we re align all proxies
   m_record.setSyncValue( SyncValue::kBeginningOfTime ) ;

//
// until 'mutable' is widely implemented need to cast away 'const'
//
   m_needReconfigure = false ;
}

void
RecordProvider::resetAllCachesOfProxies() 
{
   //for now, tell all Providers to delete their Proxies
   
   m_record.removeAll() ;
   Providers::iterator finished( m_providers.end() ) ;
   for ( Providers::iterator provider( m_providers.begin() ) ;
	 provider != finished ;
	 ++provider ) {
      (*provider)->expireProxies( m_stream );
   }

   m_needReconfigure = true ;
   
}

//
// const member functions
//

const Stream::Type&
RecordProvider::stream() const
{
   return ( m_stream ) ;
}

DABoolean
RecordProvider::isEmpty( void ) const
{
   return ( m_providers.empty() ) ;
}

Record*
RecordProvider::record( const SyncValue& aNow ) const
{
   report(DEBUG,kFacilityString) << "aNow "
				 << aNow.runNumber() <<" "
				 << aNow.eventNumber() << " "
				 << aNow.time() << endl;

//
// if zero ProxyProviders, then there has been an error
//
   if ( m_providers.empty() ) {
      report( WARNING ,
	      kFacilityString )
		 << "\nThe RecordProvider for "
		 << m_stream
		 << " has no ProxyProviders."
		 << endl ;
      return 0;
   }
//
// if necessary fill record using ProxyProviders
//
   if ( m_needReconfigure ) {
//
// have to cast away const to allow "delayed" reconfigure.  The
//    alternative would be to call 'reconfigure' each time 'add' or
//    'remove' was called!
//
      (*((RecordProvider*)this)).reconfigure() ;
   }
//
// Find "most recent" value for this stream among providers.
// Also figure out what is the time stamp should be
//
   UInt64 timeStamp=aNow.time();
   SyncValue mostRecent( SyncValue::kBeginningOfTime ) ;
   const Providers::const_iterator finished( m_providers.end() ) ;
   for ( Providers::const_iterator providerToCheck( m_providers.begin() ) ;
	 providerToCheck != finished ;
	 ++providerToCheck ) {
      report(DEBUG,kFacilityString) << "mostRecent "
      				<< m_stream << " "
				<< mostRecent.runNumber() <<" "
				<< mostRecent.eventNumber() << " "
				<< mostRecent.time() << endl;
      if(timeStamp<mostRecent.time()) timeStamp = mostRecent.time();
      mostRecent = (*(*providerToCheck)).latest( m_stream ,
						 mostRecent ,
						 aNow ) ;
      report(DEBUG,kFacilityString) << mostRecent.time() <<endl;

   }
//
// if no Provider is "tied" to a SyncValue, this is an error
//
   if ( SyncValue::kBeginningOfTime ==  mostRecent ) {
      return 0;
   }
//
// Create a new syncValue with corrent time stamp out of mostRecent
//
   report(DEBUG,kFacilityString) << "mostRecentTime " << timeStamp << endl;
   SyncValue mostRecentTime(mostRecent.runNumber(), 
                            mostRecent.eventNumber(), timeStamp);

//
// re-set Record to be "most Recent" if not the same as last setting
//   removing all the Transient Proxies.
//
   if ( mostRecent != m_record.syncValue() ) {
      m_record.eraseTransients() ;
      m_record.setSyncValue( mostRecentTime ) ;
//
// Align ProxyProviders, so that all data delivered for this Stream
//   comes from the "most Recent" SyncValue
//
      for ( Providers::const_iterator providerToAlign( m_providers.begin() ) ;
	    providerToAlign != finished ;
	    ++providerToAlign ) {
	 (*(*providerToAlign)).align( m_stream ,
				      mostRecentTime ) ;
      }
   }
//
   return ( &m_record ) ;
}


const Record&
RecordProvider::unsynchronizedRecord() const
{
//
// if zero ProxyProviders, then there has been an error
//
   if ( m_providers.empty() ) {
      report( WARNING ,
	      kFacilityString )
		 << "\nThe RecordProvider for "
		 << m_stream
		 << " has no ProxyProviders."
		 << endl ;
   }
//
// if necessary fill record using ProxyProviders
//
   if ( m_needReconfigure ) {
//
// have to cast away const to allow "delayed" reconfigure.  The
//    alternative would be to call 'reconfigure' each time 'add' or
//    'remove' was called!
//
      (*const_cast<RecordProvider*>(this)).reconfigure() ;
   }
   return ( m_record ) ;
}

DABoolean
RecordProvider::contains( const ProxyProvider& aProvider ) const
{
   Providers::const_iterator itEnd = m_providers.end();
   for( Providers::const_iterator itProvider = m_providers.begin();
	itProvider != itEnd;
	++itProvider ) {
      if( *itProvider == &aProvider ) {
	 return true;
      }
   }
   return false;
}

//
// static member functions
//
