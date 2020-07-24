#if !defined(NAVIGATIONPROD_NAVRICHPROXY_H)
#define NAVIGATIONPROD_NAVRICHPROXY_H
// -*- C++ -*-
//
// Package:     NavigationProd
// Module:      NavRichProxy
// 
// Description: Proxy to "handle" NavRich
//
// Usage:
//
// Author:      Tomasz Skwarnicki
// Created:     Sun Jul 23 05:46:23 EDT 2000
// $Id: NavRichProxy.h,v 1.1 2000/07/27 01:35:22 ts Exp $
//
// Revision history
//
// $Log: NavRichProxy.h,v $
// Revision 1.1  2000/07/27 01:35:22  ts
// NavRich added
//
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "Navigation/NavRich.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */

class NavRichServer ;

class NavRichProxy : public ProxyTemplate< FAPtrTable< NavRich > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< NavRich > value_type ;

      // Constructors and destructor
      NavRichProxy();
      virtual ~NavRichProxy();

      // member functions

      // use this form of a function when calling 'bind'
      // void boundMethod( const Record& iRecord );

      // const member functions

      // static member functions

   protected:
      // protected member functions
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record&  aRecord ,
					      const DataKey& aKey      ) ;

      // protected const member functions
   private:
      // Constructors and destructor
      NavRichProxy( const NavRichProxy& );
      // assignment operator(s)
      const NavRichProxy& operator=( const NavRichProxy& );
      // private member functions
      //this function has already been written for you
//      void bind(
//		void (NavRichProxy::*iMethod)( const Record& ),
//		const Stream::Type& iStream );
      // private const member functions
      // data members
      value_type* m_ptrTable ;

      NavRichServer* m_server ;
      // static data members
};

// inline function definitions

#endif /* NAVIGATIONPROD_NAVRICHPROXY_H */
