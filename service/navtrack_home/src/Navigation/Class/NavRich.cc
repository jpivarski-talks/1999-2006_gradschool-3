// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavRich
// 
// Description:
//
// Implementation:
//
// Author:      Tomasz Skwarnicki
// Created:     Tue Jul 20 10:46:38 EDT 2000
// $Id: NavRich.cc,v 1.2 2000/11/20 00:05:51 ts Exp $
//
// Revision history
//
// $Log: NavRich.cc,v $
// Revision 1.2  2000/11/20 00:05:51  ts
// build NavRich on top of RichTrackPrototype
//
// Revision 1.1  2000/07/27 01:31:34  ts
// NavRich added
//
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "Navigation/NavRich.h"
#include "Navigation/NavRichServer.h"

#include "RichTrackPrototypesProd/RichTrackPrototype.h"
#include "C3Rich/RichHypotheses.h"


// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kReport = "Navigation.NavRich" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavRich.cc,v 1.2 2000/11/20 00:05:51 ts Exp $";
static const char* const kTagString = "$Name:  $";
//
// static data member definitions
//

//
// constructors and destructor
//


NavRich::NavRich(  const RichTrackPrototype& aRichTrackPrototype,
		   const NavRichServer& aServer   ):
   m_RichTrack    ( aRichTrackPrototype ) ,
   m_server       ( aServer ) ,
   m_HypNumberOfPhotonsInAverage ( 0 ),
   m_HypAverageAngleStandardDeviation( 0 ),
   m_HypAverageAngleTotalError( 0 ),
   m_HypWasAnalyzed( 0 ),
   m_CRHypInCache( RichHypotheses::kHypMax ),
   m_CRInCache( 0 ),
   m_MatchedRichConnectedRegionPosition( 0 ),
   m_MatchedRichConnectedRegionPositionCov( 0 )
{
}



// NavRich::NavRich( const NavRich& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

NavRich::~NavRich()
{
  delete [] m_HypNumberOfPhotonsInAverage;
  delete [] m_HypAverageAngleStandardDeviation;
  delete [] m_HypAverageAngleTotalError;
  delete [] m_HypWasAnalyzed;
  
  delete  m_MatchedRichConnectedRegionPosition;
  delete  m_MatchedRichConnectedRegionPositionCov;
}

//
// assignment operators
//
// const NavRich& NavRich::operator=( const NavRich& rhs )
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

NavRich::Identifier                
NavRich::identifier()   const 
{
   return m_RichTrack.identifier() ;
}

FAItem<RichTrackPrototype>
NavRich::richTrackPrototype()   const 
{
   return FAItem<RichTrackPrototype>(&m_RichTrack); 
}


RichTrackIDType NavRich::trackId() const
{
  return m_RichTrack.tag();
}

