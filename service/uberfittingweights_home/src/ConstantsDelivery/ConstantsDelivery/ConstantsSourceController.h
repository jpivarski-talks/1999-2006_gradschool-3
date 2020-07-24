#if !defined(CONSTANTSDELIVERY_CONSTANTSController_H)
#define CONSTANTSDELIVERY_CONSTANTSController_H
// -*- C++ -*-
//
// Package:     ConstantsDelivery
// Module:      ConstantsSourceController
// 
// Description: A DataSourceController for DataBase access

// Usage:

// Invariants:
//
// Author:      Rich Baker
// Created:     Tues Nov 18 1997
//
// $Id: ConstantsSourceController.h,v 1.1 1998/11/16 22:03:37 mkl Exp $
//
// Revision history
//
// $Log: ConstantsSourceController.h,v $
// Revision 1.1  1998/11/16 22:03:37  mkl
// renamed ConstantsBinder/Controller ConstantsSourceBinder/Controller
//
// Revision 1.5  1998/11/10 22:12:17  baker
// Several Fixes.
// Constants Delivery Tested and Working!
// Some kludges still need to be worked on though.
//
// Revision 1.4  1998/11/07 19:13:15  baker
// Added Data Source ID information to Controller
//
// Revision 1.3  1998/11/04 15:39:20  baker
// Continuing updates to get this library compiled
//
// Revision 1.2  1998/09/30 20:40:15  baker
// Changed class names and include file names as
// necessary for migration from old DataBaseDelivery
// to new ConstantsDelivery packages.
//
// Revision 1.1  1998/09/30 18:28:58  baker
// // September 30, Rich Baker
// // Renamed header files from former DataBaseDelivery
// // for new ConstantsDelivery library
//
// Revision 1.2  1998/08/19 03:14:31  cdj
// re-engineered.  This probably won't compile.
//
// Revision 1.1.1.2  1998/01/14 20:56:57  baker
// make DataBaseDelivery library compileable
//
// Revision 1.1.1.1  1997/12/17 20:25:12  cleo3
// imported DataBaseDelivery sources
//


// system include files
#include <string>

// user include files
#include "DataDelivery/DataSourceController.h"
#include "DataHandler/Stream.h"
#include "DataHandler/StreamSet.h"
#include "DataHandler/StreamPriorityGreater.h"
#include "ConstantsDelivery/ConstantsProxyDeliverer.h"

// stl include files

// forward declarations

class ConstantsSourceController : public DataSourceController
{
      // friend classses and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      ConstantsSourceController( const Stream::Set& sSet,
                           const string& dsID,
                           const DBConstantsSelectionTag& iTags );

      virtual ~ConstantsSourceController();

      // member functions

      virtual DataSourceController::SyncStatus 
           synchronizeSource( const SyncValue& iSyncTo );

      virtual DataSourceController::SearchStatus
               findNextRecord(SyncValue& oSyncValueOfNextRecord,
			      Stream::Type& oRecordTypeOfNextRecord );

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
      ConstantsSourceController( const ConstantsSourceController& );

      // assignment operator(s)
      const ConstantsSourceController& operator=( const ConstantsSourceController& );

      // private member functions
      ConstantsProxyDeliverer& dbProxyDeliverer();

      // private const member functions

      // data members

      DBConstantsSelectionTag m_selectionTags;

// 11/7/98 Rich Baker
// Added a data member to store the source ID passed from
// the Binder during instantiation of this Controller.
      string m_sourceID;
      Stream::Type m_peekAheadRecord;
      SyncValue m_peekAheadSyncValue;
      SearchStatus m_peekAheadStatus;
      DABoolean m_havePeekedAhead;

      STL_MULTISET_COMP( Stream::Type, StreamPriorityGreater )& m_prioritizedPeekedRecords;
      // static data members

};

#endif /* CONSTANTSDELIVERY_CONSTANTSController_H */

