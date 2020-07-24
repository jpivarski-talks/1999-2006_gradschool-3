#if !defined(KALMANPROD_KALMANBASEQUALITYPROXY_H)
#define KALMANPROD_KALMANBASEQUALITYPROXY_H
// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanBaseQualityProxy
// 
// Description: Proxy to "handle" TRTrackCandidateFitQuality
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue May 11 01:58:31 EDT 1999
// $Id: KalmanBaseQualityProxy.h,v 1.1.1.1 1999/09/10 21:58:56 wsun Exp $
//
// Revision history
//
// $Log: KalmanBaseQualityProxy.h,v $
// Revision 1.1.1.1  1999/09/10 21:58:56  wsun
// Transfered from KalmanFilter
//
// Revision 1.2  1999/07/15 23:01:26  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:11:00  wsun
// First submission.
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "CleoDB/DBCandidate.h"
#include "TrackRoot/TRTrackCandidateFitQuality.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class KalmanBaseQualityProxy :
   public ProxyTemplate< FAPtrTable<TRTrackCandidateFitQuality< H > > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< TRTrackCandidateFitQuality< H > > value_type ;

      // Constructors and destructor
      KalmanBaseQualityProxy();
      virtual ~KalmanBaseQualityProxy();

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
      KalmanBaseQualityProxy( const KalmanBaseQualityProxy& );

      // assignment operator(s)
      const KalmanBaseQualityProxy& operator=( const KalmanBaseQualityProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
# include "KalmanProd/Template/KalmanBaseQualityProxy.cc"
#endif

#endif /* KALMANPROD_KALMANBASEQUALITYPROXY_H */
