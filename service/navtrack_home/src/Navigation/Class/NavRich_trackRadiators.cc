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
// $Id: NavRich_trackRadiators.cc,v 1.2 2001/10/01 00:37:07 cdj Exp $
//
// Revision history
//
// $Log: NavRich_trackRadiators.cc,v $
// Revision 1.2  2001/10/01 00:37:07  cdj
// switched to RichAngles package
//
// Revision 1.1  2000/07/27 01:31:45  ts
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
#include "RichAngles/RichTrackRadiators.h"
#include "RichAngles/RichTrajectoryRadiators.h"
#include "RichAngles/RichTrajectoryRadiator.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
#include <algorithm>
//#include <utility>


static const float kCmToM = 0.01;

FAItem<RichTrackRadiators>  NavRich::richTrackRadiators() const
{
  return m_server.richTrackRadiators(m_RichTrack.richTrkID());
}

RichSmallCountType  NavRich::numberOfRadiators( RichHypothesisIDType iHyp ) const
{
   if( iHyp < RichHypotheses::kHypMax ){

   if( richTrackRadiators().valid() ){  
    const RichTrajectoryRadiators* richTrajectoryRadiators = richTrackRadiators()->trajectoryRadiators(iHyp);
    if( 0 != richTrajectoryRadiators ){
          return richTrajectoryRadiators->numberOfRadiators();
    }
   }

   }
   return 0;
}

NavRich::Unitless NavRich::sigmasToAnEdge( RichHypothesisIDType iHyp ) const
{
   if( iHyp < RichHypotheses::kHypMax ){

   if( richTrackRadiators().valid() ){  
    const RichTrajectoryRadiators* richTrajectoryRadiators = richTrackRadiators()->trajectoryRadiators(iHyp);
    if( 0 != richTrajectoryRadiators ){
          return richTrajectoryRadiators->sigmasToAnEdge();
    }
   }

   }
   return 0.0;
}

FAItem<RichTrajectoryRadiator> NavRich::richTrajectoryRadiator( RichHypothesisIDType iHyp ) const
{
   if( iHyp < RichHypotheses::kHypMax ){

   if( richTrackRadiators().valid() ){  
    const RichTrajectoryRadiators* richTrajectoryRadiators = richTrackRadiators()->trajectoryRadiators(iHyp);
    if( 0 != richTrajectoryRadiators ){
         float lenMax=0.0;
         int   iradMax= -1;
         for( int irad=0; irad< richTrajectoryRadiators->numberOfRadiators(); ++irad ){
              const RichTrajectoryRadiator& richTrajectoryRadiator = 
                          richTrajectoryRadiators->richTrajectoryRadiators()[irad];
                          float len = richTrajectoryRadiator.length() * richTrajectoryRadiator.probability();
                          if( len > lenMax ){
                                 lenMax =len;
                                 iradMax=irad;
			  }
	 }
         if( -1 != iradMax ){ 
            return FAItem<RichTrajectoryRadiator>(&(richTrajectoryRadiators->richTrajectoryRadiators()[iradMax]));
	 } 
    }
   }

   }
   return FAItem<RichTrajectoryRadiator>(0);
}


RichRadiatorIDType NavRich::radiatorId( RichHypothesisIDType iHyp ) const
{
   if( iHyp < RichHypotheses::kHypMax ){

    FAItem<RichTrajectoryRadiator> trajectoryRadiator= richTrajectoryRadiator(iHyp);
    if( trajectoryRadiator.valid() ){
        return trajectoryRadiator->radiatorID();
    }

   }
    return 0;
}
      

NavRich::Meters NavRich::radLength( RichHypothesisIDType iHyp ) const
{
   if( iHyp < RichHypotheses::kHypMax ){

   if( richTrackRadiators().valid() ){  
    const RichTrajectoryRadiators* richTrajectoryRadiators = richTrackRadiators()->trajectoryRadiators(iHyp);
    if( 0 != richTrajectoryRadiators ){
         float lensum=0.0;
         float probsum=0;
         for( int irad=0; irad< richTrajectoryRadiators->numberOfRadiators(); ++irad ){
              const RichTrajectoryRadiator& richTrajectoryRadiator = 
                          richTrajectoryRadiators->richTrajectoryRadiators()[irad];
              lensum += richTrajectoryRadiator.length() * richTrajectoryRadiator.probability();
              probsum += richTrajectoryRadiator.probability();
	 }
         return ((probsum>0.0)?(lensum/probsum):lensum)*kCmToM;
    }
   }

   }
   return 0.0;
}





