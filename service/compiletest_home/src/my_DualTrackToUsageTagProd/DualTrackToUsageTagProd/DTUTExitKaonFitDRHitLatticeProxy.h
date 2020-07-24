// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_EXITKAONFITDRHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_EXITKAONFITDRHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTExitKaonFitDRHitLatticeProxy
//
/**\class DTUTExitKaonFitDRHitLatticeProxy DTUTExitKaonFitDRHitLatticeProxy.h DualTrackToUsageTagProd/DTUTExitKaonFitDRHitLatticeProxy.h
 
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

class DTUTExitKaonFitDRHitLatticeProxy : public ProxyBindableTemplate< ExitKaonFitDRHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ExitKaonFitDRHitLattice value_type;

      // Constructors and destructor
      DTUTExitKaonFitDRHitLatticeProxy();
      virtual ~DTUTExitKaonFitDRHitLatticeProxy();

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
      DTUTExitKaonFitDRHitLatticeProxy( const DTUTExitKaonFitDRHitLatticeProxy& );

      // assignment operator(s)
      const DTUTExitKaonFitDRHitLatticeProxy& operator=( const DTUTExitKaonFitDRHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTExitKaonFitDRHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ExitKaonFitDRHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTExitKaonFitDRHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_EXITKAONFITDRHITLATTICEPROXY_H */
