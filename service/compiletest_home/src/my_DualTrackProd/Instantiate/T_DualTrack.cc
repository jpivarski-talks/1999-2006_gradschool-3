// -*- C++ -*-
//
// Package:     DualTrackProd
// Module:      T_DualTrack
// 
// Description: Instantiate Classes for DualTrack
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 09:29:10 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// user include files
#include "DualTrackProd/DualTrack.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( DualTrack )
//Use the following if 'DualTrack' contains a ','
// also make sure there are no blank spaces within 'DualTrack'
/* INSTANTIATE_ONE_1_COMMA( DualTrack ) */
#include "FrameAccess/instantiate_one.h"

