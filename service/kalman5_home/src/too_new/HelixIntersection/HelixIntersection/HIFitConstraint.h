#if !defined(HELIXINTERSECTION_HIFITCONSTRAINT_H)
#define HELIXINTERSECTION_HIFITCONSTRAINT_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIFitConstraint
// 
// Description: <one line class summary>
//
// Usage:
//
// The r constraint equations containing q unknown parameters are given
// in the form:
//
//    H(eta,z) = H0 + dH/dEta * deltaEta + dH/dZ * deltaZ = 0
//
// where
//
//    eta is the vector of helix parameters, deltaEta = eta - eta0.
//
//    z is the vector of unknown parameters, deltaZ   = z - z0.
//
//    H0 = H(eta0,z0) is a vector of length r giving the constraint equations
//    evaluated at the initial helix parameters and initial unknown parameters.
//
//    dH/dEta is an r x N matrix ( N = # free track params ) giving the
//    derivatives of the constraints w.r.t. the track parameters.
//
//    dH/dZ is an r x q matrix giving the derivatives of the constraints
//    w.r.t. the unknown parameters.
//
//
// Author:      Werner Sun
// Created:     Thu Jul  1 14:47:00 EDT 1999
// $Id: HIFitConstraint.h,v 1.2 1999/10/14 18:52:19 wsun Exp $
//
// Revision history
//
// $Log: HIFitConstraint.h,v $
// Revision 1.2  1999/10/14 18:52:19  wsun
// Added diagnostics flag and applyConstraint() now modifies the helices.
//
// Revision 1.1  1999/07/07 16:40:11  wsun
// Added mechanism for applying constraints in track fitters.
//

// system include files

// user include files
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/Vector.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class HIFitHelix ;

class HIFitConstraint
{
      // ---------- friend classes and functions ---------------

   public:

      class Results
      {
	 public:
	    Results( DABoolean           aFitSuccessful      = false,
		     const HepVector&    aDeltaTrackParams   = HepVector(),
		     const HepSymMatrix& aErrorMatrix        = HepSymMatrix(),
		     const HepVector&    aLagrangeMultiplers = HepVector(),
		     double              aConstraintChisq    = 0.,
		     int                 aNdof               = 0,
		     const HepVector&    aUnknownParams      = HepVector(),
		     const HepSymMatrix& aUnknownError       = HepSymMatrix(),
		     const HepMatrix&    aCovarUnknownsTrackParams
		                                             = HepMatrix() )
	       : m_fitSuccessful(     aFitSuccessful ),
		 m_deltaTrackParams(  aDeltaTrackParams ),
		 m_errorMatrix(       aErrorMatrix ),
		 m_multipliers(       aLagrangeMultiplers ),
		 m_chisq(             aConstraintChisq ),
		 m_ndof(              aNdof ),
		 m_unknownParams(     aUnknownParams ),
		 m_unknownError(      aUnknownError ),
		 m_covarUnknownHelix( aCovarUnknownsTrackParams ) {}

	    Results( const Results& rhs )
	    { *this = rhs; }

	    const Results& operator=( const Results& rhs )
	    {
	       if( this != &rhs )
	       {
		  m_fitSuccessful     = rhs.m_fitSuccessful ;
		  m_deltaTrackParams  = rhs.m_deltaTrackParams ;
		  m_errorMatrix       = rhs.m_errorMatrix ;
		  m_multipliers       = rhs.m_multipliers ;
		  m_chisq             = rhs.m_chisq ;
		  m_ndof              = rhs.m_ndof ;
		  m_unknownParams     = rhs.m_unknownParams ;
		  m_unknownError      = rhs.m_unknownError ;
		  m_covarUnknownHelix = rhs.m_covarUnknownHelix ;
	       }
	       return *this ;
	    }

	    ~Results() {}

	    DABoolean fitSuccessful() const { return m_fitSuccessful ; }

	    const HepVector& deltaTrackParameters() const
	    { return m_deltaTrackParams ; }

	    const HepSymMatrix& trackParameterErrorMatrix() const
	    { return m_errorMatrix ; }

	    const HepVector& lagrangeMultipliers() const
	    { return m_multipliers ; }

	    double chisq() const { return m_chisq ; }
	    int    ndof() const { return m_ndof ; }

	    const HepVector& unknownParameters() const
	    { return m_unknownParams ; }

	    const HepSymMatrix& unknownParameterErrorMatrix() const
	    { return m_unknownError ; }

	    const HepMatrix& unknownTrackCovariance() const
	    { return m_covarUnknownHelix ; }

	 private:
	    DABoolean    m_fitSuccessful ;

	    HepVector    m_deltaTrackParams ;
	    HepSymMatrix m_errorMatrix ;

	    HepVector    m_multipliers ;
	    double       m_chisq ;
	    int          m_ndof ;

	    HepVector    m_unknownParams ;
	    HepSymMatrix m_unknownError ;
	    HepMatrix    m_covarUnknownHelix ;
      } ;

      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HIFitConstraint();
      virtual ~HIFitConstraint();

      // ---------- member functions ---------------------------

      // Set parameters of the constraint based on the given helix and
      // unknowns (usually the initial unknowns).
      virtual void setConstraintParametersOne( const HIFitHelix& aHelix ) ;
      virtual void setConstraintParametersMany(
	 const STL_VECTOR( HIFitHelix* )& aHelices ) ;

      void setPrintDiagnostics( DABoolean aFlag )
      { m_printDiagnostics = aFlag ; }

      // ---------- const member functions ---------------------

      // The helix given to this function has been fit without constraints.
      virtual void applyConstraint( HIFitHelix* aHelix ) const ;

      // The user can supply any number of helices (to vertex to a
      // common point, for example).  If no prior error matrix is given,
      // then the helices' error matrices will be concatenated.
      virtual Results applyConstraint(
	 const STL_VECTOR( HIFitHelix* )& aHelices,
	 const HepSymMatrix& aPriorErrorMatrix = HepSymMatrix() ) const ;


      //******** Subclasses need to implement these functions. ********

      virtual int numberOfConstraints() const = 0 ;

      // The constraints evaluated at the given helix parameters.  Can also
      // be functions of initialUnknowns().  Should have length r.
      virtual HepVector constraint(
	 const STL_VECTOR( HIFitHelix* )& aHelices ) const = 0 ;

      // The matrix dH/dEta (see above) -- should be N x r.
      virtual HepMatrix constraintDerivatives( 
	 const STL_VECTOR( HIFitHelix* )& aHelices ) const = 0 ;

      //***************************************************************

      // Subclasses return true if there are unknown parameters in the
      // constraints to be determined.
      virtual DABoolean solveForUnknowns() const { return m_solveForUnknowns; }
      virtual int numberOfUnknowns() const { return 0 ; }

      // Initial values for the unknown parameters -- should have length q.
      virtual HepVector initialUnknowns( 
	 const STL_VECTOR( HIFitHelix* )& aHelices ) const
      { return HepVector() ; }

      // The matrix dH/dZ -- should be q x r.
      virtual HepMatrix unknownDerivatives( 
	 const STL_VECTOR( HIFitHelix* )& aHelices ) const
      { return HepMatrix() ; }

      virtual DABoolean isComposite() const { return false ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------
      void setSolveForUnknowns( DABoolean aFlag )
      { m_solveForUnknowns = aFlag ; }

   private:
      // ---------- Constructors and destructor ----------------
      HIFitConstraint( const HIFitConstraint& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIFitConstraint& operator=( const HIFitConstraint& );
      // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_solveForUnknowns ;
      DABoolean m_printDiagnostics ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIFitConstraint.cc"
//#endif

#endif /* HELIXINTERSECTION_HIFITCONSTRAINT_H */
