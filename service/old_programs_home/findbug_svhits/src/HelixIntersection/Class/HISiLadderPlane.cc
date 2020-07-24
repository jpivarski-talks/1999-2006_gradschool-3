// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HISiLadderPlane
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Apr 19 18:32:08 EDT 1999
// $Id: HISiLadderPlane.cc,v 1.4 2001/03/29 01:40:41 llh14 Exp $
//
// Revision history
//
// $Log: HISiLadderPlane.cc,v $
// Revision 1.4  2001/03/29 01:40:41  llh14
// Separated Si Rphi and Z Hit Surfaces
//
// Revision 1.3  1999/12/15 20:43:37  wsun
// Updated to new HIIntersectionPlane ctor.
//
// Revision 1.2  1999/04/28 22:57:25  wsun
// Fixes for Solaris.
//
// Revision 1.1  1999/04/27 22:06:32  wsun
// Major reorganization and structural changes.
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
#include "ASiStorePro/ASiLadder.h"
#include "HelixIntersection/HISiLadderPlane.h"

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

static const char* const kFacilityString =
"HelixIntersection.HISiLadderPlane" ;

const string HISiLadderPlane::kSurfaceType = "HISiLadderPlane" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HISiLadderPlane.cc,v 1.4 2001/03/29 01:40:41 llh14 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HISiLadderPlane::HISiLadderPlane()
{
}

HISiLadderPlane::HISiLadderPlane(
   const DGConstMaterialPtr& aInnerMaterial,
   const DGConstMaterialPtr& aOuterMaterial,
   const ASiLadder& aLadder,
   HICondition* aCondition,
   HIHelixOperation* aOperation,
   const HepPoint3D& aCorrectionInSi)
   : HIIntersectionPlane( aInnerMaterial,
			  aOuterMaterial,
			  aLadder.unitVectorYInSi(),
			  aLadder.coorInSi() + aCorrectionInSi,
			  aLadder.halfLengthX(),
			  aCondition,
			  aOperation ),
     m_ladder( &aLadder ),
     m_CorrectionInSi( aCorrectionInSi )
{

}

// HISiLadderPlane::HISiLadderPlane( const HISiLadderPlane& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HISiLadderPlane::~HISiLadderPlane()
{
}

//
// assignment operators
//
// const HISiLadderPlane& HISiLadderPlane::operator=( const HISiLadderPlane& rhs )
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

//
// static member functions
//
