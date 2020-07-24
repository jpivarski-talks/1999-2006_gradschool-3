// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIFitConstraint
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Jul  1 14:45:20 EDT 1999
// $Id: HIFitConstraint.cc,v 1.2 1999/10/14 18:52:16 wsun Exp $
//
// Revision history
//
// $Log: HIFitConstraint.cc,v $
// Revision 1.2  1999/10/14 18:52:16  wsun
// Added diagnostics flag and applyConstraint() now modifies the helices.
//
// Revision 1.1  1999/07/07 16:41:09  wsun
// Added mechanism for applying constraints in track fitters.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include <assert.h>

// user include files
#include "Experiment/report.h"
#include "HelixIntersection/HIFitConstraint.h"
#include "HelixIntersection/HIFitHelix.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HelixIntersection.HIFitConstraint" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIFitConstraint.cc,v 1.2 1999/10/14 18:52:16 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIFitConstraint::HIFitConstraint()
   : m_solveForUnknowns( false ),
     m_printDiagnostics( false )
{
}

// HIFitConstraint::HIFitConstraint( const HIFitConstraint& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIFitConstraint::~HIFitConstraint()
{
}

//
// assignment operators
//
// const HIFitConstraint& HIFitConstraint::operator=( const HIFitConstraint& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//
void
HIFitConstraint::setConstraintParametersOne( const HIFitHelix& aHelix )
{
   report( WARNING, kFacilityString ) <<
      "setConstraintParametersOne(...) not implemented." << endl ;
}

void
HIFitConstraint::setConstraintParametersMany(
   const STL_VECTOR( HIFitHelix* )& aHelices )
{
   report( WARNING, kFacilityString ) <<
      "setConstraintParametersMany(...) not implemented." << endl ;
}


void HIFitConstraint::applyConstraint( HIFitHelix* aHelix ) const
{
   if( m_printDiagnostics )
   {
      cout << "uncons params "
	   << aHelix->curvature() << " "
	   << aHelix->phi0() << " "
	   << aHelix->d0() << " "
	   << aHelix->cotTheta() << " "
	   << aHelix->z0() << endl ;
      cout << "chisq/ndof " << aHelix->chisq()
	   << " " << aHelix->ndof() << endl;
   }

   STL_VECTOR( HIFitHelix* ) helices ;
   helices.push_back( aHelix ) ;

   Results constraintResults = applyConstraint( helices ) ;
}

HIFitConstraint::Results
HIFitConstraint::applyConstraint( const STL_VECTOR( HIFitHelix* )& aHelices,
				  const HepSymMatrix& aPriorErrorMatrix ) const
{
   int ierr ;

   // The current error matrix.
   HepSymMatrix initialErrorMatrix ;
   if( aPriorErrorMatrix.num_row() > 0 )
   {
      initialErrorMatrix = aPriorErrorMatrix ;
   }
   else if( aHelices.size() == 1 )
   {
      HIFitHelix* helix = *aHelices.begin() ;
      if( helix->hasNullErrorMatrix() )
      {
	 return Results() ;
      }

      initialErrorMatrix = helix->errorMatrix() ;
   }
   else
   {
      initialErrorMatrix =
	 HepSymMatrix( aHelices.size() * HIHelix::kNTrackParameters, 0 ) ;

      STL_VECTOR( HIFitHelix* )::const_iterator helixItr = aHelices.begin() ;
      STL_VECTOR( HIFitHelix* )::const_iterator helixEnd = aHelices.end() ;

      for( int index = 1 ;
	   helixItr != helixEnd ;
	   ++helixItr, index += HIHelix::kNTrackParameters )
      {
	 if( ( *helixItr )->hasNullErrorMatrix() )
	 {
	    return Results() ;
	 }

	 initialErrorMatrix.sub( index, ( *helixItr )->errorMatrix() ) ;
      }
   }

   HepSymMatrix infoMatrix  = initialErrorMatrix.inverse( ierr ) ;

   if( ierr )
   {
      return Results();
   }

   if( m_printDiagnostics )
   {
      cout << "uncons error" << initialErrorMatrix
	   << "uncons info " << infoMatrix ;
   }

   // Get the vector of constraint equations and their derivatives
   // evaluated at the current track parameters.
   HepVector constraints = constraint( aHelices ) ;
   HepMatrix constraintDerivs = constraintDerivatives( aHelices ).T() ; // rxN

   // A combination used over and over again.
   HepSymMatrix constraintInfo =
      ( initialErrorMatrix.similarity( constraintDerivs ) ).inverse( ierr ) ;

   if( ierr )
   {
      return Results() ;
   }

   HepVector    multipliers ;
   HepVector    unknownParams ;
   HepSymMatrix unknownError ;
   HepMatrix    covarUnknownHelix ;

   HepSymMatrix addToConstraintInfo ;
   if( solveForUnknowns() )
   {
      // Vector of initial guesses for the unknown parameters.
      HepVector initUnknowns = initialUnknowns( aHelices ) ;

      // The r (# constraints) x q (# unknown constraint parameters) matrix
      // of derivatives w.r.t. the unknown parameters.
      HepMatrix unknownDerivs = unknownDerivatives( aHelices ).T() ;

      if( m_printDiagnostics )
      {
	 cout << "init unk" << initUnknowns
	      << "unk derivs" << unknownDerivs ;
      }

      // Solve for the unknown parameters.
      unknownError =
	 ( constraintInfo.similarityT( unknownDerivs ) ).inverse( ierr ) ;

      if( ierr )
      {
	 return Results() ;
      }

      HepMatrix constraintToUnknown =
	 -unknownError * unknownDerivs.T() * constraintInfo ;

      HepVector deltaUnknowns = constraintToUnknown * constraints ;
      unknownParams = initUnknowns + deltaUnknowns ;


      // Solve for covariance( unknown params, new track params ).
      covarUnknownHelix =
	 constraintToUnknown * constraintDerivs * initialErrorMatrix ;

      if( m_printDiagnostics )
      {
	 cout << "unk error" << unknownError ;
	 cout << "unk" << unknownParams ;
	 cout << "covar(unk,eta)" << covarUnknownHelix ;
      }

      constraints += unknownDerivs * deltaUnknowns ;
      addToConstraintInfo.assign(
	 constraintInfo * unknownDerivs * constraintToUnknown ) ;
   }

   // Solve for the new track parameters with constraints applied.
   HepVector deltaParams = -initialErrorMatrix *
      constraintDerivs.T() * constraintInfo * constraints ;


   // Calculate addition to chisq.
   multipliers = constraintInfo * constraints ;

   double deltaChisq = infoMatrix.similarityT( deltaParams )( 1, 1 ) +
      ( 2. * multipliers.T() *
	( constraints + constraintDerivs * deltaParams ) )( 1, 1 ) ;


   // Solve for new track parameter error matrix.
   if( solveForUnknowns() )
   {
      constraintInfo += addToConstraintInfo ;
   }

   HepSymMatrix newError = initialErrorMatrix -
      constraintInfo.similarity( initialErrorMatrix * constraintDerivs.T() ) ;

   if( m_printDiagnostics )
   {
      cout << "constraints" << constraints
	   << "cons derivs" << constraintDerivs ;
      cout << "delta params" << deltaParams ;
      cout << "delta chisq " << deltaChisq << endl ;
      cout << "new error" << newError ;
   }

   // The new results object.
   Results results( true,         // fit successful
		    deltaParams,
		    newError,
		    multipliers,
		    deltaChisq,
		    constraints.num_row() - unknownParams.num_row(), // ndof
		    unknownParams,
		    unknownError,
		    covarUnknownHelix ) ;

   // Apply the constraint results to the helices.
   if( aHelices.size() == 1 )
   {
      ( *aHelices.begin() )->applyConstraintResults( results ) ;
   }
   else
   {
      // Only update the track parameters and error matrices.
      STL_VECTOR( HIFitHelix* )::const_iterator helixItr = aHelices.begin() ;
      STL_VECTOR( HIFitHelix* )::const_iterator helixEnd = aHelices.end() ;

      for( int index = 1 ;
	   helixItr != helixEnd ;
	   ++helixItr, index += HIHelix::kNTrackParameters )
      {
	 int maxRow = index + HIHelix::kNTrackParameters - 1 ;
	 HepVector subParams   = deltaParams.sub( index, maxRow ) ;
	 HepSymMatrix subError = newError.sub( index, maxRow ) ;

	 ( *helixItr )->setTrackParameters(
	    ( *helixItr )->trackParameters() + subParams ) ;
	 ( *helixItr )->setErrorMatrix( subError ) ;
      }
   }

   return results ;
}
//
// const member functions
//

//
// static member functions
//
