#if !defined(CHISQFITTER_HITRACKPARAMETERCONSTRAINT_H)
#define CHISQFITTER_HITRACKPARAMETERCONSTRAINT_H
// -*- C++ -*-
//
// Package:     <ChisqFitter>
// Module:      HITrackParameterConstraint
// 
// Description: <one line class summary>
//
// Usage:
//    In KalmanTrackFitter and ChisqTrackFitter, the helix parameters get
//    fixed to their values at the START of the fit.
//
// Author:      Werner Sun
// Created:     Sat Jul  3 18:06:55 EDT 1999
// $Id: HITrackParameterConstraint.h,v 1.1 1999/07/07 16:40:17 wsun Exp $
//
// Revision history
//
// $Log: HITrackParameterConstraint.h,v $
// Revision 1.1  1999/07/07 16:40:17  wsun
// Added mechanism for applying constraints in track fitters.
//

// system include files

// user include files
#include "HelixIntersection/HIFitConstraint.h"
#include "HelixIntersection/HIFitHelix.h"

// forward declarations

class HITrackParameterConstraint : public HIFitConstraint
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------

      // Users give the parameters to be fixed in any order
      // (e.g. HITrackParameterConstraint( KTHelix::kZ0, KTHelix::kD0 ) ).
      // The arguments should be given as elements of KTHelix::ParameterOrder.
      HITrackParameterConstraint(
	 int param1ToFix = 0,
	 int param2ToFix = 0,
	 int param3ToFix = 0,
	 int param4ToFix = 0,
	 int param5ToFix = 0 ) ;
      virtual ~HITrackParameterConstraint() ;

      // ---------- member functions ---------------------------
      virtual void setConstraintParametersOne( const HIFitHelix& aHelix ) ;

      void setParameterToFix( int paramToFix ) ;
      void clearFixedParameters() ;

      // ---------- const member functions ---------------------

      virtual int numberOfConstraints() const { return m_numberConstraints ; }

      // The constraints evaluated at the given helix parameters
      // and the given unknowns -- should have length r.
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
      HITrackParameterConstraint( const HITrackParameterConstraint& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HITrackParameterConstraint& operator=( const HITrackParameterConstraint& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      int m_numberConstraints ;
      DABoolean m_fixParameter[ KTHelix::kCurvature + KTHelix::kZ0 ] ;
      double m_fixedValues[ KTHelix::kCurvature + KTHelix::kZ0 ] ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ChisqFitter/Template/HITrackParameterConstraint.cc"
//#endif

#endif /* CHISQFITTER_HITRACKPARAMETERCONSTRAINT_H */
