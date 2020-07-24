#if !defined(CHISQFITPRODUCER_CHISQFITSVRHITLATTICEPROXY_H)
#define CHISQFITPRODUCER_CHISQFITSVRHITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqFitSVRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 15:54:33 EDT 1999
// $Id: ChisqFitSVRHitLatticeProxy.h,v 1.3 1999/07/17 20:28:18 cdj Exp $
//
// Revision history
//
// $Log: ChisqFitSVRHitLatticeProxy.h,v $
// Revision 1.3  1999/07/17 20:28:18  cdj
// now compiles when CC optimization is on
//
// Revision 1.2  1999/06/20 22:44:51  wsun
// Added proxies for all ten fit hypotheses.
//
// Revision 1.1.1.1  1999/06/16 02:48:26  wsun
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
class ChisqFitSVRHitLatticeProxy :
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
      ChisqFitSVRHitLatticeProxy();
      virtual ~ChisqFitSVRHitLatticeProxy();

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
      ChisqFitSVRHitLatticeProxy( const ChisqFitSVRHitLatticeProxy& );

      // assignment operator(s)
      const ChisqFitSVRHitLatticeProxy& operator=( const ChisqFitSVRHitLatticeProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_ChisqFitSVRHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ChisqFitProducer/Template/ChisqFitSVRHitLatticeProxy.cc"
//#endif

#endif /* CHISQFITPRODUCER_CHISQFITSVRHITLATTICEPROXY_H */
