// -*- C++ -*-
#if !defined(FAKEATRACKPROD_TRSEEDTRACKPROXY_H)
#define FAKEATRACKPROD_TRSEEDTRACKPROXY_H
//
// Package:     <FakeATrackProd>
// Module:      TRSeedTrackProxy
//
/**\class TRSeedTrackProxy TRSeedTrackProxy.h FakeATrackProd/TRSeedTrackProxy.h
 
 Description: Proxy to "handle" TRSeedTrack

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Fri Jun 27 14:15:41 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackRoot/TRSeedTrack.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class TRSeedTrackProxy : public ProxyBindableTemplate< FAPtrTable< TRSeedTrack > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< TRSeedTrack > value_type ;

      // Constructors and destructor
      TRSeedTrackProxy();
      virtual ~TRSeedTrackProxy();

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
      TRSeedTrackProxy( const TRSeedTrackProxy& );

      // assignment operator(s)
      const TRSeedTrackProxy& operator=( const TRSeedTrackProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (TRSeedTrackProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "FakeATrackProd/Template/TRSeedTrackProxy.cc"
//#endif

#endif /* FAKEATRACKPROD_TRSEEDTRACKPROXY_H */
