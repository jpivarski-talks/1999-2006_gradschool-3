// -*- C++ -*-
//
// Package:     DualTrackToUsageTagProd
// Module:      T_TRHelixElectronFit
// 
// Description: Instantiate Classes for TRHelixElectronFit
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
#include "TrackRoot/TRHelixElectronFit.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( TRHelixElectronFit )
//Use the following if 'TRHelixElectronFit' contains a ','
// also make sure there are no blank spaces within 'TRHelixElectronFit'
/* INSTANTIATE_MANY_1_COMMA( TRHelixElectronFit ) */

//Uncomment the following if TRHelixElectronFit ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"
