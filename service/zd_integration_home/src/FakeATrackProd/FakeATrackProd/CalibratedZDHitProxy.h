// -*- C++ -*-
#if !defined(FAKEATRACKPROD_CALIBRATEDZDHITPROXY_H)
#define FAKEATRACKPROD_CALIBRATEDZDHITPROXY_H
//
// Package:     <FakeATrackProd>
// Module:      CalibratedZDHitProxy
//
/**\class CalibratedZDHitProxy CalibratedZDHitProxy.h FakeATrackProd/CalibratedZDHitProxy.h
 
 Description: Proxy to "handle" CalibratedZDHit

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Fri Jul 11 10:23:35 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "CalibratedData/CalibratedZDHit.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class CalibratedZDHitProxy : public ProxyBindableTemplate< FAPtrTable< CalibratedZDHit > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< CalibratedZDHit > value_type ;

      // Constructors and destructor
      CalibratedZDHitProxy();
      virtual ~CalibratedZDHitProxy();

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
      CalibratedZDHitProxy( const CalibratedZDHitProxy& );

      // assignment operator(s)
      const CalibratedZDHitProxy& operator=( const CalibratedZDHitProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (CalibratedZDHitProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "FakeATrackProd/Template/CalibratedZDHitProxy.cc"
//#endif

#endif /* FAKEATRACKPROD_CALIBRATEDZDHITPROXY_H */
