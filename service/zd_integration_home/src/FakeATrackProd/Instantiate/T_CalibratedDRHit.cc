// -*- C++ -*-
//
// Package:     FakeATrackProd
// Module:      T_CalibratedDRHit
// 
// Description: Instantiate Classes for CalibratedDRHit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri Jun 27 14:46:10 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// user include files
#include "CalibratedData/CalibratedDRHit.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( CalibratedDRHit )
//Use the following if 'CalibratedDRHit' contains a ','
// also make sure there are no blank spaces within 'CalibratedDRHit'
/* INSTANTIATE_MANY_1_COMMA( CalibratedDRHit ) */

//Uncomment the following if CalibratedDRHit ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"
