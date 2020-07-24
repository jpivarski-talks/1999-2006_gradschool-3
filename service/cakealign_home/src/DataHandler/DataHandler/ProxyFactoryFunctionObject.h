#if !defined(DATAHANDLER_PROXYFACTORYFUNCTIONOBJECT_H)
#define DATAHANDLER_PROXYFACTORYFUNCTIONOBJECT_H
// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      ProxyFactoryFunctionObject
// 
/**\class ProxyFactoryFunctionObject ProxyFactoryFunctionObject.h DataHandler/ProxyFactoryFunctionObject.h

 Description: Templated proxy factory taking a function object as input that is
              used to construct the proxy.


 Usage:
    <usage>

*/
//
// Author:      Alan Magerkurth
// Created:     Mon Jan 15 13:27:12 EST 2001
// $Id: ProxyFactoryFunctionObject.h,v 1.1 2001/07/06 21:37:54 ajm36 Exp $
//
// Revision history
//

// system include files

// user include files
#include "DataHandler/ProxyFactoryBase.h"

// forward declarations

template< class Proxy, class FunctionObject >
class ProxyFactoryFunctionObject : public ProxyFactoryBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      ProxyFactoryFunctionObject(FunctionObject fo):
         m_functionObject(fo){}
      virtual ~ProxyFactoryFunctionObject(){}

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      virtual ProxyBase* makeProxy() const {
         return m_functionObject();
      }
      
      virtual DataKey makeKey( const UsageTag& iUsage,
			       const ProductionTag& iProduction ) const {
         return RecordMethods< Proxy::value_type >::makeKey(
            iUsage, iProduction );
      }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ProxyFactoryFunctionObject( const ProxyFactoryFunctionObject& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ProxyFactoryFunctionObject& operator=( const ProxyFactoryFunctionObject& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      FunctionObject m_functionObject;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DataHandler/Template/ProxyFactoryFunctionObject.cc"
//#endif

#endif /* DATAHANDLER_PROXYFACTORYFUNCTIONOBJECT_H */
