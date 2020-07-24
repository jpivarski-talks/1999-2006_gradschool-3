// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrackMaster
// 
// Description: Master block for NavTrack object, inherits from TDTrackMaster.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Mon Jul  6 14:31:27 EDT 1998
// $Id: NavTrackMaster.cc,v 1.25 2003/10/08 23:08:09 mccann Exp $
//
// Revision history
//
// $Log: NavTrackMaster.cc,v $
// Revision 1.25  2003/10/08 23:08:09  mccann
// added ZD NavTrack interface
//
// Revision 1.24  2003/07/29 20:18:27  bkh
// Add trkman, pca-BeamSpot fcns to NavTrack
//
// Revision 1.23  2003/02/04 18:13:09  bkh
// Separate off record function
//
// Revision 1.22  2003/02/03 20:16:19  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.21  2003/01/30 19:21:05  cleo3
// minor changes needed to compile under g++
//
// Revision 1.20  2002/11/07 17:42:04  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
// Revision 1.19  2002/08/06 17:34:59  cleo3
// NavTrackMaster rather than NavTrack now owns any memory inorder to decrease dependencies seen on Solaris 8
//
// Revision 1.18  2002/03/19 20:25:06  cdj
// simplified memory handling for NavTrack
//
// Revision 1.17  2000/09/26 21:21:29  lkg
// NavTrackMaster now takes production and usage tags for specification
// of the seed tracks (track finder) and fit tracks (track fitter) to be
// used by the TDTrackMaster base class
//
// Revision 1.16  2000/04/05 15:43:00  marsh
// Split class implementations into separate files based on closely-related
// functions.  This will greatly reduce the amount of linking that most
// users will have to do.
//
// Revision 1.15  1999/12/31 23:14:27  bkh
// Eliminate obsolete checking of valid() after extracts
//
// Revision 1.14  1999/12/22 21:26:26  marsh
// Removed annoying messages when users ask for something that isn't there.
//
// Revision 1.13  1999/09/16 20:38:12  bkh
// Add track shower match info
//
// Revision 1.12  1999/08/02 19:15:21  wsun
// Initialize fit/hit Lattices for exit hypotheses.
//
// Revision 1.11  1999/06/29 21:10:40  bkh
// New classes associated with analysis-level shower object
//
// Revision 1.10  1999/05/24 16:35:52  marsh
// More Lattice interface changes.
//
// Revision 1.9  1999/05/23 21:31:39  marsh
// Updated for new Lattice interface.
//
// Revision 1.8  1999/05/11 13:07:13  marsh
// Changed LtoRmap to RightData
//
// Revision 1.7  1999/05/08 18:34:09  marsh
// Fixed Lattice interface.
//
// Revision 1.6  1999/05/08 00:45:14  marsh
// Added remaining outgoing hypotheses.
//
// Revision 1.5  1999/04/23 21:35:36  marsh
// Changed NavTrack to deliver links, not link data.
//
// Revision 1.4  1999/01/20 19:44:48  marsh
// Added missing '}'
//
// Revision 1.3  1999/01/20 19:23:35  marsh
// Protected against dereferencing null pointers.
//
// Revision 1.2  1998/12/31 22:27:49  marsh
// Release memory that was dynamically allocated.
//
// Revision 1.1.1.1  1998/12/31 18:18:31  marsh
// New library for navigation objects.
//
// Revision 1.2  1998/12/29 22:20:56  marsh
// Mods for providing Lattice info.
//
// Revision 1.1.1.1  1998/07/13 23:26:21  marsh
// New library for user-level analysis objects
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "Navigation/NavTrackMaster.h"
#include "FrameAccess/FAConstPtrTable.h"
#include "Navigation/NavShower.h"

#include "FrameAccess/extract.h"

// STL classes
#include <map>
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrackMaster" ;

//
// static data member definitions
//

//
// constructors and destructor
//
NavTrackMaster::NavTrackMaster( const Record& iRecord,
			     TagPair fitterTags,
			     TagPair finderTags ) :
   TDTrackMaster( iRecord, fitterTags, finderTags ) ,
   m_showers(0),
   m_showerMatch(0),
   m_rich(0),
   m_muTrack(0),
   m_dedx(0),
   m_trkmanInfo(0),
   m_navKs ( 0 ) ,
   m_navLambda ( 0 ) ,
   m_pionDRHitLattice( 0 ) ,
   m_kaonDRHitLattice( 0 ) ,
   m_electronDRHitLattice( 0 ) ,
   m_muonDRHitLattice( 0 ) ,
   m_protonDRHitLattice( 0 ) ,
   m_exitElectronDRHitLattice( 0 ) ,
   m_exitMuonDRHitLattice( 0 ) ,
   m_exitPionDRHitLattice( 0 ) ,
   m_exitKaonDRHitLattice( 0 ) ,
   m_exitProtonDRHitLattice( 0 ) ,
   m_pionZDHitLattice( 0 ) ,
   m_kaonZDHitLattice( 0 ) ,
   m_electronZDHitLattice( 0 ) ,
   m_muonZDHitLattice( 0 ) ,
   m_protonZDHitLattice( 0 ) ,
   m_exitElectronZDHitLattice( 0 ) ,
   m_exitMuonZDHitLattice( 0 ) ,
   m_exitPionZDHitLattice( 0 ) ,
   m_exitKaonZDHitLattice( 0 ) ,
   m_exitProtonZDHitLattice( 0 ) ,
   m_pionSVRHitLattice( 0 ) ,
   m_kaonSVRHitLattice( 0 ) ,
   m_electronSVRHitLattice( 0 ) ,
   m_muonSVRHitLattice( 0 ) ,
   m_protonSVRHitLattice( 0 ) ,
   m_exitElectronSVRHitLattice( 0 ) ,
   m_exitMuonSVRHitLattice( 0 ) ,
   m_exitPionSVRHitLattice( 0 ) ,
   m_exitKaonSVRHitLattice( 0 ) ,
   m_exitProtonSVRHitLattice( 0 ) ,
   m_pionSVZHitLattice( 0 ) ,
   m_kaonSVZHitLattice( 0 ) ,
   m_electronSVZHitLattice( 0 ) ,
   m_muonSVZHitLattice( 0 ) ,
   m_protonSVZHitLattice( 0 ) ,
   m_exitElectronSVZHitLattice( 0 ) ,
   m_exitMuonSVZHitLattice( 0 ) ,
   m_exitPionSVZHitLattice( 0 ) ,
   m_exitKaonSVZHitLattice( 0 ) ,
   m_exitProtonSVZHitLattice( 0 ) ,
   m_seedDRHitLattice( 0 ) ,
   m_seedZDHitLattice( 0 ) ,
   m_seedSVRHitLattice( 0 ) ,
   m_seedSVZHitLattice( 0 ) ,
   m_beamSpot ( 0 ) ,
   m_mcTag ( 0 ) ,
   m_mcTags ( 0 ) 
{
}

NavTrackMaster::~NavTrackMaster()
{
   STL_VECTOR(FAConstPtrTable<NavShower>*)::iterator itEnd = 
      m_bremShowers.end();
   for( STL_VECTOR(FAConstPtrTable<NavShower>*)::iterator itShower =
	   m_bremShowers.begin();
	itShower != itEnd;
	++itShower ) {
      delete *itShower;
   }
   delete m_mcTag ;
}
