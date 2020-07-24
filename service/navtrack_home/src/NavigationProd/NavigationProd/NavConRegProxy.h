#if !defined(NAVIGATIONPROD_NAVCONREGPROXY_H)
#define NAVIGATIONPROD_NAVCONREGPROXY_H
// -*- C++ -*-
//
// Package:     NavigationProd
// Module:      NavConRegProxy
// 
// Description: Proxy to "handle" NavConReg
//
// Usage:
//
// Author:      Brian K. Heltsley
// Created:     Fri Jun 25 12:00:25 EDT 1999
// $Id: NavConRegProxy.h,v 1.1 1999/08/09 16:27:33 bkh Exp $
//
// Revision history
//
// $Log: NavConRegProxy.h,v $
// Revision 1.1  1999/08/09 16:27:33  bkh
// Renaming and addition of pi0, eta proxies
//
// Revision 1.1.1.1  1999/07/19 16:47:45  bkh
// imported NavigationProd sources
//
// Revision 1.1  1999/06/29 21:14:46  bkh
// Add Proxies for high level shower object, connected regions, matches
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "Navigation/NavConReg.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */

class NavConRegServer ;

class NavConRegProxy : public ProxyTemplate< FAPtrTable< NavConReg > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< NavConReg > value_type ;

      // Constructors and destructor
      NavConRegProxy();
      virtual ~NavConRegProxy();

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
      NavConRegProxy( const NavConRegProxy& );

      // assignment operator(s)
      const NavConRegProxy& operator=( const NavConRegProxy& );

      // private member functions

      //this function has already been written for you
//      void bind(
//		void (NavConRegProxy::*iMethod)( const Record& ),
//		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      NavConRegServer* m_server ;

      // static data members
};

// inline function definitions

#endif /* NAVIGATIONPROD_NAVCONREGPROXY_H */
