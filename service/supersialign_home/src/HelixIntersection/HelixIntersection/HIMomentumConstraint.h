#if !defined(HELIXINTERSECTION_HIMOMENTUMCONSTRAINT_H)
#define HELIXINTERSECTION_HIMOMENTUMCONSTRAINT_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIMomentumConstraint
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sun Jul  4 04:30:22 EDT 1999
// $Id: HIMomentumConstraint.h,v 1.1 1999/07/07 16:40:16 wsun Exp $
//
// Revision history
//
// $Log: HIMomentumConstraint.h,v $
// Revision 1.1  1999/07/07 16:40:16  wsun
// Added mechanism for applying constraints in track fitters.
//

// system include files

// user include files
#include "HelixIntersection/HIFitConstraint.h"
#include "HelixIntersection/HIFitHelix.h"
#include "HelixIntersection/HIMagField.h"

// forward declarations

class HIMomentumConstraint : public HIFitConstraint
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HIMomentumConstraint() ;
      virtual ~HIMomentumConstraint() ;

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      virtual int numberOfConstraints() const { return 1 ; }

      // Set parameters of the constraint based on the given helix and
      // unknowns (usually the initial unknowns).
      virtual void setConstraintParametersOne( const HIFitHelix& aHelix ) ;

      // The constraints evaluated at the given helix parameters.  Can also
      // be functions of initialUnknowns().  Should have length r.
      virtual HepVector constraint(
	 const STL_VECTOR( HIFitHelix* )& aHelices ) const ;

      // The matrix dH/dEta (see above) -- should be N x r.
      virtual HepMatrix constraintDerivatives(
	 const STL_VECTOR( HIFitHelix* )& aHelices ) const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIMomentumConstraint( const HIMomentumConstraint& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIMomentumConstraint& operator=( const HIMomentumConstraint& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      GeV m_momentum ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIMomentumConstraint.cc"
//#endif

#endif /* HELIXINTERSECTION_HIMOMENTUMCONSTRAINT_H */
