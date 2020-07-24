#if !defined(KALMANPROD_KALMANFITDRHITLATTICEPROXY_H)
#define KALMANPROD_KALMANFITDRHITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <McCannProd>
// Module:      McCannFitDRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon May 10 23:49:19 EDT 1999
// $Id: McCannFitDRHitLatticeProxy.h,v 1.1.1.1 1999/09/10 21:58:56 wsun Exp $
//
// Revision history
//
// $Log: McCannFitDRHitLatticeProxy.h,v $
// Revision 1.1.1.1  1999/09/10 21:58:56  wsun
// Transfered from McCannFilter
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
class McCannFitDRHitLatticeProxy :
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
      McCannFitDRHitLatticeProxy();
      virtual ~McCannFitDRHitLatticeProxy();

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
      McCannFitDRHitLatticeProxy( const McCannFitDRHitLatticeProxy& );

      // assignment operator(s)
      const McCannFitDRHitLatticeProxy& operator=( const McCannFitDRHitLatticeProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_McCannFitDRHitLattice ;

      // static data members

};

// inline function definitions

#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
# include "McCannProd/Template/McCannFitDRHitLatticeProxy.cc"
#endif

#endif /* KALMANPROD_KALMANFITDRHITLATTICEPROXY_H */
