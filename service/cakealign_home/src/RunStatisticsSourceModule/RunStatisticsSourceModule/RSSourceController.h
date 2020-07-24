#if !defined(RUNSTATISTICSSOURCEMODULE_RSSOURCECONTROLLER_H)
#define RUNSTATISTICSSOURCEMODULE_RSSOURCECONTROLLER_H
// -*- C++ -*-
//
// Package:     RunStatisticsSourceModule
// Module:      RSSourceController
// 
/**\class RSSourceController RSSourceController.h RunStatisticsSourceModule/RSSourceController.h

 Description: Source controller that reads from RunStatistics database

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Tue Jan 25 11:22:56 EST 2000
// $Id: RSSourceController.h,v 1.4 2003/03/04 00:07:16 cleo3 Exp $
//
// Revision history
//
// $Log: RSSourceController.h,v $
// Revision 1.4  2003/03/04 00:07:16  cleo3
// fixed bug under mico since I assumed CORBA _var variables used reference counting
//
// Revision 1.3  2001/10/25 19:47:27  cdj
// if don't get startrun the first time, try again (needed for Online)
//
// Revision 1.2  2000/05/08 19:52:14  cdj
// now compiles on OSF with MICO
//
// Revision 1.1.1.1  2000/01/29 20:47:36  cdj
// imported RunStatisticsSourceModule source
//

// system include files
#include <vector>

// user include files
#include "DataDelivery/DataSourceController.h"

#include "CLEORB.hxx" //defines CLEORB_CLIENT_HH
#include CLEORB_CLIENT_HH(DBRunStatistics)

// forward declarations
#include "STLUtility/fwd_vector.h"
class RSProxyDeliverer;

class RSSourceController : public DataSourceController
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      static const char* const kDataSourceID;

      // ---------- Constructors and destructor ----------------
      RSSourceController( const Stream::Set& iStreams );
      virtual ~RSSourceController();

      // ---------- member functions ---------------------------
      virtual DataSourceController::SyncStatus 
               synchronizeSource( const SyncValue& iSyncTo );

      virtual DataSourceController::SearchStatus
               findNextRecord(SyncValue& oSyncValueOfNextRecord,
			      Stream::Type& oRecordTypeOfNextRecord ) ;

      virtual DataSourceController::SearchStatus 
               gotoNextRecord() ;

      // ---------- const member functions ---------------------
      virtual DABoolean canRandomAccess() const ;
      virtual string dataSourceID() const ;
      virtual string parameters() const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementSetForcedSequentialAccess( DABoolean iFlag ) ;

      // ---------- protected const member functions -----------
      virtual ProxyDeliverer* createProxyDeliverer() const ;

   private:
      // ---------- Constructors and destructor ----------------
      RSSourceController( const RSSourceController& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const RSSourceController& operator=( const RSSourceController& ); // stop default

      // ---------- private member functions -------------------
      RSProxyDeliverer& rsProxyDeliverer() const;
      DataSourceController::SyncStatus getDataForRun( unsigned int iRunNumber );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DBRunStatistics::DataBase_var m_rsdb;
      unsigned long m_runNumber;
      unsigned int m_numberOfRetrievedRecords;
      
      STL_VECTOR(const DBRunStatistics::RunStatistics* ) m_rsList;
      STL_VECTOR(DBRunStatistics::RunStatisticsList* ) m_rsListList;
      STL_VECTOR(InfoX::Type) m_usedStatesList;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "RunStatisticsSourceModule/Template/RSSourceController.cc"
//#endif

#endif /* RUNSTATISTICSSOURCEMODULE_RSSOURCECONTROLLER_H */
