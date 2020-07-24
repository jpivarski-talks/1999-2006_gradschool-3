// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavEtaToGG_mctag
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Fri Jun 25 17:29:00 EDT 1999
// $Id: NavEtaToGG_mcTag.cc,v 1.2 2003/06/04 20:17:31 tom Exp $
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
#include "Navigation/NavShowerServer.h"

#include "FrameAccess/FAItem.h"
#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "Navigation/MCCCEtaTagger.h"
#include "MCInfo/MCParticleProperty/MCParticleProperty.h"
#include "MCInfo/MCDecayTree/MCDecayTree.h"
#include "Navigation/NavEtaToGG.h"

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

static const char* const kReport = "Navigation.NavShowerServer" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavEtaToGG_mcTag.cc,v 1.2 2003/06/04 20:17:31 tom Exp $";
static const char* const kTagString = "$Name:  $";


FAItem<MCParticle> 
NavEtaToGG::mcTag() const 
{
   if( 0 == m_mcTag )
   {
      MCCCEtaTagger tagger ( hiEnShower().record().frame() ) ;
      const_cast<NavEtaToGG*>(this)->m_mcTag = 
	 tagger.matchMCEtaToNavEta(*this) ;
   }
   return FAItem<MCParticle>( m_mcTag ) ;
}
