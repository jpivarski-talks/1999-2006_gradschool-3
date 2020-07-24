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
// $Id: NavRich_improvedTrack.cc,v 1.1 2000/07/27 01:31:42 ts Exp $
//
// Revision history
//
// $Log: NavRich_improvedTrack.cc,v $
// Revision 1.1  2000/07/27 01:31:42  ts
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

#include "RichTracksProd/RichTrack.h"
#include "RichImprovedTracksProd/RichImprovedTrack.h"
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



FAItem<RichImprovedTrack>  
NavRich::richImprovedTrack() const
{
  return m_server.richImprovedTrack(m_RichTrack.richTrkID());
}


FAItem<KTHelix> 
NavRich::richImprovedTrajectory( RichHypothesisIDType iHyp ) const
{
  if( iHyp < RichHypotheses::kHypMax ){
  FAItem<RichImprovedTrack> improvedTrack=richImprovedTrack();
  if( improvedTrack.valid() ){
     RichTrajectoryIDType itraj = improvedTrack->hypothesisTrajectory()[iHyp];
     if( itraj != 0 ){
         return FAItem<KTHelix>(&(improvedTrack->trajectory()[ itraj-1 ] ));
     } 
  } }
  return FAItem<KTHelix>(0);
}  

DABoolean 
NavRich::trajectoryWasImproved( RichHypothesisIDType iHyp ) const
{
  if( iHyp < RichHypotheses::kHypMax ){
  FAItem<KTHelix> initialHelix( richTrajectory( iHyp ) );
  if( initialHelix.valid()  ){
     FAItem<KTHelix> improvedHelix( richImprovedTrajectory( iHyp ) );
     if( improvedHelix.valid() ){
         return ( *initialHelix != *improvedHelix );
     }
  } }
  return false;
}

