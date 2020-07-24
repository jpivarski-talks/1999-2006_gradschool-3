// -*- C++ -*-
//
// Package:     DualTrackProd
// Module:      T_DualTrackZDHitLattice
// 
// Description: Instantiate Classes for DualTrackZDHitLattice
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 16:41:33 EST 2003
// $Id: T_DualTrackZDHitLattice.cc,v 1.1 2003/12/15 17:56:16 mccann Exp $
//
// Revision history
//
// $Log: T_DualTrackZDHitLattice.cc,v $
// Revision 1.1  2003/12/15 17:56:16  mccann
// adding hit lattices for dual-constrained residuals
//
//

#include "Experiment/Experiment.h"

// user include files
#include "DualTrackProd/DualTrackZDHitLink.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( DualTrackZDHitLattice )
//Use the following if 'DualTrackZDHitLattice' contains a ','
// also make sure there are no blank spaces within 'DualTrackZDHitLattice'
/* INSTANTIATE_ONE_1_COMMA( DualTrackZDHitLattice ) */
#include "FrameAccess/instantiate_one.h"

