// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_TRHelixProtonFit
// 
// Description: Instantiate Classes for TRHelixProtonFit
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
#include "TrackRoot/TRHelixProtonFit.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( TRHelixProtonFit )
//Use the following if 'TRHelixProtonFit' contains a ','
// also make sure there are no blank spaces within 'TRHelixProtonFit'
/* INSTANTIATE_MANY_1_COMMA( TRHelixProtonFit ) */

//Uncomment the following if TRHelixProtonFit ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"
