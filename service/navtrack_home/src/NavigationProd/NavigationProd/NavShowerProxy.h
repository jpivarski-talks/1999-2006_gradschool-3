#if !defined(NAVIGATIONPROD_NAVSHOWERPROXY_H)
#define NAVIGATIONPROD_NAVSHOWERPROXY_H
// -*- C++ -*-
//
// Package:     NavigationProd
// Module:      NavShowerProxy
// 
// Description: Proxy to "handle" NavShower
//
// Usage:
//
// Author:      Brian K. Heltsley
// Created:     Sun Jun 27 19:58:56 EDT 1999
// $Id: NavShowerProxy.h,v 1.1.1.1 1999/07/19 16:47:45 bkh Exp $
//
// Revision history
//
// $Log: NavShowerProxy.h,v $
// Revision 1.1.1.1  1999/07/19 16:47:45  bkh
// imported NavigationProd sources
//
// Revision 1.1  1999/06/29 21:14:47  bkh
// Add Proxies for high level shower object, connected regions, matches
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "Navigation/NavShower.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */

class NavShowerServer ;

class NavShowerProxy : public ProxyTemplate< FAPtrTable< NavShower > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< NavShower > value_type ;

      // Constructors and destructor
      NavShowerProxy();
      virtual ~NavShowerProxy();

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
      NavShowerProxy( const NavShowerProxy& );
      // assignment operator(s)
      const NavShowerProxy& operator=( const NavShowerProxy& );
      // private member functions
      //this function has already been written for you
//      void bind(
//		void (NavShowerProxy::*iMethod)( const Record& ),
//		const Stream::Type& iStream );
      // private const member functions
      // data members
      value_type* m_ptrTable ;

      NavShowerServer* m_server ;
      // static data members
};

// inline function definitions

#endif /* NAVIGATIONPROD_NAVSHOWERPROXY_H */
