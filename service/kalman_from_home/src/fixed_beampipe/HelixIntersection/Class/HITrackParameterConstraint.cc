// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HITrackParameterConstraint
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Jul  4 23:54:28 EDT 1999
// $Id: HITrackParameterConstraint.cc,v 1.1 1999/07/07 16:41:14 wsun Exp $
//
// Revision history
//
// $Log: HITrackParameterConstraint.cc,v $
// Revision 1.1  1999/07/07 16:41:14  wsun
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
#include "HelixIntersection/HITrackParameterConstraint.h"

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

static const char* const kFacilityString =
"HelixIntersection.HITrackParameterConstraint" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HITrackParameterConstraint.cc,v 1.1 1999/07/07 16:41:14 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

HITrackParameterConstraint::HITrackParameterConstraint(
   int param1ToFix,
   int param2ToFix,
   int param3ToFix,
   int param4ToFix,
   int param5ToFix )
   : m_numberConstraints( 0 )
{
   for( int i = 0 ; i < KTHelix::kCurvature + KTHelix::kZ0 ; ++i )
   {
      m_fixParameter[ i ] = false ;
      m_fixedValues[ i ] = 0. ;
   }

   if( param1ToFix != 0 ) setParameterToFix( param1ToFix ) ;
   if( param2ToFix != 0 ) setParameterToFix( param2ToFix ) ;
   if( param3ToFix != 0 ) setParameterToFix( param3ToFix ) ;
   if( param4ToFix != 0 ) setParameterToFix( param4ToFix ) ;
   if( param5ToFix != 0 ) setParameterToFix( param5ToFix ) ;
}

// HITrackParameterConstraint::HITrackParameterConstraint( const HITrackParameterConstraint& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HITrackParameterConstraint::~HITrackParameterConstraint()
{
}

//
// assignment operators
//
// const HITrackParameterConstraint& HITrackParameterConstraint::operator=( const HITrackParameterConstraint& rhs )
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
HITrackParameterConstraint::setConstraintParametersOne(
   const HIFitHelix& aHelix )
{
   HepVector params( aHelix.trackParameters() ) ;

   for( int i = KTHelix::kCurvature ; i <= KTHelix::kZ0 ; ++i )
   {
      if( m_fixParameter[ i ] )
      {
	 m_fixedValues[ i ] = params( i ) ;
      }
   }
}

void
HITrackParameterConstraint::setParameterToFix( int paramToFix )
{
   if( !m_fixParameter[ paramToFix ] )
   {
      m_fixParameter[ paramToFix ] = true ;
      ++m_numberConstraints ;
   }
}

void
HITrackParameterConstraint::clearFixedParameters()
{
   for( int i = 0 ; i < KTHelix::kCurvature + KTHelix::kZ0 ; ++i )
   {
      m_fixParameter[ i ] = false ;
      m_fixedValues[ i ] = 0. ;
   }
   m_numberConstraints = 0 ;
}


//
// const member functions
//

HepVector
HITrackParameterConstraint::constraint( 
   const STL_VECTOR( HIFitHelix* )& aHelices ) const
{
   assert( aHelices.size() == 1 ) ;

   HepVector params( ( *aHelices.begin() )->trackParameters() ) ;
   HepVector tmp( m_numberConstraints, 0 ) ;

   int index = 0 ;
   for( int i = KTHelix::kCurvature ; i <= KTHelix::kZ0 ; ++i )
   {
      if(  m_fixParameter[ i ] )
      {
	 tmp( ++index ) = params( i ) - m_fixedValues[ i ] ;
      }
   }

   return tmp ;
}

HepMatrix
HITrackParameterConstraint::constraintDerivatives(
   const STL_VECTOR( HIFitHelix* )& aHelices ) const
{
   assert( aHelices.size() == 1 ) ;

   HepMatrix tmp( HIHelix::kNTrackParameters, m_numberConstraints, 0 ) ;

   int index = 0 ;
   for( int i = KTHelix::kCurvature ; i <= KTHelix::kZ0 ; ++i )
   {
      if( m_fixParameter[ i ] )
      {
	 tmp( i, ++index ) = 1. ;
      }
   }

   return tmp ;
}

//
// static member functions
//
