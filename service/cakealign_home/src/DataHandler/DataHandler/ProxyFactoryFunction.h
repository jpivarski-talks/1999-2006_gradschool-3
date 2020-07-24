#if !defined(DATAHANDLER_PROXYFACTORYFUNCTION_H)
#define DATAHANDLER_PROXYFACTORYFUNCTION_H
// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      ProxyFactoryFunction
// 
// Description: templated proxy factory taking a factory function as input
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner, Chris Function
// Created:     Tue Nov 3 11:35:09 EST 1998
// $Id: ProxyFactoryFunction.h,v 1.1 1998/11/06 00:14:38 mkl Exp $
//
// Revision history
//
// $Log: ProxyFactoryFunction.h,v $
// Revision 1.1  1998/11/06 00:14:38  mkl
// added ProxyFactoryFunction
//
//

// system include files

// user include files
#include "DataHandler/ProxyFactoryBase.h"

// forward declarations

template< class Producer, class ProxyType >
class ProxyFactoryFunction : public ProxyFactoryBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      // allow both (Producer::*)() const and non-const functions
      typedef ProxyBase* (Producer::*FactoryFunction)();
      typedef ProxyBase* (Producer::*ConstFactoryFunction)() const;

      // ---------- Constructors and destructor ----------------
      ProxyFactoryFunction( Producer& iProducer, 
			    FactoryFunction iFunction );
      ProxyFactoryFunction( const Producer& iProducer, 
			    ConstFactoryFunction iFunction );
      virtual ~ProxyFactoryFunction();

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
      ProxyFactoryFunction( const ProxyFactoryFunction& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ProxyFactoryFunction& operator=( 
	 const ProxyFactoryFunction& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Producer& m_producer;
      FactoryFunction m_function;
      ConstFactoryFunction m_constFunction;
      DABoolean m_useConstFunction;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
# include "DataHandler/Template/ProxyFactoryFunction.cc"
#endif

#endif /* DATAHANDLER_PROXYFACTORYFUNCTION_H */
