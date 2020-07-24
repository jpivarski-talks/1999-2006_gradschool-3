#if !defined(KALMANPROD_KALMANFITSVRHITLATTICEPROXY_H)
#define KALMANPROD_KALMANFITSVRHITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <McCannProd>
// Module:      McCannFitSVRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon May 10 23:49:19 EDT 1999
// $Id: McCannFitSVRHitLatticeProxy.h,v 1.1.1.1 1999/09/10 21:58:56 wsun Exp $
//
// Revision history
//
// $Log: McCannFitSVRHitLatticeProxy.h,v $
// Revision 1.1.1.1  1999/09/10 21:58:56  wsun
// Transfered from McCannFilter
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
class McCannFitSVRHitLatticeProxy :
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
      McCannFitSVRHitLatticeProxy();
      virtual ~McCannFitSVRHitLatticeProxy();

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
      McCannFitSVRHitLatticeProxy( const McCannFitSVRHitLatticeProxy& );

      // assignment operator(s)
      const McCannFitSVRHitLatticeProxy& operator=( const McCannFitSVRHitLatticeProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_McCannFitSVRHitLattice ;

      // static data members

};

// inline function definitions

#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
# include "McCannProd/Template/McCannFitSVRHitLatticeProxy.cc"
#endif

#endif /* KALMANPROD_KALMANFITSVRHITLATTICEPROXY_H */
