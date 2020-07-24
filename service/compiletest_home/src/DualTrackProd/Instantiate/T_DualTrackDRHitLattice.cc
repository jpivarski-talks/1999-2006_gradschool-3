// -*- C++ -*-
//
// Package:     DualTrackProd
// Module:      T_DualTrackDRHitLattice
// 
// Description: Instantiate Classes for DualTrackDRHitLattice
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 16:41:33 EST 2003
// $Id: T_DualTrackDRHitLattice.cc,v 1.1 2003/12/15 17:56:14 mccann Exp $
//
// Revision history
//
// $Log: T_DualTrackDRHitLattice.cc,v $
// Revision 1.1  2003/12/15 17:56:14  mccann
// adding hit lattices for dual-constrained residuals
//
//

#include "Experiment/Experiment.h"

// user include files
#include "DualTrackProd/DualTrackDRHitLink.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( DualTrackDRHitLattice )
//Use the following if 'DualTrackDRHitLattice' contains a ','
// also make sure there are no blank spaces within 'DualTrackDRHitLattice'
/* INSTANTIATE_ONE_1_COMMA( DualTrackDRHitLattice ) */
#include "FrameAccess/instantiate_one.h"

