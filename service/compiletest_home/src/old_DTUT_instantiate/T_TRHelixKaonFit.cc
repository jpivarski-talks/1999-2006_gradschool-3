// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_TRHelixKaonFit
// 
// Description: Instantiate Classes for TRHelixKaonFit
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
#include "TrackRoot/TRHelixKaonFit.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( TRHelixKaonFit )
//Use the following if 'TRHelixKaonFit' contains a ','
// also make sure there are no blank spaces within 'TRHelixKaonFit'
/* INSTANTIATE_MANY_1_COMMA( TRHelixKaonFit ) */

//Uncomment the following if TRHelixKaonFit ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"
