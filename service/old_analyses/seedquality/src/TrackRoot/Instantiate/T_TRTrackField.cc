// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrack
// 
// Description: Instantiate Classes for TRTrack
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Simon Patton
// Created:     Sat May 25 14:32:44 EDT 1996
// $Id: T_TRTrackField.cc,v 1.3 2000/01/05 19:03:32 cdj Exp $
//
// Revision history
//
// $Log: T_TRTrackField.cc,v $
// Revision 1.3  2000/01/05 19:03:32  cdj
// updated include file's directory
//
// Revision 1.2  1999/11/04 14:44:56  mkl
// added storage helper for TRTrack
//
// Revision 1.1  1999/06/24 12:43:52  marsh
// Moved DBTrack into TrackRoot.
//
// Revision 1.6  1998/09/29 19:00:15  cdj
// switched to new instantiation macro
//
// Revision 1.5  1997/12/05 04:15:30  mkl
// moved DH to FA for FrameAccess
//
// Revision 1.4  1997/08/27 03:40:07  sjp
// Updated to new name for bug flags
//
// Revision 1.3  1997/08/06 18:07:02  sjp
// Converted to use instantiation include files
//
// Revision 1.2  1997/07/09 18:02:09  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.1  1997/06/27 18:56:58  sjp
// Instantiate file for class
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackRoot/TRTrack.h"

#include "FrameAccess/instantiate_macros.h" 
INSTANTIATE_MANY( TRTrack ) 

#include "TrackRoot/Storage/ManyTRTrackStorageHelper.h"
SM_REGISTER_STORAGE( ManyTRTrackStorageHelper )

#include "FrameAccess/instantiate_many.h"
