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
// $Id: NavRich_trackPhotons.cc,v 1.2 2001/10/01 00:37:07 cdj Exp $
//
// Revision history
//
// $Log: NavRich_trackPhotons.cc,v $
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
#include "RichAngles/RichTrackPhotons.h"
#include "RichAngles/RichHypPhotons.h"
#include "RichAngles/RichPhotonAngles.h"
#include "RichAngles/RichRayAngle.h"


// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
#include <algorithm>
//#include <utility>


FAItem<RichTrackPhotons>  NavRich::richTrackPhotons() const
{
  return m_server.richTrackPhotons(m_RichTrack.richTrkID());
}
                       
NavRich::RichPhotonAnglesTable  NavRich::richPhotonAnglesPtrs( RichHypothesisIDType iHyp ) const
{
   RichPhotonAnglesTable pTable;
   if( iHyp < RichHypotheses::kHypMax ){
   if( richTrackPhotons().valid() ){
   const RichTrackPhotons& trackPhotons = *richTrackPhotons();
   if( 0 != trackPhotons.hypPhotons()[iHyp] ) {
       const RichHypPhotons& richHypPhotons = *(trackPhotons.hypPhotons()[iHyp]);
       for( int iph=0; iph<richHypPhotons.numberOfPhotons(); ++iph ){
         pTable.push_back( (const RichPhotonAngles*)&(richHypPhotons.photonAngles()[iph]) );
       }
   }
   }
   }
   return pTable;
}

NavRich::RichRayAngleTable      NavRich::richRayAnglePtrs( RichHypothesisIDType iHyp ) const
{
   RichRayAngleTable pTable;
   if( iHyp < RichHypotheses::kHypMax ){
   if( richTrackPhotons().valid() ){
   const RichTrackPhotons& trackPhotons = *richTrackPhotons();
   if( 0 != trackPhotons.hypPhotons()[iHyp] ) {
       const RichHypPhotons& richHypPhotons = *(trackPhotons.hypPhotons()[iHyp]);
       for( int iph=0; iph<richHypPhotons.numberOfPhotons(); ++iph ){
            const RichPhotonAngles& richPhotonAngles = richHypPhotons.photonAngles()[iph];
            RichRayPatternIDType iRayMax = richPhotonAngles.rayPatternWithMaxLikelihood();
            pTable.push_back( (const RichRayAngle*)&(richPhotonAngles.rayAngles()[iRayMax]) );
       }
   }
   }
   }
   return pTable;
}


RichPhotonIDType NavRich::numberOfPhotonsLinked( RichHypothesisIDType iHyp ) const
{
   if( iHyp < RichHypotheses::kHypMax ){
   if( richTrackPhotons().valid() ){
   const RichTrackPhotons& trackPhotons = *richTrackPhotons();
   if( 0 != trackPhotons.hypPhotons()[iHyp] ) {
       const RichHypPhotons& richHypPhotons = *(trackPhotons.hypPhotons()[iHyp]);
       return richHypPhotons.numberOfPhotons();
   } } }
   return 0;       
}




