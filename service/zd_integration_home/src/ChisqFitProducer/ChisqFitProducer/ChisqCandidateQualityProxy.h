#if !defined(CHISQFITPRODUCER_CHISQCANDIDATEQUALITYPROXY_H)
#define CHISQFITPRODUCER_CHISQCANDIDATEQUALITYPROXY_H
// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqCandidateQualityProxy
// 
// Description: Proxy to "handle" ChisqCandidateQuality
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 16:17:22 EDT 1999
// $Id: ChisqCandidateQualityProxy.h,v 1.4 1999/09/10 22:13:07 wsun Exp $
//
// Revision history
//
// $Log: ChisqCandidateQualityProxy.h,v $
// Revision 1.4  1999/09/10 22:13:07  wsun
// Moved ChisqCandidateQuality.h to ChisqFitter.
//
// Revision 1.3  1999/07/17 20:28:17  cdj
// now compiles when CC optimization is on
//
// Revision 1.2  1999/06/20 22:44:49  wsun
// Added proxies for all ten fit hypotheses.
//
// Revision 1.1.1.1  1999/06/16 02:48:26  wsun
// First submission.
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
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
class ChisqCandidateQualityProxy :
   public ProxyTemplate< FAPtrTable< ChisqCandidateQuality< H > > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< ChisqCandidateQuality< H > > value_type ;

      // Constructors and destructor
      ChisqCandidateQualityProxy();
      virtual ~ChisqCandidateQualityProxy();

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
      ChisqCandidateQualityProxy( const ChisqCandidateQualityProxy& );

      // assignment operator(s)
      const ChisqCandidateQualityProxy& operator=( const ChisqCandidateQualityProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ChisqFitProducer/Template/ChisqCandidateQualityProxy.cc"
//#endif

#endif /* CHISQFITPRODUCER_CHISQCANDIDATEQUALITYPROXY_H */
