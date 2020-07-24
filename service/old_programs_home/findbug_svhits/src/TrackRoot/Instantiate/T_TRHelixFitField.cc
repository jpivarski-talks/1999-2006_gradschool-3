// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRHelixFit
// 
// Description: Instantiate Classes for TRHelixFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Wed Jun 24 14:06:59 EDT 1998
// $Id: T_TRHelixFitField.cc,v 1.2 1998/09/30 17:06:21 cdj Exp $
//
// Revision history
//
// $Log: T_TRHelixFitField.cc,v $
// Revision 1.2  1998/09/30 17:06:21  cdj
// use new instantiate macro
//
// Revision 1.1.1.1  1998/07/20 21:16:11  marsh
// New library for base-level track objects
//
// Revision 1.1.1.1  1998/07/13 23:28:01  marsh
// New library for track objects
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackRoot/TRHelixFit.h"

#include "FrameAccess/instantiate_macros.h" 
INSTANTIATE_MANY( TRHelixFit ) 

#include "FrameAccess/instantiate_many.h"
