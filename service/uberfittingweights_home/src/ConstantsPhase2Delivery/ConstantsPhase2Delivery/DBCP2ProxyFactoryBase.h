#if !defined(CONSTANTSPHASE2DELIVERY_DBCP2PROXYFACTORYBASE_H)
#define CONSTANTSPHASE2DELIVERY_DBCP2PROXYFACTORYBASE_H
// -*- C++ -*-
//
// Package:     ConstantsPhase2Delivery
// Module:      DBCP2ProxyFactoryBase
// 
// Description: Abstract interface for Factories that build a DBCP2Proxy
//
// Usage:
//    <usage>
//
// Author:      Valentine Kouznetsov
// Created:     Tue Jan 28 12:47:41 EST 2003
// $Id: DBCP2ProxyFactoryBase.h,v 1.1.1.1 2003/01/28 21:49:57 vk Exp $
//
// Revision history
//
// $Log: DBCP2ProxyFactoryBase.h,v $
// Revision 1.1.1.1  2003/01/28 21:49:57  vk
// imported ConstantsPhase2Delivery sources
//
//
// First working version with new DB API.
//

// system include files

// user include files
#include "DataHandler/KeyedProxy.h"

// forward declarations
//class DBCP2SelectionTag;
class DBCP2Info;
#include "ConstantsDelivery/DBConstantsSelectionTag.h"

class DBCP2ProxyFactoryBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DBCP2ProxyFactoryBase() {}
      virtual ~DBCP2ProxyFactoryBase() {}

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      virtual void createProxy( const string& iBootFilePath,
				const DBConstantsSelectionTag& iTags,
				KeyedProxy& oKeyedProxy,
				DBCP2Info*& iRecordInfo) const = 0;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DBCP2ProxyFactoryBase( const DBCP2ProxyFactoryBase& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DBCP2ProxyFactoryBase& operator=( const DBCP2ProxyFactoryBase& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ConstantsPhase2Delivery/Template/DBCP2ProxyFactoryBase.cc"
//#endif

#endif /* CONSTANTSPHASE2DELIVERY_DBCP2PROXYFACTORYBASE_H */
