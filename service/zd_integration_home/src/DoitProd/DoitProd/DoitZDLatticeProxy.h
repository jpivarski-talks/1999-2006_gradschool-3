#if !defined(DOITPROD_DOITZDLATTICEPROXY_H)
#define DOITPROD_DOITZDLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <DoitProd>
// Module:      DoitZDLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Adam Lyon
// Created:     Mon Aug 23 16:17:05 EDT 1999
// $Id: DoitZDLatticeProxy.h,v 1.1 2002/09/27 21:04:23 bkh Exp $
//
// Revision history
//
// $Log: DoitZDLatticeProxy.h,v $
// Revision 1.1  2002/09/27 21:04:23  bkh
// For ZD capability
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFinder/SeedZDHitLink.h"

// forward declarations

class DoitZDLatticeProxy : public ProxyBindableTemplate< SeedTrackZDHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef SeedTrackZDHitLattice value_type;

      // Constructors and destructor
      DoitZDLatticeProxy();
      virtual ~DoitZDLatticeProxy();

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
      DoitZDLatticeProxy( const DoitZDLatticeProxy& );

      // assignment operator(s)
      const DoitZDLatticeProxy& operator=( const DoitZDLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DoitZDLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_DoitZDLattice ;

      // static data members

};

// inline function definitions

#endif /* DOITPROD_DOITZDLATTICEPROXY_H */
