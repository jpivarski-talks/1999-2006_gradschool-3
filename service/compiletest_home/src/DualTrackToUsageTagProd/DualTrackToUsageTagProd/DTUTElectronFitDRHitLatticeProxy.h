// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_ELECTRONFITDRHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_ELECTRONFITDRHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTElectronFitDRHitLatticeProxy
//
/**\class DTUTElectronFitDRHitLatticeProxy DTUTElectronFitDRHitLatticeProxy.h DualTrackToUsageTagProd/DTUTElectronFitDRHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTElectronFitDRHitLatticeProxy.h,v 1.1.1.1 2003/12/17 21:00:54 mccann Exp $
//
// Revision history
//
// $Log: DTUTElectronFitDRHitLatticeProxy.h,v $
// Revision 1.1.1.1  2003/12/17 21:00:54  mccann
// imported DualTrackToUsageTagProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFitter/TrackFitDRHitLink.h"

// forward declarations

class DTUTElectronFitDRHitLatticeProxy : public ProxyBindableTemplate< ElectronFitDRHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ElectronFitDRHitLattice value_type;

      // Constructors and destructor
      DTUTElectronFitDRHitLatticeProxy();
      virtual ~DTUTElectronFitDRHitLatticeProxy();

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
      DTUTElectronFitDRHitLatticeProxy( const DTUTElectronFitDRHitLatticeProxy& );

      // assignment operator(s)
      const DTUTElectronFitDRHitLatticeProxy& operator=( const DTUTElectronFitDRHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTElectronFitDRHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ElectronFitDRHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTElectronFitDRHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_ELECTRONFITDRHITLATTICEPROXY_H */
