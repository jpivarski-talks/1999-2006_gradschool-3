#if !defined(TRACKFITTER_TRACKFITLATTICE_H)
#define TRACKFITTER_TRACKFITLATTICE_H
// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      TrackFitLattice
// 
// Description: This is basically a shell for creating Lattices.
//
// Usage:
//    <usage>
//
// Author:      Michael Marsh
// Created:     Wed Nov 18 14:20:34 EST 1998
// $Id: TrackFitLattice.h,v 1.1 1998/12/15 20:33:49 marsh Exp $
//
// Revision history
//
// $Log: TrackFitLattice.h,v $
// Revision 1.1  1998/12/15 20:33:49  marsh
// Added definitions of fit track to hit Lattices and link data.
//

// system include files

// user include files
#include "CleoDB/DBCandidate.h" // for Hypo
#include "TrackRoot/TRHelixCandidateFit.h"
#include "TrackFitter/TrackFitLinkData.h"
#include "Lattice/Lattice.h"

// forward declarations

template< DBCandidate::Hypo H , class Hit >
class TrackFitLattice :
   public Lattice< TRHelixCandidateFit< H > , Hit , TrackFitLinkData >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef Lattice< TRHelixCandidateFit< H > ,
                       Hit ,
                       TrackFitLinkData > LatticeType;

      // Constructors and destructor
      TrackFitLattice(
         const FATable< TRHelixCandidateFit< H > left_table ,
         const FATable< Hit > right_table ) : LatticeType(
            left_table,
            right_table,
            LatticeTopology::Default,
            LatticeTopology::Default,
            LatticeTopology::Default,
            LatticeTopology::Default ) {}

      TrackFitLattice(
         const FAPtrTable< TRHelixCandidateFit< H >* left_table ,
         const FATable< Hit > right_table ) : LatticeType(
            left_table,
            right_table,
            LatticeTopology::Default,
            LatticeTopology::Default,
            LatticeTopology::Default,
            LatticeTopology::Default ) {}

      TrackFitLattice(
         const FATable< TRHelixCandidateFit< H > left_table ,
         const FAPtrTable< Hit >* right_table ) : LatticeType(
            left_table,
            right_table,
            LatticeTopology::Default,
            LatticeTopology::Default,
            LatticeTopology::Default,
            LatticeTopology::Default ) {}

      TrackFitLattice(
         const FAPtrTable< TRHelixCandidateFit< H >* left_table ,
         const FAPtrTable< Hit >* right_table ) : LatticeType(
            left_table,
            right_table,
            LatticeTopology::Default,
            LatticeTopology::Default,
            LatticeTopology::Default,
            LatticeTopology::Default ) {}

      virtual ~TrackFitLattice();

      // member functions

      // const member functions

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      TrackFitLattice();
      TrackFitLattice( const TrackFitLattice& );

      // assignment operator(s)
      const TrackFitLattice& operator=( const TrackFitLattice& );

      // private member functions

      // private const member functions

      // data members

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "package/Template/TrackFitLattice.cc"
//#endif

#endif /* TRACKFITTER_TRACKFITLATTICE_H */
