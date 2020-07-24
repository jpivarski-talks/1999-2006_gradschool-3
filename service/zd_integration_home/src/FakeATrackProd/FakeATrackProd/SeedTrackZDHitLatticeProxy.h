// -*- C++ -*-
#if !defined(FAKEATRACKPROD_SEEDTRACKZDHITLATTICEPROXY_H)
#define FAKEATRACKPROD_SEEDTRACKZDHITLATTICEPROXY_H
//
// Package:     <FakeATrackProd>
// Module:      SeedTrackZDHitLatticeProxy
//
/**\class SeedTrackZDHitLatticeProxy SeedTrackZDHitLatticeProxy.h FakeATrackProd/SeedTrackZDHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Fri Jul 11 11:46:22 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFinder/SeedZDHitLink.h"

// forward declarations

class SeedTrackZDHitLatticeProxy : public ProxyBindableTemplate< SeedTrackZDHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef SeedTrackZDHitLattice value_type;

      // Constructors and destructor
      SeedTrackZDHitLatticeProxy();
      virtual ~SeedTrackZDHitLatticeProxy();

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
      SeedTrackZDHitLatticeProxy( const SeedTrackZDHitLatticeProxy& );

      // assignment operator(s)
      const SeedTrackZDHitLatticeProxy& operator=( const SeedTrackZDHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (SeedTrackZDHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_SeedTrackZDHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "FakeATrackProd/Template/SeedTrackZDHitLatticeProxy.cc"
//#endif

#endif /* FAKEATRACKPROD_SEEDTRACKZDHITLATTICEPROXY_H */
