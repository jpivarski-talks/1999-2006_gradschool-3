#if !defined(CHISQFITPRODUCER_CHISQFITZDHITLATTICEPROXY_H)
#define CHISQFITPRODUCER_CHISQFITZDHITLATTICEPROXY_H
// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqFitZDHitLatticeProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 15:35:09 EDT 1999
// $Id: ChisqFitZDHitLatticeProxy.h,v 1.1 2003/04/17 17:00:42 bkh Exp $
//
// Revision history
//
// $Log: ChisqFitZDHitLatticeProxy.h,v $
// Revision 1.1  2003/04/17 17:00:42  bkh
// Add ZD options & throw exceptions for wrong config
//
// Revision 1.3  1999/07/17 20:28:18  cdj
// now compiles when CC optimization is on
//
// Revision 1.2  1999/06/20 22:44:50  wsun
// Added proxies for all ten fit hypotheses.
//
// Revision 1.1.1.1  1999/06/16 02:48:26  wsun
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
class ChisqFitZDHitLatticeProxy :
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
      ChisqFitZDHitLatticeProxy();
      virtual ~ChisqFitZDHitLatticeProxy();

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
      ChisqFitZDHitLatticeProxy( const ChisqFitZDHitLatticeProxy& );

      // assignment operator(s)
      const ChisqFitZDHitLatticeProxy& operator=( const ChisqFitZDHitLatticeProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_ChisqFitZDHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ChisqFitProducer/Template/ChisqFitZDHitLatticeProxy.cc"
//#endif

#endif /* CHISQFITPRODUCER_CHISQFITZDHITLATTICEPROXY_H */
