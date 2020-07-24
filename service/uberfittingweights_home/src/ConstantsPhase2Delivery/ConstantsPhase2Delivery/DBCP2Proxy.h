#if !defined(CONSTANTSPHASE2DELIVERY_DBCP2PROXY_H)
#define CONSTANTSPHASE2DELIVERY_DBCP2PROXY_H
// -*- C++ -*-
//
// Package:     ConstantsPhase2Delivery
// Module:      DBCP2Proxy
// 
// Description: Proxy that delivers CLEOConstants objects
//
// Usage:
//    <usage>
//
// Author:      Valentine Kouznetsov
// Created:     Tue Jan 28 12:47:41 EST 2003
// $Id: DBCP2Proxy.h,v 1.2 2003/02/07 22:09:48 vk Exp $
//
// Revision history
//
// $Log: DBCP2Proxy.h,v $
// Revision 1.2  2003/02/07 22:09:48  vk
//
// Add NextRecordSync functionality
//
// Revision 1.1.1.1  2003/01/28 21:49:56  vk
// imported ConstantsPhase2Delivery sources
//
//
// First version which works with new DB API.
//

// system include files

#include "DataDelivery/DataSourceProxy.h" // superclass
#include "DBConstants.hxx"
#include "ConstantsDelivery/DBConstantsSelectionTag.h"
#include "ConstantsPhase2Delivery/DBCP2Info.h"

// forward declarations

template < class T >
class DBCP2Proxy : public DataSourceProxy< CLEOConstants<T> >, 
			 public DBCP2Info
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef CLEOConstants< T > value_type; 
      // ---------- Constructors and destructor ----------------
      DBCP2Proxy( const string& iBootFilePath,
			const DBConstantsSelectionTag& iTag );
      virtual ~DBCP2Proxy();

      // ---------- member functions ---------------------------
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record& aRecord ,
					      const DataKey& aKey );


      // ---------- const member functions ---------------------
      virtual DABoolean syncValueOfNextRecord( SyncValue& oSyncValue ) const;
      virtual DABoolean closestSyncValueTo( const SyncValue& iSyncValue, 
					    SyncValue& oClosestSyncValue ) const ;
      virtual SyncValue presentSyncValue() const;

      virtual DABoolean foundServer() const;

      virtual const char* nameOfConstant() const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // cache
      class ValidRange {
	 public: 
	    ValidRange() {}
	    ValidRange( const SyncValue& iBegin,
			const SyncValue& iEnd ) :
	       m_begin(iBegin), m_end(iEnd) {}
	    const SyncValue& begin() const { return m_begin; }
	    const SyncValue& end() const { return m_end; }
	 private:
	    SyncValue m_begin;
	    SyncValue m_end;
      };

      // ---------- Constructors and destructor ----------------
      DBCP2Proxy(); //stop default
      DBCP2Proxy( const DBCP2Proxy& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DBCP2Proxy& operator=( const DBCP2Proxy& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DBConstants< T > m_constants;
      ValidRange m_validRange;
      ValidRange m_validNextRange;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
# include "ConstantsPhase2Delivery/Template/DBCP2Proxy.cc"
#endif

#endif /* CONSTANTSPHASE2DELIVERY_DBCP2PROXY_H */
