// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_EXITMUONFITZDHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_EXITMUONFITZDHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTExitMuonFitZDHitLatticeProxy
//
/**\class DTUTExitMuonFitZDHitLatticeProxy DTUTExitMuonFitZDHitLatticeProxy.h DualTrackToUsageTagProd/DTUTExitMuonFitZDHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTExitMuonFitZDHitLatticeProxy.h,v 1.1.1.1 2003/12/17 21:00:55 mccann Exp $
//
// Revision history
//
// $Log: DTUTExitMuonFitZDHitLatticeProxy.h,v $
// Revision 1.1.1.1  2003/12/17 21:00:55  mccann
// imported DualTrackToUsageTagProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFitter/TrackFitZDHitLink.h"

// forward declarations

class DTUTExitMuonFitZDHitLatticeProxy : public ProxyBindableTemplate< ExitMuonFitZDHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ExitMuonFitZDHitLattice value_type;

      // Constructors and destructor
      DTUTExitMuonFitZDHitLatticeProxy();
      virtual ~DTUTExitMuonFitZDHitLatticeProxy();

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
      DTUTExitMuonFitZDHitLatticeProxy( const DTUTExitMuonFitZDHitLatticeProxy& );

      // assignment operator(s)
      const DTUTExitMuonFitZDHitLatticeProxy& operator=( const DTUTExitMuonFitZDHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTExitMuonFitZDHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ExitMuonFitZDHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTExitMuonFitZDHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_EXITMUONFITZDHITLATTICEPROXY_H */
