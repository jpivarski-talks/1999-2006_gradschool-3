// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_TRHELIXELECTRONFITPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_TRHELIXELECTRONFITPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTTRHelixElectronFitProxy
//
/**\class DTUTTRHelixElectronFitProxy DTUTTRHelixElectronFitProxy.h DualTrackToUsageTagProd/DTUTTRHelixElectronFitProxy.h
 
 Description: Proxy to "handle" TRHelixElectronFit

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
#include "TrackRoot/TRHelixElectronFit.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class DTUTTRHelixElectronFitProxy : public ProxyBindableTemplate< FAPtrTable< TRHelixElectronFit > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< TRHelixElectronFit > value_type ;

      // Constructors and destructor
      DTUTTRHelixElectronFitProxy();
      virtual ~DTUTTRHelixElectronFitProxy();

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
      DTUTTRHelixElectronFitProxy( const DTUTTRHelixElectronFitProxy& );

      // assignment operator(s)
      const DTUTTRHelixElectronFitProxy& operator=( const DTUTTRHelixElectronFitProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTTRHelixElectronFitProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTTRHelixElectronFitProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_TRHELIXELECTRONFITPROXY_H */
