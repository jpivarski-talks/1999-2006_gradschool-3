// -*- C++ -*-
//
// Package:     <McCannFilter>
// Module:      KSurfaceVector
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Apr 23 09:24:59 EDT 2003
// $Id: KSurfaceVector.cc,v 1.1 2003/04/23 16:01:41 cdj Exp $
//
// Revision history
//
// $Log: KSurfaceVector.cc,v $
// Revision 1.1  2003/04/23 16:01:41  cdj
// fixed double deletion bug by creating new class, KSurfaceVector, to manage the memory
//

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

// user include files
//#include "Experiment/report.h"
#include "McCannFilter/KSurfaceVector.h"

#include "HelixIntersection/HICompositeSurface.h"

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "McCannFilter.KSurfaceVector" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: KSurfaceVector.cc,v 1.1 2003/04/23 16:01:41 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
//KSurfaceVector::KSurfaceVector()
//{
//}

// KSurfaceVector::KSurfaceVector( const KSurfaceVector& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

KSurfaceVector::~KSurfaceVector()
{
   STL_VECTOR( HIIntersectionSurface* )::iterator itr = begin() ;
   for( ; itr != end() ; ++itr )
   {
      if( ( *itr )->isComposite() )
      {
         ( ( HICompositeSurface* )( *itr ) )->deleteConditions() ;
      }
      delete *itr ;
   }
}

//
// assignment operators
//
// const KSurfaceVector& KSurfaceVector::operator=( const KSurfaceVector& rhs )
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
KSurfaceVector::clearSurfaces()
{
   STL_VECTOR( HIIntersectionSurface* )::iterator itr = begin() ;
   for( ; itr != end() ; ++itr )
   {
      if( ( *itr )->isComposite() )
      {
         ( ( HICompositeSurface* )( *itr ) )->deleteConditions() ;
      }
      delete *itr ;
   }
   clear();
}
//
// const member functions
//

//
// static member functions
//
