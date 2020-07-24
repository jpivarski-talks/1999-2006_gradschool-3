// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_TRHelixPionFit
// 
// Description: Instantiate Classes for TRHelixPionFit
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
#include "TrackRoot/TRHelixPionFit.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( TRHelixPionFit )
//Use the following if 'TRHelixPionFit' contains a ','
// also make sure there are no blank spaces within 'TRHelixPionFit'
/* INSTANTIATE_MANY_1_COMMA( TRHelixPionFit ) */

//Uncomment the following if TRHelixPionFit ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"
