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
// $Id: NavRich_richTrack.cc,v 1.1 2000/11/20 00:05:54 ts Exp $
//
// Revision history
//
// $Log: NavRich_richTrack.cc,v $
// Revision 1.1  2000/11/20 00:05:54  ts
// build NavRich on top of RichTrackPrototype
//
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

#include "RichTracksProd/RichTrack.h"
#include "C3Rich/RichHypotheses.h"

#include "KinematicTrajectory/KTHelix.h"


// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
#include <algorithm>
//#include <utility>


FAItem<RichTrack>
NavRich::richTrack()   const 
{
   return m_server.richTrack(m_RichTrack.richTrkID());
}

FAItem<KTHelix> 
NavRich::richTrajectory( RichHypothesisIDType iHyp ) const
{
  if( iHyp < RichHypotheses::kHypMax ){
  FAItem<RichTrack> track=richTrack();
  if( track.valid() ){
     RichTrajectoryIDType itraj = track->hypothesisTrajectory()[iHyp];
     if( itraj != 0 ){
         return FAItem<KTHelix>(&(track->trajectory()[ itraj-1 ] ));
     } 
  } }
  return FAItem<KTHelix>(0);
}  
