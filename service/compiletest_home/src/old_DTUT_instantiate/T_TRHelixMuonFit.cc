// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_TRHelixMuonFit
// 
// Description: Instantiate Classes for TRHelixMuonFit
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
#include "TrackRoot/TRHelixMuonFit.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( TRHelixMuonFit )
//Use the following if 'TRHelixMuonFit' contains a ','
// also make sure there are no blank spaces within 'TRHelixMuonFit'
/* INSTANTIATE_MANY_1_COMMA( TRHelixMuonFit ) */

//Uncomment the following if TRHelixMuonFit ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"
