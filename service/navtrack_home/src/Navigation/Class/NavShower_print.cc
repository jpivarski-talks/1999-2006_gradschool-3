// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavShower_print
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Tue Jun 22 10:46:38 EDT 1999
// $Id: NavShower_print.cc,v 1.3 2002/08/16 15:27:20 cleo3 Exp $
//
// Revision history
//
// $Log: NavShower_print.cc,v $
// Revision 1.3  2002/08/16 15:27:20  cleo3
// replaced iostream.h with C++/iostream.h
//
// Revision 1.2  2000/08/31 16:32:41  bkh
// Pretty up output and add other showers matched to same track to output
//
// Revision 1.1  2000/08/11 00:20:59  bkh
// Add operator<< functionality to these classes
//
//

#include "Experiment/Experiment.h"

// system include files
#include "C++Std/iostream.h"
#include <iomanip.h>
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

#include "Navigation/NavShower.h"
#include "Navigation/NavShowerServer.h"
#include "Navigation/NavConReg.h"
#include "Navigation/NavTkShMatch.h"

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
static const char* const kIdString  = "$Id: NavShower_print.cc,v 1.3 2002/08/16 15:27:20 cleo3 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

ostream& operator<<( ostream& os, const NavShower& nSh )
{
   os << nSh.attributes() ;

   if( nSh.noTrackMatch() )
   {
      os << " -----> is *NOT* matched to any charged track.\n" ; 
   }
   else
   {
      os << " -----> is *MATCHED* to chgd track " ;
      DABoolean first ( true ) ;
      FATable< NavTkShMatch > matches ( nSh.trackMatches() ) ;
      for( FATable< NavTkShMatch >::const_iterator iM ( matches.begin() ) ;
	   iM != matches.end() ; ++iM )
      {
	 if( !first )
	 {
	    os << "                                   " ;
	 }
	 os << setiosflags( ios::fixed ) << setw(2) 
	    << iM->trackId() << " w/ E/p = " 
	    << setw(6) << setprecision(3) << iM->eOverP()
	    << " & distance = "
	    << setw(6) << setprecision(3) << iM->distance()
	    << "\n" ;

	 const NavTkShMatch::ShowerIdList& sList ( iM->showerIds() ) ;
	 if( 1 < sList.size() )
	 {
	    os << "                                      " ;
	    os << "along with shower(s) " ;
	    for( NavTkShMatch::ShowerIdList::const_iterator iS 
		    ( sList.begin() ) ; iS != sList.end() ; ++iS )
	    {
	       const NavTkShMatch::ShowerIdList::value_type id ( *iS ) ;
	       if( nSh.identifier() != id )
	       {
		  os << setw(3) << *iS << "," ;
	       }
	    }
	    os << "\n" ;
	 }

	 first = false ;
      }
   }

   os << "     It has assigned energy hits : \n" ;
   FATable< CcAssignedEnergyHit > hits ( nSh.assignedHits() ) ;
   for( FATable< CcAssignedEnergyHit >::const_iterator iH ( hits.begin() ) ;
	iH != hits.end() ; ++iH )
   {
      os << (*iH) ;
   }
   os << "\n" ;

   return os ;
}


//
// static member functions
//
