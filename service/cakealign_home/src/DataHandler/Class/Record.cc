// -*- C++ -*-
//
// Package:     DataHandler
// Module:      Record
// 
// Description: Base type for all Data Records
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Simon Patton
// Created:     Wed Oct 30 10:36:11 EST 1996
// $Id: Record.cc,v 1.25 2003/05/31 23:46:36 cdj Exp $
//
// Revision history
//
// $Log: Record.cc,v $
// Revision 1.25  2003/05/31 23:46:36  cdj
// Using change in syncValue() to determine if a Record has changed is insufficient.  I added the method Record::cacheIdentifier() which returns a unique value each time a Record is cleared.
//
// Revision 1.24  2002/06/04 18:03:00  cdj
// bug fix: caching of last DataKey requested in Record could cause stale pointer to be examined
//
// Revision 1.23  1999/04/05 17:49:02  cdj
// now holds a non-const pointer to the Frame
//
// Revision 1.22  1998/09/30 21:07:36  cdj
// added iterator
//
// Revision 1.21  1998/09/25 19:31:28  cdj
// no longer use ProxyDict
//
// Revision 1.20  1998/07/07 19:13:10  mkl
// undid Chris' fix to delete transient proxies
//
// Revision 1.19  1998/04/09 18:39:29  cdj
// bug fix: now deletes transient proxies in destructor
//
// Revision 1.18  1998/02/03 21:28:25  mkl
// switch over to STLUtility library
//
// Revision 1.17  1997/11/19 15:02:23  sjp
// Updated doc and interface for RecordProvider use
//
// Revision 1.16  1997/09/18 21:10:17  cdj
// changed Boolean to DABoolean
//
// Revision 1.15  1997/09/03 20:06:13  sjp
// Uses CPP macros for STL containers
//
// Revision 1.14  1997/09/01 15:35:17  sjp
// Changed report include to be in Experiment
//
// Revision 1.13  1997/08/27 03:39:06  sjp
// Updated to new name for bug flags
//
// Revision 1.12  1997/08/14 11:42:42  sjp
// Mods to run under CW Pro 1
//
// Revision 1.11  1997/08/06 14:51:19  sjp
// Changed to handle Proxies, and be a subclass of ProxyDict
//
// Revision 1.10  1997/07/24 16:16:30  sjp
// Modified ready for conversion to Ifd:
// 	datafield -> get
// 	add -> insert
// 	replace -> add
// 	xxxContainer -> xxxFrame
//   removed "canContains", whose function is now taken by "find".
//
// Revision 1.9  1997/07/09 17:59:08  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.8  1997/06/27 18:35:57  sjp
// Modified so DataField knows in which Frame it is living
//
// Revision 1.7  1997/06/03 17:25:55  sjp
// Fixed potential bug in Record by adding canContain
//
// Revision 1.6  1997/05/07 19:58:10  sjp
// Added kNoLabel constant, and now use DataFieldLabel class
//
// Revision 1.5  1997/04/11 18:57:38  sjp
// Cleaned up message issuing
//
// Revision 1.4  1997/04/04 16:39:20  sjp
// Added SyncValue to Record
//
// Revision 1.3  1997/04/01 16:03:38  sjp
// renamed clear function to be removeAll
//
// Revision 1.2  1997/03/29 16:22:52  sjp
// Finished first implementation
//
// Revision 1.1  1997/03/19 22:20:14  sjp
// New definitions for class
//

#include "Experiment/Experiment.h"
// system include files
#include <stdlib.h>  // For 'exit'
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <set>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h" // For error report
#include "DataHandler/Record.h"
#include "DataHandler/ProxyBase.h"
#include "DataHandler/RecordKeyItr.h"

// STL classes
#include <set>
#include <map>

//
// constants, enums and typedefs
//

const string kFacilityString = "DataHandler.Record" ;

typedef STL_MAP( DataKey , const ProxyBase* ) ProxyMap ;
typedef STL_SET( DataKey ) ProxySet ;

//
// static data member definitions
//

//
// constructors and destructor
//
Record::Record( const Stream::Type& aStream ) :
   m_stream( aStream ) ,
   m_visible( *new ProxyMap ) ,
   m_displaced( *new ProxyMap ) ,
   m_extras( *new ProxySet ) ,
   m_frame( 0 ) ,
   m_latestProxy( 0 ),
   m_cacheIdentifier(1)
{
   if ( ( 0 == &m_visible ) ||
	( 0 == &m_displaced ) ||
	( 0 == &m_extras ) ) {
      delete &m_extras ;
      delete &m_displaced ;
      delete &m_visible ;
      report( EMERGENCY ,
	      kFacilityString )
		 << "Unable to allocate memory"
		 << endl ;
      exit( 1 ) ;
   }
}

// Record::Record( const Record& )
// {
// }

Record::~Record()
{
#if defined(CLEO_DEBUG)
   report( WARNING, kFacilityString )
      << "please fix memory leak involving transient proxies" << endl;
   //Since we own the transients, we need to delete them
// causes segfault:
//  eraseTransients();
#endif
   delete &m_extras ;
   delete &m_displaced ;
   delete &m_visible ;
}

//
// assignment operators
//
// const Record& Record::operator=( const Record& )
// {
// }

//
// member functions
//

DABoolean
Record::insert( const DataKey& aKey ,
		const ProxyBase* pProxy )
{
//
// if key alreay exists in Record then forbid entry
//   (This is current BaBar policy - should be reviewed)
//
   if ( 0 != find( aKey ) ) {
      report( WARNING ,
	      kFacilityString )
		 << "\n\tKey is already in Record, Proxy can not be added"
		 << endl ;
      return ( false ) ;
   }
/*   (Not yet implemeted)
//
// if Proxy owned by Record or Provider forbid entry
//
//
   if ( 0 != (*pProxy).owner() ) {
      report( WARNING ,
	      kFacilityString )
		 << "\n\tKey is already in Record, Proxy can not be added"
		 << endl ;
      return ( false ) ;
   }*/
//
// if Key is duplicate of current transient Proxy simply replace
//   that Proxy and delete it.
//
   if ( ( m_displaced.find( aKey ) != m_displaced.end() ) ||
	( m_extras.find( aKey ) != m_extras.end() ) ) {
//
// ( we already know the field exist, so do not need to check against end() )
//
      ProxyMap::iterator transient( m_visible.find( aKey ) ) ;
//
// Record owns the transient, to delete it need to cast of const 
//
      delete (ProxyBase*)(*transient).second ;
/*   (Not yet implemeted) ;
//
// have to cast away const to set owner
//
      (*((ProxyBase*)pProxy)).setOwner( this ) ;*/
      (*transient).second = pProxy ;
   }
//
// Key is either duplicate of non-transient, or a new transient Key
//
   else {
      ProxyMap::iterator original( m_visible.find( aKey ) ) ;
//
// if Key is duplicate of non-transient, move non-transient to
//   group of displaced non-transients
//
      if ( original != m_visible.end() ) {
	 m_displaced.insert( *original ) ;
	 (*original).second = pProxy ;
      }
//
// if Key new transient Key
//
      else {
	 m_extras.insert( aKey ) ;
/*   (Not yet implemeted) ;
//
// have to cast away const to set owner
//
	 (*((ProxyBase*)pProxy)).setOwner( this ) ;*/
	 m_visible.insert( ProxyMap::value_type( aKey , pProxy ) ) ;
      }
   }
//
   m_latestProxy = 0 ;
   return ( !false ) ;
}

void
Record::eraseTransients()
{
//
// delete all transient Proxies not displacing non-transients
//
   ProxySet::iterator finishedExtras( m_extras.end() ) ;
   for ( ProxySet::iterator extra( m_extras.begin() ) ;
	 extra != finishedExtras ;
	 ++extra ) {
      ProxyMap::iterator transient( m_visible.find( (*extra) ) ) ;
//
// Record owns the transient, to delete it need to cast of const 
//
      delete (ProxyBase*)(*transient).second ;
      m_visible.erase( transient ) ;
   }
   m_extras.erase( m_extras.begin() , m_extras.end() ) ;
//
// delete all the transient Proxies displacing non-transients
//
   ProxyMap::iterator finishedDisplaced( m_displaced.end() ) ;
   for ( ProxyMap::iterator field( m_displaced.begin() ) ;
	 field != finishedDisplaced ;
	 ++field ) {
      ProxyMap::iterator transient( m_visible.find( (*field).first ) ) ;
//
// Record owns the transient, to delete it need to cast of const 
//
      delete (ProxyBase*)((*transient).second) ;
      (*transient).second = (*field).second ;
   }
   m_displaced.erase( m_displaced.begin() , m_displaced.end() ) ;
//

   //change our cacheIdentifier to 
   ++m_cacheIdentifier;
   m_latestProxy = 0 ;
}

void
Record::removeAll()
{
//
// remove all tranisents
//
   eraseTransients() ;
//
// clear non-transients map
//
   m_visible.erase( m_visible.begin() , m_visible.end() ) ;
}

DABoolean
Record::setFrame( Frame& aFrame )
{
   if ( 0 != m_frame ) {
      if ( &aFrame != m_frame ) {
	 return ( false ) ;
      }
      return ( !false ) ;
   }
   m_frame = &aFrame ;
   return ( !false ) ;
}

void
Record::clearFrame()
{
   m_frame = 0 ;
}

void
Record::setSyncValue(  const SyncValue& aSyncValue )
{
   m_syncValue = aSyncValue ;
}

DABoolean
Record::add( const DataKey& aKey ,
	     const ProxyBase* pProxy )
{
   if ( ( ! m_displaced.empty() ) ||
	( ! m_extras.empty() ) ) {
      report( ERROR ,
	      kFacilityString )
		 << "\n\tTrying to `insert a non-Transient Proxy in to the Record,\n"
		 << "\t\twhile transient Proxies exist!"
		 << endl ;
      return( false ) ;
   }
//
   if ( 0 != find( aKey ) ) {
//
// we already know the field exist, so do not need to check against end()
//
      (*m_visible.find( aKey )).second = pProxy ;
   }
   else {
      m_visible.insert( ProxyMap::value_type( aKey , pProxy ) ) ;
   }
//
   m_latestProxy = 0 ;
   return ( !false ) ;
}

//
// const member functions
//

const Stream::Type&
Record::stream() const
{
   return ( m_stream ) ;
}

Frame&
Record::frame() const
{
   return ( *m_frame ) ;
}

const SyncValue&
Record::syncValue() const
{
   return ( m_syncValue ) ;
}

const ProxyBase*
Record::find( const DataKey& aKey ) const
{
   if ( ! ( ( 0 != m_latestProxy ) &&
	    ( aKey == m_latestKey ) ) ) {
      ProxyMap::const_iterator noEntry( m_visible.end() ) ;
      ProxyMap::const_iterator entry( m_visible.find( aKey ) ) ;
      if ( entry != noEntry ) {
//
// until `mutable' in generally available have to cast off const
//
	 const_cast<Record*>(this)->m_latestProxy = (*entry).second ;
	 const_cast<Record*>(this)->m_latestKey = (*entry).first ;
// NOTE: copy the DataKey from entry rather than aKey since the
//  const char*'s in Usage and Production tags in aKey are not guaranteed
//  to have a lifetime greater than this call
      }
      else {
//
// until `mutable' in generally available have to cast off const
//
	 const_cast<Record*>(this)->m_latestProxy = 0 ;
      }
   }
   return ( m_latestProxy ) ;
}

Record::const_key_iterator 
Record::begin_key() const
{
   return const_key_iterator( m_visible.begin() );
}

Record::const_key_iterator 
Record::end_key() const
{
   return const_key_iterator( m_visible.end() );
}
//
// static member functions
//




