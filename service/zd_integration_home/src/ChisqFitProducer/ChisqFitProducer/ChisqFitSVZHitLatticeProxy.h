#if !defined(CHISQFITPRODUCER_CHISQFITSVZHITLATTICEPROXY_H)
#define CHISQFITPRODUCER_CHISQFITSVZHITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqFitSVZHitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 16:00:53 EDT 1999
// $Id: ChisqFitSVZHitLatticeProxy.h,v 1.3 1999/07/17 20:28:19 cdj Exp $
//
// Revision history
//
// $Log: ChisqFitSVZHitLatticeProxy.h,v $
// Revision 1.3  1999/07/17 20:28:19  cdj
// now compiles when CC optimization is on
//
// Revision 1.2  1999/06/20 22:44:52  wsun
// Added proxies for all ten fit hypotheses.
//
// Revision 1.1.1.1  1999/06/16 02:48:26  wsun
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
class ChisqFitSVZHitLatticeProxy :
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
      ChisqFitSVZHitLatticeProxy();
      virtual ~ChisqFitSVZHitLatticeProxy();

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
      ChisqFitSVZHitLatticeProxy( const ChisqFitSVZHitLatticeProxy& );

      // assignment operator(s)
      const ChisqFitSVZHitLatticeProxy& operator=( const ChisqFitSVZHitLatticeProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_ChisqFitSVZHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ChisqFitProducer/Template/ChisqFitSVZHitLatticeProxy.cc"
//#endif

#endif /* CHISQFITPRODUCER_CHISQFITSVZHITLATTICEPROXY_H */
