// -*- C++ -*-
//
// Package:     <ConstantsDelivery>
// Module:      DBConstantsRecordInfo
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Mon Oct 25 17:52:00 EDT 1999
// $Id: DBConstantsRecordInfo.cc,v 1.3 2000/06/17 19:21:08 cdj Exp $
//
// Revision history
//
// $Log: DBConstantsRecordInfo.cc,v $
// Revision 1.3  2000/06/17 19:21:08  cdj
// properly handle case where can not find server or no constant for this run
//
// Revision 1.2  1999/12/06 20:35:24  cdj
// updated to use new DBConstants code & improved error reports
//
// Revision 1.1  1999/11/03 03:33:20  cdj
// now handles multiple constants per stream
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include <assert.h>

// user include files
#include "Experiment/report.h"
#include "ConstantsDelivery/DBConstantsRecordInfo.h"
#include "ConstantsDelivery/DBConstantsInfo.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ConstantsDelivery.DBConstantsRecordInfo" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DBConstantsRecordInfo.cc,v 1.3 2000/06/17 19:21:08 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DBConstantsRecordInfo::DBConstantsRecordInfo( const Stream::Type& iStream ) 
   : m_stream( iStream ),
     m_constantsInfos( *( new STL_VECTOR(DBConstantsInfo*) ) )
{
   assert( 0 != &m_constantsInfos );
   m_constantsInfos.reserve(1);
}

// DBConstantsRecordInfo::DBConstantsRecordInfo( const DBConstantsRecordInfo& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DBConstantsRecordInfo::~DBConstantsRecordInfo()
{
   delete &m_constantsInfos;
}

//
// assignment operators
//
// const DBConstantsRecordInfo& DBConstantsRecordInfo::operator=( const DBConstantsRecordInfo& rhs )
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
DBConstantsRecordInfo::addConstantsInfo( DBConstantsInfo* iInfo)
{
   m_constantsInfos.push_back( iInfo );
}
//
// const member functions
//
DABoolean 
DBConstantsRecordInfo::syncValueOfNextRecord(SyncValue& oSyncValue ) const
{
   assert( 0 != m_constantsInfos.size() );
   DABoolean returnValue = true;

   //get first value
   SyncValue nextRecordSync;
   if( ! (returnValue = 
	  m_constantsInfos[0]->syncValueOfNextRecord( nextRecordSync ) ) ) {
      //failed to find next value
      return false;
   }
   

   //now check to make sure all other constants agree that this is next
   SyncValue temp;
   STL_VECTOR(DBConstantsInfo*)::const_iterator itEnd = m_constantsInfos.end();
   STL_VECTOR(DBConstantsInfo*)::const_iterator itInfo =
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
	 returnValue = false;
	 break;
      }
   }
   return returnValue;
}

DABoolean 
DBConstantsRecordInfo::closestSyncValueTo( const SyncValue& iSyncValue, 
					   SyncValue& oClosestSyncValue ) const
{
   DABoolean returnValue = false;
   oClosestSyncValue = SyncValue::kBeginningOfTime;

   SyncValue temp;
   STL_VECTOR(DBConstantsInfo*)::const_iterator itEnd = m_constantsInfos.end();
   for( STL_VECTOR(DBConstantsInfo*)::const_iterator itInfo = 
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
	       << endl;
	 }
	 if( oClosestSyncValue < temp ) {
	    //If it's larger, we need to use this for next sync value
	    oClosestSyncValue = temp;
	 }
      } else {
	 report(DEBUG, kFacilityString) 
	    << "failed to find " 
	    << m_constantsInfos[0]->nameOfConstant() 
	    << " for " << iSyncValue << endl;	 
      }
   }
   
   return returnValue;

}

SyncValue 
DBConstantsRecordInfo::presentSyncValue() const
{
   return m_constantsInfos[0]->presentSyncValue();
}

DABoolean 
DBConstantsRecordInfo::foundServers() const 
{
   DABoolean returnValue = true;

   STL_VECTOR(DBConstantsInfo*)::const_iterator itEnd = m_constantsInfos.end();
   for( STL_VECTOR(DBConstantsInfo*)::const_iterator itInfo =
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
