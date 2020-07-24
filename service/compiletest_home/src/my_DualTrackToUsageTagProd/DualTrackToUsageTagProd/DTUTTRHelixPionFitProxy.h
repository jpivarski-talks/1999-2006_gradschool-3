// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_TRHELIXPIONFITPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_TRHELIXPIONFITPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTTRHelixPionFitProxy
//
/**\class DTUTTRHelixPionFitProxy DTUTTRHelixPionFitProxy.h DualTrackToUsageTagProd/DTUTTRHelixPionFitProxy.h
 
 Description: Proxy to "handle" TRHelixPionFit

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:39:50 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackRoot/TRHelixPionFit.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class DTUTTRHelixPionFitProxy : public ProxyBindableTemplate< FAPtrTable< TRHelixPionFit > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< TRHelixPionFit > value_type ;

      // Constructors and destructor
      DTUTTRHelixPionFitProxy();
      virtual ~DTUTTRHelixPionFitProxy();

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
      DTUTTRHelixPionFitProxy( const DTUTTRHelixPionFitProxy& );

      // assignment operator(s)
      const DTUTTRHelixPionFitProxy& operator=( const DTUTTRHelixPionFitProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTTRHelixPionFitProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTTRHelixPionFitProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_TRHELIXPIONFITPROXY_H */
