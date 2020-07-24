#if !defined(CONSTANTSDELIVERY_DBCONSTANTSINFO_H)
#define CONSTANTSDELIVERY_DBCONSTANTSINFO_H
// -*- C++ -*-
//
// Package:     ConstantsDelivery
// Module:      DBConstantsInfo
// 
// Description: Abstract interface to allow DBConstantsRecordInfo to get
//                SyncValue info from Proxy using a DBConstants<> object.
//
// Usage:
//    Used by ConstantsProxyDeliverer
//
// Author:      Chris D Jones
// Created:     Tue Aug 18 13:40:29 EDT 1998
// $Id: DBConstantsInfo.h,v 1.1 1999/11/03 03:33:23 cdj Exp $
//
// Revision history
//
// $Log: DBConstantsInfo.h,v $
// Revision 1.1  1999/11/03 03:33:23  cdj
// now handles multiple constants per stream
//
//

// system include files

// user include files

// forward declarations
class SyncValue;
#include "DataHandler/SyncValue.h"
class DBConstantsInfo
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DBConstantsInfo() {}
      virtual ~DBConstantsInfo() {}

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      virtual DABoolean syncValueOfNextRecord(SyncValue& oSyncValue ) const = 0;
      virtual DABoolean closestSyncValueTo( const SyncValue& iSyncValue, 
					    SyncValue& oClosestSyncValue ) const = 0;
      virtual SyncValue presentSyncValue() const= 0;

      virtual DABoolean foundServer() const = 0;

      virtual const char* nameOfConstant() const = 0;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      //DBConstantsInfo( const DBConstantsInfo& ); // stop default

      // ---------- assignment operator(s) ---------------------
      //const DBConstantsInfo& operator=( const DBConstantsInfo& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ConstantsDelivery/Template/DBConstantsInfo.cc"
//#endif

#endif /* CONSTANTSDELIVERY_DBCONSTANTSINFO_H */
