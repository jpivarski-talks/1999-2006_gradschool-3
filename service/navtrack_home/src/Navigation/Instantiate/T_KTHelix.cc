// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavRich
// 
// Description: Instantiate FItem<RichTrajectoryRadiator>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Tomasz Skwarnicki
// Created:     Sun Jul 23 22:21:56 EDT 2000
// $Id: T_KTHelix.cc,v 1.1 2000/07/27 01:32:00 ts Exp $
//
// Revision history
//
// $Log: T_KTHelix.cc,v $
// Revision 1.1  2000/07/27 01:32:00  ts
// NavRich added
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "KinematicTrajectory/KTHelix.h"

#include "FrameAccess/instantiate_macros.h" 
INSTANTIATE_ONE( KTHelix ) 

#include "FrameAccess/instantiate_one.h"

