#if !defined(CONSTANTSDELIVERY_DBCONSTANTSRECORDINFO_H)
#define CONSTANTSDELIVERY_DBCONSTANTSRECORDINFO_H
// -*- C++ -*-
//
// Package:     ConstantsDelivery
// Module:      DBConstantsRecordInfo
// 
// Description: Collects information from DBConstantsInfo for one Record
//
// Usage:
//    Used by ConstantsProxyDeliverer
//
// Author:      Chris D Jones
// Created:     Tue Aug 18 13:40:29 EDT 1998
// $Id: DBConstantsRecordInfo.h,v 1.7 1999/11/03 03:33:26 cdj Exp $
//
// Revision history
//
// $Log: DBConstantsRecordInfo.h,v $
// Revision 1.7  1999/11/03 03:33:26  cdj
// now handles multiple constants per stream
//
// Revision 1.6  1999/07/14 21:22:33  cdj
// now uses bootfile path to find servers
//
// Revision 1.5  1999/07/10 21:03:35  cdj
// now inform user if server is unavailable
//
// Revision 1.4  1998/11/05 18:52:10  baker
// Another batch of updates to get this library finally compiled
// *AND* linkable
//
// Revision 1.3  1998/11/04 18:55:52  baker
// Fix stupid errors so that this library actually compiles (finally!)
//
// Revision 1.2  1998/09/30 20:40:18  baker
// Changed class names and include file names as
// necessary for migration from old DataBaseDelivery
// to new ConstantsDelivery packages.
//
// Revision 1.1  1998/09/30 18:29:01  baker
// // September 30, Rich Baker
// // Renamed header files from former DataBaseDelivery
// // for new ConstantsDelivery library
//
// Revision 1.2  1998/08/19 14:25:26  cdj
// needed some additional member functions
//
// Revision 1.1  1998/08/19 03:17:39  cdj
// first submission
//

// system include files

// user include files
#include "DataHandler/SyncValue.h"
#include "DataHandler/Stream.h"

// forward declarations
#include "STLUtility/fwd_vector.h"
class DBConstantsInfo;

class DBConstantsRecordInfo
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DBConstantsRecordInfo( const Stream::Type& iStream );
      virtual ~DBConstantsRecordInfo();

      // ---------- member functions ---------------------------
      ///does not take ownership of the memory
      void addConstantsInfo( DBConstantsInfo* );

      // ---------- const member functions ---------------------
      virtual DABoolean syncValueOfNextRecord(SyncValue& oSyncValue ) const ;
      virtual DABoolean closestSyncValueTo( const SyncValue& iSyncValue, 
					    SyncValue& oClosestSyncValue ) const ;
      virtual SyncValue presentSyncValue() const;

      virtual DABoolean foundServers() const ;

      const Stream::Type& stream() const { return m_stream; }
      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      //DBConstantsRecordInfo( const DBConstantsRecordInfo& ); // stop default

      // ---------- assignment operator(s) ---------------------
      //const DBConstantsRecordInfo& operator=( const DBConstantsRecordInfo& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Stream::Type m_stream;
      STL_VECTOR(DBConstantsInfo*)& m_constantsInfos;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ConstantsDelivery/Template/DBConstantsRecordInfo.cc"
//#endif

#endif /* CONSTANTSDELIVERY_DBCONSTANTSRECORDINFO_H */
