#if !defined(KALMANPROD_KALMANFITCATHODEHITLATTICEPROXY_H)
#define KALMANPROD_KALMANFITCATHODEHITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <McCannProd>
// Module:      McCannFitCathodeHitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Sep  7 01:01:26 EDT 1999
// $Id: McCannFitCathodeHitLatticeProxy.h,v 1.1.1.1 1999/09/10 21:58:57 wsun Exp $
//
// Revision history
//
// $Log: McCannFitCathodeHitLatticeProxy.h,v $
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from McCannFilter
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
class McCannFitCathodeHitLatticeProxy :
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
      McCannFitCathodeHitLatticeProxy();
      virtual ~McCannFitCathodeHitLatticeProxy();

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
      McCannFitCathodeHitLatticeProxy( const McCannFitCathodeHitLatticeProxy& );

      // assignment operator(s)
      const McCannFitCathodeHitLatticeProxy& operator=( const McCannFitCathodeHitLatticeProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_McCannFitCathodeHitLattice ;

      // static data members

};

// inline function definitions

#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
# include "McCannProd/Template/McCannFitCathodeHitLatticeProxy.cc"
#endif

#endif /* KALMANPROD_KALMANFITCATHODEHITLATTICEPROXY_H */
