// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_KAONFITDRHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_KAONFITDRHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTKaonFitDRHitLatticeProxy
//
/**\class DTUTKaonFitDRHitLatticeProxy DTUTKaonFitDRHitLatticeProxy.h DualTrackToUsageTagProd/DTUTKaonFitDRHitLatticeProxy.h
 
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
#include "TrackFitter/TrackFitDRHitLink.h"

// forward declarations

class DTUTKaonFitDRHitLatticeProxy : public ProxyBindableTemplate< KaonFitDRHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef KaonFitDRHitLattice value_type;

      // Constructors and destructor
      DTUTKaonFitDRHitLatticeProxy();
      virtual ~DTUTKaonFitDRHitLatticeProxy();

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
      DTUTKaonFitDRHitLatticeProxy( const DTUTKaonFitDRHitLatticeProxy& );

      // assignment operator(s)
      const DTUTKaonFitDRHitLatticeProxy& operator=( const DTUTKaonFitDRHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTKaonFitDRHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_KaonFitDRHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTKaonFitDRHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_KAONFITDRHITLATTICEPROXY_H */
