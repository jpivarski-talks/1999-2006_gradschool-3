// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_EXITMUONFITDRHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_EXITMUONFITDRHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTExitMuonFitDRHitLatticeProxy
//
/**\class DTUTExitMuonFitDRHitLatticeProxy DTUTExitMuonFitDRHitLatticeProxy.h DualTrackToUsageTagProd/DTUTExitMuonFitDRHitLatticeProxy.h
 
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

class DTUTExitMuonFitDRHitLatticeProxy : public ProxyBindableTemplate< ExitMuonFitDRHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ExitMuonFitDRHitLattice value_type;

      // Constructors and destructor
      DTUTExitMuonFitDRHitLatticeProxy();
      virtual ~DTUTExitMuonFitDRHitLatticeProxy();

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
      DTUTExitMuonFitDRHitLatticeProxy( const DTUTExitMuonFitDRHitLatticeProxy& );

      // assignment operator(s)
      const DTUTExitMuonFitDRHitLatticeProxy& operator=( const DTUTExitMuonFitDRHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTExitMuonFitDRHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ExitMuonFitDRHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTExitMuonFitDRHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_EXITMUONFITDRHITLATTICEPROXY_H */
