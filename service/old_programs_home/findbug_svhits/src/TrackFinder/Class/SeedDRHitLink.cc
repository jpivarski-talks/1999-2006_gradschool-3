// -*- C++ -*-
//
// Package:     <TrackFinder>
// Module:      SeedDRHitLink
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Mon Jan 31 17:07:35 EST 2000
// $Id: SeedDRHitLink.cc,v 1.5 2000/10/29 02:43:38 wsun Exp $
//
// Revision history
//
// $Log: SeedDRHitLink.cc,v $
// Revision 1.5  2000/10/29 02:43:38  wsun
// kFITABLE --> kFITTABLE
//
// Revision 1.4  2000/10/25 18:35:03  lyon
// Remove fittingweights from SeedDRHitLink and its storage helper
//
// Revision 1.3  2000/10/25 15:07:17  lyon
// Added new fitFlag to SeedDRHitLink link data
//
// Revision 1.2  2000/09/29 19:53:49  mccann
// Added fittingweight as member function of linkdata.
//
// Revision 1.1  2000/02/08 18:29:47  lyon
// Add stuff necessary for storage helpers
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
#include "TrackFinder/SeedDRHitLink.h"

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

static const char* const kFacilityString = "TrackFinder.SeedDRHitLink" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: SeedDRHitLink.cc,v 1.5 2000/10/29 02:43:38 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
SeedDRHitLink::SeedDRHitLink() :
  m_signedDriftDistance( 0.0 ),
  m_pull( 0.0 ),
  m_fitFlag( kFITTABLE )
{}

SeedDRHitLink::SeedDRHitLink(float signedDriftDistance, float pull) : 
  m_signedDriftDistance( signedDriftDistance ),
  m_pull( pull ),
  m_fitFlag( kFITTABLE )
{}

SeedDRHitLink::SeedDRHitLink(float signedDriftDistance, float pull,
			     fitFlagEnum fitFlag) : 
  m_signedDriftDistance( signedDriftDistance ),
  m_pull( pull ),
  m_fitFlag( fitFlag )
{}


// SeedDRHitLink::SeedDRHitLink( const SeedDRHitLink& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

SeedDRHitLink::~SeedDRHitLink()
{}

//
// assignment operators
//
// const SeedDRHitLink& SeedDRHitLink::operator=( const SeedDRHitLink& rhs )
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
