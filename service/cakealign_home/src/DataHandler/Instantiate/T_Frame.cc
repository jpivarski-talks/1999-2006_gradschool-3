// -*- C++ -*-
//
// Package:     package
// Module:      T_SubFrame
// 
// Description: Instantiate Classes for Frame
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Mon Dec 11 17:46:28 EST 2000
// $Id: T_Frame.cc,v 1.1 2000/12/16 18:01:02 cdj Exp $
//
// Revision history
//
// $Log: T_Frame.cc,v $
// Revision 1.1  2000/12/16 18:01:02  cdj
// add instantiations necessary to extract a Frame from a Record (needed for SubFrame)
//
//

#include "Experiment/Experiment.h"

// user include files
#include "DataHandler/Frame.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( Frame )
//Use the following if 'Frame' contains a ','
// also make sure there are no blank spaces within 'Frame'
/* INSTANTIATE_ONE_1_COMMA( Frame ) */
#include "FrameAccess/instantiate_one.h"

