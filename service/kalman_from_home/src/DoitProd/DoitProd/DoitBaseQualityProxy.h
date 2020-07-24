#if !defined(DOITPRODUCER_DOITBASEQUALITYPROXY_H)
#define DOITPRODUCER_DOITBASEQUALITYPROXY_H
// -*- C++ -*-
//
// Package:     <DoitProducer>
// Module:      DoitBaseQualityProxy
// 
// Description: Proxy to "handle" DoitBaseQuality
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Thu Jun  3 01:10:01 EDT 1999
// $Id: DoitBaseQualityProxy.h,v 1.2 2000/08/16 19:45:48 wsun Exp $
//
// Revision history
//
// $Log: DoitBaseQualityProxy.h,v $
// Revision 1.2  2000/08/16 19:45:48  wsun
// Updated to new TRSeedTrackQuality.
//
// Revision 1.1  1999/10/31 16:09:58  lyon
// Need proxy for the base quality information
//
// Revision 1.1.1.1  1999/06/04 07:23:25  wsun
// Imported sources.
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "Doit/DoitSeedQuality.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class DoitBaseQualityProxy :
   public ProxyBindableTemplate< FAPtrTable< TRSeedTrackQuality > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< TRSeedTrackQuality > value_type ;

      // Constructors and destructor
      DoitBaseQualityProxy();
      virtual ~DoitBaseQualityProxy();

      // member functions

      // use this form of a function when calling 'bind'
      // void boundMethod( const Record& iRecord );

      // const member functions

      // static meSoloBaseQualityProxy.hmber functions

   protected:
      // protected member functions
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record& aRecord,
					      const DataKey& aKey );

      // protected const member functions

   private:
      // Constructors and destructor
      DoitBaseQualityProxy( const DoitBaseQualityProxy& );

      // assignment operator(s)
      const DoitBaseQualityProxy& operator=( const DoitBaseQualityProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DoitBaseQualityProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DoitProducer/Template/DoitBaseQualityProxy.cc"
//#endif

#endif /* DOITPRODUCER_DOITBASEQUALITYPROXY_H */
