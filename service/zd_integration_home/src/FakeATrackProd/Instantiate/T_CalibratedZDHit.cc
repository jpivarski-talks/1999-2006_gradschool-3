// -*- C++ -*-
//
// Package:     FakeATrackProd
// Module:      T_CalibratedZDHit
// 
// Description: Instantiate Classes for CalibratedZDHit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri Jul 11 10:23:36 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// user include files
#include "CalibratedData/CalibratedZDHit.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( CalibratedZDHit )
//Use the following if 'CalibratedZDHit' contains a ','
// also make sure there are no blank spaces within 'CalibratedZDHit'
/* INSTANTIATE_MANY_1_COMMA( CalibratedZDHit ) */

//Uncomment the following if CalibratedZDHit ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"
