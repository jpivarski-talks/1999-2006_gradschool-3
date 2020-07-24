#if !defined(CHISQFITPRODUCER_CHISQHELIXFITPROXY_H)
#define CHISQFITPRODUCER_CHISQHELIXFITPROXY_H
// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqHelixFitProxy
// 
// Description: Proxy to "handle" ChisqHelixFit
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 15:15:50 EDT 1999
// $Id: ChisqHelixFitProxy.h,v 1.3 1999/07/17 20:28:22 cdj Exp $
//
// Revision history
//
// $Log: ChisqHelixFitProxy.h,v $
// Revision 1.3  1999/07/17 20:28:22  cdj
// now compiles when CC optimization is on
//
// Revision 1.2  1999/06/20 22:44:53  wsun
// Added proxies for all ten fit hypotheses.
//
// Revision 1.1.1.1  1999/06/16 02:48:26  wsun
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
class ChisqHelixFitProxy :
   public ProxyTemplate< FAPtrTable< TRHelixCandidateFit< H > > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< TRHelixCandidateFit< H > > value_type ;

      // Constructors and destructor
      ChisqHelixFitProxy();
      virtual ~ChisqHelixFitProxy();

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
      ChisqHelixFitProxy( const ChisqHelixFitProxy& );

      // assignment operator(s)
      const ChisqHelixFitProxy& operator=( const ChisqHelixFitProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ChisqFitProducer/Template/ChisqHelixFitProxy.cc"
//#endif

#endif /* CHISQFITPRODUCER_CHISQHELIXFITPROXY_H */
