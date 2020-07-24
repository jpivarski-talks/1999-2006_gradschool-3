#if !defined(KALMANPROD_KALMANFITCATHODEHITLATTICEPROXY_H)
#define KALMANPROD_KALMANFITCATHODEHITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <PivarskiProd>
// Module:      PivarskiFitCathodeHitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Sep  7 01:01:26 EDT 1999
// $Id: PivarskiFitCathodeHitLatticeProxy.h,v 1.1.1.1 1999/09/10 21:58:57 wsun Exp $
//
// Revision history
//
// $Log: PivarskiFitCathodeHitLatticeProxy.h,v $
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from PivarskiFilter
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
class PivarskiFitCathodeHitLatticeProxy :
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
      PivarskiFitCathodeHitLatticeProxy();
      virtual ~PivarskiFitCathodeHitLatticeProxy();

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
      PivarskiFitCathodeHitLatticeProxy( const PivarskiFitCathodeHitLatticeProxy& );

      // assignment operator(s)
      const PivarskiFitCathodeHitLatticeProxy& operator=( const PivarskiFitCathodeHitLatticeProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_PivarskiFitCathodeHitLattice ;

      // static data members

};

// inline function definitions

#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
# include "PivarskiProd/Template/PivarskiFitCathodeHitLatticeProxy.cc"
#endif

#endif /* KALMANPROD_KALMANFITCATHODEHITLATTICEPROXY_H */
