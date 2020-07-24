// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_TRHelixExitPionFit
// 
// Description: Instantiate Classes for TRHelixExitPionFit
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
#include "TrackRoot/TRHelixExitPionFit.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( TRHelixExitPionFit )
//Use the following if 'TRHelixExitPionFit' contains a ','
// also make sure there are no blank spaces within 'TRHelixExitPionFit'
/* INSTANTIATE_MANY_1_COMMA( TRHelixExitPionFit ) */

//Uncomment the following if TRHelixExitPionFit ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"
