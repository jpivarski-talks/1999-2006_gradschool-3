// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_EXITPROTONFITDRHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_EXITPROTONFITDRHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTExitProtonFitDRHitLatticeProxy
//
/**\class DTUTExitProtonFitDRHitLatticeProxy DTUTExitProtonFitDRHitLatticeProxy.h DualTrackToUsageTagProd/DTUTExitProtonFitDRHitLatticeProxy.h
 
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

class DTUTExitProtonFitDRHitLatticeProxy : public ProxyBindableTemplate< ExitProtonFitDRHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ExitProtonFitDRHitLattice value_type;

      // Constructors and destructor
      DTUTExitProtonFitDRHitLatticeProxy();
      virtual ~DTUTExitProtonFitDRHitLatticeProxy();

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
      DTUTExitProtonFitDRHitLatticeProxy( const DTUTExitProtonFitDRHitLatticeProxy& );

      // assignment operator(s)
      const DTUTExitProtonFitDRHitLatticeProxy& operator=( const DTUTExitProtonFitDRHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTExitProtonFitDRHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ExitProtonFitDRHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTExitProtonFitDRHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_EXITPROTONFITDRHITLATTICEPROXY_H */
