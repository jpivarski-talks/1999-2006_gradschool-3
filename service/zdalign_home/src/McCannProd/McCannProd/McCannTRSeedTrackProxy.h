// -*- C++ -*-
#if !defined(MCCANNPROD_MCCANNTRSEEDTRACKPROXY_H)
#define MCCANNPROD_MCCANNTRSEEDTRACKPROXY_H
//
// Package:     <McCannProd>
// Module:      McCannTRSeedTrackProxy
//
/**\class McCannTRSeedTrackProxy McCannTRSeedTrackProxy.h McCannProd/McCannTRSeedTrackProxy.h
 
 Description: Proxy to "handle" McCannTRSeedTrack

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu Oct 23 17:28:09 EDT 2003
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


class McCannTRSeedTrackProxy : public ProxyBindableTemplate< FAPtrTable< TRSeedTrack > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< TRSeedTrack > value_type ;

      // Constructors and destructor
      McCannTRSeedTrackProxy();
      virtual ~McCannTRSeedTrackProxy();

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
      McCannTRSeedTrackProxy( const McCannTRSeedTrackProxy& );

      // assignment operator(s)
      const McCannTRSeedTrackProxy& operator=( const McCannTRSeedTrackProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (McCannTRSeedTrackProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "McCannProd/Template/McCannTRSeedTrackProxy.cc"
//#endif

#endif /* MCCANNPROD_MCCANNTRSEEDTRACKPROXY_H */
