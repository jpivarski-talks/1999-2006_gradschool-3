// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrackMaster_vees
// 
// Description: for muon track info
//
// Implementation:
//
// Author:      Brian Heltsley
// Created:     Mon Jun  5 14:31:27 EDT 2000
// $Id: NavTrackMaster_vees.cc,v 1.2 2003/02/03 20:16:19 bkh Exp $
//
// Revision history
//
// $Log: NavTrackMaster_vees.cc,v $
// Revision 1.2  2003/02/03 20:16:19  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.1  2002/11/07 17:42:04  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
//
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

#include "FrameAccess/extract.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAPtrTable.h"
#include "Navigation/NavKs.h"
#include "Navigation/NavLambda.h"

// STL classes
#include <map>
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrackMaster_vees" ;

const NavKs* 
NavTrackMaster::navKs( const NavTrack::Identifier aId ) const 
{
   if( 0 == m_navKs )
   {
      FATable<NavKs>   ksTracks;
      extract( dict(), ksTracks );

      const_cast< NavTrackMaster* >( this )->m_navKs = ksTracks.getContents() ;
   }

   const NavKs* found ( 0 ) ;
   double chisq ( 99999999. ) ;

   for( FATable<NavKs>::const_iterator iK ( m_navKs->begin() ) ;
	iK != m_navKs->end() ; ++iK )
   {
      if( iK->piPlus(). identifier() == aId ||
	  iK->piMinus().identifier() == aId    )
      {
	 if( 0 != found )
	 {
	    const double newChisq ( iK->kShort().fitChiSquare() ) ;
	    if( newChisq < chisq )
	    {
	       chisq = newChisq ;
	       found = &(*iK) ;
	    }
	 }
	 else
	 {
	    chisq = iK->kShort().fitChiSquare() ;
	    found = &(*iK) ;
	 }
      }
   }

   return found ;
}

const NavLambda* 
NavTrackMaster::navLambda( const NavTrack::Identifier aId ) const 
{
   const NavLambda* laPtr ( 0 ) ;
   if( 0 == m_navLambda )
   {
      FATable<NavLambda>   laTracks;
      extract( dict(),     laTracks );

      const_cast< NavTrackMaster* >( this )->m_navLambda = 
	 laTracks.getContents() ;
   }

   const NavLambda* found ( 0 ) ;
   double chisq ( 99999999. ) ;

   for( FATable<NavLambda>::const_iterator iL ( m_navLambda->begin() ) ;
	iL != m_navLambda->end() ; ++iL )
   {
      if( iL->pion()  .identifier() == aId ||
	  iL->proton().identifier() == aId    )
      {
	 if( 0 != found )
	 {
	    const double newChisq ( iL->lambda().fitChiSquare() ) ;
	    if( newChisq < chisq )
	    {
	       chisq = newChisq ;
	       found = &(*iL) ;
	    }
	 }
	 else
	 {
	    chisq = iL->lambda().fitChiSquare() ;
	    found = &(*iL) ;
	 }
      }
   }

   return found ;
}
