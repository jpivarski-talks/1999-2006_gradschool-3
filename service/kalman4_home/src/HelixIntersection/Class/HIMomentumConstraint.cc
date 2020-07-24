// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIMomentumConstraint
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Jul  5 15:57:30 EDT 1999
// $Id: HIMomentumConstraint.cc,v 1.1 1999/07/07 16:41:13 wsun Exp $
//
// Revision history
//
// $Log: HIMomentumConstraint.cc,v $
// Revision 1.1  1999/07/07 16:41:13  wsun
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
//#include "Experiment/report.h"
#include "HelixIntersection/HIMomentumConstraint.h"

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

static const char* const kFacilityString = "HelixIntersection.HIMomentumConstraint" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIMomentumConstraint.cc,v 1.1 1999/07/07 16:41:13 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIMomentumConstraint::HIMomentumConstraint()
{
}

// HIMomentumConstraint::HIMomentumConstraint( const HIMomentumConstraint& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIMomentumConstraint::~HIMomentumConstraint()
{
}

//
// assignment operators
//
// const HIMomentumConstraint& HIMomentumConstraint::operator=( const HIMomentumConstraint& rhs )
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
HIMomentumConstraint::setConstraintParametersOne( const HIFitHelix& aHelix )
{
   m_momentum = aHelix.momentum().mag() ;
}


//
// const member functions
//

HepVector
HIMomentumConstraint::constraint(
   const STL_VECTOR( HIFitHelix* )& aHelices ) const
{
   assert( aHelices.size() == 1 );
   HepVector tmp( 1, 0 ) ;
   tmp( 1 ) = ( *aHelices.begin() )->momentum().mag() - m_momentum ;
   return tmp ;
}

HepMatrix
HIMomentumConstraint::constraintDerivatives(
   const STL_VECTOR( HIFitHelix* )& aHelices ) const
{
   assert( aHelices.size() == 1 );

   HIFitHelix* helix = *aHelices.begin() ;
   double K = helix->curvature() ;
   double t = helix->cotTheta() ;
   double cscTh = sqrt( 1. + t * t ) ;

   HepMatrix tmp( HIHelix::kNTrackParameters, 1, 0 ) ;
   tmp( HIHelix::kCurvature, 1 ) = -cscTh / ( K * K ) ;
   tmp( HIHelix::kCotTheta, 1 )  = t / K / cscTh ;
   tmp *= 0.014989629 *  // 0.5 * magnetic field constant
      HIMagField::instance()->bfield( helix->position() ).mag() ;
   return tmp ;
}

//
// static member functions
//
