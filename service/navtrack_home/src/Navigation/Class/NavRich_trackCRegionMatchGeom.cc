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
// $Id: NavRich_trackCRegionMatchGeom.cc,v 1.1 2000/07/27 01:31:45 ts Exp $
//
// Revision history
//
// $Log: NavRich_trackCRegionMatchGeom.cc,v $
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
#include "RichTrackClusterMatchProd/RichTrackChamberCoord.h"
#include "RichTrackClusterMatchProd/RichTrackCRegionMatch.h"
#include "RichClustChamberCoord/RichCRegionChamberCoord.h"
#include "RichClustering/RichConnectedRegion.h"


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
//
// static data member definitions
//

//
// constructors and destructor
//


HepPoint3D NavRich::matchedRichConnectedRegionPosition( RichHypothesisIDType iHyp ) const
{
  if( iHyp < RichHypotheses::kHypMax ){

   if( iHyp != m_CRHypInCache ){ getCRHypXyz(iHyp); }
   return HepPoint3D(*m_MatchedRichConnectedRegionPosition);

  }
  return HepPoint3D(0.0,0.0,0.0);
}

HepSymMatrix NavRich::matchedRichConnectedRegionPositionCov( RichHypothesisIDType iHyp ) const
{
  if( iHyp < RichHypotheses::kHypMax ){

   if( iHyp != m_CRHypInCache ){ getCRHypXyz(iHyp); }
   return HepSymMatrix(*m_MatchedRichConnectedRegionPositionCov);

  }
  return HepSymMatrix(3,0);
}


extern "C" void rico_make_crxyz_( const FInteger& connectedRegionID, FReal* xyz, FReal* xyz_cov ); 

void NavRich::getCRHypXyz(RichHypothesisIDType iHyp ) const
{
   const_cast<NavRich*>(this)->m_CRHypInCache = iHyp;
   FReal xyz[3]={0.0,0.0,0.0}, xyz_cov[3][3]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
   if( matchedRichConnectedRegionPulseHeight(iHyp)>0.0 ){
      FInteger crid = richConnectedRegion(iHyp)->crID();
      if( crid != m_CRInCache ){
          m_server.geomUpdate();
          rico_make_crxyz_( crid, &xyz[0], &xyz_cov[0][0]);
      } else { return; }
      const_cast<NavRich*>(this)->m_CRInCache = crid; 
   }
   if( 0 == m_MatchedRichConnectedRegionPosition ){ 
         const_cast<NavRich*>(this)->m_MatchedRichConnectedRegionPosition = 
             new HepPoint3D(xyz[0]*kCmToM,xyz[1]*kCmToM,xyz[2]*kCmToM); 
   } else {
         *(const_cast<NavRich*>(this)->m_MatchedRichConnectedRegionPosition) = 
                 HepPoint3D(xyz[0]*kCmToM,xyz[1]*kCmToM,xyz[2]*kCmToM); 
   }
   if( 0 == m_MatchedRichConnectedRegionPositionCov ){ const_cast<NavRich*>(this)->m_MatchedRichConnectedRegionPositionCov = new HepSymMatrix(3,0); }
   (*const_cast<NavRich*>(this)->m_MatchedRichConnectedRegionPositionCov)(1,1)= xyz_cov[0][0]*kCmToM*kCmToM;
   (*const_cast<NavRich*>(this)->m_MatchedRichConnectedRegionPositionCov)(2,1)= xyz_cov[1][0]*kCmToM*kCmToM;
   (*const_cast<NavRich*>(this)->m_MatchedRichConnectedRegionPositionCov)(3,1)= xyz_cov[2][0]*kCmToM*kCmToM;
   (*const_cast<NavRich*>(this)->m_MatchedRichConnectedRegionPositionCov)(2,2)= xyz_cov[1][1]*kCmToM*kCmToM;
   (*const_cast<NavRich*>(this)->m_MatchedRichConnectedRegionPositionCov)(3,2)= xyz_cov[2][1]*kCmToM*kCmToM;
   (*const_cast<NavRich*>(this)->m_MatchedRichConnectedRegionPositionCov)(3,3)= xyz_cov[2][2]*kCmToM*kCmToM;
}

