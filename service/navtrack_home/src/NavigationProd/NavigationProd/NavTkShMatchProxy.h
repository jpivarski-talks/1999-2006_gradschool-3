#if !defined(NAVIGATIONPROD_NAVTKSHMATCHPROXY_H)
#define NAVIGATIONPROD_NAVTKSHMATCHPROXY_H
// -*- C++ -*-
//
// Package:     NavigationProd
// Module:      NavTkShMatchProxy
// 
// Description: Proxy to "handle" NavTkShMatch
//
// Usage:
//
// Author:      Brian K. Heltsley
// Created:     Tue Jun 22 17:00:54 EDT 1999
// $Id: NavTkShMatchProxy.h,v 1.1 1999/08/09 16:27:35 bkh Exp $
//
// Revision history
//
// $Log: NavTkShMatchProxy.h,v $
// Revision 1.1  1999/08/09 16:27:35  bkh
// Renaming and addition of pi0, eta proxies
//
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
#include "Navigation/NavTkShMatch.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */

class NavTkShMatchServer ;

class NavTkShMatchProxy : public ProxyTemplate< FAPtrTable< NavTkShMatch > >
{
      // friend classes and functions
   public:
      // constants, enums and typedefs
      typedef FAPtrTable< NavTkShMatch > value_type ;

      // Constructors and destructor
      NavTkShMatchProxy();
      virtual ~NavTkShMatchProxy();

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
      NavTkShMatchProxy( const NavTkShMatchProxy& );
      // assignment operator(s)
      const NavTkShMatchProxy& operator=( const NavTkShMatchProxy& );
      // private member functions

      //this function has already been written for you
//      void bind(
//	 void (NavTkShMatchProxy::*iMethod)( const Record& ),
//	 const Stream::Type& iStream );
      // private const member functions

      // data members
      value_type* m_ptrTable ;

      NavTkShMatchServer* m_server ;

      // static data members
};

// inline function definitions

#endif /* NAVIGATIONPROD_NAVTKSHMATCHPROXY_H */
