// -*- C++ -*-
//
// Package:     McCannProd
// Module:      T_McCannTRSeedTrack
// 
// Description: Instantiate Classes for McCannTRSeedTrack
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Oct 23 17:28:09 EDT 2003
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
//Use the following if 'McCannTRSeedTrack' contains a ','
// also make sure there are no blank spaces within 'McCannTRSeedTrack'
/* INSTANTIATE_MANY_1_COMMA( McCannTRSeedTrack ) */

//Uncomment the following if McCannTRSeedTrack ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"
