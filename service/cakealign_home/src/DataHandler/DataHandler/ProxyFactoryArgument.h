#if !defined(DATAHANDLER_PROXYFACTORYARGUMENT_H)
#define DATAHANDLER_PROXYFACTORYARGUMENT_H
// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      ProxyFactoryArgument
// 
/**\class ProxyFactoryArgument ProxyFactoryArgument.h DataHandler/ProxyFactoryArgument.h

 Description: Templated proxy factory taking an argument as input that is
              used to construct the proxy.


 Usage:
    <usage>

*/
//
// Author:      Alan Magerkurth
// Created:     Mon Jan 15 13:27:12 EST 2001
// $Id: ProxyFactoryArgument.h,v 1.2 2003/01/29 21:02:36 cleo3 Exp $
//
// Revision history
//
// $Log: ProxyFactoryArgument.h,v $
// Revision 1.2  2003/01/29 21:02:36  cleo3
// added missing typename keyword needed for g++
//
// Revision 1.1  2001/01/15 22:45:56  ajm36
// added ProxyFactoryArgument for proxies that have one argument in their constructor
//

// system include files

// user include files
#include "DataHandler/ProxyFactoryBase.h"

// forward declarations

template< class Proxy, class Argument >
class ProxyFactoryArgument : public ProxyFactoryBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      ProxyFactoryArgument(Argument arg):m_arg(arg){}
      virtual ~ProxyFactoryArgument(){}

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      virtual ProxyBase* makeProxy() const {
         return new Proxy(m_arg);
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
      ProxyFactoryArgument( const ProxyFactoryArgument& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ProxyFactoryArgument& operator=( const ProxyFactoryArgument& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Argument m_arg;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DataHandler/Template/ProxyFactoryArgument.cc"
//#endif

#endif /* DATAHANDLER_PROXYFACTORYARGUMENT_H */
