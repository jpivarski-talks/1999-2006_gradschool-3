// -*- C++ -*-
//
// Package:     DualTrackProd
// Module:      T_DualTrackFitHelix
// 
// Description: Instantiate Classes for DualTrackFitHelix
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 16:18:33 EST 2003
// $Id: T_DualTrackFitHelix.cc,v 1.1 2003/12/15 17:56:15 mccann Exp $
//
// Revision history
//
// $Log: T_DualTrackFitHelix.cc,v $
// Revision 1.1  2003/12/15 17:56:15  mccann
// adding hit lattices for dual-constrained residuals
//
//

#include "Experiment/Experiment.h"

// user include files
#include "DualTrackProd/DualTrackFitHelix.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( DualTrackFitHelix )
//Use the following if 'DualTrackFitHelix' contains a ','
// also make sure there are no blank spaces within 'DualTrackFitHelix'
/* INSTANTIATE_MANY_1_COMMA( DualTrackFitHelix ) */

//Uncomment the following if DualTrackFitHelix ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"
