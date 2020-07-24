// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_TRHelixExitKaonFit
// 
// Description: Instantiate Classes for TRHelixExitKaonFit
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
#include "TrackRoot/TRHelixExitKaonFit.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( TRHelixExitKaonFit )
//Use the following if 'TRHelixExitKaonFit' contains a ','
// also make sure there are no blank spaces within 'TRHelixExitKaonFit'
/* INSTANTIATE_MANY_1_COMMA( TRHelixExitKaonFit ) */

//Uncomment the following if TRHelixExitKaonFit ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"
