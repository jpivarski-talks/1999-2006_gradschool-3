// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_TRHELIXMUONFITPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_TRHELIXMUONFITPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTTRHelixMuonFitProxy
//
/**\class DTUTTRHelixMuonFitProxy DTUTTRHelixMuonFitProxy.h DualTrackToUsageTagProd/DTUTTRHelixMuonFitProxy.h
 
 Description: Proxy to "handle" TRHelixMuonFit

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
#include "TrackRoot/TRHelixMuonFit.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class DTUTTRHelixMuonFitProxy : public ProxyBindableTemplate< FAPtrTable< TRHelixMuonFit > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< TRHelixMuonFit > value_type ;

      // Constructors and destructor
      DTUTTRHelixMuonFitProxy();
      virtual ~DTUTTRHelixMuonFitProxy();

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
      DTUTTRHelixMuonFitProxy( const DTUTTRHelixMuonFitProxy& );

      // assignment operator(s)
      const DTUTTRHelixMuonFitProxy& operator=( const DTUTTRHelixMuonFitProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTTRHelixMuonFitProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTTRHelixMuonFitProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_TRHELIXMUONFITPROXY_H */
