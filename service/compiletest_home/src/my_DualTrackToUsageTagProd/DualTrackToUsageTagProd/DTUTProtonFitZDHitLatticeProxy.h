// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_PROTONFITZDHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_PROTONFITZDHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTProtonFitZDHitLatticeProxy
//
/**\class DTUTProtonFitZDHitLatticeProxy DTUTProtonFitZDHitLatticeProxy.h DualTrackToUsageTagProd/DTUTProtonFitZDHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFitter/TrackFitZDHitLink.h"

// forward declarations

class DTUTProtonFitZDHitLatticeProxy : public ProxyBindableTemplate< ProtonFitZDHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ProtonFitZDHitLattice value_type;

      // Constructors and destructor
      DTUTProtonFitZDHitLatticeProxy();
      virtual ~DTUTProtonFitZDHitLatticeProxy();

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
      DTUTProtonFitZDHitLatticeProxy( const DTUTProtonFitZDHitLatticeProxy& );

      // assignment operator(s)
      const DTUTProtonFitZDHitLatticeProxy& operator=( const DTUTProtonFitZDHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTProtonFitZDHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ProtonFitZDHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTProtonFitZDHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_PROTONFITZDHITLATTICEPROXY_H */
