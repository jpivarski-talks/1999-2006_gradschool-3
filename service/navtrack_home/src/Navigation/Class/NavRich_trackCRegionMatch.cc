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
// $Id: NavRich_trackCRegionMatch.cc,v 1.2 2000/07/31 18:43:59 ts Exp $
//
// Revision history
//
// $Log: NavRich_trackCRegionMatch.cc,v $
// Revision 1.2  2000/07/31 18:43:59  ts
// correct indexing bug
//
// Revision 1.1  2000/07/27 01:31:44  ts
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
#include "RichTrackClusterMatchProd/RichTrackChamberCoord.h"
#include "RichTrackClusterMatchProd/RichTrackCRegionMatch.h"
#include "RichClustChamberCoord/RichCRegionChamberCoord.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//


static const float kCmToM = 0.01;
static const float kLarge = 1.0E6;
//
// static data member definitions
//

//
// constructors and destructor
//

FAItem<RichTrackChamberCoord>   NavRich::richTrackChamberCoord() const
{
 return m_server.richTrackChamberCoord(m_RichTrack.richTrkID());
}

FAItem<RichTrackCRegionMatch>   NavRich::richTrackCRegionMatch() const
{
 return m_server.richTrackCRegionMatch(m_RichTrack.richTrkID());
}

FAItem<RichCRegionChamberCoord> NavRich::richCRegionChamberCoord( RichHypothesisIDType iHyp ) const
{
  if( iHyp < RichHypotheses::kHypMax ){

  if( richTrackCRegionMatch().valid() ){
    RichTrajectoryIDType itraj = m_RichTrack.hypothesisTrajectory()[iHyp];
    if( itraj != 0 ){
        RichConnectedRegionIDType crId ( richTrackCRegionMatch()->connectedRegion()[itraj-1] );
        if( 0 != crId ){
           return  m_server.richCRegionChamberCoord(crId);
	}
    }
  }

  }
  return FAItem<RichCRegionChamberCoord>(0);
}

NavRich::Meters NavRich::trackRichHitDeltaZ( RichHypothesisIDType iHyp ) const
{
  if( iHyp < RichHypotheses::kHypMax ){

  FAItem<RichCRegionChamberCoord> crCoord ( richCRegionChamberCoord( iHyp ) );
  if( crCoord.valid() ){
     RichTrajectoryIDType itraj ( m_RichTrack.hypothesisTrajectory()[iHyp] );
     if( itraj != 0 ){
        const RichPositionMomentum* trackCoord = richTrackChamberCoord()->trajectoryChamberCrossing(itraj-1);     
        if( 0 != trackCoord ){
             return (trackCoord->y() - crCoord->y())*kCmToM;
	}
     }
  }

  }
  return kLarge;
}

NavRich::Meters NavRich::trackRichHitDeltaZError( RichHypothesisIDType iHyp ) const
{
  if( iHyp < RichHypotheses::kHypMax ){

  FAItem<RichCRegionChamberCoord> crCoord ( richCRegionChamberCoord( iHyp ) );
  if( crCoord.valid() ){
     RichTrajectoryIDType itraj ( m_RichTrack.hypothesisTrajectory()[iHyp] );
     if( itraj != 0 ){
        const RichPositionMomentum* trackCoord = richTrackChamberCoord()->trajectoryChamberCrossing(itraj-1);     
        if( 0 != trackCoord ){
             return sqrt(  crCoord->covyy()+ trackCoord->covarianceElementF(2,2) )*kCmToM;
	}
     }
  }

  }
  return kLarge;
}

NavRich::Meters NavRich::trackRichHitDeltaRphi( RichHypothesisIDType iHyp ) const
{
  if( iHyp < RichHypotheses::kHypMax ){

  FAItem<RichCRegionChamberCoord> crCoord ( richCRegionChamberCoord( iHyp ) );
  if( crCoord.valid() ){
     RichTrajectoryIDType itraj ( m_RichTrack.hypothesisTrajectory()[iHyp] );
     if( itraj != 0 ){
        const RichPositionMomentum* trackCoord = richTrackChamberCoord()->trajectoryChamberCrossing(itraj-1);     
        if( 0 != trackCoord ){
             return (trackCoord->x() - crCoord->x())*kCmToM;
	}
     }
  }

  }
  return kLarge;
}

NavRich::Meters NavRich::trackRichHitDeltaRphiError( RichHypothesisIDType iHyp ) const
{
  if( iHyp < RichHypotheses::kHypMax ){

  FAItem<RichCRegionChamberCoord> crCoord ( richCRegionChamberCoord( iHyp ) );
  if( crCoord.valid() ){
     RichTrajectoryIDType itraj ( m_RichTrack.hypothesisTrajectory()[iHyp] );
     if( itraj != 0 ){
        const RichPositionMomentum* trackCoord = richTrackChamberCoord()->trajectoryChamberCrossing(itraj-1);     
        if( 0 != trackCoord ){
             return sqrt(  crCoord->covxx()+ trackCoord->covarianceElementF(1,1) )*kCmToM;
	}
     }
  }

  }
  return kLarge;
}

