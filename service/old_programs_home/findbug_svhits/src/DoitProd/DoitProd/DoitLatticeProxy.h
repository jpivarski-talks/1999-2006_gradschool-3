#if !defined(DOITPROD_DOITLATTICEPROXY_H)
#define DOITPROD_DOITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <DoitProd>
// Module:      DoitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Adam Lyon
// Created:     Mon Aug 23 16:17:05 EDT 1999
// $Id: DoitLatticeProxy.h,v 1.1.1.1 1999/09/08 18:53:32 lyon Exp $
//
// Revision history
//
// $Log: DoitLatticeProxy.h,v $
// Revision 1.1.1.1  1999/09/08 18:53:32  lyon
// imported DoitProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFinder/SeedDRHitLink.h"

// forward declarations

class DoitLatticeProxy : public ProxyBindableTemplate< SeedTrackDRHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef SeedTrackDRHitLattice value_type;

      // Constructors and destructor
      DoitLatticeProxy();
      virtual ~DoitLatticeProxy();

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
      DoitLatticeProxy( const DoitLatticeProxy& );

      // assignment operator(s)
      const DoitLatticeProxy& operator=( const DoitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DoitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_DoitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DoitProd/Template/DoitLatticeProxy.cc"
//#endif

#endif /* DOITPROD_DOITLATTICEPROXY_H */
