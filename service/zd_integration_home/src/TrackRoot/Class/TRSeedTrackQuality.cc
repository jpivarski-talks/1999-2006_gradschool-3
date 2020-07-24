// -*- C++ -*-
//
// Package:     <TrackRoot>
// Module:      TRSeedTrackQuality
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Jul 30 23:47:22 EDT 2000
// $Id: TRSeedTrackQuality.cc,v 1.4 2002/08/16 19:36:56 cleo3 Exp $
//
// Revision history
//
// $Log: TRSeedTrackQuality.cc,v $
// Revision 1.4  2002/08/16 19:36:56  cleo3
// replaced iostream.h with C++/iostream.h
//
// Revision 1.3  2001/01/05 05:27:31  lyon
// Added LayerMap and other changes to quality objects
//
// Revision 1.2  2000/10/28 20:34:42  ajm36
// added numberLayers, numberMultipleHits, useForExtrapolation to seed qualities
//
// Revision 1.1  2000/08/01 23:25:08  wsun
// First submission.
//

#include "Experiment/Experiment.h"
#include "C++Std/iostream.h"
#include <string.h>
#include <ctype.h>

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "TrackRoot/TRSeedTrackQuality.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "TrackRoot.TRSeedTrackQuality" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: TRSeedTrackQuality.cc,v 1.4 2002/08/16 19:36:56 cleo3 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
TRSeedTrackQuality::TRSeedTrackQuality()
   : TRTrackQuality(),
     m_chiSquare(          0.0 ),
     m_degreesOfFreedom(   0 ),
     m_numberHitsExpected( 0 ),
     m_numberLayers(       0 ),
     m_numberMultipleHits( 0 ),
     m_zEndpointUsed(      false ),
     m_originUsed(         false ),
     m_useForExtrapolation(false)
{
}

TRSeedTrackQuality::TRSeedTrackQuality( Identifier identifier,
					double chiSquare,
					int degreesOfFreedom,
					int numberHits,
					int numberHitsExpected,
                                        int numberLayers,
                                        int numberMultipleHits,
					DABoolean zEndpointUsed,
					DABoolean originUsed,
                                        DABoolean useForExtrapolation)
   : TRTrackQuality( identifier, numberHits ),
     m_chiSquare(          chiSquare ) ,
     m_degreesOfFreedom(   degreesOfFreedom ) ,
     m_numberHitsExpected( numberHitsExpected ) ,
     m_numberLayers(       numberLayers),
     m_numberMultipleHits( numberMultipleHits),
     m_zEndpointUsed(      zEndpointUsed ) ,
     m_originUsed(         originUsed ),
     m_useForExtrapolation(useForExtrapolation)
{}

// TRSeedTrackQuality::TRSeedTrackQuality( const TRSeedTrackQuality& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

TRSeedTrackQuality::~TRSeedTrackQuality()
{
}

//
// assignment operators
//
// const TRSeedTrackQuality& TRSeedTrackQuality::operator=( const TRSeedTrackQuality& rhs )
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
void TRSeedTrackQuality::reportFlag(ostream& s, const char* text, 
				   DABoolean flag) const
{
  
  s << " " << text << "=";

  if ( flag ) 
  {
    s << "TRUE";
  }
  else
  {
    s << "false";
  }
}


void TRSeedTrackQuality::printout(ostream& s) const
{
  // Make a nice output report
  s << "TRSeedTrackQuality: \n";

  
  s << " ChiSquare = " << chiSquare() << "  nDOF = " << degreesOfFreedom() 
    << "\n";
  
  s << " # Hits Expected = " << numberHitsExpected() 
    << " # Hit layers = " << numberLayers()
    << " # multiple hits = " << numberMultipleHits() << "\n";
  
  reportFlag(s, "Z_Endpoint_used", zEndpointUsed());
  reportFlag(s, "origin_used", originUsed());
  reportFlag(s, "ok_for_extrapolation", useForExtrapolation() );
  
  s << "\n";
}

//
// static member functions
//

