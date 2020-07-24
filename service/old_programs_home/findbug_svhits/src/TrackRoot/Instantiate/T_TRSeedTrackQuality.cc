// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      T_TrackMany
// 
// Description: Instantiate Classes for TRSeedTrackQuality
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Jul 31 00:21:52 EDT 2000
// $Id: T_TRSeedTrackQuality.cc,v 1.1 2000/08/01 23:25:00 wsun Exp $
//
// Revision history
//
// $Log: T_TRSeedTrackQuality.cc,v $
// Revision 1.1  2000/08/01 23:25:00  wsun
// First submission.
//
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <vector>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include <map>
#include <vector>
#include <set>

// user include files
#include "TrackRoot/TRSeedTrackQuality.h"

#include "FrameAccess/instantiate_macros.h" 
INSTANTIATE_MANY( TRSeedTrackQuality ) 

#include "FrameAccess/instantiate_many.h"

#include "TrackRoot/Storage/ManyTRSeedTrackQualityStorageHelper.h"
SM_REGISTER_STORAGE(ManyTRSeedTrackQualityStorageHelper)
