#if !defined(CHISQFITPRODUCER_CHISQFITCATHODEHITLATTICEPROXY_H)
#define CHISQFITPRODUCER_CHISQFITCATHODEHITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqFitCathodeHitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Fri Sep 10 19:08:44 EDT 1999
// $Id: ChisqFitCathodeHitLatticeProxy.h,v 1.1 1999/09/11 19:34:31 wsun Exp $
//
// Revision history
//
// $Log: ChisqFitCathodeHitLatticeProxy.h,v $
// Revision 1.1  1999/09/11 19:34:31  wsun
// Added DR cathodes.
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "TrackFitter/TrackFitCathodeHitLink.h"

// forward declarations

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class ChisqFitCathodeHitLatticeProxy :
   public ProxyTemplate< Lattice< TRHelixCandidateFit< H >,
                                  CalibratedCathodeHit,
				  TrackFitCathodeHitLink > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef Lattice< TRHelixCandidateFit< H >,
         CalibratedCathodeHit,
         TrackFitCathodeHitLink > value_type;

      // Constructors and destructor
      ChisqFitCathodeHitLatticeProxy();
      virtual ~ChisqFitCathodeHitLatticeProxy();

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
      ChisqFitCathodeHitLatticeProxy( const ChisqFitCathodeHitLatticeProxy& );

      // assignment operator(s)
      const ChisqFitCathodeHitLatticeProxy& operator=( const ChisqFitCathodeHitLatticeProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_ChisqFitCathodeHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ChisqFitProducer/Template/ChisqFitCathodeHitLatticeProxy.cc"
//#endif

#endif /* CHISQFITPRODUCER_CHISQFITCATHODEHITLATTICEPROXY_H */
