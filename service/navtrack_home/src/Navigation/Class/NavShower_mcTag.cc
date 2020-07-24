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
// $Id: NavShower_mcTag.cc,v 1.2 2003/02/03 20:16:18 bkh Exp $
//
// Revision history
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

#include "Navigation/NavShower.h"
#include "Navigation/NavShowerServer.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kReport = "Navigation.NavShower_mcTag" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavShower_mcTag.cc,v 1.2 2003/02/03 20:16:18 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//


//
// const member functions
//


FAItem< MCParticle >
NavShower::mcTag() const 
{
   return FAItem< MCParticle > ( m_server.mcTag( identifier() ) ) ;
}

FAItem< MCCCTagsByShower >
NavShower::mcTags() const 
{
   return FAItem< MCCCTagsByShower > ( m_server.mcTags( identifier() ) ) ;
}





