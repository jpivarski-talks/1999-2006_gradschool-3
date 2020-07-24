#if !defined(DATAHANDLER_PROXYFACTORYSINGLEINSTANCE_H)
#define DATAHANDLER_PROXYFACTORYSINGLEINSTANCE_H
// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      ProxyFactorySingleInstance
// 
/**\class ProxyFactorySingleInstance ProxyFactorySingleInstance.h DataHandler/ProxyFactorySingleInstance.h

 Description: Templated proxy factory taking an argument as input that is
              used to construct the proxy.  Only a single version of
              the proxy created (at construction time), and is
              returned repeatedly


 Usage:
    <usage>

*/
//
// Author:      Lawrence Gibbons
// Created:     Fri Apr 26 13:27:12 EDT 2002
// $Id: ProxyFactorySingleInstance.h,v 1.3 2002/12/09 16:21:04 cleo3 Exp $
//
// Revision history
//
// $Log: ProxyFactorySingleInstance.h,v $
// Revision 1.3  2002/12/09 16:21:04  cleo3
// added typename
//
// Revision 1.2  2002/07/16 23:19:30  cdj
// fixed memory problem in ProxyFactorySingleInstance that affect MC generation
//
// Revision 1.1  2002/05/02 20:39:45  lkg
// Factory that provides a single instance of a proxy, which is invoked
// at proxy registration time.  Helpful if one wishes to hang parameters
// off of a proxy and have the parameters appear when the producer is
// loaded.
//
//

// system include files

// user include files
#include "DataHandler/ProxyFactoryBase.h"

// forward declarations

template< class Proxy, class Argument >
class ProxyFactorySingleInstance : public ProxyFactoryBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      ProxyFactorySingleInstance(Argument arg):m_instance(new Proxy(arg)){}
      virtual ~ProxyFactorySingleInstance(){ delete m_instance;}

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      virtual ProxyBase* makeProxy() const {
	 //Need to release ownership of the Proxy to avoid
	 //case of multiple deletion attempts
	 ProxyBase* returnValue = m_instance;
	 const_cast<ProxyFactorySingleInstance*>(this)->m_instance = 0;
         return returnValue;
      }
      
      virtual DataKey makeKey( const UsageTag& iUsage,
			       const ProductionTag& iProduction ) const {
         return RecordMethods< typename Proxy::value_type >::makeKey(
            iUsage, iProduction );
      }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ProxyFactorySingleInstance( const ProxyFactorySingleInstance& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ProxyFactorySingleInstance& operator=( const ProxyFactorySingleInstance& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Proxy* m_instance;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DataHandler/Template/ProxyFactorySingleInstance.cc"
//#endif

#endif /* DATAHANDLER_PROXYFACTORYSINGLEINSTANCE_H */
