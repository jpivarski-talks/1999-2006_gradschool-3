// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrackMaster
// 
// Description: Master block for NavTrack object, for showers
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Mon Jul  6 14:31:27 EDT 1998
// $Id: NavTrackMaster_shower.cc,v 1.2 2002/08/06 17:35:04 cleo3 Exp $
//
// Revision history
//
// $Log: NavTrackMaster_shower.cc,v $
// Revision 1.2  2002/08/06 17:35:04  cleo3
// NavTrackMaster rather than NavTrack now owns any memory inorder to decrease dependencies seen on Solaris 8
//
// Revision 1.1  2001/10/26 21:53:54  bkh
// Add features to showers and connected region objects
//
// Revision 1.1  2000/04/05 15:43:02  marsh

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
#include "Navigation/NavShower.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAPtrTable.h"
#include "FrameAccess/FAConstPtrTable.h"

// STL classes
#include <map>
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrackMaster" ;

FATable< NavShower > 
NavTrackMaster::navShowers() const 
{
   if( 0 == m_showers )
   {
      FATable<NavShower> showerList;
      extract( dict(), showerList);

      const_cast< NavTrackMaster* >( this )->m_showers = showerList.getContents() ;
   }
   return m_showers ;
}

const FAPtrTable< NavShower >*
NavTrackMaster::bremShowers(
   const NavTrack::Identifier aId ) const
{
   FATable<NavShower> showers( navShowers() );

   const_cast<NavTrackMaster*>(this)->m_bremShowers.push_back( new FAConstPtrTable<NavShower>() );
   FAConstPtrTable<NavShower>* returnValue =
      const_cast<NavTrackMaster*>(this)->m_bremShowers.back();


   const FATable< NavShower >::const_iterator sEnd ( showers.end() ) ;
   for( FATable< NavShower >::const_iterator iS ( showers.begin() ) ;
	iS != sEnd ; ++iS )
   {
      if( iS->bremTrack().valid() )
      {
	 if( aId == ( iS->bremTrack()->identifier()) )
	 {
	    returnValue->insert( &(*iS) ) ;
	 } 
      }
   }

   return returnValue;   
}
