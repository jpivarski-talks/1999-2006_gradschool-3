// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_TRHELIXEXITMUONFITPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_TRHELIXEXITMUONFITPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTTRHelixExitMuonFitProxy
//
/**\class DTUTTRHelixExitMuonFitProxy DTUTTRHelixExitMuonFitProxy.h DualTrackToUsageTagProd/DTUTTRHelixExitMuonFitProxy.h
 
 Description: Proxy to "handle" TRHelixExitMuonFit

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:39:50 EST 2003
// $Id: DTUTTRHelixExitMuonFitProxy.h,v 1.1.1.1 2003/12/17 21:00:54 mccann Exp $
//
// Revision history
//
// $Log: DTUTTRHelixExitMuonFitProxy.h,v $
// Revision 1.1.1.1  2003/12/17 21:00:54  mccann
// imported DualTrackToUsageTagProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackRoot/TRHelixExitMuonFit.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class DTUTTRHelixExitMuonFitProxy : public ProxyBindableTemplate< FAPtrTable< TRHelixExitMuonFit > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< TRHelixExitMuonFit > value_type ;

      // Constructors and destructor
      DTUTTRHelixExitMuonFitProxy();
      virtual ~DTUTTRHelixExitMuonFitProxy();

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
      DTUTTRHelixExitMuonFitProxy( const DTUTTRHelixExitMuonFitProxy& );

      // assignment operator(s)
      const DTUTTRHelixExitMuonFitProxy& operator=( const DTUTTRHelixExitMuonFitProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTTRHelixExitMuonFitProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTTRHelixExitMuonFitProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_TRHELIXEXITMUONFITPROXY_H */
