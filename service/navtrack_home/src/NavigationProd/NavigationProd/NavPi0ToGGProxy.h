#if !defined(NAVIGATION_NAVPI0TOGGPROXY_H)
#define NAVIGATION_NAVPI0TOGGPROXY_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavPi0ToGGProxy
// 
// Description: Proxy to "handle" NavPi0ToGG
//
// Usage:
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:20:44 EDT 1999
// $Id: NavPi0ToGGProxy.h,v 1.1 1999/08/09 16:27:34 bkh Exp $
//
// Revision history
//
// $Log: NavPi0ToGGProxy.h,v $
// Revision 1.1  1999/08/09 16:27:34  bkh
// Renaming and addition of pi0, eta proxies
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "Navigation/NavPi0ToGG.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class NavPi0ToGGProxy : public ProxyBindableTemplate< FAPtrTable< NavPi0ToGG > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< NavPi0ToGG > value_type ;

      // Constructors and destructor
      NavPi0ToGGProxy();
      virtual ~NavPi0ToGGProxy();

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
      NavPi0ToGGProxy( const NavPi0ToGGProxy& );

      // assignment operator(s)
      const NavPi0ToGGProxy& operator=( const NavPi0ToGGProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (NavPi0ToGGProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions
#endif /* NAVIGATION_NAVPI0TOGGPROXY_H */
