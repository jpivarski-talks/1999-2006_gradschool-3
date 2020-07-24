// -*- C++ -*-
//
// Package:     DataDelivery
// Module:      ProxyProvider
// 
// Description: Manages the creation and provision of non-tranient Proxies
//
// Implimentation:
//     All the work is done in the concrete classes that inherit from
//     ProxyProvider
//
// Author:      Simon Patton
// Created:     Thu May 28 15:24:50 EST 1997
// $Id: ProxyProvider.cc,v 1.17 2000/03/17 15:31:08 cdj Exp $
//
// Revision history
//
// $Log: ProxyProvider.cc,v $
// Revision 1.17  2000/03/17 15:31:08  cdj
// expireProxies now calls eraseAll
//
// Revision 1.16  1998/11/01 20:11:01  mkl
// move keyedProxies kludge from ProxyProvider to AsciiSourceController (which is really the culprit)
//
// Revision 1.15  1998/01/23 05:18:53  mkl
// needed to add back in the kludge to register proxies at registerStream time
//
// Revision 1.14  1998/01/22 16:43:21  mkl
// removed call to KeyedProxies in supplies and canSupply
//
// Revision 1.13  1998/01/22 05:00:45  mkl
// explicit registration of Proxies in ssupplies and canSupply methods
//
// Revision 1.12  1997/12/09 01:17:52  mkl
// kludge to get reading of first event working again
//
// Revision 1.11  1997/12/08 19:22:19  mkl
// got rid of unnecessary statement (probably due to much editing on Simon's part
//
// Revision 1.10  1997/11/19 15:04:11  sjp
// Added facility to expire Proxies allowing reloading
//
// Revision 1.9  1997/09/18 21:10:15  cdj
// changed Boolean to DABoolean
//
// Revision 1.8  1997/09/03 20:06:11  sjp
// Uses CPP macros for STL containers
//
// Revision 1.7  1997/09/01 15:35:17  sjp
// Changed report include to be in Experiment
//
// Revision 1.6  1997/08/27 03:39:05  sjp
// Updated to new name for bug flags
//
// Revision 1.5  1997/08/14 11:42:41  sjp
// Mods to run under CW Pro 1
//
// Revision 1.4  1997/08/06 14:50:25  sjp
// Changed to handle Proxies
//
// Revision 1.3  1997/07/24 16:27:26  sjp
// Changed call to Record::add to be Record::insert
//
// Revision 1.2  1997/07/09 18:00:36  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.1  1997/06/03 17:32:06  sjp
// new class to add DataFields to Records
//

#include "Experiment/Experiment.h"
// system include files
#include <stdlib.h>  // For 'exit'
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <set>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "DataHandler/Record.h"
#include "DataHandler/ProxyProvider.h"


// STL classes
#include <vector>
#include <map>

//
// constants, enums and typedefs
//

const string kFacilityString = "DataDelivery.ProxyProvider" ;

typedef _proxyprovider_proxies_ StreamProxyMap ;
typedef _proxyprovider_expired_ Expired ;

//
// static data member definitions
//

//
// constructors and destructor
//
ProxyProvider::ProxyProvider( const Identifier& aId ) :
   m_id( aId ) ,
   m_streamProxies( *(new StreamProxyMap ) ) ,
   m_expired( *(new Expired ) )
{
   if ( ( 0 == &m_streamProxies ) ||
	( 0 == &m_expired ) ) {
      delete &m_streamProxies ;
      delete &m_expired ;
      report( EMERGENCY ,
	      kFacilityString )
		 << "Unable to allocate memory"
		 << endl ;
      exit( 1 ) ;
   }
}

// ProxyProvider::ProxyProvider( const ProxyProvider& )
// {
// }

ProxyProvider::~ProxyProvider()
{
   StreamProxyMap::iterator finished( m_streamProxies.end() ) ;
   for ( StreamProxyMap::iterator proxyList( m_streamProxies.begin() ) ;
	 proxyList != finished ;
	 ++proxyList ) {
      KeyedProxies::iterator finished( ((*proxyList).second).end() ) ;
      for ( KeyedProxies::iterator keyedProxy( ((*proxyList).second).begin() ) ;
	    keyedProxy != finished ;
	    ++keyedProxy ) {
	 delete (*keyedProxy).second ;
      }
   }
//
   delete &m_expired ;
   delete &m_streamProxies ;
}

//
// assignment operators
//
// const ProxyProvider& ProxyProvider::operator=( const ProxyProvider& )
// {
// }

//
// member functions
//

void
ProxyProvider::invalidateProxies( const Stream::Type& aStream )
{
   KeyedProxies& proxyList( (*(m_streamProxies.find( aStream ))).second ) ;
   KeyedProxies::iterator finished( proxyList.end() ) ;
   for ( KeyedProxies::iterator keyedProxy( proxyList.begin() ) ;
	 keyedProxy != finished ;
	 ++keyedProxy ) {
      (*((*keyedProxy).second)).invalidate() ;
   }
}

void
ProxyProvider::registerStream( const Stream::Type& aStream )
{
#if 0
   if ( canSupply( aStream ) ) {
      report( ERROR ,
	      kFacilityString )
		 << "\n\tTwo calls to `registerStream' with the same stream!"
		 << endl ;
      exit( 1 ) ;
   }
#endif

//
// The following function does not really use the `hint'.  This
//   signature was chosen so that the function can use the iterator
//   returned without worrying about success/fail as the above test
//   guarentees success.
//
   StreamProxyMap::iterator proxyList(
      m_streamProxies.insert( m_streamProxies.begin() ,
			      StreamProxyMap::value_type( aStream ,
							  KeyedProxies() ) ) ) ;
   ((*proxyList).second).reserve( kDefaultNumberOfProxies ) ;
   m_expired.insert( Expired::value_type( aStream ,
					  !false ) ) ;

}

void
ProxyProvider::expireProxies( const Stream::Type& aStream )
{
   ((*(m_expired.find( aStream ))).second) = !false ;
   eraseAll( aStream );
}

void
ProxyProvider::eraseAll( const Stream::Type& aStream )
{
   KeyedProxies& proxyList( (*(m_streamProxies.find( aStream ))).second ) ;

   KeyedProxies::iterator finished( proxyList.end() ) ;
   if( m_streamProxies.end() != m_streamProxies.find( aStream ) ) {
      KeyedProxies& proxyList( (*(m_streamProxies.find( aStream ))).second ) ;
      for ( KeyedProxies::iterator keyedProxy( proxyList.begin() ) ;
	    keyedProxy != finished ;
	    ++keyedProxy ) {
	 delete (*keyedProxy).second ;
      }
      proxyList.erase( proxyList.begin() ,
		       proxyList.end() ) ;
   }
}

//
// const member functions
//

const ProxyProvider::Identifier&
ProxyProvider::identifier() const
{
   return ( m_id ) ;
}

DABoolean
ProxyProvider::canSupply( const Stream::Type& aStream ) const
{
   return ( m_streamProxies.find( aStream ) != m_streamProxies.end() ) ;
}

const ProxyProvider::KeyedProxies&
ProxyProvider::keyedProxies( const Stream::Type& aStream ) const
{
   KeyedProxies& proxyList( (*(m_streamProxies.find( aStream ))).second ) ;
   if ( ((*(m_expired.find( aStream ))).second) ) {
//
// have to cast away const to allow "delayed" registration of Proxies.
//
      (*((ProxyProvider*)this)).registerProxies( aStream ,
						 proxyList ) ;
      ((*((*((ProxyProvider*)this)).m_expired.find( aStream ))).second) = false ;
// add code to set registation to be valid
   }
   return( proxyList ) ;
}

Stream::Set
ProxyProvider::supplies() const
{
   Stream::Set returnValue;
   StreamProxyMap::const_iterator itEnd = m_streamProxies.end();
   
   for( StreamProxyMap::const_iterator itStream= m_streamProxies.begin();
	itStream != itEnd;
	++itStream){
      returnValue.add( (*itStream).first );
   }
   return returnValue;
}

//
// static member functions
//
