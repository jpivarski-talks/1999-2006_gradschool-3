#if !defined(DOITPROD_DOITCATHODELATTICEPROXY_H)
#define DOITPROD_DOITCATHODELATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <DoitProd>
// Module:      DoitCathodeLatticeProxy
//
/**\class DoitCathodeLatticeProxy DoitCathodeLatticeProxy.h DoitProd/DoitCathodeLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Adam Lyon
// Created:     Mon May 22 11:43:28 EDT 2000
// $Id: DoitCathodeLatticeProxy.h,v 1.1 2000/05/27 05:31:30 lyon Exp $
//
// Revision history
//
// $Log: DoitCathodeLatticeProxy.h,v $
// Revision 1.1  2000/05/27 05:31:30  lyon
// Handle cathode clusters on tracks
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFinder/SeedTrackCathodeHitLattice.h"

// forward declarations

class DoitCathodeLatticeProxy : public ProxyBindableTemplate< 
                                                SeedTrackCathodeHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef SeedTrackCathodeHitLattice value_type;

      // Constructors and destructor
      DoitCathodeLatticeProxy();
      virtual ~DoitCathodeLatticeProxy();

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
      DoitCathodeLatticeProxy( const DoitCathodeLatticeProxy& );

      // assignment operator(s)
      const DoitCathodeLatticeProxy& operator=( const DoitCathodeLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DoitCathodeLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_DoitCathodeLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DoitProd/Template/DoitCathodeLatticeProxy.cc"
//#endif

#endif /* DOITPROD_DOITCATHODELATTICEPROXY_H */
