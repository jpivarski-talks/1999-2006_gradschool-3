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
// $Id: NavRich_trackRadiatorsGeom.cc,v 1.2 2001/10/01 00:37:08 cdj Exp $
//
// Revision history
//
// $Log: NavRich_trackRadiatorsGeom.cc,v $
// Revision 1.2  2001/10/01 00:37:08  cdj
// switched to RichAngles package
//
// Revision 1.1  2000/07/27 01:31:46  ts
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

//
// constants, enums and typedefs
//

static const float kCmToM = 0.01;

      enum PositionAndDirectionKey {kLocalX=0,
                                    kLocalY,
                                    kLocalZ,
                                    kLocalCosX,
                                    kLocalCosY,
                                    kLocalCosZ};

//
// static data member definitions
//

//
// constructors and destructor
//


  extern "C" void rico_track_proj_( const FDouble* t1, const FDouble* t1cov,
                                    const FDouble* origin, const FDouble* axes,
                                    FDouble* t2, FDouble* t2cov); 
  extern "C" void rich_radiator_coord_( const FInteger& id,
                                        FDouble* origin, FDouble* axes );


HepPoint3D NavRich::trackRadiatorIntersect( RichHypothesisIDType iHyp ) const
{
   if( iHyp < RichHypotheses::kHypMax ){

    FAItem<RichTrajectoryRadiator> trajectoryRadiator( richTrajectoryRadiator(iHyp) );
    if( trajectoryRadiator.valid() ){

                 m_server.geomUpdate();

                 FDouble t[6];
                 t[0] = trajectoryRadiator->x();  // these are in master reference system 
                 t[1] = trajectoryRadiator->y();
                 t[2] = trajectoryRadiator->z();
                 t[3] = trajectoryRadiator->Dx();
                 t[4] = trajectoryRadiator->Dy();
                 t[5] = trajectoryRadiator->Dz();
                 FDouble tcov[6*6]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//dummy

                 FDouble origin[3], axis[3*3];
                 FInteger radiator=trajectoryRadiator->radiatorID();
                 rich_radiator_coord_( radiator, origin, axis );

                 FDouble tn[6],tncov[6*6];
                 rico_track_proj_( t, tcov, origin, axis, tn, tncov ); 

                 return HepPoint3D(tn[kLocalX]*kCmToM,tn[kLocalY]*kCmToM,tn[kLocalZ]*kCmToM);
    }

   }
    return HepPoint3D(0.0,0.0,0.0);
}

HepVector3D NavRich::trackDirectionAtRadiatorIntersect( RichHypothesisIDType iHyp ) const
{
   if( iHyp < RichHypotheses::kHypMax ){

    FAItem<RichTrajectoryRadiator> trajectoryRadiator( richTrajectoryRadiator(iHyp) );
    if( trajectoryRadiator.valid() ){

                 m_server.geomUpdate();

                 FDouble t[6];
                 t[0] = trajectoryRadiator->x();  // these are in master reference system 
                 t[1] = trajectoryRadiator->y();
                 t[2] = trajectoryRadiator->z();
                 t[3] = trajectoryRadiator->Dx();
                 t[4] = trajectoryRadiator->Dy();
                 t[5] = trajectoryRadiator->Dz();
                 FDouble tcov[6*6]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//dummy

                 FDouble origin[3], axis[3*3];
                 FInteger radiator=trajectoryRadiator->radiatorID();
                 rich_radiator_coord_( radiator, origin, axis );

                 FDouble tn[6],tncov[6*6];
                 rico_track_proj_( t, tcov, origin, axis, tn, tncov ); 

                 return HepVector3D(tn[kLocalCosX],tn[kLocalCosY],tn[kLocalCosZ]);
    }

   }
    return HepVector3D(0.0,0.0,0.0);
}


