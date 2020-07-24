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
// $Id: NavShowerServer.cc,v 1.18 2003/02/03 20:16:16 bkh Exp $
//
// Revision history
//
// $Log: NavShowerServer.cc,v $
// Revision 1.18  2003/02/03 20:16:16  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.17  2002/11/21 16:52:39  bkh
// Add new NavShower fcn giving back track matches in same con reg
// Require NavTrack mcTag to have same charge as track
//
// Revision 1.16  2002/08/08 16:55:08  cleo3
// NavShower no longer owns any additional memory
//
// Revision 1.15  2002/04/10 18:46:14  bkh
// Speed up way connected region information is obtained with direct
// lattice access
//
// Revision 1.14  2002/03/25 11:52:55  bkh
// various optimizations for speed
//
// Revision 1.13  2002/03/21 01:51:42  cdj
// NavShower::noTrackMatch now just looks at Lattice
//
// Revision 1.12  2001/11/13 16:05:35  bkh
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
#include "Navigation/NavTkShMatch.h"

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
static const char* const kIdString  = "$Id: NavShowerServer.cc,v 1.18 2003/02/03 20:16:16 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//
//
// assignment operators
//
// const NavShowerServer& NavShowerServer::operator=( const NavShowerServer& rhs )
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

const Record&
NavShowerServer::record() const
{
   return m_record ;
}

const NavShower& 
NavShowerServer::navShower(
   const NavShower::Identifier aNavShowerId           ) const 
{
   if( !m_navShower.valid() )
   {
      extract( m_record, 
	       const_cast< NavShowerServer* >( this )->m_navShower ) ;
   }
   return m_navShower[ aNavShowerId ] ;
}

FATable< NavShower >
NavShowerServer::navShowers() const
{
   if( !m_navShower.valid() )
   {
      extract( m_record, 
	       const_cast< NavShowerServer* >( this )->m_navShower ) ;
   }
   return m_navShower ;
}

FATable< NavTrack >
NavShowerServer::navTracks() const
{
   if( !m_navTrack.valid() )
   {
      extract( m_record, 
	       const_cast< NavShowerServer* >( this )->m_navTrack ) ;
   }
   return m_navTrack ;
}

FATable< NavTkShMatch > 
NavShowerServer::tkShMatches() const 
{
   if( !m_match.valid() )
   {
      extract( m_record,
	       const_cast< NavShowerServer* >( this )->m_match ) ;
   }
   return m_match ;
}

DABoolean
NavShowerServer::hasTrackMatch( const NavShower::Identifier aId ) const
{
   if( 0 == m_tkShLattice ) {
      FAItem< TrackShowerLattice > lattice;
      extract( m_record, lattice );
      const_cast<NavShowerServer*>(this)->m_tkShLattice = &(*lattice);
   }

   const TrackShowerLattice::VectorLeftID * tracks = m_tkShLattice->vLeftGivenRight( aId);
   return ( (tracks != 0) &&( ! tracks->empty()) ) ;
}

const FAPtrTable< NavTkShMatch>*
NavShowerServer::matchedTracks(
   const NavShower::Identifier aNavShowerId           ) const 
{
   FAConstPtrTable<NavTkShMatch>* trackMatches =
      new FAConstPtrTable< NavTkShMatch > ;
   const_cast<NavShowerServer*>(this)->m_matchedTracks.push_back(trackMatches);
   FATable< NavTkShMatch > table ( tkShMatches() ) ;
   FATable< NavTkShMatch >::const_iterator tEnd ( table.end() ) ;
   for( FATable< NavTkShMatch >::const_iterator iM ( table.begin() ) ;
	iM != tEnd ; ++iM )
   {
      const NavTkShMatch* match ( &(*iM) ) ;
      const NavTkShMatch::ShowerIdList& idList ( match->showerIds() ) ;
      if( find( idList.begin() ,
		idList.end()   ,
		aNavShowerId     ) != idList.end() )
      {
	 trackMatches->insert( match ) ;
      }
   }
   return trackMatches ;   
};

const FAPtrTable< NavTkShMatch>*
NavShowerServer::conRegMatchedTracks(
   const NavShower::Identifier aNavShowerId           ) const 
{
   typedef NavConReg::ShowerIdList SList ;
   const SList& conRegShowerIds ( conReg( aNavShowerId).showerIds() ) ;

   FAConstPtrTable<NavTkShMatch>* trackMatches =
      new FAConstPtrTable< NavTkShMatch > ;
   const_cast<NavShowerServer*>(this)->
      m_conRegMatchedTracks.push_back(trackMatches);

   FATable< NavTkShMatch > table ( tkShMatches() ) ;
   FATable< NavTkShMatch >::const_iterator tEnd ( table.end() ) ;
   for( FATable< NavTkShMatch >::const_iterator iM ( table.begin() ) ;
	iM != tEnd ; ++iM )
   {
      const NavTkShMatch* match ( &(*iM) ) ;
      const NavTkShMatch::ShowerIdList& idList ( match->showerIds() ) ;

      SList::const_iterator cEnd ( conRegShowerIds.end() ) ;
      for( SList::const_iterator iL ( conRegShowerIds.begin() ) ;
	   iL != cEnd ; ++iL )
      {
	 const NavShower::Identifier id ( *iL ) ;
	 if( find( idList.begin() ,
		   idList.end()   ,
		   id     ) != idList.end() )
	 {
	    trackMatches->insert( match ) ;
	 }
      }
   }
   return trackMatches ;   
};

const NavConReg& 
NavShowerServer::conReg(
   const NavShower::Identifier aNavShowerId           ) const 
{
   if( !m_conReg.valid() )
   {
      extract( m_record, 
	       const_cast< NavShowerServer* >( this )->m_conReg ) ;
   }

   if( 0 == m_crLattice )
   {
      FAItem< CcConRegShowerLattice > lat ;
      extract( m_record,              lat ) ;
      const_cast< NavShowerServer* >( this )->m_crLattice = &(*lat);
   }

   const CcConRegShowerLattice::LeftID* ptr (
      m_crLattice->leftIDGivenRight( aNavShowerId ) ) ;

   if( 0 == ptr )
   {
      report( ERROR,  kReport ) << "No NavConReg found for showerId="
				<< aNavShowerId << endl ;
      assert( false ) ;
      ::exit( 1 ) ;
   }
   return m_conReg[ *ptr ] ;
}

const CcEnergyHit& 
NavShowerServer::energyHit(
   const CcEnergyHit::Identifier aHitId               ) const 
{
   if( !m_energyHits.valid() )
   {
      extract( m_record, 
	       const_cast< NavShowerServer* >( this )->m_energyHits ) ;
   }
   return m_energyHits[ aHitId ] ;
}

const CcHitShowerLattice::Links& 
NavShowerServer::links(
   const NavShower::Identifier aNavShowerId           ) const 
{
   if( !m_lattice.valid() )
   {
      extract( m_record, 
	       const_cast< NavShowerServer* >( this )->m_lattice ) ;
   }

   const CcHitShowerLattice::Links& links (
      (*m_lattice).linksGivenRight( aNavShowerId ) ) ;

   return links ;
}


double
NavShowerServer::angSimpleMatch(
   const NavShower::Identifier aNavShowerId           ) const 
{
   if( 0 == m_simpleMatch ) 
   {
      const NavTrack* trk ( simpleMatch( aNavShowerId ) ) ; // triggers filling
   }
   return ( aNavShowerId > 0 && aNavShowerId <= m_angSimpleMatch->size() ?
	    (*m_angSimpleMatch)[ aNavShowerId - 1 ] : 999 ) ;
}

const NavTrack*
NavShowerServer::simpleMatch(
   const NavShower::Identifier aNavShowerId           ) const 
{
   if( 0 == m_simpleMatch ) 
   {
      const NavShower& show ( navShower( 1 ) ) ; // triggrs filling m_navShower
      const_cast<NavShowerServer*>(this)->m_simpleMatch =
	 new STL_VECTOR( const NavTrack* ) ( m_navShower.size() ,
					     (const NavTrack*) 0 ) ;
      const_cast<NavShowerServer*>(this)->m_angSimpleMatch =
	 new STL_VECTOR( double          ) ( m_navShower.size() ,
					     (double) 999. ) ;
      FATable<NavTrack>  trks   ;
      extract( m_record, trks ) ;
      FATable<NavTrack >::const_iterator trkBeg ( trks.begin() ) ;
      FATable<NavTrack >::const_iterator trkEnd ( trks.end()   ) ;

      for( FATable<NavTrack>::const_iterator iT ( trkBeg ) ;
	   iT != trkEnd ; ++iT )
      {
	 const NavShower* iS ( 0 ) ;
	 const double angle ( simpleMatch( iS, *iT ) ) ;

	 if( 0 != iS )
	 {
	    const UInt16 index ( iS->identifier()-1 ) ;
	    (*const_cast<NavShowerServer*>(this)->m_simpleMatch    )[index] =
	       &*iT ;
	    (*const_cast<NavShowerServer*>(this)->m_angSimpleMatch )[index] =
	       angle ;
	 }
      }
   }
   return ( aNavShowerId > 0 && aNavShowerId <= m_simpleMatch->size() ?
	    (*m_simpleMatch)[ aNavShowerId - 1 ] : 0 ) ;
}

double 
NavShowerServer::simpleMatch ( const NavShower*& aShower ,
			       const NavTrack&   aTrack    ) const 
{
   static const HepPoint3D k_origin  ( 0,0,0       ) ;
   static const double     k_zEnd    ( 1.42        ) ;
   static const double     k_twopi   ( 2*M_PI      ) ;
   static const double     k_bRad    ( 1.03 + 0.05 ) ;
   static const double     k_rMinEnd ( 0.43        ) ;
   static const double     k_rMaxEnd ( 0.94        ) ;
   static const double     k_SMALL   ( 1.e-10      ) ;

   DABoolean  barrel ( false ) ; // good projection to barrel
   DABoolean  endcap ( false ) ; // good projection to endcap
   double     ang    ( 9999  ) ; // return variable angle
   double     cang   ( -1.   ) ; // cosine of angle for speed
   HepPoint3D point  (0,0,9999); // point of intersection

   //---------------------------------------------------- get helix
   FAItem< TRHelixFit > jHelix ( aTrack.pionHelix() ) ;
   if( !jHelix.valid() )
   {
      report( ERROR, kReport ) << "No helix fit stored ... exiting" 
			       << endl ;
      return 9999. ;
   }
   FAItem< KTHelix > iHelix ( &(*jHelix) ) ;

   KTHelix*  pHelix   ( 0 ) ;
   DABoolean newHelix ( false ) ;
   if( k_origin != iHelix->referencePoint() )
   {
      pHelix = new KTHelix ( *iHelix, false ) ; // don't bring error matrix!
      newHelix = true ;
      KTHelix::ValueType distMov ;
      KTHelix::MoveStatus stat ( pHelix->moveToReferencePoint(
	 k_origin, distMov, KTMoveControl::kDirectionBackward ) ) ;
      if( KTMoveControl::kMoveOK != stat )
      {
	 report( WARNING, kReport ) << "Helix move not ok for "
				    << "track " 
				    << aTrack.identifier() << endl ;
	 return 9999. ;
      }
      iHelix = FAItem< KTHelix >( pHelix ) ;
   }
   //----------------------------------------------------- done getting helix

   //-------------------------------------------------------------- method doing calculation by hand
   const KTHelix::ValueType ct ( iHelix->cotTheta()  ) ;

   const KTHelix::ValueType d0 ( iHelix->d0()        ) ;
   const KTHelix::ValueType fi ( iHelix->phi0()      ) ;
   const KTHelix::ValueType cv ( iHelix->curvature() ) ;
   const KTHelix::ValueType z0 ( iHelix->z0()        ) ;
   const double sfi ( sin( fi ) ) ;
   const double cfi ( cos( fi ) ) ;
   const double cu // if too small set it to be a non-zero small number
      ( fabs( cv ) < k_SMALL ? ( cv < 0 ? -k_SMALL : k_SMALL ) : cv ) ;
   const double r2D ( 0.5/cu ) ;

   const HepPoint3D  ctr2D ( ( r2D + d0 )*HepVector3D( -sfi, cfi, 0 ) ) ;
   const HepVector3D cUnit ( ctr2D.unit() ) ; // unit vector from origin to ctr

   const double sign  ( r2D > 0 ? 1 : -1 ) ; // sign of curvature
   const double hRad  ( fabs( r2D ) )      ; // helix radius
   const double cRad  ( ctr2D.mag() )      ; // dist to ctr of helix from orig
   const double len   ( ( hRad*hRad + cRad*cRad - k_bRad*k_bRad )/( 2*cRad ) );
   const double zFact ( ct*hRad ) ; // get it ready for later

   if( -( k_bRad - d0 )  < len          &&    // require len to be physical
       hRad              > fabs( len )     )
   {
      const double bend ( acos( len/hRad ) ) ; // bend angle in plane
      const double alfa ( M_PI - bend ) ;
      const double salfa ( sign>0 ? 2*M_PI - alfa : alfa ) ;

      const HepRotateZ3D rotat  ( salfa ) ; // rotation transform

      const HepVector3D  vec ( hRad*HepPoint3D( rotat*cUnit ) ) ;

      const HepPoint3D   barInt2D ( ctr2D + vec ) ;

      point = HepPoint3D( barInt2D.x(), barInt2D.y(), z0 + zFact*bend ) ;

      barrel = ( k_zEnd > fabs( point.z() ) ) ;
   }

   if( !barrel                &&
       fabs( ct ) > k_SMALL       )
   {  // do endcap
      const double zlimit ( ct > 0 ? k_zEnd : -k_zEnd ) ;
      const double bendR   ( fabs( ( zlimit - z0 )/zFact ) ) ;
      if( bendR < k_twopi )
      {
	 const double alf ( M_PI - bendR ) ;
	 const double alf2 ( alf<0 && sign>0 ? -alf :
			     ( alf<0 && sign<0 ? k_twopi+alf :
			       ( alf > 0 && sign<0 ? alf : k_twopi-alf )));
	 const HepRotateZ3D rotZ   ( alf2 ) ;
	 const HepVector3D  vec    ( hRad*ctr2D.unit() ) ;
	 const HepPoint3D   p2D    ( ctr2D + rotZ*vec ) ;
	 point = HepPoint3D( p2D.x(), p2D.y(), zlimit ) ;
	 endcap = true ;
      }
   }
   //---------------------------------------------end by hand projection
/*
   //-------- method using official projection codes in KTHelix object---
   //HepPoint3D point2  ( 0,0,9999 ) ;
   KTHelix    tHelix ( *iHelix, false ) ; // writeable copy 
   KTHelix::ValueType distMov ;

   KTHelix::MoveStatus stat ( tHelix.moveToRadius(
      k_bRad, distMov, KTMoveControl::kDirectionForward ) ) ;
   if( KTMoveControl::kMoveOK == stat )
   {
      point = tHelix.referencePoint() ;
   }
   if( fabs( point.z() ) < k_zEnd )
   {
      barrel = true ;
//      if( ( HepVector3D( point - point2 ).mag() > 0.01 ) )
//	 cout << "&&& point="<<point<<", p2="<<point2<<", ct=" << ct << ",r2D="
//	      << r2D << ", bend=" << ( point.z()-z0 )/ct << endl;
   }
   else
   {
      point = HepPoint3D( 0,0,9999 ) ;
      tHelix = KTHelix( *iHelix, false ) ; // fresh copy to move in z
      stat = tHelix.moveToZPosition(
	 (ct<0?-1:1)*k_zEnd , distMov, KTMoveControl::kDirectionForward ) ;
      if( KTMoveControl::kMoveOK == stat )
      {
	 point = tHelix.referencePoint() ;
	 endcap = true ;
      }
      else
      {
	 if( endcap ) cout << "**Failed move**" << stat << endl ;
      }
   }
   //------------------------------------------- end KTHelix method
   */
//   if( endcap && ( HepVector3D( point - point2 ).mag() > 0.01 ) )
//      cout << "&&& point="<<point<<", p2="<<point2<<", ct=" << ct << ",r2D="
//	   << r2D << ", bend=" << ( point.z()-z0 )/ct << endl;
   //---------------------------------------- loop over showers ---------

   const double pointmag2 ( point.mag2() ) ;

   if( barrel || endcap )
   {
      FATable<NavShower>::const_iterator shwBeg ( m_navShower.begin() ) ;
      FATable<NavShower>::const_iterator shwEnd ( m_navShower.end()   ) ;
      for( FATable<NavShower>::const_iterator iS ( shwBeg ) ;
	   iS != shwEnd ; ++iS )
      {
	 if( !iS->attributes().hot() )
	 {
	    const HepPoint3D& sPos0 ( iS->attributes().position() ) ;
	    const double cangle 
	       ( sPos0.dot( point )/sqrt(sPos0.mag2()*pointmag2) ) ;

	    if( cangle >  cang     &&
		cangle >  0.93969     ) // 20 degrees
	    {
	       cang    = cangle ;
	       aShower = &*iS  ;
	    }
	 }
      }
   }

   if( 0 != aShower )
   {
      if( cang>1 )
      {
	 ang=0 ;
      }
      else
      {
	 if( cang<-1 )
	 {
	    ang=999. ;
	 }
	 else
	 {
	    ang = acos( cang ) ;
	 }
      }
   }

   if( newHelix )
   {
      delete pHelix ;
   }
   return ang ;
}

//
// static member functions
//


