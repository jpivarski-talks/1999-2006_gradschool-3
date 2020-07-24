// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_TRHelixExitMuonFit
// 
// Description: Instantiate Classes for TRHelixExitMuonFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:39:50 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// user include files
#include "TrackRoot/TRHelixExitMuonFit.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( TRHelixExitMuonFit )
//Use the following if 'TRHelixExitMuonFit' contains a ','
// also make sure there are no blank spaces within 'TRHelixExitMuonFit'
/* INSTANTIATE_MANY_1_COMMA( TRHelixExitMuonFit ) */

//Uncomment the following if TRHelixExitMuonFit ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"
