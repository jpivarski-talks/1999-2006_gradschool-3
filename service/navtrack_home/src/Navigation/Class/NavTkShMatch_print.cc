// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTkShMatch_print
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Mon Jun 21 17:28:50 EDT 1999
// $Id: NavTkShMatch_print.cc,v 1.2 2002/08/16 15:27:20 cleo3 Exp $
//
// Revision history
//
// $Log: NavTkShMatch_print.cc,v $
// Revision 1.2  2002/08/16 15:27:20  cleo3
// replaced iostream.h with C++/iostream.h
//
// Revision 1.1  2000/08/11 00:21:00  bkh
// Add operator<< functionality to these classes
//
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#include "C++Std/iostream.h"
#include <iomanip.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"

#include "FrameAccess/FATable.h"
#include "FrameAccess/FAConstPtrTable.h"

#include "Navigation/NavTkShMatch.h"
#include "Navigation/NavTkShMatchServer.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kReport = "Navigation.NavTkShMatch" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavTkShMatch_print.cc,v 1.2 2002/08/16 15:27:20 cleo3 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

ostream& operator<<( ostream& os, const NavTkShMatch& nTSM )
{
   os << " NavTkShMatch Id=" << nTSM.identifier() 
      << " matches NavTrack Id=" << nTSM.trackId()
      << " with NavShower id(s) = " ;

   const NavTkShMatch::ShowerIdList& sIds ( nTSM.showerIds() ) ;
   for( NavTkShMatch::ShowerIdList::const_iterator iS ( sIds.begin() ) ;
	iS != sIds.end() ; ++iS )
   {
      os << *iS ;
   }
   
   os << "\n" ;

   os  << setiosflags( ios::fixed )
       << " It has matched energy = " 
       << setw(6) << setprecision(3) << nTSM.matchedEnergy() 
       << ", E/p = " << setw(6) << setprecision(3) << nTSM.eOverP() 
       << ", & match distance = "
       << setw(6) << setprecision(3) << nTSM.distance() << "\n\n" ;

   return os ;
}


//
// static member functions
//
