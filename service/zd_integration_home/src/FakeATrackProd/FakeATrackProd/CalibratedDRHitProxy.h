// -*- C++ -*-
#if !defined(FAKEATRACKPROD_CALIBRATEDDRHITPROXY_H)
#define FAKEATRACKPROD_CALIBRATEDDRHITPROXY_H
//
// Package:     <FakeATrackProd>
// Module:      CalibratedDRHitProxy
//
/**\class CalibratedDRHitProxy CalibratedDRHitProxy.h FakeATrackProd/CalibratedDRHitProxy.h
 
 Description: Proxy to "handle" CalibratedDRHit

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Fri Jun 27 14:46:10 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "CalibratedData/CalibratedDRHit.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class CalibratedDRHitProxy : public ProxyBindableTemplate< FAPtrTable< CalibratedDRHit > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< CalibratedDRHit > value_type ;

      // Constructors and destructor
      CalibratedDRHitProxy();
      virtual ~CalibratedDRHitProxy();

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
      CalibratedDRHitProxy( const CalibratedDRHitProxy& );

      // assignment operator(s)
      const CalibratedDRHitProxy& operator=( const CalibratedDRHitProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (CalibratedDRHitProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "FakeATrackProd/Template/CalibratedDRHitProxy.cc"
//#endif

#endif /* FAKEATRACKPROD_CALIBRATEDDRHITPROXY_H */
