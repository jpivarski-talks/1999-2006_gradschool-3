#if !defined(CHISQFITPRODUCER_CHISQFITDRHITLATTICEPROXY_H)
#define CHISQFITPRODUCER_CHISQFITDRHITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqFitDRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 15:35:09 EDT 1999
// $Id: ChisqFitDRHitLatticeProxy.h,v 1.3 1999/07/17 20:28:18 cdj Exp $
//
// Revision history
//
// $Log: ChisqFitDRHitLatticeProxy.h,v $
// Revision 1.3  1999/07/17 20:28:18  cdj
// now compiles when CC optimization is on
//
// Revision 1.2  1999/06/20 22:44:50  wsun
// Added proxies for all ten fit hypotheses.
//
// Revision 1.1.1.1  1999/06/16 02:48:26  wsun
// First submission.
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "TrackFitter/TrackFitDRHitLink.h"

// forward declarations

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class ChisqFitDRHitLatticeProxy :
   public ProxyTemplate< Lattice< TRHelixCandidateFit< H >,
                                  CalibratedDRHit,
				  TrackFitDRHitLink > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef Lattice< TRHelixCandidateFit< H >,
         CalibratedDRHit,
         TrackFitDRHitLink > value_type;

      // Constructors and destructor
      ChisqFitDRHitLatticeProxy();
      virtual ~ChisqFitDRHitLatticeProxy();

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
      ChisqFitDRHitLatticeProxy( const ChisqFitDRHitLatticeProxy& );

      // assignment operator(s)
      const ChisqFitDRHitLatticeProxy& operator=( const ChisqFitDRHitLatticeProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_ChisqFitDRHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ChisqFitProducer/Template/ChisqFitDRHitLatticeProxy.cc"
//#endif

#endif /* CHISQFITPRODUCER_CHISQFITDRHITLATTICEPROXY_H */
