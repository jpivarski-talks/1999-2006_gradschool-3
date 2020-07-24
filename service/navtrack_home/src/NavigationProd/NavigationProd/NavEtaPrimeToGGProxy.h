#if !defined(NAVIGATION_NAVETAPRIMETOGGPROXY_H)
#define NAVIGATION_NAVETAPRIMETOGGPROXY_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavEtaPrimeToGGProxy
// 
// Description: Proxy to "handle" NavEtaPrimeToGG
//
// Usage:
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:20:44 EDT 1999
// $Id: NavEtaPrimeToGGProxy.h,v 1.1 2001/05/10 19:56:46 bkh Exp $
//
// Revision history
//
// $Log: NavEtaPrimeToGGProxy.h,v $
// Revision 1.1  2001/05/10 19:56:46  bkh
// Addition of etaPrime classes
//
//
//
// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "Navigation/NavEtaPrimeToGG.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class NavEtaPrimeToGGProxy : public ProxyBindableTemplate< FAPtrTable< NavEtaPrimeToGG > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< NavEtaPrimeToGG > value_type ;

      // Constructors and destructor
      NavEtaPrimeToGGProxy();
      virtual ~NavEtaPrimeToGGProxy();

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
      NavEtaPrimeToGGProxy( const NavEtaPrimeToGGProxy& );

      // assignment operator(s)
      const NavEtaPrimeToGGProxy& operator=( const NavEtaPrimeToGGProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (NavEtaPrimeToGGProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions
#endif /* NAVIGATION_NAVETAPRIMETOGGPROXY_H */
