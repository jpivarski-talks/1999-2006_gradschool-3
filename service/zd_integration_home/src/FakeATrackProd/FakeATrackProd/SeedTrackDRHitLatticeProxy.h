// -*- C++ -*-
#if !defined(FAKEATRACKPROD_SEEDTRACKDRHITLATTICEPROXY_H)
#define FAKEATRACKPROD_SEEDTRACKDRHITLATTICEPROXY_H
//
// Package:     <FakeATrackProd>
// Module:      SeedTrackDRHitLatticeProxy
//
/**\class SeedTrackDRHitLatticeProxy SeedTrackDRHitLatticeProxy.h FakeATrackProd/SeedTrackDRHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Jul  1 16:39:52 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFinder/SeedDRHitLink.h"

// forward declarations

class SeedTrackDRHitLatticeProxy : public ProxyBindableTemplate< SeedTrackDRHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef SeedTrackDRHitLattice value_type;

      // Constructors and destructor
      SeedTrackDRHitLatticeProxy();
      virtual ~SeedTrackDRHitLatticeProxy();

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
      SeedTrackDRHitLatticeProxy( const SeedTrackDRHitLatticeProxy& );

      // assignment operator(s)
      const SeedTrackDRHitLatticeProxy& operator=( const SeedTrackDRHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (SeedTrackDRHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_SeedTrackDRHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "FakeATrackProd/Template/SeedTrackDRHitLatticeProxy.cc"
//#endif

#endif /* FAKEATRACKPROD_SEEDTRACKDRHITLATTICEPROXY_H */
