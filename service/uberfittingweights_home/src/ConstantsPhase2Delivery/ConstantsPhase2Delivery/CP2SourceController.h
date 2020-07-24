#if !defined(CONSTANTSPHASE2DELIVERY_CP2SourceController_H)
#define CONSTANTSPHASE2DELIVERY_CP2SourceController_H
// -*- C++ -*-
//
// Package:     ConstantsPhase2Delivery
// Module:      CP2SourceController
// 
// Description: A DataSourceController for DataBase access

// Usage:

// Invariants:
//
// Author:      Valentine Kouznetsov
// Created:     Wed Jan 15 10:57:59 EST 2003
//
// $Id: CP2SourceController.h,v 1.2 2003/02/07 22:09:48 vk Exp $
//
// Revision history
//
// $Log: CP2SourceController.h,v $
// Revision 1.2  2003/02/07 22:09:48  vk
//
// Add NextRecordSync functionality
//
// Revision 1.1.1.1  2003/01/28 21:49:56  vk
// imported ConstantsPhase2Delivery sources
//
//
// First working version of code with new DB API
//

// system include files
#include <string>

// user include files
#include "DataDelivery/DataSourceController.h"
#include "DataHandler/Stream.h"
#include "DataHandler/StreamSet.h"
#include "DataHandler/StreamPriorityGreater.h"
#include "ConstantsPhase2Delivery/CP2ProxyDeliverer.h"
#include "ConstantsDelivery/DBConstantsSelectionTag.h"

// stl include files

// forward declarations
template<class T> class DBConstants;

class CP2SourceController : public DataSourceController
{
      // friend classses and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      CP2SourceController( const Stream::Set& sSet,
                           const string& dsID,
                           const DBConstantsSelectionTag& iTags );

      virtual ~CP2SourceController();

      // member functions

      virtual DataSourceController::SyncStatus 
           synchronizeSource( const SyncValue& iSyncTo );

      virtual DataSourceController::SearchStatus
               findNextRecord(SyncValue& oSyncValueOfNextRecord,
			      Stream::Type& oStreamTypeOfNextRecord );

      virtual DataSourceController::SearchStatus 
               gotoNextRecord( void );


      // const member functions
      virtual DABoolean canRandomAccess( void ) const;

      virtual string dataSourceID( void ) const;
      virtual string parameters( void ) const;

      // const member functions

      // static member functions

   protected:
      // protected member functions
      virtual void implementSetForcedSequentialAccess( DABoolean iFlag );

      // protected const member functions
      virtual ProxyDeliverer* createProxyDeliverer( void ) const;


   private:
      // Constructors and destructor
      CP2SourceController( const CP2SourceController& );

      // assignment operator(s)
      const CP2SourceController& operator=( const CP2SourceController& );

      // private member functions
      CP2ProxyDeliverer& CP2dbProxyDeliverer();

      // private const member functions

      // data members
      //DBVersionManager::DataBase_var m_vmdb;

      DBConstantsSelectionTag m_selectionTags;

      string m_sourceID;
      Stream::Type m_peekAheadRecord;
      SyncValue m_peekAheadSyncValue;
      SearchStatus m_peekAheadStatus;
      DABoolean m_havePeekedAhead;

      STL_MULTISET_COMP( Stream::Type, StreamPriorityGreater )& m_prioritizedPeekedRecords;
      // static data members

};

#endif /* CONSTANTSPHASE2DELIVERY_CP2SourceController_H */

