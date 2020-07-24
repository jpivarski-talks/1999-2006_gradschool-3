// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_PIONFITZDHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_PIONFITZDHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTPionFitZDHitLatticeProxy
//
/**\class DTUTPionFitZDHitLatticeProxy DTUTPionFitZDHitLatticeProxy.h DualTrackToUsageTagProd/DTUTPionFitZDHitLatticeProxy.h
 
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

class DTUTPionFitZDHitLatticeProxy : public ProxyBindableTemplate< PionFitZDHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef PionFitZDHitLattice value_type;

      // Constructors and destructor
      DTUTPionFitZDHitLatticeProxy();
      virtual ~DTUTPionFitZDHitLatticeProxy();

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
      DTUTPionFitZDHitLatticeProxy( const DTUTPionFitZDHitLatticeProxy& );

      // assignment operator(s)
      const DTUTPionFitZDHitLatticeProxy& operator=( const DTUTPionFitZDHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTPionFitZDHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_PionFitZDHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTPionFitZDHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_PIONFITZDHITLATTICEPROXY_H */
