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
// $Id: NavShower.cc,v 1.20 2003/02/03 20:16:16 bkh Exp $
//
// Revision history
//
// $Log: NavShower.cc,v $
// Revision 1.20  2003/02/03 20:16:16  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.19  2002/11/21 16:52:39  bkh
// Add new NavShower fcn giving back track matches in same con reg
// Require NavTrack mcTag to have same charge as track
//
// Revision 1.18  2002/08/08 16:55:08  cleo3
// NavShower no longer owns any additional memory
//
// Revision 1.17  2002/05/23 20:49:28  bkh
// add uncertainties in position (large) and correlation with momentum components
//
// Revision 1.16  2002/03/26 22:46:51  bkh
// Speed up with inlining per cdj request
//
// Revision 1.15  2002/03/25 11:52:55  bkh
// various optimizations for speed
//
// Revision 1.14  2002/03/21 01:51:42  cdj
// NavShower::noTrackMatch now just looks at Lattice
//
// Revision 1.13  2001/11/13 16:05:35  bkh
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

#include "DataHandler/Record.h"
#include "Navigation/NavTkShMatch.h"
#include "Navigation/NavShower.h"
#include "Navigation/NavShowerServer.h"
#include "Navigation/NavConReg.h"
//#include "Navigation/NavPi0ToGG.h"

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

static const char* const kReport = "Navigation.NavShower" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavShower.cc,v 1.20 2003/02/03 20:16:16 bkh Exp $";
static const char* const kTagString = "$Name:  $";

static const CcAngle k_bremAngleCut = 0.100 ; // 100mrad cut

//
// static data member definitions
//

//
// constructors and destructor
//

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

const NavConReg&  
NavShower::conReg()       const 
{
   if( 0 == m_conRegPtr )
   {
      const_cast< NavShower* >( this )->m_conRegPtr =
	 &( m_server.conReg( identifier() ) ) ;
   }
   return *m_conRegPtr ;
}

const KTKinematicData&    
NavShower::photon()       const 
{
   if( 0 == m_photonPtr )
   {
      const CcShowerAttributes& atts ( attributes() ) ;

      const HepSymMatrix& four ( atts.errMatMom() ) ;
      HepSymMatrix seven ( 7, 0 ) ; // init to zeroes

      const double f11 ( four.fast(1,1) ) ;
      const double f22 ( four.fast(2,2) ) ;
      const double f33 ( four.fast(3,3) ) ;

      seven.fast(1,1) = f11 ;
      seven.fast(2,2) = f22 ;
      seven.fast(3,3) = f33 ;
      seven.fast(4,4) = four.fast(4,4) ;
      seven.fast(2,1) = four.fast(2,1) ;
      seven.fast(3,1) = four.fast(3,1) ;
      seven.fast(3,2) = four.fast(3,2) ;
      seven.fast(4,1) = four.fast(4,1) ;
      seven.fast(4,2) = four.fast(4,2) ;
      seven.fast(4,3) = four.fast(4,3) ;

      // here are terms for origin (unknown to within ~1 meter)
      seven.fast(5,5) = 1 ;
      seven.fast(6,6) = 1 ;
      seven.fast(7,7) = 1 ;

      assert( f11 >= 0 ) ;
      assert( f22 >= 0 ) ;
      assert( f33 >= 0 ) ;

      seven.fast(5,1) = -sqrt(f11) ; // correlation terms
      seven.fast(6,2) = -sqrt(f22) ;
      seven.fast(7,3) = -sqrt(f33) ;

      const_cast< NavShower* >( this )->m_photonPtr = 
	 new KTKinematicData( atts.momentum() ,
			      atts.origin()   ,
			      0               ,
			      0               ,
			      seven            ) ;
      assert( 0 != m_photonPtr ) ;
   }
   return *m_photonPtr ;
}

DABoolean                 
NavShower::noTrackMatch() const 
{
   if( m_hasTrackMatch == kMatchUnknown ) {
      const_cast<NavShower*>(this)->m_hasTrackMatch = 
	 ( m_server.hasTrackMatch(identifier())? kMatchTrue : kMatchFalse);
   }
   return (m_hasTrackMatch == kMatchFalse); 
}

DABoolean                 
NavShower::noTrackMatchConReg() const 
{
   return noTrackMatch() && conReg().noTrackMatchConReg() ;
}

FATable< NavTkShMatch >
NavShower::trackMatches() const 
{
   if( 0 == m_trackMatches ) 
   {
      const_cast< NavShower* >( this )->m_trackMatches =
	 m_server.matchedTracks( identifier() );
   }
   return FATable< NavTkShMatch > ( m_trackMatches ) ;
}

FATable< NavTkShMatch >
NavShower::conRegTrackMatches() const 
{
   if( 0 == m_conRegTrackMatches ) 
   {
      const_cast< NavShower* >( this )->m_conRegTrackMatches =
	 m_server.conRegMatchedTracks( identifier() );
   }
   return FATable< NavTkShMatch > ( m_conRegTrackMatches ) ;
}

FATable< CcAssignedEnergyHit >
NavShower::assignedHits() const 
{
   if( 0 == m_assignedHits ) 
   {
      const_cast< NavShower* >( this )->m_assignedHits =
	 new FAPtrTable< CcAssignedEnergyHit > ;

      const CcHitShowerLattice::Links& links 
	 ( m_server.links( identifier() ) ) ;

      CcAssignedEnergyHit::Identifier id ( 0 ) ;
      CcHitShowerLattice::LinkItr lEnd ( links.end() ) ;
      for( CcHitShowerLattice::LinkItr iLink ( links.begin() ) ;
	   iLink != lEnd ; ++iLink )
      {
	 const CcHitShowerLattice::Link& link ( *( *iLink ) ) ;
	 const CcEnergyHit::Identifier hitId ( *( link.leftID() ) ) ;
	 const CcEnergyHit& hit ( m_server.energyHit( hitId ) ) ;
	 CcAssignedEnergyHit* assHit ( new CcAssignedEnergyHit(
	    ++id, hit, hit.energy()*link.linkData().fraction() ) ) ;
	 m_assignedHits->insert( assHit ) ;
      }
   }
   return FATable< CcAssignedEnergyHit > ( m_assignedHits ) ;
}


const NavShower::ShowerPtrList&
NavShower::nearestMatchedShowers()   const 
{
   if( 0 == m_nearestMatchedShowers )
   {
      const_cast< NavShower* >( this )->m_nearestMatchedShowers =
	 new ShowerPtrList ;

      FATable< NavShower > table ( m_server.navShowers() ) ;
      FATable< NavShower >::const_iterator tEnd ( table.end() ) ;
      for( FATable< NavShower >::const_iterator iS ( table.begin() ) ;
	   iS != tEnd ; ++iS )
      {
	 const NavShower* navSh ( &( *iS ) ) ;
	 if( !navSh->noTrackMatch() && navSh != this )
	 {
	    m_nearestMatchedShowers->push_back( navSh ) ;
	 }
      }

      sort( m_nearestMatchedShowers->begin(), 
	    m_nearestMatchedShowers->end() ,
	    CompareShowersByDistance(*this)) ;
   }
   return *m_nearestMatchedShowers ;
}

const NavShower::ShowerPtrList&
NavShower::nearestUnmatchedShowers() const 
{
   if( 0 == m_nearestUnmatchedShowers )
   {
      const_cast< NavShower* >( this )->m_nearestUnmatchedShowers =
	 new ShowerPtrList ;

      FATable< NavShower > table ( m_server.navShowers() ) ;
      FATable< NavShower >::const_iterator tEnd ( table.end() ) ;
      for( FATable< NavShower >::const_iterator iS ( table.begin() ) ;
	   iS != tEnd ; ++iS )
      {
	 const NavShower* navSh ( &( *iS ) ) ;
	 if( navSh->noTrackMatch() && navSh != this )
	 {
	    m_nearestUnmatchedShowers->push_back( navSh ) ;
	 }
      }

      sort( m_nearestUnmatchedShowers->begin() ,
	    m_nearestUnmatchedShowers->end()   , 
	    CompareShowersByDistance( *this )   ) ;
   }
   return *m_nearestUnmatchedShowers ;
}

const NavShower::TrackPtrList&
NavShower::nearestTracks()           const 
{
   if( 0 == m_nearestTracks )
   {
      const_cast< NavShower* >( this )->m_nearestTracks = new TrackPtrList ;

      FATable< NavTkShMatch > table ( m_server.tkShMatches() ) ;
      FATable< NavTkShMatch >::const_iterator mEnd ( table.end() ) ;
      for( FATable< NavTkShMatch >::const_iterator iM ( table.begin() ) ;
	   iM != mEnd ; ++iM )
      {
	 const NavTkShMatch& match ( *iM ) ;

	 m_nearestTracks->push_back( &match.navTrack() ) ;
      }

      sort( m_nearestTracks->begin(), m_nearestTracks->end() , 
	    CompareShowerToTrack( *this ) ) ;
   }
   return *m_nearestTracks ;
}

FAItem< NavTrack >
NavShower::bremTrack()           const 
{
   if( 0 == m_bremTrack )
   {
      if( noTrackMatch() )
      {
	 CcAngle ang ( M_PI ) ;
	 FATable< NavTrack > table ( m_server.navTracks() ) ;
	 FATable< NavTrack >::const_iterator tEnd ( table.end() ) ;
	 for( FATable< NavTrack >::const_iterator iT ( table.begin() ) ;
	      iT != tEnd ; ++iT )
	 {
	    const NavTrack& track ( *iT ) ;

	    assert( track.pionFit().valid() ) ;

	    const CcAngle angle ( track.pionFit()->momentum().angle( 
	       this->attributes().momentum() ) ) ;

	    if( angle < k_bremAngleCut && angle < ang )
	    {
	       const_cast< NavShower* >( this )->m_bremTrack = &track ;
	       ang = angle ;
	    }
	 }
      }
   }
   return FAItem< NavTrack >( m_bremTrack ) ;
}

CcMeters                       
NavShower::distance( const NavShower& aSh ) const 
{
   return Hep3Vector(      attributes().position() -
		       aSh.attributes().position()    ).mag() ;
}

CcMeters                       
NavShower::distance( const NavTrack&  aTr ) const 
{
   return ( !aTr.trackShowerMatch().valid() ? 9.99 :
	    Hep3Vector(         attributes().position() -
			aTr.trackShowerMatch()->navShowers().
			front().attributes().position() ).mag() );
}


const Record&
NavShower::record() const
{
   return m_server.record() ;
}

DABoolean
NavShower::noSimpleMatch() const
{
   return !simpleMatch().valid() ;
}

FAItem< NavTrack >
NavShower::simpleMatch() const
{
   if( !m_hasSimpleMatch )
   {
      const_cast<NavShower*>(this)->m_angSimpleMatch = m_server.angSimpleMatch( identifier() ) ;
      const_cast<NavShower*>(this)->m_simpleMatch = m_server.simpleMatch( identifier() ) ;
      const_cast<NavShower*>(this)->m_hasSimpleMatch = true ;
   }
   return FAItem< NavTrack > ( m_simpleMatch ) ;
}

double
NavShower::angSimpleMatch() const
{
   if( !m_hasSimpleMatch )
   {
      FAItem< NavTrack > track ( simpleMatch() ) ; // triggers filling
   }
   return m_angSimpleMatch ;
}


DABoolean
NavShower::noTrackMatchConReg50() const
{
   if( !m_conReg50 )
   {
      const_cast<NavShower*>(this)->m_conReg50 = !false ;

      const NavShower* sh ( conReg().matchedShower().valid() ?
			    &*conReg().matchedShower() : 0 ) ;
      const_cast<NavShower*>(this)->m_noConReg50 =
	 noTrackMatch() &&
	 ( noTrackMatchConReg() ||
	   ( 0 != sh &&
	     attributes().energy() > 0.5*sh->attributes().energy() ) ) ;
   }
   return m_noConReg50 ;
}
//
// static member functions
//

