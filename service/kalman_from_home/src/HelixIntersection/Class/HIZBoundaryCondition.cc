// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIZBoundaryCondition
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Sep 25 15:17:08 EDT 1998
// $Id: HIZBoundaryCondition.cc,v 1.2 1999/04/27 22:06:37 wsun Exp $
//
// Revision history
//
// $Log: HIZBoundaryCondition.cc,v $
// Revision 1.2  1999/04/27 22:06:37  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:28  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HIZBoundaryCondition.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HelixIntersection.HIZBoundaryCondition" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HIZBoundaryCondition::HIZBoundaryCondition()
{
}

HIZBoundaryCondition::HIZBoundaryCondition( Meters aZmin, Meters aZmax ) :
   m_zmin( aZmin ), m_zmax( aZmax )
{
}

// HIZBoundaryCondition::HIZBoundaryCondition( const HIZBoundaryCondition& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIZBoundaryCondition::~HIZBoundaryCondition()
{
}

//
// assignment operators
//
// const HIZBoundaryCondition& HIZBoundaryCondition::operator=( const HIZBoundaryCondition& rhs )
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
HIZBoundaryCondition::satisfied( const HIHelix& aHelix,
				 HIIntersectionSurface& aSurface ) const
{
   Meters helixZ = aHelix.position().z() ;

   if ( helixZ >= m_zmin && helixZ <= m_zmax )
   {
      return true ;
   }
   else
   {
      if( printDiagnostics() )
      {
	 report( INFO, kFacilityString )
	    << "Helix failed z boundary condition: [" << m_zmin << ","
	    << m_zmax << "], helix z " << helixZ << endl ;
      }
      return false ;
   }
}

//
// static member functions
//
