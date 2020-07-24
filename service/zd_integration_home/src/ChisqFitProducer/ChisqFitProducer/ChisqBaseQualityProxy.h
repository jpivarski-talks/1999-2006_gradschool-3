#if !defined(CHISQFITPRODUCER_CHISQBASEQUALITYPROXY_H)
#define CHISQFITPRODUCER_CHISQBASEQUALITYPROXY_H
// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqBaseQualityProxy
// 
// Description: Proxy to "handle" ChisqBaseQuality
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 16:25:26 EDT 1999
// $Id: ChisqBaseQualityProxy.h,v 1.4 1999/09/10 22:13:08 wsun Exp $
//
// Revision history
//
// $Log: ChisqBaseQualityProxy.h,v $
// Revision 1.4  1999/09/10 22:13:08  wsun
// Moved ChisqCandidateQuality.h to ChisqFitter.
//
// Revision 1.3  1999/07/17 20:28:16  cdj
// now compiles when CC optimization is on
//
// Revision 1.2  1999/06/20 22:44:47  wsun
// Added proxies for all ten fit hypotheses.
//
// Revision 1.1.1.1  1999/06/16 02:48:26  wsun
// First submission.
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "CleoDB/DBCandidate.h"
#include "TrackRoot/TRTrackCandidateFitQuality.h"
#include "ChisqFitter/ChisqCandidateQuality.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class ChisqBaseQualityProxy : public ProxyTemplate<
   FAPtrTable< TRTrackCandidateFitQuality< H > > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable<
         TRTrackCandidateFitQuality< H > > value_type ;

      // Constructors and destructor
      ChisqBaseQualityProxy();
      virtual ~ChisqBaseQualityProxy();

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
      ChisqBaseQualityProxy( const ChisqBaseQualityProxy& );

      // assignment operator(s)
      const ChisqBaseQualityProxy& operator=( const ChisqBaseQualityProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ChisqFitProducer/Template/ChisqBaseQualityProxy.cc"
//#endif

#endif /* CHISQFITPRODUCER_CHISQBASEQUALITYPROXY_H */
