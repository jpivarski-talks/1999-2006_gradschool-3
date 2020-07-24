// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavKs_mctag
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Fri Jun 25 17:29:00 EDT 1999
// $Id: NavKs_mcTag.cc,v 1.3 2003/06/04 20:17:31 tom Exp $
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
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Navigation/NavTrackMaster.h"
#include "Navigation/NavTrack.h"
#include "Navigation/NavKs.h"

#include "FrameAccess/FAItem.h"
#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "Navigation/MCTKShortTagger.h"
#include "MCInfo/MCDecayTree/MCParticle.h"

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
static const char* const kIdString  = "$Id: NavKs_mcTag.cc,v 1.3 2003/06/04 20:17:31 tom Exp $";
static const char* const kTagString = "$Name:  $";


FAItem<MCParticle> 
NavKs::mcTag() const 
{
   if( 0 == m_mcTag )
   {
      MCTKShortTagger tagger ( piPlus().record().frame() ) ;
      const_cast<NavKs*>(this)->m_mcTag = 
	 tagger.matchMCKsToNavKs(*this) ;
   }
   return FAItem<MCParticle>( m_mcTag ) ;
}
