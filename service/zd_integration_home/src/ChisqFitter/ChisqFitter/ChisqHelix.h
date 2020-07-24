#if !defined(CHISQFITTER_CHISQHELIX_H)
#define CHISQFITTER_CHISQHELIX_H
// -*- C++ -*-
//
// Package:     <ChisqFitter>
// Module:      ChisqHelix
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Wed Jun  9 02:10:20 EDT 1999
// $Id: ChisqHelix.h,v 1.7 2000/08/16 20:31:42 wsun Exp $
//
// Revision history
//
// $Log: ChisqHelix.h,v $
// Revision 1.7  2000/08/16 20:31:42  wsun
// Updated to new TRTrackFitQuality.
//
// Revision 1.6  1999/11/21 22:10:45  wsun
// Update to new HIHelix interface.
//
// Revision 1.5  1999/07/07 16:44:04  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.4  1999/06/24 22:52:13  wsun
// * Added use of HIMagField and new SiHits.
// * Added ability to fix any combination of parameters in track fitting.
//
// Revision 1.3  1999/06/20 21:16:53  wsun
// Allow user to fix parameters.
//
// Revision 1.2  1999/06/18 19:16:21  wsun
// Allow user to fix parameters.
//
// Revision 1.1.1.1  1999/06/16 02:46:47  wsun
// First submission.
//

// system include files

// user include files
#include "HelixIntersection/HIFitHelix.h"

#include "CLHEP/Matrix/SymMatrix.h"

// forward declarations
class Hep3Vector ;


class ChisqHelix : public HIFitHelix
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      static const string kHelixType ;

      // ---------- Constructors and destructor ----------------

      ChisqHelix();
      ChisqHelix( const KTHelix& aHelix,
		  GeV aMass = 0.13957,  // pion mass
		  DABoolean reportFitResultsAtInitRefPt = false,
		  Meters anArcLength = 0.,
		  Identifier id = 0 ) ;

      ChisqHelix( const ChisqHelix& );
      const ChisqHelix& operator=( const ChisqHelix& );
      virtual ~ChisqHelix();

      // ---------- member functions ---------------------------

      // *********** IMPORTANT ************
      // Subclasses need to reimplement these two functions so that the
      // snapshot is of the right type.
      virtual HIHelix* newSnapshot() { return new ChisqHelix( *this ) ; }
      virtual void revertToSnapshot( const HIHelix* aSnapshot )
      {
         if( aSnapshot->helixType() == helixType() )
            *this = *( ( ChisqHelix* )aSnapshot ) ;
      }

      // The information matrix and residual vector do not include
      // fixed parameters.
      void incrementInfoMatrix( const HepSymMatrix& aInfoMatrix )
      { m_infoMatrix += aInfoMatrix ; }
      void setInfoMatrix( const HepSymMatrix& aInfoMatrix )
      { m_infoMatrix = aInfoMatrix ; }
      void clearInfoMatrix()
      { m_infoMatrix = HepSymMatrix( numberOfFreeParameters(), 0 ) ; }

      void incrementResidualVector( const HepVector& aResidualVector )
      { m_residualVector += aResidualVector ; }
      void setResidualVector( const HepVector& aResidualVector )
      { m_residualVector = aResidualVector ; }
      void clearResidualVector()
      { m_residualVector = HepVector( numberOfFreeParameters(), 0 ) ; }

      void fixParameter( int aParam ) ;
      void fixCurvature() { fixParameter( kCurvature ) ; }
      void fixPhi0()      { fixParameter( kPhi0 ) ; }
      void fixD0()        { fixParameter( kD0 ) ; }
      void fixCotTheta()  { fixParameter( kCotTheta ) ; }
      void fixZ0()        { fixParameter( kZ0 ) ; }

      void releaseParameter( int aParam ) ;
      void releaseCurvature() { releaseParameter( kCurvature ) ; }
      void releasePhi0()      { releaseParameter( kPhi0 ) ; }
      void releaseD0()        { releaseParameter( kD0 ) ; }
      void releaseCotTheta()  { releaseParameter( kCotTheta ) ; }
      void releaseZ0()        { releaseParameter( kZ0 ) ; }

      // Reset ref pt to that of the initial helix, apply new params
      // in the HepVector (which contains only the free params), and
      // return the constrained parameters to the values to which they
      // were fixed.
      void setTrackParamsAtInitRefPt( const HepVector& aParams ) ;

      // Return the helix to its position at the start of the iteration,
      // i.e. to the reference point where the fitted parameters are to be
      // given.  The error matrix is not copied.
      void returnToStart() ;

      void setErrorMatrix( const HepSymMatrix& aMatrix ) ;

      void setNumberIterations( int aNumber ) { m_nIterations = aNumber ; }

      // ---------- const member functions ---------------------

      // *********** IMPORTANT ************
      // Subclasses should reimplement helixType(), which is used to
      // test for the helix' actual type when casting base class pointers
      // into the appropriate subclass.
      virtual const string& helixType() const
      { return ChisqHelix::kHelixType ; }

      const HepSymMatrix& informationMatrix() const { return m_infoMatrix ; }

      const HepVector& residualVector() const { return m_residualVector ; }

      // The unconstrained track parameters, with length m_numberFreeParams.
      HepVector freeTrackParameters() const ;

      DABoolean parameterFree( int aParam )
      { return m_free[ aParam ] ; }

      int numberIterations() const { return m_nIterations ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      HepSymMatrix m_infoMatrix ;
      HepVector    m_residualVector ;

      DABoolean m_free[ kZ0 + kCurvature ] ;

      // fixed parameters *at the init helix ref pt*.
      double m_paramsFixedTo[ kZ0 + kCurvature ] ;

      // Save the helix parameters at the start of each iteration.
      KTHelix m_helixAtStart ;

      // Quantites for quality object.
      int m_nIterations ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ChisqFitter/Template/ChisqHelix.cc"
//#endif

#endif /* CHISQFITTER_CHISQHELIX_H */
