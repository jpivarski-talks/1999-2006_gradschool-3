// -*- C++ -*-
//
// Package:     DRGeom
// Module:      T_G3iExtraVolSpecsStore
// 
// Description: Instantiate Classes for G3iExtraVolSpecsStore
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Aren Jansen
// Created:     Thu Jun 17 17:47:14 EDT 1999
// $Id: T_G3iExtraVolSpecsStore.cc,v 1.1.1.1 1999/07/22 14:38:43 lkg Exp $
//
// Revision history
//
// $Log: T_G3iExtraVolSpecsStore.cc,v $
// Revision 1.1.1.1  1999/07/22 14:38:43  lkg
// First release
//
//

#include "Experiment/Experiment.h"

// user include files
#include <map>
#include "Geant3Interface/G3iExtraVolSpecsStore.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( G3iExtraVolSpecsStore )
//Use the following if 'G3iExtraVolSpecsStore' contains a ','
// also make sure there are no blank spaces within 'G3iExtraVolSpecsStore'
/* INSTANTIATE_ONE_1_COMMA( G3iExtraVolSpecsStore ) */
#include "FrameAccess/instantiate_one.h"

