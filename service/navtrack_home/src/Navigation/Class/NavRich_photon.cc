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
// $Id: NavRich_photon.cc,v 1.2 2001/10/01 00:37:07 cdj Exp $
//
// Revision history
//
// $Log: NavRich_photon.cc,v $
// Revision 1.2  2001/10/01 00:37:07  cdj
// switched to RichAngles package
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

#include "RichAngles/RichTrackPhotons.h"
#include "RichAngles/RichHypPhotons.h"
#include "RichAngles/RichPhotonAngles.h"
#include "RichAngles/RichRayAngle.h"
#include "RichPhotonsProd/RichPhoton.h"
#include "C3Rich/RichHypotheses.h"


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

NavRich::RichPhotonTable  NavRich::richPhotonPtrs( RichHypothesisIDType iHyp, const float minStdDev, const float maxStdDev ) const
{
   RichPhotonTable pTable;
   if( iHyp < RichHypotheses::kHypMax ){
   if( richTrackPhotons().valid() ){
   const RichTrackPhotons& trackPhotons = *richTrackPhotons();
   if( 0 != trackPhotons.hypPhotons()[iHyp] ) {
       const RichHypPhotons& richHypPhotons = *(trackPhotons.hypPhotons()[iHyp]);
       for( int iph=0; iph<richHypPhotons.numberOfPhotons(); ++iph ){
            const RichPhotonAngles& richPhotonAngles = richHypPhotons.photonAngles()[iph];
            RichRayPatternIDType iRayMax = richPhotonAngles.rayPatternWithMaxLikelihood();
            const RichRayAngle& angle = richPhotonAngles.rayAngles()[iRayMax];
            float std = angle.thetaDeviation()/angle.totalError();
            if( ( std > minStdDev ) && (  std <  maxStdDev ) ){
               pTable.push_back( (const RichPhoton*)(&*m_server.richPhoton( richPhotonAngles.photonID() )) );
	    }
       }
   }
   } }
   return pTable;
} 



