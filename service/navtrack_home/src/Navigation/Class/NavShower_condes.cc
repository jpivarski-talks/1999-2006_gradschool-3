// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavShower
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Tue Jun 22 10:46:38 EDT 1999
// $Id: NavShower_condes.cc,v 1.6 2003/02/03 20:16:17 bkh Exp $
//
// Revision history
//
// $Log: NavShower_condes.cc,v $
// Revision 1.6  2003/02/03 20:16:17  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.5  2002/11/21 16:52:40  bkh
// Add new NavShower fcn giving back track matches in same con reg
// Require NavTrack mcTag to have same charge as track
//
// Revision 1.4  2002/08/08 16:55:09  cleo3
// NavShower no longer owns any additional memory
//
// Revision 1.3  2002/03/21 01:51:43  cdj
// NavShower::noTrackMatch now just looks at Lattice
//
// Revision 1.2  2001/12/13 20:54:04  bkh
// Implement eta-->gamgam access from NavShower just as with pi0
// For track match object, get energy and momentum directly because
//    shower energy can have run-time adjustments
//
// Revision 1.1  2001/11/13 16:05:36  bkh
// Separated off pi0 pieces to avoid making everyone link to PhotonDecays
//
// Revision 1.12  2001/11/09 20:26:39  bkh
// Added in shower-->pi0  functionality
//
// Revision 1.11  2001/10/30 16:55:40  bkh
// Fix bug(s) with con regs
//
// Revision 1.10  2001/10/26 21:53:53  bkh
// Add features to showers and connected region objects
//
// Revision 1.9  2001/07/09 20:49:11  bkh
// Fix nearestTracks() implementation to include all matched tracks
//
// Revision 1.8  2001/04/03 16:57:56  bkh
// Implement lists of nearby showers/tracks based on distance
//
// Revision 1.7  2000/10/04 20:00:20  bkh
// Remove pi0-vetoing from NavShower; bad idea
//
// Revision 1.6  2000/10/03 18:40:48  bkh
// Add pi0/eta access to NavShower for vetoing, etc.
//
// Revision 1.5  2000/08/11 00:20:59  bkh
// Add operator<< functionality to these classes
//
// Revision 1.4  2000/01/20 16:02:04  bkh
// Make consistent with new object for hit-shower link datum
//
// Revision 1.3  1999/12/06 18:12:42  bkh
// Change interface to use FATables where possible not vectors
//
// Revision 1.2  1999/08/09 16:25:09  bkh
// Massive renaming
//
// Revision 1.1  1999/06/29 21:10:38  bkh
// New classes associated with analysis-level shower object
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
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAConstPtrTable.h"

#include "Navigation/NavTkShMatch.h"
#include "Navigation/NavShower.h"
#include "Navigation/NavShowerServer.h"
#include "Navigation/NavConReg.h"
#include "Navigation/NavPi0ToGG.h"
#include "Navigation/NavEtaToGG.h"
//#include "PhotonDecays/PhdPi0.h"

#include "KinematicTrajectory/KTKinematicData.h"

#include "C3cc/CcAssignedEnergyHit.h"

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

static const char* const kReport = "Navigation.NavShower_condes" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavShower_condes.cc,v 1.6 2003/02/03 20:16:17 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

NavShower::NavShower( const CcShowerAttributes& aAtts   ,
		      const NavShowerServer&    aServer   ) :
   m_attributes   ( aAtts   ) ,
   m_server       ( aServer ) ,
   m_conRegPtr    ( 0       ) ,
   m_photonPtr    ( 0       ) ,
   m_hasTrackMatch( NavShower::kMatchUnknown),
   m_trackMatches ( 0       ) ,
   m_conRegTrackMatches ( 0       ) ,
   m_assignedHits ( 0       ) ,
   m_nearestMatchedShowers   ( 0 ) ,
   m_nearestUnmatchedShowers ( 0 ) ,
   m_nearestTracks ( 0 ) ,
   m_bremTrack    ( 0 ) ,
   m_hasSimpleMatch ( false ) ,
   m_simpleMatch  ( 0 ) ,
   m_angSimpleMatch ( 999 ) ,
   m_pi0s         ( 0 ) ,
   m_etas         ( 0 ) ,
   m_conReg50     ( false ) ,
   m_noConReg50   ( false )
{
}

// NavShower::NavShower( const NavShower& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

NavShower::~NavShower()
{
   delete m_assignedHits ;
   delete m_photonPtr    ;
   //delete m_trackMatches ;

   delete m_nearestMatchedShowers ;
   delete m_nearestUnmatchedShowers ;
   delete m_nearestTracks ;
   //delete m_pi0s ;
   //delete m_etas ;
}

//
// assignment operators
//
// const NavShower& NavShower::operator=( const NavShower& rhs )
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

