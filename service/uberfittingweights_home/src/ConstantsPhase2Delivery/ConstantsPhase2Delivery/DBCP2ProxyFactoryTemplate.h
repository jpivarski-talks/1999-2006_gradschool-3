#if !defined(CONSTANTSPHASE2DELIVERY_DBCP2PROXYFACTORYTEMPLATE_H)
#define CONSTANTSPHASE2DELIVERY_DBCP2PROXYFACTORYTEMPLATE_H
// -*- C++ -*-
//
// Package:     ConstantsPhase2Delivery
// Module:      DBCP2ProxyFactoryTemplate
// 
// Description: Creates a DBCP2Proxy<T>
//
// Usage:
//    <usage>
//
// Author:      Valentine Kouznetsov
// Created:     Tue Jan 28 12:47:41 EST 2003
// $Id: DBCP2ProxyFactoryTemplate.h,v 1.1.1.1 2003/01/28 21:49:57 vk Exp $
//
// Revision history
//
// $Log: DBCP2ProxyFactoryTemplate.h,v $
// Revision 1.1.1.1  2003/01/28 21:49:57  vk
// imported ConstantsPhase2Delivery sources
//
//
// First working version with new DB API.
//

// system include files

// user include files
#include "ConstantsPhase2Delivery/DBCP2ProxyFactoryBase.h"

// forward declarations
template< class T >
class DBCP2ProxyFactoryTemplate : public DBCP2ProxyFactoryBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DBCP2ProxyFactoryTemplate();
      virtual ~DBCP2ProxyFactoryTemplate();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      void createProxy( const string& iBootFilePath,
			const DBConstantsSelectionTag& iTags,
                        KeyedProxy& oKeyedProxy,
			DBCP2Info*& oRecordInfo ) const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DBCP2ProxyFactoryTemplate( const DBCP2ProxyFactoryTemplate& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DBCP2ProxyFactoryTemplate& operator=( const DBCP2ProxyFactoryTemplate& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
# include "ConstantsPhase2Delivery/Template/DBCP2ProxyFactoryTemplate.cc"
#endif

#endif /* CONSTANTSPHASE2DELIVERY_DBCP2PROXYFACTORYTEMPLATE_H */
