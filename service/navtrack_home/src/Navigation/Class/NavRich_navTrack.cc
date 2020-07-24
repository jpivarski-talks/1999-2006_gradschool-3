// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavRich
// 
// Description:
//
// Implementation:
//
// Author:      Tomasz Skwarnicki
// Created:     Tue Jul 20 10:46:38 EDT 2000
// $Id: NavRich_navTrack.cc,v 1.3 2001/12/21 20:46:01 cdj Exp $
//
// Revision history
//
// $Log: NavRich_navTrack.cc,v $
// Revision 1.3  2001/12/21 20:46:01  cdj
// modified NavRich related code so no longer need to load RichTracks
//
// Revision 1.2  2000/11/20 00:05:53  ts
// build NavRich on top of RichTrackPrototype
//
// Revision 1.1  2000/07/27 01:31:43  ts
// NavRich added
//
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
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "Navigation/NavRich.h"
#include "Navigation/NavRichServer.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
#include <algorithm>
//#include <utility>


FAItem<NavTrack>  NavRich::navTrack() const
{
   if( m_server.richTracksMethod().valid() ){
      if( trackId() != 0 ) {
          return m_server.navTrack(trackId());
      }
   }
   return FAItem<NavTrack>(0);
}

