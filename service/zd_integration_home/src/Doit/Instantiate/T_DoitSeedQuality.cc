// -*- C++ -*-
//
// Package:     Doit
// Module:      T_DoitSeedQuality
// 
// Description: Instantiate Classes for DoitSeedQuality
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     24 Aug 99
// $Id: T_DoitSeedQuality.cc,v 1.3 2000/08/17 17:06:08 wsun Exp $
//
// Revision history
//
// $Log: T_DoitSeedQuality.cc,v $
// Revision 1.3  2000/08/17 17:06:08  wsun
// Added vector and set includes for OSF1.
//
// Revision 1.2  2000/08/16 19:43:21  wsun
// Updated to new TRSeedTrackQuality.
//
// Revision 1.1  1999/08/31 21:18:04  lyon
// Doit Seed Quality Instantiation
//
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Doit/DoitSeedQuality.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( DoitSeedQuality )
//Use the following if 'DoitSeedQuality' contains a ','
// also make sure there are no blank spaces within 'DoitSeedQuality'
/* INSTANTIATE_MANY_1_COMMA( DoitSeedQuality ) */
#include "FrameAccess/instantiate_many.h"

#include "Doit/Storage/ManyDoitSeedQualityStorageHelper.h"
SM_REGISTER_STORAGE(ManyDoitSeedQualityStorageHelper)
