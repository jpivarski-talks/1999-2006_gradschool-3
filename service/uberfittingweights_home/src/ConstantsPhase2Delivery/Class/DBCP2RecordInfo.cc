// -*- C++ -*-
//
// Package:     <ConstantsPhase2Delivery>
// Module:      DBCP2RecordInfo
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Valentine Kouznetsov
// Created:     Mon Jan 20 21:36:42 EST 2003
// $Id: DBCP2RecordInfo.cc,v 1.3 2003/05/22 19:07:13 vk Exp $
//
// Revision history
//
// $Log: DBCP2RecordInfo.cc,v $
// Revision 1.3  2003/05/22 19:07:13  vk
// Add more warning info for closest SyncValue
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

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include <assert.h>

// user include files
#include "Experiment/report.h"
#include "ConstantsPhase2Delivery/DBCP2RecordInfo.h"
#include "ConstantsPhase2Delivery/DBCP2Info.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ConstantsPhase2Delivery.DBCP2RecordInfo" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DBCP2RecordInfo.cc,v 1.3 2003/05/22 19:07:13 vk Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DBCP2RecordInfo::DBCP2RecordInfo( const Stream::Type& iStream ) 
   : m_stream( iStream ),
     m_constantsInfos( *( new STL_VECTOR(DBCP2Info*) ) )
{
   assert( 0 != &m_constantsInfos );
   m_constantsInfos.reserve(1);
}

// DBCP2RecordInfo::DBCP2RecordInfo( const DBCP2RecordInfo& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DBCP2RecordInfo::~DBCP2RecordInfo()
{
   delete &m_constantsInfos;
}

//
// assignment operators
//
// const DBCP2RecordInfo& DBCP2RecordInfo::operator=( const DBCP2RecordInfo& rhs )
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
void 
DBCP2RecordInfo::addConstantsInfo( DBCP2Info* iInfo)
{
   m_constantsInfos.push_back( iInfo );
}
//
// const member functions
//
DABoolean 
DBCP2RecordInfo::syncValueOfNextRecord(SyncValue& oSyncValue ) const
{
   assert( 0 != m_constantsInfos.size() );
   DABoolean returnValue = true;

   //get first value
   SyncValue nextRecordSync;
   if( ! (returnValue = 
	  m_constantsInfos[0]->syncValueOfNextRecord( nextRecordSync ) ) ) {
      //failed to find next value
      report( WARNING, kFacilityString ) 
        << "syncValueOfNextRecord failed for 0 constantsInfo" << endl;
      return false;
   }
   

   //now check to make sure all other constants agree that this is next
   SyncValue temp;
   STL_VECTOR(DBCP2Info*)::const_iterator itEnd = m_constantsInfos.end();
   STL_VECTOR(DBCP2Info*)::const_iterator itInfo =
      m_constantsInfos.begin(); 
   for( ++itInfo ; //get the one past the beginning
	itInfo != itEnd;
	++itInfo ) {
      if( ( returnValue = (*itInfo)->syncValueOfNextRecord( temp ) ) ) {
	 if( nextRecordSync != temp ) {
	    report( WARNING, kFacilityString ) 
	       <<"constant "
	       << (*itInfo)->nameOfConstant()
	       <<" has different next SyncValue"
	       << endl;
	    if( nextRecordSync > temp ) {
	       //If it's smaller, we need to use this for next sync value
	       nextRecordSync = temp;
	    }
	 }
      } else {
	 report( WARNING, kFacilityString ) <<"constant "
					    << (*itInfo)->nameOfConstant()
					    <<" has no next record\n"
					    <<" but other constants do"
					    << endl;
	 //we had a problem
	 oSyncValue = nextRecordSync;
	 returnValue = false;
	 break;
      }
   }
   oSyncValue = nextRecordSync;
   return returnValue;
}

DABoolean 
DBCP2RecordInfo::closestSyncValueTo( const SyncValue& iSyncValue, 
					   SyncValue& oClosestSyncValue ) const
{
   DABoolean returnValue = false;
   oClosestSyncValue = SyncValue::kBeginningOfTime;

   SyncValue temp;
   STL_VECTOR(DBCP2Info*)::const_iterator itEnd = m_constantsInfos.end();
   for( STL_VECTOR(DBCP2Info*)::const_iterator itInfo = 
	   m_constantsInfos.begin();    
	itInfo != itEnd;
	++itInfo ) {

      if( ((*itInfo)->closestSyncValueTo( iSyncValue,
					  temp       ) ) ) {
	 returnValue = true;
	 if( oClosestSyncValue != temp &&
	     oClosestSyncValue != SyncValue::kBeginningOfTime ) {
	    report( WARNING, kFacilityString ) 
	       <<"constant "
	       << (*itInfo)->nameOfConstant()
	       <<" in Stream " << stream() 
	       <<" has different closest SyncValue "
	       << temp
	       << endl
	       << " current SyncValue " << iSyncValue
	       << " found closest one " << oClosestSyncValue
	       << endl;
	 }
	 if( oClosestSyncValue < temp ) {
	    //If it's larger, we need to use this for next sync value
	    oClosestSyncValue = temp;
	 }
      } else {
	 report(DEBUG, kFacilityString) 
	    << "failed to find " 
	    << (*itInfo)->nameOfConstant() 
	    //<< m_constantsInfos[0]->nameOfConstant() 
	    << " for " << iSyncValue << endl;	 
      }
   }
   
   return returnValue;

}

SyncValue 
DBCP2RecordInfo::presentSyncValue() const
{
   return m_constantsInfos[0]->presentSyncValue();
}

DABoolean 
DBCP2RecordInfo::foundServers() const 
{
   DABoolean returnValue = true;

   STL_VECTOR(DBCP2Info*)::const_iterator itEnd = m_constantsInfos.end();
   for( STL_VECTOR(DBCP2Info*)::const_iterator itInfo =
	   m_constantsInfos.begin();
	itInfo != itEnd;
	++itInfo ) {

      if( ! (*itInfo)->foundServer() ) {
	 report( ERROR, kFacilityString ) 
	    << "connection to server failed for constant "
	    << (*itInfo)->nameOfConstant()
	    << " in stream "
	    << stream()
	    << endl;
	 returnValue = false;
	 //keep looping to check all servers

      }
   }
   return returnValue;
}

//
// static member functions
//
