#if !defined(CONSTANTSDELIVERY_DBCONSTANTSPROXY_CC)
#define CONSTANTSDELIVERY_DBCONSTANTSPROXY_CC
// -*- C++ -*-
//
// Package:     ConstantsDelivery
// Module:      DBConstantsProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Tue Aug 18 22:39:16 EDT 1998
// $Id: DBConstantsProxy.cc,v 1.17 2001/10/31 21:36:03 cdj Exp $
//
// Revision history
//
// $Log: DBConstantsProxy.cc,v $
// Revision 1.17  2001/10/31 21:36:03  cdj
// added report message about what version is begin used
//
// Revision 1.16  2001/09/07 19:01:21  cleo3
// added typename
//
// Revision 1.15  2000/07/27 18:19:55  cdj
// now much faster when reading from multiple runs in one job
//
// Revision 1.14  2000/07/25 18:45:21  cdj
// update to new DBConstants::getClosestSyncValue return type (first step to speed increase)
//
// Revision 1.13  2000/06/17 19:14:02  cdj
// can now tell the difference between a missing server and no version for this run
//
// Revision 1.12  2000/06/14 18:16:30  cdj
// constants from database are now properly sorted
//
// Revision 1.11  2000/04/27 21:14:06  cdj
// now works on OSF using MICO ORB
//
// Revision 1.10  1999/12/06 20:35:28  cdj
// updated to use new DBConstants code & improved error reports
//
// Revision 1.9  1999/07/14 21:22:36  cdj
// now uses bootfile path to find servers
//
// Revision 1.8  1999/07/10 21:02:10  cdj
// -run #0 once again means that something went wrong
// -now inform user if server is unavailable
//
// Revision 1.7  1999/07/09 22:04:46  cdj
// allow run# 0 to be a valid return value
//
// Revision 1.6  1999/04/29 18:45:43  cdj
// updated to use new constants implementation
//
// Revision 1.5  1998/11/10 22:12:29  baker
// Several Fixes.
// Constants Delivery Tested and Working!
// Some kludges still need to be worked on though.
//
// Revision 1.4  1998/11/05 18:52:18  baker
// Another batch of updates to get this library finally compiled
// *AND* linkable
//
// Revision 1.3  1998/09/30 20:40:27  baker
// Changed class names and include file names as
// necessary for migration from old DataBaseDelivery
// to new ConstantsDelivery packages.
//
// Revision 1.2  1998/08/19 14:25:33  cdj
// needed some additional member functions
//
// Revision 1.1  1998/08/19 03:17:53  cdj
// first submission
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ConstantsDelivery/DBConstantsProxy.h"
#include "ConstantsDelivery/CDNoServerFoundException.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kDBConstantsProxyFacilityString = "ConstantsDelivery.DBConstantsProxy" ;

//
// static data member definitions
//

//
// constructors and destructor
//
template< class T >
DBConstantsProxy<T>::DBConstantsProxy( const string& iBootFilePath,
				       const DBConstantsSelectionTag& iTag ) :
   m_constants( iBootFilePath, iTag.selectionTags() , iTag.type(), true )
{
}

// DBConstantsProxy::DBConstantsProxy( const DBConstantsProxy& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }
template< class T >
DBConstantsProxy<T>::~DBConstantsProxy()
{
}

//
// assignment operators
//
// const DBConstantsProxy& DBConstantsProxy::operator=( const DBConstantsProxy& rhs )
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
template< class T>
void
DBConstantsProxy<T>::invalidateCache() {
   //don't do anything
}

template< class T>
const typename DBConstantsProxy<T>::value_type* 
DBConstantsProxy<T>::faultHandler( const Record& aRecord ,
				   const DataKey& aKey )
{

   m_constants.synchronize( aRecord.syncValue().runNumber() );

   if( m_constants.isValid() ) {
      report(NOTICE,kDBConstantsProxyFacilityString) <<nameOfConstant() 
      <<" using version "
      <<m_constants.getVersionInfo().versionMajor()
      <<"."
      <<m_constants.getVersionInfo().versionMinor()
      << endl;
      return &m_constants;
   }

   if( !foundServer() ) {
      throw CDNoServerFoundException(nameOfConstant()) ;
   }

   return (value_type*) 0;
}


//
// const member functions
//
template< class T>
DABoolean 
DBConstantsProxy<T>::syncValueOfNextRecord( SyncValue& oSyncValue ) const
{
   int runNumber = m_constants.getNextSyncValue();
   oSyncValue = SyncValue( runNumber, 0 );

   return m_constants.isValid();
}


template< class T>
DABoolean 
DBConstantsProxy<T>::closestSyncValueTo( const SyncValue& iSyncValue, 
					 SyncValue& oClosestSyncValue ) const
{
   if ( m_validRange.begin() <= iSyncValue && 
	m_validRange.end() >= iSyncValue ) {
      //we have cached the info we would get from the database
      oClosestSyncValue = m_validRange.begin();
   } else {
      VersionInfo versionInfo = m_constants.getClosestSyncValue( iSyncValue.runNumber() );

      if( 0 == versionInfo.startRun() ) {
	 //this operation failed
	 
	 if( ! foundServer() ) {
	    //we can't get ahold of the server
	    throw CDNoServerFoundException(nameOfConstant());
	 }
	 
	 oClosestSyncValue = SyncValue::kUndefined;
	 return false;
      }
      oClosestSyncValue = SyncValue( versionInfo.startRun(), 0 );
      //m_validRange should be mutable
      ValidRange& temp = const_cast<DBConstantsProxy<T>*>(this)->m_validRange;
      temp = ValidRange( oClosestSyncValue,
			 SyncValue( versionInfo.stopRun(), 0) );
   }
   return true;
}

template< class T>
SyncValue
DBConstantsProxy<T>::presentSyncValue() const
{
   if( 0 == m_constants.getVersionInfo().startRun() ) {
      report( DEBUG, kDBConstantsProxyFacilityString ) <<
	 "startRun method returned 0" << endl;
      //something went wrong
      return SyncValue::kUndefined;
   }
   return SyncValue( m_constants.getVersionInfo().startRun(), 0 );
}


template< class T>
DABoolean 
DBConstantsProxy<T>::foundServer() const
{
   return ( 0 != m_constants.bindStatus() );
}

template< class T>
const char*
DBConstantsProxy<T>::nameOfConstant() const {
   return m_constants.bdlName();
}

//
// static member functions
//

#endif /* CONSTANTSDELIVERY_DBCONSTANTSPROXY_CC */
