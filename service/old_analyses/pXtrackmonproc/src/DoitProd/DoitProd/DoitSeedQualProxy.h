#if !defined(DOITPROD_DOITTRSEEDTRACKPROXY_H)
#define DOITPROD_DOITTRSEEDTRACKPROXY_H
// -*- C++ -*-
//
// Package:     <DoitProd>
// Module:      DoitSeedQualityProxy
// 
// Description: Proxy to "handle" DoitSeedQuality
//
// Usage:
//    <usage>
//
// Author:      Adam Lyon
// Created:     Mon Aug 23 15:44:37 EDT 1999
// $Id: DoitSeedQualProxy.h,v 1.1.1.1 1999/09/08 18:53:32 lyon Exp $
//
// Revision history
//
// $Log: DoitSeedQualProxy.h,v $
// Revision 1.1.1.1  1999/09/08 18:53:32  lyon
// imported DoitProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "Doit/DoitSeedQuality.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class DoitSeedQualityProxy : 
  public ProxyBindableTemplate< FAPtrTable< DoitSeedQuality > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< DoitSeedQuality > value_type ;

      // Constructors and destructor
      DoitSeedQualityProxy();
      virtual ~DoitSeedQualityProxy();

      // member functions

      // use this form of a function when calling 'bind'
      // void boundMethod( const Record& iRecord );

      // const member functions

      // static member functions

   protected:
      // protected member functions
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record& aRecord,
					      const DataKey& aKey );

      // protected const member functions

   private:
      // Constructors and destructor
      DoitSeedQualityProxy( const DoitSeedQualityProxy& );

      // assignment operator(s)
      const DoitSeedQualityProxy& operator=( const DoitSeedQualityProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DoitSeedQualityProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DoitProd/Template/DoitSeedQualityProxy.cc"
//#endif

#endif /* DOITPROD_DOITTRSEEDTRACKPROXY_H */
