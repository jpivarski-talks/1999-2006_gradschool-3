// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_TRHELIXEXITPROTONFITPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_TRHELIXEXITPROTONFITPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTTRHelixExitProtonFitProxy
//
/**\class DTUTTRHelixExitProtonFitProxy DTUTTRHelixExitProtonFitProxy.h DualTrackToUsageTagProd/DTUTTRHelixExitProtonFitProxy.h
 
 Description: Proxy to "handle" TRHelixExitProtonFit

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:39:50 EST 2003
// $Id: DTUTTRHelixExitProtonFitProxy.h,v 1.1.1.1 2003/12/17 21:00:55 mccann Exp $
//
// Revision history
//
// $Log: DTUTTRHelixExitProtonFitProxy.h,v $
// Revision 1.1.1.1  2003/12/17 21:00:55  mccann
// imported DualTrackToUsageTagProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackRoot/TRHelixExitProtonFit.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class DTUTTRHelixExitProtonFitProxy : public ProxyBindableTemplate< FAPtrTable< TRHelixExitProtonFit > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< TRHelixExitProtonFit > value_type ;

      // Constructors and destructor
      DTUTTRHelixExitProtonFitProxy();
      virtual ~DTUTTRHelixExitProtonFitProxy();

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
      DTUTTRHelixExitProtonFitProxy( const DTUTTRHelixExitProtonFitProxy& );

      // assignment operator(s)
      const DTUTTRHelixExitProtonFitProxy& operator=( const DTUTTRHelixExitProtonFitProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTTRHelixExitProtonFitProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTTRHelixExitProtonFitProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_TRHELIXEXITPROTONFITPROXY_H */
