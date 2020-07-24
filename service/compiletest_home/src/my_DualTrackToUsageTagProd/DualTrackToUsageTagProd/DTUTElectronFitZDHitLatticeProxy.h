// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_ELECTRONFITZDHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_ELECTRONFITZDHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTElectronFitZDHitLatticeProxy
//
/**\class DTUTElectronFitZDHitLatticeProxy DTUTElectronFitZDHitLatticeProxy.h DualTrackToUsageTagProd/DTUTElectronFitZDHitLatticeProxy.h
 
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

class DTUTElectronFitZDHitLatticeProxy : public ProxyBindableTemplate< ElectronFitZDHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ElectronFitZDHitLattice value_type;

      // Constructors and destructor
      DTUTElectronFitZDHitLatticeProxy();
      virtual ~DTUTElectronFitZDHitLatticeProxy();

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
      DTUTElectronFitZDHitLatticeProxy( const DTUTElectronFitZDHitLatticeProxy& );

      // assignment operator(s)
      const DTUTElectronFitZDHitLatticeProxy& operator=( const DTUTElectronFitZDHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTElectronFitZDHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ElectronFitZDHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTElectronFitZDHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_ELECTRONFITZDHITLATTICEPROXY_H */
