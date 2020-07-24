#if !defined(KALMANPROD_KALMANFITSVZHITLATTICEPROXY_H)
#define KALMANPROD_KALMANFITSVZHITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <McCannProd>
// Module:      McCannFitSVZHitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon May 10 23:49:19 EDT 1999
// $Id: McCannFitSVZHitLatticeProxy.h,v 1.1.1.1 1999/09/10 21:58:56 wsun Exp $
//
// Revision history
//
// $Log: McCannFitSVZHitLatticeProxy.h,v $
// Revision 1.1.1.1  1999/09/10 21:58:56  wsun
// Transfered from McCannFilter
//
// Revision 1.2  1999/07/15 23:01:29  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:11:05  wsun
// First submission.
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "TrackFitter/TrackFitSVZHitLink.h"

// forward declarations

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class McCannFitSVZHitLatticeProxy :
   public ProxyTemplate< Lattice< TRHelixCandidateFit< H >,
                                 CalibratedSVZHit,
				 TrackFitSVZHitLink > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef Lattice< TRHelixCandidateFit< H >,
	 CalibratedSVZHit,
	 TrackFitSVZHitLink > value_type;

      // Constructors and destructor
      McCannFitSVZHitLatticeProxy();
      virtual ~McCannFitSVZHitLatticeProxy();

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
      McCannFitSVZHitLatticeProxy( const McCannFitSVZHitLatticeProxy& );

      // assignment operator(s)
      const McCannFitSVZHitLatticeProxy& operator=( const McCannFitSVZHitLatticeProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_McCannFitSVZHitLattice ;

      // static data members

};

// inline function definitions

#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
# include "McCannProd/Template/McCannFitSVZHitLatticeProxy.cc"
#endif

#endif /* KALMANPROD_KALMANFITSVZHITLATTICEPROXY_H */
