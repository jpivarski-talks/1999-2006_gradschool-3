// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_MUONFITZDHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_MUONFITZDHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTMuonFitZDHitLatticeProxy
//
/**\class DTUTMuonFitZDHitLatticeProxy DTUTMuonFitZDHitLatticeProxy.h DualTrackToUsageTagProd/DTUTMuonFitZDHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTMuonFitZDHitLatticeProxy.h,v 1.1.1.1 2003/12/17 21:00:54 mccann Exp $
//
// Revision history
//
// $Log: DTUTMuonFitZDHitLatticeProxy.h,v $
// Revision 1.1.1.1  2003/12/17 21:00:54  mccann
// imported DualTrackToUsageTagProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFitter/TrackFitZDHitLink.h"

// forward declarations

class DTUTMuonFitZDHitLatticeProxy : public ProxyBindableTemplate< MuonFitZDHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef MuonFitZDHitLattice value_type;

      // Constructors and destructor
      DTUTMuonFitZDHitLatticeProxy();
      virtual ~DTUTMuonFitZDHitLatticeProxy();

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
      DTUTMuonFitZDHitLatticeProxy( const DTUTMuonFitZDHitLatticeProxy& );

      // assignment operator(s)
      const DTUTMuonFitZDHitLatticeProxy& operator=( const DTUTMuonFitZDHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTMuonFitZDHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_MuonFitZDHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTMuonFitZDHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_MUONFITZDHITLATTICEPROXY_H */
