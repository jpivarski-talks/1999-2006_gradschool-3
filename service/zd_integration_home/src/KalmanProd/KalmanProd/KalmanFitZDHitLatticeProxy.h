#if !defined(KALMANPROD_KALMANFITZDHITLATTICEPROXY_H)
#define KALMANPROD_KALMANFITZDHITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanFitZDHitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon May 10 23:49:19 EDT 1999
// $Id: KalmanFitZDHitLatticeProxy.h,v 1.1 2003/04/17 17:05:10 bkh Exp $
//
// Revision history
//
// $Log: KalmanFitZDHitLatticeProxy.h,v $
// Revision 1.1  2003/04/17 17:05:10  bkh
// Add ZD Lattice Proxy & throw exceptions if wrong config
//
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
#include "TrackFitter/TrackFitZDHitLink.h"

// forward declarations

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class KalmanFitZDHitLatticeProxy :
   public ProxyTemplate< Lattice< TRHelixCandidateFit< H >,
                                 CalibratedZDHit,
				 TrackFitZDHitLink > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef Lattice< TRHelixCandidateFit< H >,
	 CalibratedZDHit,
	 TrackFitZDHitLink > value_type;

      // Constructors and destructor
      KalmanFitZDHitLatticeProxy();
      virtual ~KalmanFitZDHitLatticeProxy();

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
      KalmanFitZDHitLatticeProxy( const KalmanFitZDHitLatticeProxy& );

      // assignment operator(s)
      const KalmanFitZDHitLatticeProxy& operator=( const KalmanFitZDHitLatticeProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_KalmanFitZDHitLattice ;

      // static data members

};

// inline function definitions

#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
# include "KalmanProd/Template/KalmanFitZDHitLatticeProxy.cc"
#endif

#endif /* KALMANPROD_KALMANFITZDHITLATTICEPROXY_H */
