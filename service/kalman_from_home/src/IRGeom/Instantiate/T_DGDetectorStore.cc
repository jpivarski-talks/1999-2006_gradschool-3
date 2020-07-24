// -*- C++ -*-
//
// Package:     package
// Module:      T_IRGeometryProxy
// 
// Description: Instantiate Classes for DGDetectorStore
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lawrence Gibbons
// Created:     Mon Jan  4 08:43:31 EST 1999
// $Id: T_DGDetectorStore.cc,v 1.1.1.1 1999/07/22 14:38:43 lkg Exp $
//
// Revision history
//
// $Log: T_DGDetectorStore.cc,v $
// Revision 1.1.1.1  1999/07/22 14:38:43  lkg
// First release
//
//

#include "Experiment/Experiment.h"

// user include files
#include "DetectorGeometry/DGDetectorStore.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( DGDetectorStore )
//Use the following if 'DGDetectorStore' contains a ','
// also make sure there are no blank spaces within 'DGDetectorStore'
/* INSTANTIATE_ONE_1_COMMA( DGDetectorStore ) */
#include "FrameAccess/instantiate_one.h"
