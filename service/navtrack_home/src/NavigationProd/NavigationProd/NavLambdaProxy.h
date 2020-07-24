// -*- C++ -*-
#if !defined(NAVIGATION_NAVLAMBDAPROXY_H)
#define NAVIGATION_NAVLAMBDAPROXY_H
//
// Package:     <Navigation>
// Module:      NavLambdaProxy
//
/**\class NavLambdaProxy NavLambdaProxy.h Navigation/NavLambdaProxy.h
 
 Description: Proxy to "handle" NavLambda

 Usage:
    <usage>

*/
//
// Author:      David Urner
// Created:     Tue Mar 20 14:41:53 EST 2001
// $Id: NavLambdaProxy.h,v 1.1 2001/09/07 23:11:04 eckhart Exp $
//
// Revision history
//
// $Log: NavLambdaProxy.h,v $
// Revision 1.1  2001/09/07 23:11:04  eckhart
// Created File
//
// Revision 1.1  2001/03/26 20:15:17  urner
// added Nav Lambda
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "Navigation/NavLambda.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class NavLambdaProxy : public ProxyBindableTemplate< FAPtrTable< NavLambda > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< NavLambda > value_type ;

      // Constructors and destructor
      NavLambdaProxy();
      virtual ~NavLambdaProxy();

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
      NavLambdaProxy( const NavLambdaProxy& );

      // assignment operator(s)
      const NavLambdaProxy& operator=( const NavLambdaProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (NavLambdaProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "Navigation/Template/NavLambdaProxy.cc"
//#endif

#endif /* NAVIGATION_NAVLAMBDAPROXY_H */
