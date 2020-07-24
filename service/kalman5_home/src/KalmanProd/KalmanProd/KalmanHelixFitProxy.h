#if !defined(KALMANPROD_KALMANHELIXFITPROXY_H)
#define KALMANPROD_KALMANHELIXFITPROXY_H
// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanHelixFitProxy
// 
// Description: Proxy to "handle" KalmanHelixFit
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon May 10 22:49:11 EDT 1999
// $Id: KalmanHelixFitProxy.h,v 1.1.1.1 1999/09/10 21:58:56 wsun Exp $
//
// Revision history
//
// $Log: KalmanHelixFitProxy.h,v $
// Revision 1.1.1.1  1999/09/10 21:58:56  wsun
// Transfered from KalmanFilter
//
// Revision 1.2  1999/07/15 23:01:29  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:11:06  wsun
// First submission.
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "CleoDB/DBCandidate.h"  // for DBCandidate::Hypo
#include "TrackRoot/TRHelixCandidateFit.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class KalmanHelixFitProxy :
   public ProxyTemplate< FAPtrTable< TRHelixCandidateFit< H > > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< TRHelixCandidateFit< H > > value_type ;

      // Constructors and destructor
      KalmanHelixFitProxy();
      virtual ~KalmanHelixFitProxy();

      // member functions

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
      KalmanHelixFitProxy( const KalmanHelixFitProxy& );

      // assignment operator(s)
      const KalmanHelixFitProxy& operator=( const KalmanHelixFitProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
# include "KalmanProd/Template/KalmanHelixFitProxy.cc"
#endif

#endif /* KALMANPROD_KALMANHELIXFITPROXY_H */
