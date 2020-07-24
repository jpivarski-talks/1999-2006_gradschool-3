#if !defined(CONSTANTSPHASE2DELIVERY_DBCP2PROXY_CC)
#define CONSTANTSPHASE2DELIVERY_DBCP2PROXY_CC
// -*- C++ -*-
//
// Package:     ConstantsPass2Delivery
// Module:      DBCP2Proxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Valentine Kouznetsov
// Created:     Tue Jan 28 12:37:00 EST 2003
// $Id: DBCP2Proxy.cc,v 1.4 2003/02/13 22:01:20 vk Exp $
//
// Revision history
//
// $Log: DBCP2Proxy.cc,v $
// Revision 1.4  2003/02/13 22:01:20  vk
// Adjust to new API, now accept failureMode argument in DBConstants.
//
// Revision 1.3  2003/02/07 22:09:56  vk
// Add NextRecordSync functionality
//
// Revision 1.2  2003/01/29 20:54:50  vk
// Add work out of special case of infinite end value, example basegeometry
// constants which valid from run 1 till 0 (infinity).
//
// Revision 1.1.1.1  2003/01/28 21:49:58  vk
// imported ConstantsPhase2Delivery sources
//
//
// First working version of proxy with new interface
// The ConstantDelivery/ConstantDelivery/DBConstantsProxy.cc
// code was used as a baseground to have this work.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ConstantsPhase2Delivery/DBCP2Proxy.h"
#include "ConstantsDelivery/CDNoServerFoundException.h"
#include "DAException/DAException.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kDBCP2ProxyFacilityString = "ConstantsPhase2Delivery.DBCP2Proxy" ;

//
// static data member definitions
//

//
// constructors and destructor
//
template< class T >
DBCP2Proxy<T>::DBCP2Proxy( const string& iBootFilePath,
				       const DBConstantsSelectionTag& iTag ) :
   m_constants( iBootFilePath, iTag.selectionTags() , iTag.type(), true ),
   m_validRange(SyncValue::kBeginningOfTime,SyncValue::kBeginningOfTime),
   m_validNextRange(SyncValue::kBeginningOfTime,SyncValue::kBeginningOfTime)
{

   // Temp fixes to use new interface of DB constants.
   // they should go to DBConstants.hxx at some time.

   // Explicitly call new interface
   m_constants.useNewInterfaces();

   // To fix bind bug in DB I need to explicitly reset fdbName.
   //m_constants.set_fdbName(iBootFilePath.c_str());

   // needed to bind to the server
   m_constants.bind();
   
}

// DBCP2Proxy::DBCP2Proxy( const DBCP2Proxy& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }
template< class T >
DBCP2Proxy<T>::~DBCP2Proxy()
{
}

//
// assignment operators
//
// const DBCP2Proxy& DBCP2Proxy::operator=( const DBCP2Proxy& rhs )
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
DBCP2Proxy<T>::invalidateCache() {
   //don't do anything
}

template< class T>
const typename DBCP2Proxy<T>::value_type* 
DBCP2Proxy<T>::faultHandler( const Record& aRecord ,
				   const DataKey& aKey )
{

   // New API
   PMCTRY {
     VersionInfo::LoadFailureMode failureMode;
     Int16 error = m_constants.synchronizeFromHistory( aRecord.syncValue().runNumber(), failureMode );
     if(error==0) report(ERROR,kDBCP2ProxyFacilityString) 
                  << "faultHandler fail in synchronizeFromHistory" << endl;
     // be verbose
     if(failureMode == VersionInfo::CouldNotContactServer) {
       report(EMERGENCY,kDBCP2ProxyFacilityString)
         << "could not contact the server, see server logs" << endl;
     } else if(failureMode == VersionInfo::NoTagSpecified) {
       report(ERROR,kDBCP2ProxyFacilityString)
         << "either wrong tag or no tag specified, connection refused" << endl;
     }
   } 
   PMCCATCH(CORBA::SystemException, excep) {
     cerr << "Could not retrieve a constant from VM server." << excep << endl;
   }
   PMCEND_CATCH
   
   if( m_constants.isValid() ) {
      report(NOTICE,kDBCP2ProxyFacilityString) << nameOfConstant() 
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
DBCP2Proxy<T>::syncValueOfNextRecord( SyncValue& oSyncValue ) const
{

   // let's check our chache, for infinite end value no request needed
   if( m_validNextRange.begin().runNumber() && 
      !m_validNextRange.end().runNumber() ) {
      oSyncValue = SyncValue( SyncValue::kMaxRunEventValue,
                              SyncValue::kMaxRunEventValue );
      return true;
   }

   // New DB API, see DBConstants.hxx
   UInt32 startRun=0, stopRun=0;
   VersionInfo::LoadFailureMode failureMode;
   
   SCBoolean isOK =
     const_cast<DBCP2Proxy<T>*>(this)->m_constants.getNextValidityRange(startRun,stopRun,failureMode);

   if( isOK ) {

     report(DEBUG,kDBCP2ProxyFacilityString)
       <<"NextValidityRange (" <<startRun<<","<<stopRun<<") for constant "
       << nameOfConstant() << endl;

     oSyncValue = SyncValue( startRun, 0 );

     // m_validRange should be mutable
     ValidRange& temp = const_cast<DBCP2Proxy<T>*>(this)->m_validNextRange;
     temp = ValidRange( oSyncValue, SyncValue(stopRun, 0) );

   } else {
     // be verbose
     if(failureMode == VersionInfo::CouldNotContactServer) {
       report(EMERGENCY,kDBCP2ProxyFacilityString)
         << "could not contact the server, see server logs" << endl;
     } else if(failureMode == VersionInfo::NoTagSpecified) {
       report(ERROR,kDBCP2ProxyFacilityString)
         << "either wrong tag or no tag specified, connection refused" << endl;
     }
     report(DEBUG,kDBCP2ProxyFacilityString)
       << "fail in DBConstants.getNextValidityRange for "
       << nameOfConstant() << ", startRun " << startRun
       << " stopRun " << stopRun << endl;
     return false;
   }
   return true;
}


template< class T>
DABoolean 
DBCP2Proxy<T>::closestSyncValueTo( const SyncValue& iSyncValue, 
					 SyncValue& oClosestSyncValue ) const
{
   if ( m_validRange.begin() <= iSyncValue && m_validRange.end() >= iSyncValue ) {
      // we have cached the info we would get from the database
      oClosestSyncValue = m_validRange.begin();
   } else if( m_validRange.begin().runNumber() && !m_validRange.end().runNumber() ) {
      // special case of infinite end value
      oClosestSyncValue = m_validRange.begin();
   } else {

      // NEW API require to call validy range
      UInt32 startRun=0, stopRun=0;
      VersionInfo::LoadFailureMode failureMode;
      
      // Get run number from iSyncValue
      UInt32 my_run = iSyncValue.runNumber();
      // in DBConstants Hubert modified the object and we need to handle this case
      SCBoolean isOK = 
        const_cast<DBCP2Proxy<T>*>(this)->m_constants.getValidityRange(my_run,startRun,stopRun,failureMode);
      
      if( isOK ) {
        report(DEBUG,kDBCP2ProxyFacilityString)
              << "new validity range (" <<startRun<<","<<stopRun<<") for constant "
              << nameOfConstant() 
              << endl;
        oClosestSyncValue = SyncValue( startRun, 0 );
        // m_validRange should be mutable
        ValidRange& temp = const_cast<DBCP2Proxy<T>*>(this)->m_validRange;
        temp = ValidRange( oClosestSyncValue, SyncValue(stopRun, 0) );
      } else {
        // be verbose
        if(failureMode == VersionInfo::CouldNotContactServer) {
          report(EMERGENCY,kDBCP2ProxyFacilityString)
            << "could not contact the server, see server logs" << endl;
        } else if(failureMode == VersionInfo::NoTagSpecified) {
          report(ERROR,kDBCP2ProxyFacilityString)
            << "either wrong tag or no tag specified, connection refused" << endl;
        }
        report(DEBUG,kDBCP2ProxyFacilityString)
              << "fail in DBConstants.getValidityRange call, run # " << my_run << endl;
        return false;
      }
   }
   return true;
}

template< class T>
SyncValue
DBCP2Proxy<T>::presentSyncValue() const
{
   if( 0 == m_constants.getVersionInfo().startRun() ) {
      report( DEBUG, kDBCP2ProxyFacilityString ) <<
	 "startRun method returned 0" << endl;
      //something went wrong
      return SyncValue::kUndefined;
   }
   return SyncValue( m_constants.getVersionInfo().startRun(), 0 );
}


template< class T>
DABoolean 
DBCP2Proxy<T>::foundServer() const
{
   return ( 0 != m_constants.bindStatus() );
}

template< class T>
const char*
DBCP2Proxy<T>::nameOfConstant() const {
   return m_constants.bdlName();
}

//
// static member functions
//

#endif /* CONSTANTSPHASE2DELIVERY_DBCP2PROXY_CC */
