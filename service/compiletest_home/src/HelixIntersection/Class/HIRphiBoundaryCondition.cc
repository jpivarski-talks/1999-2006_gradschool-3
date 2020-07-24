// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIRphiBoundaryCondition
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Oct 29 18:52:08 EST 1998
// $Id: HIRphiBoundaryCondition.cc,v 1.3 1999/08/03 21:56:33 wsun Exp $
//
// Revision history
//
// $Log: HIRphiBoundaryCondition.cc,v $
// Revision 1.3  1999/08/03 21:56:33  wsun
// abs --> fabs
//
// Revision 1.2  1999/04/27 22:06:31  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:24  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "CalibratedData/CalibratedHit.h"

#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HIRphiBoundaryCondition.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HelixIntersection.HIRphiBoundaryCondition" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HIRphiBoundaryCondition::HIRphiBoundaryCondition()
{
}

HIRphiBoundaryCondition::HIRphiBoundaryCondition(
   const HepPoint3D& aCenterPoint,
   const Hep3Vector& aTangentVector,
   Meters aHalfWidth )
   : m_centerPoint( aCenterPoint ),
     m_tangentVector( aTangentVector.unit() ),
     m_halfWidth( aHalfWidth )
{
}

// HIRphiBoundaryCondition::HIRphiBoundaryCondition( const HIRphiBoundaryCondition& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIRphiBoundaryCondition::~HIRphiBoundaryCondition()
{
}

//
// assignment operators
//
// const HIRphiBoundaryCondition& HIRphiBoundaryCondition::operator=( const HIRphiBoundaryCondition& rhs )
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

//
// const member functions
//

DABoolean
HIRphiBoundaryCondition::satisfied(
   const HIHelix& aHelix,
   HIIntersectionSurface& aSurface ) const
{
   Hep3Vector differenceVector = aHelix.position() - m_centerPoint ;
   if( fabs( differenceVector.dot( m_tangentVector ) ) <= m_halfWidth )
   {
      return true ;
   }
   else
   {
      if( printDiagnostics() )
      {
	 report( INFO, kFacilityString )
	    << "Helix failed r-phi boundary condition." << endl ;
      }
      return false ;
   }
}

//
// static member functions
//
