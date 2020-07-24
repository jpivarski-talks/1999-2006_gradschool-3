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
// $Id: NavRich_trackAveAngles.cc,v 1.1 2000/07/27 01:31:44 ts Exp $
//
// Revision history
//
// $Log: NavRich_trackAveAngles.cc,v $
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
#include "RichAverageAnglesProd/RichTrackAveAngles.h"
#include "RichAverageAnglesProd/RichHypAveAngle.h"

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

static const float kLarge = 1.0E6;

//
// static data member definitions
//

//
// constructors and destructor
//


FAItem<RichTrackAveAngles> NavRich::richTrackAveAngles() const
{
 return m_server.richTrackAveAngles(m_RichTrack.richTrkID());
}

RichHypothesisIDType NavRich::hypWithMaxLikelihood() const
{
   if( richTrackAveAngles().valid() ){
      return richTrackAveAngles()->hypWithMaxLikelihood();
   } else {
      return RichHypotheses::kPion;
   }
}

NavRich::ChisquareWithArbitraryOffset NavRich::hypLogLikelihood( RichHypothesisIDType iHyp ) const 
{
   if( iHyp < RichHypotheses::kHypMax ){
        if( richTrackAveAngles().valid() ){
             return richTrackAveAngles()->hypLogLikelihood()[iHyp];
	}
   }
   return kLarge;				      
}

RichPhotonIDType NavRich::hypNumberOfPhotonsInAverage( RichHypothesisIDType aHyp ) const
{
   if( aHyp < RichHypotheses::kHypMax ){

   if( 0 == m_HypNumberOfPhotonsInAverage ){
     const_cast<NavRich*>(this)->m_HypNumberOfPhotonsInAverage = new RichPhotonIDType[RichHypotheses::kHypMax];
     if( richTrackAveAngles().valid() ){
      const RichTrackAveAngles & trackAveAngles = *(richTrackAveAngles());
      for( RichHypothesisIDType iHyp=0; iHyp<RichHypotheses::kHypMax; ++iHyp ){
           const_cast<NavRich*>(this)->m_HypNumberOfPhotonsInAverage[iHyp]=0;
           RichHypAveAngle* hypAveAngle = trackAveAngles.hypAveAngle()[iHyp];
           if( 0 != hypAveAngle ){
                const_cast<NavRich*>(this)->m_HypNumberOfPhotonsInAverage[iHyp]=hypAveAngle->numberOfPhotons();
	   }
      }
     } else {
      for( RichHypothesisIDType iHyp=0; iHyp<RichHypotheses::kHypMax; ++iHyp ){
               const_cast<NavRich*>(this)->m_HypNumberOfPhotonsInAverage[iHyp]=0;
      }
     }

   }
   return m_HypNumberOfPhotonsInAverage[aHyp];

   }
   return 0;
}
  
NavRich::Unitless NavRich::hypAverageAngleStandardDeviation( RichHypothesisIDType aHyp ) const
{
   if( aHyp < RichHypotheses::kHypMax ){

   if( 0 == m_HypAverageAngleStandardDeviation ){
     const_cast<NavRich*>(this)->m_HypAverageAngleStandardDeviation = new float[RichHypotheses::kHypMax];
     if( richTrackAveAngles().valid() ){
      const RichTrackAveAngles & trackAveAngles = *(richTrackAveAngles());
      for( RichHypothesisIDType iHyp=0; iHyp<RichHypotheses::kHypMax; ++iHyp ){
           const_cast<NavRich*>(this)->m_HypAverageAngleStandardDeviation[iHyp]= kLarge;
           RichHypAveAngle* hypAveAngle = trackAveAngles.hypAveAngle()[iHyp];
           if( 0 != hypAveAngle ){
	      if( hypAveAngle->totalError() > 0 ){
                const_cast<NavRich*>(this)->m_HypAverageAngleStandardDeviation[iHyp]=hypAveAngle->thetaDeviation()/hypAveAngle->totalError();
	      }
	   }
      }
     } else {
      for( RichHypothesisIDType iHyp=0; iHyp<RichHypotheses::kHypMax; ++iHyp ){
                const_cast<NavRich*>(this)->m_HypAverageAngleStandardDeviation[iHyp]=kLarge;
      }
     }
   }
   return m_HypAverageAngleStandardDeviation[aHyp];

   }
   return kLarge;
}


NavRich::Radians  NavRich::hypAverageAngleTotalError( RichHypothesisIDType aHyp ) const
{
   if( aHyp < RichHypotheses::kHypMax ){

   if( 0 == m_HypAverageAngleTotalError ){
      const_cast<NavRich*>(this)->m_HypAverageAngleTotalError = new float[RichHypotheses::kHypMax];
      const RichTrackAveAngles & trackAveAngles = *(richTrackAveAngles());
      for( RichHypothesisIDType iHyp=0; iHyp<RichHypotheses::kHypMax; ++iHyp ){
           const_cast<NavRich*>(this)->m_HypAverageAngleTotalError[iHyp]= kLarge;
           RichHypAveAngle* hypAveAngle = trackAveAngles.hypAveAngle()[iHyp];
           if( 0 != hypAveAngle ){
                Radians error =  hypAveAngle->totalError();
                error = (error>0.0)?error:kLarge; 
                const_cast<NavRich*>(this)->m_HypAverageAngleTotalError[iHyp]=error;
	   }
      }
   }
   return m_HypAverageAngleTotalError[aHyp];

   }
   return kLarge;
}


DABoolean  NavRich::hypWasAnalyzed( RichHypothesisIDType aHyp ) const
{
   if( aHyp < RichHypotheses::kHypMax ){

   if( 0 == m_HypWasAnalyzed ){
     const_cast<NavRich*>(this)->m_HypWasAnalyzed = new DABoolean[RichHypotheses::kHypMax];
     if( richTrackAveAngles().valid() ){
      const RichTrackAveAngles & trackAveAngles = *(richTrackAveAngles());
      for( RichHypothesisIDType iHyp=0; iHyp<RichHypotheses::kHypMax; ++iHyp ){
           const_cast<NavRich*>(this)->m_HypWasAnalyzed[iHyp]= ( 0 !=trackAveAngles.hypAveAngle()[iHyp]);
      }
     } else {
      for( RichHypothesisIDType iHyp=0; iHyp<RichHypotheses::kHypMax; ++iHyp ){
           const_cast<NavRich*>(this)->m_HypWasAnalyzed[iHyp]= false;
      }
     }
   }
   return m_HypWasAnalyzed[aHyp];

   }
   return false;
}







