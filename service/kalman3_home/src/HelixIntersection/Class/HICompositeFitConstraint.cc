// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICompositeFitConstraint
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Jul  4 05:11:01 EDT 1999
// $Id: HICompositeFitConstraint.cc,v 1.1 1999/07/07 16:41:07 wsun Exp $
//
// Revision history
//
// $Log: HICompositeFitConstraint.cc,v $
// Revision 1.1  1999/07/07 16:41:07  wsun
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

// user include files
//#include "Experiment/report.h"
#include "HelixIntersection/HICompositeFitConstraint.h"
#include "HelixIntersection/HIHelix.h"

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

static const char* const kFacilityString = "HelixIntersection.HICompositeFitConstraint" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HICompositeFitConstraint.cc,v 1.1 1999/07/07 16:41:07 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HICompositeFitConstraint::HICompositeFitConstraint()
   : HIFitConstraint(),
     m_constraints( new STL_VECTOR( HIFitConstraint* ) ),
     m_numberConstraints( 0 ),
     m_numberUnknowns( 0 ),
     m_dtorDeletesChildren( true )
{
}

// HICompositeFitConstraint::HICompositeFitConstraint( const HICompositeFitConstraint& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HICompositeFitConstraint::~HICompositeFitConstraint()
{
   if( m_dtorDeletesChildren )
   {
      deleteChildren() ;
   }

   delete m_constraints ;
}

//
// assignment operators
//
// const HICompositeFitConstraint& HICompositeFitConstraint::operator=( const HICompositeFitConstraint& rhs )
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
HICompositeFitConstraint::setConstraintParametersOne(
   const HIFitHelix& aHelix )
{
   STL_VECTOR( HIFitConstraint* )::iterator itr = m_constraints->begin() ;
   STL_VECTOR( HIFitConstraint* )::iterator end = m_constraints->end() ;

   for( ; itr != end ; ++itr )
   {
      ( *itr )->setConstraintParametersOne( aHelix ) ;
   }
}

void
HICompositeFitConstraint::setConstraintParametersMany(
   const STL_VECTOR( HIFitHelix* )& aHelices )
{
   STL_VECTOR( HIFitConstraint* )::iterator itr = m_constraints->begin() ;
   STL_VECTOR( HIFitConstraint* )::iterator end = m_constraints->end() ;

   for( ; itr != end ; ++itr )
   {
      ( *itr )->setConstraintParametersMany( aHelices ) ;
   }
}

void
HICompositeFitConstraint::addChild( HIFitConstraint* aConstraint )
{
   m_constraints->push_back( aConstraint ) ;
   m_numberConstraints += aConstraint->numberOfConstraints() ;
   m_numberUnknowns += aConstraint->numberOfUnknowns() ;
   setSolveForUnknowns( solveForUnknowns() || aConstraint->solveForUnknowns());
}

void
HICompositeFitConstraint::deleteChildren()
{
   STL_VECTOR( HIFitConstraint* )::const_iterator itr =
      m_constraints->begin();
   STL_VECTOR( HIFitConstraint* )::const_iterator end =
      m_constraints->end() ;

   for( ; itr != end ; ++itr )
   {
      delete *itr ;
   }

   m_constraints->clear() ;
   m_numberConstraints = 0 ;
   m_numberUnknowns = 0 ;
   setSolveForUnknowns( false ) ;
}

void
HICompositeFitConstraint::recalculateNumberOfConstraints()
{
   m_numberConstraints = 0 ;

   STL_VECTOR( HIFitConstraint* )::const_iterator itr =
      m_constraints->begin();
   STL_VECTOR( HIFitConstraint* )::const_iterator end =
      m_constraints->end() ;

   for( ; itr != end ; ++itr )
   {
      m_numberConstraints += ( *itr )->numberOfConstraints() ;
   }
}

//
// const member functions
//
const HIFitConstraint*
HICompositeFitConstraint::getChild( int iChild ) const
{
   return ( *m_constraints )[ iChild ] ;
}

int
HICompositeFitConstraint::numberOfChildren() const
{
   STL_VECTOR( HIFitConstraint* )::const_iterator itr =
      m_constraints->begin();
   STL_VECTOR( HIFitConstraint* )::const_iterator end =
      m_constraints->end() ;

   int nChildren = 0 ;
   for( ; itr != end ; ++itr )
   {
      if( ( *itr )->isComposite() )
      {
         HICompositeFitConstraint* cast =
	    ( HICompositeFitConstraint* ) ( *itr ) ;
         nChildren += cast->numberOfChildren() ;
      }
      else
      {
         ++nChildren ;
      }
   }
   return nChildren ;
}

HepVector
HICompositeFitConstraint::constraint(
   const STL_VECTOR( HIFitHelix* )& aHelices ) const
{
   STL_VECTOR( HIFitConstraint* )::const_iterator itr =
      m_constraints->begin();
   STL_VECTOR( HIFitConstraint* )::const_iterator end =
      m_constraints->end() ;

   int index = 1 ;
   HepVector tmp( m_numberConstraints, 0 ) ;
   for( ; itr != end ; ++itr )
   {
      tmp.sub( index, ( *itr )->constraint( aHelices ) ) ;
      index += ( *itr )->numberOfConstraints() ;
   }

   return tmp ;
}

HepMatrix
HICompositeFitConstraint::constraintDerivatives(
   const STL_VECTOR( HIFitHelix* )& aHelices ) const
{
   STL_VECTOR( HIFitConstraint* )::const_iterator itr =
      m_constraints->begin();
   STL_VECTOR( HIFitConstraint* )::const_iterator end =
      m_constraints->end() ;

   int index = 1 ;
   HepMatrix tmp( HIHelix::kNTrackParameters, m_numberConstraints, 0 ) ;
   for( ; itr != end ; ++itr )
   {
      tmp.sub( 1, index, ( *itr )->constraintDerivatives( aHelices ) ) ;
      index += ( *itr )->numberOfConstraints() ;
   }

   return tmp ;
}

HepVector
HICompositeFitConstraint::initialUnknowns(
   const STL_VECTOR( HIFitHelix* )& aHelices ) const
{
   STL_VECTOR( HIFitConstraint* )::const_iterator itr =
      m_constraints->begin();
   STL_VECTOR( HIFitConstraint* )::const_iterator end =
      m_constraints->end() ;

   int index = 1 ;
   HepVector tmp( m_numberUnknowns, 0 ) ;
   for( ; itr != end ; ++itr )
   {
      tmp.sub( index, ( *itr )->initialUnknowns( aHelices ) ) ;
      index += ( *itr )->numberOfUnknowns() ;
   }

   return tmp ;
}

HepMatrix
HICompositeFitConstraint::unknownDerivatives(
   const STL_VECTOR( HIFitHelix* )& aHelices ) const
{
   STL_VECTOR( HIFitConstraint* )::const_iterator itr =
      m_constraints->begin();
   STL_VECTOR( HIFitConstraint* )::const_iterator end =
      m_constraints->end() ;

   int indexUnknown = 1 ;
   int indexConstraint = 1 ;
   HepMatrix tmp( m_numberUnknowns, m_numberConstraints, 0 ) ;
   for( ; itr != end ; ++itr )
   {
      tmp.sub( indexUnknown,
	       indexConstraint,
	       ( *itr )->unknownDerivatives( aHelices ) ) ;

      indexUnknown += ( *itr )->numberOfUnknowns() ;
      indexConstraint += ( *itr )->numberOfConstraints() ;
   }

   return tmp ;
}


//
// static member functions
//
