#if !defined(KALMANPROD_KALMANFITSVRHITLATTICEPROXY_H)
#define KALMANPROD_KALMANFITSVRHITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanFitSVRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon May 10 23:49:19 EDT 1999
// $Id: KalmanFitSVRHitLatticeProxy.h,v 1.1.1.1 1999/09/10 21:58:56 wsun Exp $
//
// Revision history
//
// $Log: KalmanFitSVRHitLatticeProxy.h,v $
// Revision 1.1.1.1  1999/09/10 21:58:56  wsun
// Transfered from KalmanFilter
//
// Revision 1.2  1999/07/15 23:01:28  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:11:04  wsun
// First submission.
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "TrackFitter/TrackFitSVRHitLink.h"

// forward declarations

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class KalmanFitSVRHitLatticeProxy :
   public ProxyTemplate< Lattice< TRHelixCandidateFit< H >,
                                 CalibratedSVRphiHit,
				 TrackFitSVRHitLink > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef Lattice< TRHelixCandidateFit< H >,
	 CalibratedSVRphiHit,
	 TrackFitSVRHitLink > value_type;

      // Constructors and destructor
      KalmanFitSVRHitLatticeProxy();
      virtual ~KalmanFitSVRHitLatticeProxy();

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
      KalmanFitSVRHitLatticeProxy( const KalmanFitSVRHitLatticeProxy& );

      // assignment operator(s)
      const KalmanFitSVRHitLatticeProxy& operator=( const KalmanFitSVRHitLatticeProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_KalmanFitSVRHitLattice ;

      // static data members

};

// inline function definitions

#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
# include "KalmanProd/Template/KalmanFitSVRHitLatticeProxy.cc"
#endif

#endif /* KALMANPROD_KALMANFITSVRHITLATTICEPROXY_H */
