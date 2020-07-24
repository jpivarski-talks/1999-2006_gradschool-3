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
// $Id: NavRich_connectedRegion.cc,v 1.2 2000/07/31 18:43:36 ts Exp $
//
// Revision history
//
// $Log: NavRich_connectedRegion.cc,v $
// Revision 1.2  2000/07/31 18:43:36  ts
// correct indexing bug
//
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

#include "RichTrackClusterMatchProd/RichTrackCRegionMatch.h"
#include "RichClustering/RichConnectedRegion.h"
#include "C3Rich/RichHypotheses.h"



FAItem<RichConnectedRegion>     NavRich::richConnectedRegion( RichHypothesisIDType iHyp ) const
{
   if( iHyp < RichHypotheses::kHypMax ){
        RichTrajectoryIDType itraj = m_RichTrack.hypothesisTrajectory()[iHyp];
        if( itraj != 0 ){
           if( richTrackCRegionMatch().valid() ){
           RichConnectedRegionIDType crId = richTrackCRegionMatch()->connectedRegion()[itraj-1];
               if( 0 != crId ){
                   return m_server.richConnectedRegion(crId);
               }
	   }
	}
   }
   return FAItem<RichConnectedRegion>(0);
}

NavRich::ADCcounts NavRich::matchedRichConnectedRegionPulseHeight( RichHypothesisIDType iHyp ) const
{
   if( iHyp < RichHypotheses::kHypMax ){
        FAItem<RichConnectedRegion> connectedRegion = richConnectedRegion(iHyp);
        if( connectedRegion.valid() ){
               return   connectedRegion->pulseHeight();
        }
   }
   return 0.0;
}




