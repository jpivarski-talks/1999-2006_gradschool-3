// -*- C++ -*-
#if !defined(NAVIGATION_NAVKSPROXY_H)
#define NAVIGATION_NAVKSPROXY_H
//
// Package:     <Navigation>
// Module:      NavKsProxy
//
/**\class NavKsProxy NavKsProxy.h Navigation/NavKsProxy.h
 
 Description: Proxy to "handle" NavKs

 Usage:
    <usage>

*/
//
// Author:      David Urner
// Created:     Tue Mar 20 14:41:53 EST 2001
// $Id: NavKsProxy.h,v 1.1 2001/03/26 20:15:17 urner Exp $
//
// Revision history
//
// $Log: NavKsProxy.h,v $
// Revision 1.1  2001/03/26 20:15:17  urner
// added Nav Ks
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "Navigation/NavKs.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class NavKsProxy : public ProxyBindableTemplate< FAPtrTable< NavKs > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< NavKs > value_type ;

      // Constructors and destructor
      NavKsProxy();
      virtual ~NavKsProxy();

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
      NavKsProxy( const NavKsProxy& );

      // assignment operator(s)
      const NavKsProxy& operator=( const NavKsProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (NavKsProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "Navigation/Template/NavKsProxy.cc"
//#endif

#endif /* NAVIGATION_NAVKSPROXY_H */
