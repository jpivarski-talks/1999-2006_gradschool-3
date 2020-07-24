// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_TRHelixExitProtonFit
// 
// Description: Instantiate Classes for TRHelixExitProtonFit
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
#include "TrackRoot/TRHelixExitProtonFit.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( TRHelixExitProtonFit )
//Use the following if 'TRHelixExitProtonFit' contains a ','
// also make sure there are no blank spaces within 'TRHelixExitProtonFit'
/* INSTANTIATE_MANY_1_COMMA( TRHelixExitProtonFit ) */

//Uncomment the following if TRHelixExitProtonFit ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"
