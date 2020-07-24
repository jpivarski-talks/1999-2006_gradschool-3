#if !defined(CONSTANTSPHASE2DELIVERY_DBCP2RECORDINFO_H)
#define CONSTANTSPHASE2DELIVERY_DBCP2RECORDINFO_H
// -*- C++ -*-
//
// Package:     ConstantsPhase2Delivery
// Module:      DBCP2RecordInfo
// 
// Description: Collects information from DBCP2Info for one Record
//
// Usage:
//    Used by ConstantsProxyDeliverer
//
// Author:      Valentine Kouznetsov
// Created:     Tue Jan 28 12:47:41 EST 2003
// $Id: DBCP2RecordInfo.h,v 1.1.1.1 2003/01/28 21:49:58 vk Exp $
//
// Revision history
//
// $Log: DBCP2RecordInfo.h,v $
// Revision 1.1.1.1  2003/01/28 21:49:58  vk
// imported ConstantsPhase2Delivery sources
//
//
// First version which works with new DB API.
//

// system include files

// user include files
#include "DataHandler/SyncValue.h"
#include "DataHandler/Stream.h"

// forward declarations
#include "STLUtility/fwd_vector.h"
class DBCP2Info;

class DBCP2RecordInfo
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DBCP2RecordInfo( const Stream::Type& iStream );
      virtual ~DBCP2RecordInfo();

      // ---------- member functions ---------------------------
      ///does not take ownership of the memory
      void addConstantsInfo( DBCP2Info* );

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
      //DBCP2RecordInfo( const DBCP2RecordInfo& ); // stop default

      // ---------- assignment operator(s) ---------------------
      //const DBCP2RecordInfo& operator=( const DBCP2RecordInfo& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Stream::Type m_stream;
      STL_VECTOR(DBCP2Info*)& m_constantsInfos;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ConstantsPhase2Delivery/Template/DBCP2RecordInfo.cc"
//#endif

#endif /* CONSTANTSPHASE2DELIVERY_DBCP2RECORDINFO_H */
