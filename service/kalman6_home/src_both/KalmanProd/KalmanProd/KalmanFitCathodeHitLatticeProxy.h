#if !defined(KALMANPROD_KALMANFITCATHODEHITLATTICEPROXY_H)
#define KALMANPROD_KALMANFITCATHODEHITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanFitCathodeHitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Sep  7 01:01:26 EDT 1999
// $Id: KalmanFitCathodeHitLatticeProxy.h,v 1.1.1.1 1999/09/10 21:58:57 wsun Exp $
//
// Revision history
//
// $Log: KalmanFitCathodeHitLatticeProxy.h,v $
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from KalmanFilter
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
class KalmanFitCathodeHitLatticeProxy :
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
      KalmanFitCathodeHitLatticeProxy();
      virtual ~KalmanFitCathodeHitLatticeProxy();

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
      KalmanFitCathodeHitLatticeProxy( const KalmanFitCathodeHitLatticeProxy& );

      // assignment operator(s)
      const KalmanFitCathodeHitLatticeProxy& operator=( const KalmanFitCathodeHitLatticeProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_KalmanFitCathodeHitLattice ;

      // static data members

};

// inline function definitions

#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
# include "KalmanProd/Template/KalmanFitCathodeHitLatticeProxy.cc"
#endif

#endif /* KALMANPROD_KALMANFITCATHODEHITLATTICEPROXY_H */
