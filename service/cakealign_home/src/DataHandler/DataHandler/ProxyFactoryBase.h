#if !defined(DATAHANDLER_PROXYFACTORYBASE_H)
#define DATAHANDLER_PROXYFACTORYBASE_H
// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      ProxyFactoryBase
// 
// Description: Base class for all ProxyFactories
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner, Chris Jones
// Created:     Sun Nov  1 11:35:00 EST 1998
// $Id: ProxyFactoryBase.h,v 1.2 1998/11/06 00:14:11 mkl Exp $
//
// Revision history
//
// $Log: ProxyFactoryBase.h,v $
// Revision 1.2  1998/11/06 00:14:11  mkl
// added ProxyFactoryFunction
//
// Revision 1.1  1998/11/02 16:19:38  mkl
// moved ProxyFactory* stuff to DataHandler
//
// Revision 1.1  1998/11/01 20:14:48  mkl
// switched to new proxy factory registration scheme
//

// system include files

// user include files
#include "DataHandler/DataKey.h"

// forward declarations
class ProxyBase;

class ProxyFactoryBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      ProxyFactoryBase() {}
      virtual ~ProxyFactoryBase() {}

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      virtual ProxyBase* makeProxy() const = 0;

      virtual DataKey makeKey( const UsageTag& iUsage,
			       const ProductionTag& iProduction ) const = 0;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ProxyFactoryBase( const ProxyFactoryBase& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ProxyFactoryBase& operator=( const ProxyFactoryBase& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* DATAHANDLER_PROXYFACTORYBASE_H */
