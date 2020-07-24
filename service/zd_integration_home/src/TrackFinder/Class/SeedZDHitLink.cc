// -*- C++ -*-
//
// Package:     <TrackFinder>
// Module:      SeedZDHitLink
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Mon Jan 31 17:07:35 EST 2000
// $Id: SeedZDHitLink.cc,v 1.1 2002/09/27 21:02:45 bkh Exp $
//
// Revision history
//
// $Log: SeedZDHitLink.cc,v $
// Revision 1.1  2002/09/27 21:02:45  bkh
// Add ZD capability
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
#include "TrackFinder/SeedZDHitLink.h"

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

static const char* const kFacilityString = "TrackFinder.SeedZDHitLink" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: SeedZDHitLink.cc,v 1.1 2002/09/27 21:02:45 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
SeedZDHitLink::SeedZDHitLink() :
  m_signedDriftDistance( 0.0 ),
  m_pull( 0.0 ),
  m_fitFlag( kFITTABLE )
{}

SeedZDHitLink::SeedZDHitLink(float signedDriftDistance, float pull) : 
  m_signedDriftDistance( signedDriftDistance ),
  m_pull( pull ),
  m_fitFlag( kFITTABLE )
{}

SeedZDHitLink::SeedZDHitLink(float signedDriftDistance, float pull,
			     fitFlagEnum fitFlag) : 
  m_signedDriftDistance( signedDriftDistance ),
  m_pull( pull ),
  m_fitFlag( fitFlag )
{}


// SeedZDHitLink::SeedZDHitLink( const SeedZDHitLink& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

SeedZDHitLink::~SeedZDHitLink()
{}

//
// assignment operators
//
// const SeedZDHitLink& SeedZDHitLink::operator=( const SeedZDHitLink& rhs )
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
