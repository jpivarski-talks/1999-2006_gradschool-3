#if !defined(DATAHANDLER_DATAKEY_H)
#define DATAHANDLER_DATAKEY_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      DataKey
// 
// Description: Key used to identify data within a Record
//
// Usage:
//    <usage>
//
// Author:      Chris D. Jones
// Created:     Wed Sep 23 16:12:58 EDT 1998
// $Id: DataKey.h,v 1.1 1998/09/25 19:31:42 cdj Exp $
//
// Revision history
//
// $Log: DataKey.h,v $
// Revision 1.1  1998/09/25 19:31:42  cdj
// no longer use ProxyDict
//

// system include files

// user include files
#include "DataHandler/DataKeyTags.h"

// forward declarations

class DataKey
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      // ---------- Constructors and destructor ----------------
      DataKey();
      DataKey( const TypeTag& iType, const IdTags& iId ) :
	 m_type( iType ), 
	 m_usage( iId.usage() ), 
	 m_production( iId.production() ) {}
      DataKey(const TypeTag& iType, 
	      const UsageTag& iUsage,
	      const ProductionTag& iProduction ) :
	 m_type( iType ),
	 m_usage( iUsage ),
	 m_production( iProduction ) {}

      //virtual ~DataKey(); //use default

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      const TypeTag&       type() const       { return m_type; }
      const UsageTag&      usage() const      { return m_usage; }
      const ProductionTag& production() const { return m_production; }

      DABoolean operator==( const DataKey& iRHS ) const;
      DABoolean operator<( const DataKey& iRHS ) const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      //DataKey( const DataKey& ); // use default

      // ---------- assignment operator(s) ---------------------
      //const DataKey& operator=( const DataKey& ); // use default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      TypeTag m_type;
      UsageTag m_usage;
      ProductionTag m_production;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DataHandler/Template/DataKey.cc"
//#endif

#endif /* DATAHANDLER_DATAKEY_H */
