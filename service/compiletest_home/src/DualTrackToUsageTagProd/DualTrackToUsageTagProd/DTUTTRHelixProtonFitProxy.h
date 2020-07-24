// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_TRHELIXPROTONFITPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_TRHELIXPROTONFITPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTTRHelixProtonFitProxy
//
/**\class DTUTTRHelixProtonFitProxy DTUTTRHelixProtonFitProxy.h DualTrackToUsageTagProd/DTUTTRHelixProtonFitProxy.h
 
 Description: Proxy to "handle" TRHelixProtonFit

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:39:50 EST 2003
// $Id: DTUTTRHelixProtonFitProxy.h,v 1.1.1.1 2003/12/17 21:00:54 mccann Exp $
//
// Revision history
//
// $Log: DTUTTRHelixProtonFitProxy.h,v $
// Revision 1.1.1.1  2003/12/17 21:00:54  mccann
// imported DualTrackToUsageTagProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackRoot/TRHelixProtonFit.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class DTUTTRHelixProtonFitProxy : public ProxyBindableTemplate< FAPtrTable< TRHelixProtonFit > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< TRHelixProtonFit > value_type ;

      // Constructors and destructor
      DTUTTRHelixProtonFitProxy();
      virtual ~DTUTTRHelixProtonFitProxy();

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
      DTUTTRHelixProtonFitProxy( const DTUTTRHelixProtonFitProxy& );

      // assignment operator(s)
      const DTUTTRHelixProtonFitProxy& operator=( const DTUTTRHelixProtonFitProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTTRHelixProtonFitProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTTRHelixProtonFitProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_TRHELIXPROTONFITPROXY_H */
