#if !defined(DOITPROD_DOITTRSEEDTRACKPROXY_H)
#define DOITPROD_DOITTRSEEDTRACKPROXY_H
// -*- C++ -*-
//
// Package:     <DoitProd>
// Module:      DoitTRSeedTrackProxy
// 
// Description: Proxy to "handle" DoitTRSeedTrack
//
// Usage:
//    <usage>
//
// Author:      Adam Lyon
// Created:     Mon Aug 23 15:44:37 EDT 1999
// $Id: DoitTRSeedTrackProxy.h,v 1.1.1.1 1999/09/08 18:53:32 lyon Exp $
//
// Revision history
//
// $Log: DoitTRSeedTrackProxy.h,v $
// Revision 1.1.1.1  1999/09/08 18:53:32  lyon
// imported DoitProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackRoot/TRSeedTrack.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class DoitTRSeedTrackProxy : 
  public ProxyBindableTemplate< FAPtrTable< TRSeedTrack > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< TRSeedTrack > value_type ;

      // Constructors and destructor
      DoitTRSeedTrackProxy();
      virtual ~DoitTRSeedTrackProxy();

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
      DoitTRSeedTrackProxy( const DoitTRSeedTrackProxy& );

      // assignment operator(s)
      const DoitTRSeedTrackProxy& operator=( const DoitTRSeedTrackProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DoitTRSeedTrackProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DoitProd/Template/DoitTRSeedTrackProxy.cc"
//#endif

#endif /* DOITPROD_DOITTRSEEDTRACKPROXY_H */
