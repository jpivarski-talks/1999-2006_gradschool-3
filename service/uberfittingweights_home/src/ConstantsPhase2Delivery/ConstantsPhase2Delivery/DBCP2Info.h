#if !defined(CONSTANTSPHASE2DELIVERY_DBCP2INFO_H)
#define CONSTANTSPHASE2DELIVERY_DBCP2INFO_H
// -*- C++ -*-
//
// Package:     ConstantsPhase2Delivery
// Module:      DBCP2Info
// 
// Description: Abstract interface to allow DBCP2RecordInfo to get
//                SyncValue info from Proxy using a DBCP2<> object.
//
// Usage:
//    Used by CP2Proxy
//
// Author:      Valentine Kouznetsov
// Created:     Tue Jan 28 12:47:41 EST 2003
// $Id: DBCP2Info.h,v 1.1.1.1 2003/01/28 21:49:56 vk Exp $
//
// Revision history
//
// $Log: DBCP2Info.h,v $
// Revision 1.1.1.1  2003/01/28 21:49:56  vk
// imported ConstantsPhase2Delivery sources
//
//
// First working version with new DB API.
//

// system include files

// user include files

// forward declarations
class SyncValue;
#include "DataHandler/SyncValue.h"
class DBCP2Info
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DBCP2Info() {}
      virtual ~DBCP2Info() {}

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
      //DBCP2Info( const DBCP2Info& ); // stop default

      // ---------- assignment operator(s) ---------------------
      //const DBCP2Info& operator=( const DBCP2Info& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ConstantsPhase2Delivery/Template/DBCP2Info.cc"
//#endif

#endif /* CONSTANTSPHASE2DELIVERY_DBCP2INFO_H */
