// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavMuonId_print
// 
// Description: 
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Thu Jun  1 08:59:25 EDT 2000
// $Id: NavMuonId_print.cc,v 1.2 2002/08/16 15:27:19 cleo3 Exp $
//
// Revision history
//
// $Log: NavMuonId_print.cc,v $
// Revision 1.2  2002/08/16 15:27:19  cleo3
// replaced iostream.h with C++/iostream.h
//
// Revision 1.1  2000/08/11 00:20:58  bkh
// Add operator<< functionality to these classes
//
//

#include "Experiment/Experiment.h"

// system include files
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
//#include "Experiment/report.h"
#include "Navigation/NavMuonId.h"
#include "Navigation/NavTrack.h"
#include "Navigation/NavTkShMatch.h"
#include "C3Mu/MuTrack.h"

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

static const char* const kFacilityString = "Navigation.NavMuonId" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavMuonId_print.cc,v 1.2 2002/08/16 15:27:19 cleo3 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

ostream& operator<<( ostream& os, const NavMuonId& aMu )
{
   const string simple1 ( aMu.simpleMuonId1() ? "true" : "false" ) ;
   const string simple2 ( aMu.simpleMuonId2() ? "true" : "false" ) ;
   os << " NavMuonId: " 
      << "simpleMuonId1,2 = " << simple1 
      << ", " << simple2 
      << ", Emat="
      << setiosflags( ios::fixed ) 
      << setw(6) << setprecision(3) << aMu.matchedEnergy()
      << ", depth=" 
      << setw(4) << setprecision(1) << aMu.depth()
      << "\n" ;

   return os ;
}


//
// static member functions
//
