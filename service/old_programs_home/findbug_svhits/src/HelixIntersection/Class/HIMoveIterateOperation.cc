// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIMoveIterateOperation
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Wed May 19 23:49:13 EDT 1999
// $Id: HIMoveIterateOperation.cc,v 1.5 1999/08/27 20:14:48 wsun Exp $
//
// Revision history
//
// $Log: HIMoveIterateOperation.cc,v $
// Revision 1.5  1999/08/27 20:14:48  wsun
// New signature for HIHelixOperation::perform().
//
// Revision 1.4  1999/07/12 06:51:12  wsun
// Cleaned up code.
//
// Revision 1.3  1999/06/24 22:25:01  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.2  1999/06/18 19:25:59  wsun
// More improvements to low momentum fitting.
//
// Revision 1.1  1999/06/02 23:05:03  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HIMoveIterateOperation.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HIMoveIterateOperation" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIMoveIterateOperation.cc,v 1.5 1999/08/27 20:14:48 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIMoveIterateOperation::HIMoveIterateOperation( HIHelixOperation* aOperation )
   : m_operation( aOperation ),
     m_stepArcLength( 0. ),
     m_numberIterations( 0 )
{
}

// HIMoveIterateOperation::HIMoveIterateOperation( const HIMoveIterateOperation& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIMoveIterateOperation::~HIMoveIterateOperation()
{
   delete m_operation ;
}

//
// assignment operators
//
// const HIMoveIterateOperation& HIMoveIterateOperation::operator=( const HIMoveIterateOperation& rhs )
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
HIMoveIterateOperation::perform(
   HIHelix& aHelixAfterOperation,
   const HIHelix& aHelixBeforeOperation,
   const HIHelix& aHelixAtLastIntersection,
   HIIntersectionSurface& aSurface,
   const HIIntersectionSurface& aLastSurface ) const
{
   if( m_stepArcLength == 0. )
   {
      return true ;
   }

//    aHelixAfterOperation.saveCurvatureAndEnergy() ;
//    aHelixAfterOperation.setUpdateErrorMatrix( false ) ;

   // Cast away the const on aHelixBeforeOperation.
   HIHelix* helixNonConst = const_cast< HIHelix* >( &aHelixBeforeOperation ) ;

   DABoolean success ;

   for( int i = 0 ; i < m_numberIterations ; ++i )
   {
      if( aHelixAfterOperation.moveByArcLength(  m_stepArcLength ) !=
	  KTMoveControl::kMoveOK )
      {
	 return false ;
      }

      // The operations following this one assume that aHelixBeforeOperation
      // is the helix AFTER it has been moved, so we must modify it...

      *helixNonConst = aHelixAfterOperation ;
      success = m_operation->perform( aHelixAfterOperation,
				      aHelixBeforeOperation,
				      aHelixAtLastIntersection,
				      aSurface,
				      aLastSurface ) ;
      if( !success ) return false ;
   }

   aHelixAfterOperation.setUpdateErrorMatrix( true ) ;

   return success ;
}


//
// static member functions
//
