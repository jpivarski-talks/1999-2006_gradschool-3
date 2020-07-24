// -*- C++ -*-
//
// Package:     FakeATrackProd
// Module:      T_TRSeedTrack
// 
// Description: Instantiate Classes for TRSeedTrack
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri Jun 27 14:15:41 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// user include files
#include "TrackRoot/TRSeedTrack.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( TRSeedTrack )
//Use the following if 'TRSeedTrack' contains a ','
// also make sure there are no blank spaces within 'TRSeedTrack'
/* INSTANTIATE_MANY_1_COMMA( TRSeedTrack ) */

//Uncomment the following if TRSeedTrack ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"
