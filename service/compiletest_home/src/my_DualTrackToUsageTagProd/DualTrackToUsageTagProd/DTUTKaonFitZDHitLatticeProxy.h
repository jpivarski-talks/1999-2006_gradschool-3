// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_KAONFITZDHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_KAONFITZDHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTKaonFitZDHitLatticeProxy
//
/**\class DTUTKaonFitZDHitLatticeProxy DTUTKaonFitZDHitLatticeProxy.h DualTrackToUsageTagProd/DTUTKaonFitZDHitLatticeProxy.h
 
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

class DTUTKaonFitZDHitLatticeProxy : public ProxyBindableTemplate< KaonFitZDHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef KaonFitZDHitLattice value_type;

      // Constructors and destructor
      DTUTKaonFitZDHitLatticeProxy();
      virtual ~DTUTKaonFitZDHitLatticeProxy();

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
      DTUTKaonFitZDHitLatticeProxy( const DTUTKaonFitZDHitLatticeProxy& );

      // assignment operator(s)
      const DTUTKaonFitZDHitLatticeProxy& operator=( const DTUTKaonFitZDHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTKaonFitZDHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_KaonFitZDHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTKaonFitZDHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_KAONFITZDHITLATTICEPROXY_H */
