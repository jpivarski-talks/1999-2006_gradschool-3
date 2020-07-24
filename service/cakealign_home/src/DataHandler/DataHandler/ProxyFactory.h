#if !defined(DATAHANDLER_PROXYFACTORY_H)
#define DATAHANDLER_PROXYFACTORY_H
// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      ProxyFactory
// 
// Description: templated proxy factory
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner, Chris Jones
// Created:     Sun Nov  1 11:35:09 EST 1998
// $Id: ProxyFactory.h,v 1.1 1998/11/02 16:19:38 mkl Exp $
//
// Revision history
//
// $Log: ProxyFactory.h,v $
// Revision 1.1  1998/11/02 16:19:38  mkl
// moved ProxyFactory* stuff to DataHandler
//
// Revision 1.1  1998/11/01 20:14:48  mkl
// switched to new proxy factory registration scheme
//

// system include files

// user include files
#include "DataHandler/ProxyFactoryBase.h"

// forward declarations

template< class ProxyType >
class ProxyFactory : public ProxyFactoryBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      ProxyFactory();
      virtual ~ProxyFactory();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      virtual ProxyBase* makeProxy() const;
      virtual DataKey makeKey( const UsageTag& iUsage,
			       const ProductionTag& iProduction ) const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ProxyFactory( const ProxyFactory& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ProxyFactory& operator=( const ProxyFactory& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
# include "DataHandler/Template/ProxyFactory.cc"
#endif

#endif /* DATAHANDLER_PROXYFACTORY_H */
