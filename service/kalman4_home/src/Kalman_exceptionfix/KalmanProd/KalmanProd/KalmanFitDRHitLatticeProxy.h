#if !defined(KALMANPROD_KALMANFITDRHITLATTICEPROXY_H)
#define KALMANPROD_KALMANFITDRHITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanFitDRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon May 10 23:49:19 EDT 1999
// $Id: KalmanFitDRHitLatticeProxy.h,v 1.1.1.1 1999/09/10 21:58:56 wsun Exp $
//
// Revision history
//
// $Log: KalmanFitDRHitLatticeProxy.h,v $
// Revision 1.1.1.1  1999/09/10 21:58:56  wsun
// Transfered from KalmanFilter
//
// Revision 1.2  1999/07/15 23:01:27  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:11:04  wsun
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
class KalmanFitDRHitLatticeProxy :
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
      KalmanFitDRHitLatticeProxy();
      virtual ~KalmanFitDRHitLatticeProxy();

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
      KalmanFitDRHitLatticeProxy( const KalmanFitDRHitLatticeProxy& );

      // assignment operator(s)
      const KalmanFitDRHitLatticeProxy& operator=( const KalmanFitDRHitLatticeProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_KalmanFitDRHitLattice ;

      // static data members

};

// inline function definitions

#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
# include "KalmanProd/Template/KalmanFitDRHitLatticeProxy.cc"
#endif

#endif /* KALMANPROD_KALMANFITDRHITLATTICEPROXY_H */
