#if !defined(KALMANPROD_KALMANCANDIDATEQUALITYPROXY_H)
#define KALMANPROD_KALMANCANDIDATEQUALITYPROXY_H
// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanCandidateQualityProxy
// 
// Description: Proxy to "handle" KalmanCandidateQuality
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue May 11 01:58:31 EDT 1999
// $Id: KalmanCandidateQualityProxy.h,v 1.1.1.1 1999/09/10 21:58:56 wsun Exp $
//
// Revision history
//
// $Log: KalmanCandidateQualityProxy.h,v $
// Revision 1.1.1.1  1999/09/10 21:58:56  wsun
// Transfered from KalmanFilter
//
// Revision 1.2  1999/07/15 23:01:27  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:11:02  wsun
// First submission.
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "KalmanFilter/KalmanCandidateQuality.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class KalmanCandidateQualityProxy :
   public ProxyTemplate< FAPtrTable< KalmanCandidateQuality< H > > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< KalmanCandidateQuality< H > > value_type ;

      // Constructors and destructor
      KalmanCandidateQualityProxy();
      virtual ~KalmanCandidateQualityProxy();

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
      KalmanCandidateQualityProxy( const KalmanCandidateQualityProxy& );

      // assignment operator(s)
      const KalmanCandidateQualityProxy& operator=( const KalmanCandidateQualityProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
# include "KalmanProd/Template/KalmanCandidateQualityProxy.cc"
#endif

#endif /* KALMANPROD_KALMANCANDIDATEQUALITYPROXY_H */
