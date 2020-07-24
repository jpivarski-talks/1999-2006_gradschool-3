#if !defined(HELIXINTERSECTION_HICOMPOSITEFITCONSTRAINT_H)
#define HELIXINTERSECTION_HICOMPOSITEFITCONSTRAINT_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICompositeFitConstraint
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sun Jul  4 05:02:13 EDT 1999
// $Id: HICompositeFitConstraint.h,v 1.1 1999/07/07 16:40:10 wsun Exp $
//
// Revision history
//
// $Log: HICompositeFitConstraint.h,v $
// Revision 1.1  1999/07/07 16:40:10  wsun
// Added mechanism for applying constraints in track fitters.
//

// system include files

// user include files
#include "HelixIntersection/HIFitConstraint.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class HICompositeFitConstraint : public HIFitConstraint
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HICompositeFitConstraint();
      virtual ~HICompositeFitConstraint();

      // ---------- member functions ---------------------------

      // Set parameters of the constraint based on the given helix and
      // unknowns (usually the initial unknowns).
      virtual void setConstraintParametersOne( const HIFitHelix& aHelix ) ;
      virtual void setConstraintParametersMany(
	 const STL_VECTOR( HIFitHelix* )& aHelices ) ;

      // Children added with this function will be deleted when the
      // destructor is called.
      virtual void addChild( HIFitConstraint* aConstraint ) ;
      virtual void deleteChildren() ;

      // In case the number of constraints of one of the children has
      // been dynamically changed.
      void recalculateNumberOfConstraints() ;

      // Set this to false to prevent the destructor from deleting its children
      void setDtorDeletesChildren( DABoolean aFlag )
      { m_dtorDeletesChildren = aFlag ; }

      // ---------- const member functions ---------------------
      virtual const HIFitConstraint* getChild( int iChild ) const ;
      virtual int numberOfChildren() const ;
      virtual DABoolean isComposite() const { return true ; }

      // Subclasses need to implement these functions.
      virtual int numberOfConstraints() const { return m_numberConstraints ; }

      // The constraints evaluated at the given helix parameters.  Can also
      // be functions of initialUnknowns().  Should have length r.
      virtual HepVector constraint(
	 const STL_VECTOR( HIFitHelix* )& aHelices ) const ;

      virtual int numberOfUnknowns() const { return m_numberUnknowns ; }

      // The matrix dH/dEta (see above) -- should be N x r.
      virtual HepMatrix constraintDerivatives(
	 const STL_VECTOR( HIFitHelix* )& aHelices ) const ;

      // Initial values for the unknown parameters -- should have length q.
      virtual HepVector initialUnknowns(
	 const STL_VECTOR( HIFitHelix* )& aHelices ) const ;

      // The matrix dH/dZ -- should be q x r.
      virtual HepMatrix unknownDerivatives(
	 const STL_VECTOR( HIFitHelix* )& aHelices ) const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HICompositeFitConstraint( const HICompositeFitConstraint& );
      // stop default

      // ---------- assignment operator(s) ---------------------
      const HICompositeFitConstraint& operator=(
	 const HICompositeFitConstraint& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_dtorDeletesChildren ;
      STL_VECTOR( HIFitConstraint* )* m_constraints ;
      int m_numberConstraints ;
      int m_numberUnknowns ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HICompositeFitConstraint.cc"
//#endif

#endif /* HELIXINTERSECTION_HICOMPOSITEFITCONSTRAINT_H */
