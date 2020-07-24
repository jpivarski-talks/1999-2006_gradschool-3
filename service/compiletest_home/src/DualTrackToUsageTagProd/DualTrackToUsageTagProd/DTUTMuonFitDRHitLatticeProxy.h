// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_MUONFITDRHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_MUONFITDRHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTMuonFitDRHitLatticeProxy
//
/**\class DTUTMuonFitDRHitLatticeProxy DTUTMuonFitDRHitLatticeProxy.h DualTrackToUsageTagProd/DTUTMuonFitDRHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTMuonFitDRHitLatticeProxy.h,v 1.1.1.1 2003/12/17 21:00:54 mccann Exp $
//
// Revision history
//
// $Log: DTUTMuonFitDRHitLatticeProxy.h,v $
// Revision 1.1.1.1  2003/12/17 21:00:54  mccann
// imported DualTrackToUsageTagProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFitter/TrackFitDRHitLink.h"

// forward declarations

class DTUTMuonFitDRHitLatticeProxy : public ProxyBindableTemplate< MuonFitDRHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef MuonFitDRHitLattice value_type;

      // Constructors and destructor
      DTUTMuonFitDRHitLatticeProxy();
      virtual ~DTUTMuonFitDRHitLatticeProxy();

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
      DTUTMuonFitDRHitLatticeProxy( const DTUTMuonFitDRHitLatticeProxy& );

      // assignment operator(s)
      const DTUTMuonFitDRHitLatticeProxy& operator=( const DTUTMuonFitDRHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTMuonFitDRHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_MuonFitDRHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTMuonFitDRHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_MUONFITDRHITLATTICEPROXY_H */
