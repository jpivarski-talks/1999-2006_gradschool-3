// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavShowerServer
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Fri Jun 25 17:29:00 EDT 1999
// $Id: NavShowerServer_condes.cc,v 1.10 2003/02/03 20:16:17 bkh Exp $
//
// Revision history
//
// $Log: NavShowerServer_condes.cc,v $
// Revision 1.10  2003/02/03 20:16:17  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.9  2003/01/30 19:21:05  cleo3
// minor changes needed to compile under g++
//
// Revision 1.8  2002/11/21 16:52:40  bkh
// Add new NavShower fcn giving back track matches in same con reg
// Require NavTrack mcTag to have same charge as track
//
// Revision 1.7  2002/11/07 17:42:02  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
// Revision 1.6  2002/08/23 21:01:35  cdj
// fixed memory leak in NavShowerServer (should start loops at begin() not end())
//
// Revision 1.5  2002/08/08 16:55:08  cleo3
// NavShower no longer owns any additional memory
//
// Revision 1.4  2002/04/10 18:46:14  bkh
// Speed up way connected region information is obtained with direct
// lattice access
//
// Revision 1.3  2002/03/21 01:51:43  cdj
// NavShower::noTrackMatch now just looks at Lattice
//
// Revision 1.2  2001/12/13 20:54:02  bkh
// Implement eta-->gamgam access from NavShower just as with pi0
// For track match object, get energy and momentum directly because
//    shower energy can have run-time adjustments
//
// Revision 1.1  2001/11/13 16:05:35  bkh
// Separated off pi0 pieces to avoid making everyone link to PhotonDecays
//
// Revision 1.11  2001/11/09 20:26:39  bkh
// Added in shower-->pi0  functionality
//
// Revision 1.10  2001/10/30 16:55:40  bkh
// Fix bug(s) with con regs
//
// Revision 1.9  2001/10/26 21:53:53  bkh
// Add features to showers and connected region objects
//
// Revision 1.8  2001/06/26 18:01:41  cdj
// modified to work with new Lattice interface
//
// Revision 1.7  2001/04/03 16:57:57  bkh
// Implement lists of nearby showers/tracks based on distance
//
// Revision 1.6  2000/10/04 20:00:20  bkh
// Remove pi0-vetoing from NavShower; bad idea
//
// Revision 1.5  2000/10/03 18:40:48  bkh
// Add pi0/eta access to NavShower for vetoing, etc.
//
// Revision 1.4  1999/12/31 23:14:25  bkh
// Eliminate obsolete checking of valid() after extracts
//
// Revision 1.3  1999/12/06 18:12:43  bkh
// Change interface to use FATables where possible not vectors
//
// Revision 1.2  1999/08/09 16:25:11  bkh
// Massive renaming
//
// Revision 1.1  1999/06/29 21:10:39  bkh
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
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Navigation/NavShowerServer.h"
#include "Navigation/NavPi0ToGG.h"
#include "Navigation/NavEtaToGG.h"
#include "MCCCTagger/MCCCTags.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FAConstPtrTable.h"

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

static const char* const kReport = "Navigation.NavShowerServer" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavShowerServer_condes.cc,v 1.10 2003/02/03 20:16:17 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavShowerServer::NavShowerServer( const Record& aRecord ) :
   m_record     ( aRecord ) ,
   m_navShower  () ,
   m_match      () ,
   m_tkShLattice(0),
   m_conReg     () ,
   m_energyHits () ,
   m_lattice    () ,
   m_navTrack   () ,
   m_navPi0     ( 0 ) ,
   m_navEta     ( 0 ) ,
   m_crLattice  ( 0 ) ,
   m_mcTag      ( 0 ) ,
   m_mcTags     ( 0 ) ,
   m_simpleMatch( 0 ) ,
   m_angSimpleMatch ( 0 ) 
{
}

// NavShowerServer::NavShowerServer( const NavShowerServer& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

NavShowerServer::~NavShowerServer()
{
   STL_VECTOR(FAConstPtrTable<NavPi0ToGG>*)::iterator itPi0End = 
      m_matchedPi0s.end();

   for(STL_VECTOR(FAConstPtrTable<NavPi0ToGG>*)::iterator itPi0 =
	  m_matchedPi0s.begin();
       itPi0 != itPi0End;
       ++itPi0 ) {
      delete *itPi0;
   }


   STL_VECTOR(FAConstPtrTable<NavEtaToGG>*)::iterator itEtaEnd = 
      m_matchedEtas.end();

   for(STL_VECTOR(FAConstPtrTable<NavEtaToGG>*)::iterator itEta =
	  m_matchedEtas.begin();
       itEta != itEtaEnd;
       ++itEta ) {
      delete *itEta;
   }


   STL_VECTOR(FAConstPtrTable<NavTkShMatch>*)::iterator itTrackEnd = 
      m_matchedTracks.end();

   for(STL_VECTOR(FAConstPtrTable<NavTkShMatch>*)::iterator itTrack =
	  m_matchedTracks.begin();
       itTrack != itTrackEnd;
       ++itTrack ) {
      delete *itTrack;
   }

   itTrackEnd =  m_conRegMatchedTracks.end();

   for(STL_VECTOR(FAConstPtrTable<NavTkShMatch>*)::iterator itTrackC =
	  m_conRegMatchedTracks.begin();
       itTrackC != itTrackEnd;
       ++itTrackC ) {
      delete *itTrackC;
   }

   delete m_mcTag ;

   delete m_angSimpleMatch ;
   delete m_simpleMatch ;
}




