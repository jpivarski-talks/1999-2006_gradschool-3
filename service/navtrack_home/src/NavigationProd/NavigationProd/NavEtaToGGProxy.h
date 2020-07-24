#if !defined(NAVIGATION_NAVETATOGGPROXY_H)
#define NAVIGATION_NAVETATOGGPROXY_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavEtaToGGProxy
// 
// Description: Proxy to "handle" NavEtaToGG
//
// Usage:
//    <usage>
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:20:44 EDT 1999
// $Id: NavEtaToGGProxy.h,v 1.1 1999/08/09 16:27:33 bkh Exp $
//
// Revision history
//
// $Log: NavEtaToGGProxy.h,v $
// Revision 1.1  1999/08/09 16:27:33  bkh
// Renaming and addition of pi0, eta proxies
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "Navigation/NavEtaToGG.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class NavEtaToGGProxy : public ProxyBindableTemplate< FAPtrTable< NavEtaToGG > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< NavEtaToGG > value_type ;

      // Constructors and destructor
      NavEtaToGGProxy();
      virtual ~NavEtaToGGProxy();

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
      NavEtaToGGProxy( const NavEtaToGGProxy& );

      // assignment operator(s)
      const NavEtaToGGProxy& operator=( const NavEtaToGGProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (NavEtaToGGProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions
#endif /* NAVIGATION_NAVETATOGGPROXY_H */
