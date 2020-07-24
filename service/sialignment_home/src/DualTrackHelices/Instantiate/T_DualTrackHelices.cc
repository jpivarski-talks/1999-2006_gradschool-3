// -*- C++ -*-
//
// Package:     DualTrackHelices
// Module:      T_DualTrackHelices
// 
// Description: Instantiate Classes for DualTrackHelices
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lauren Hsu
// Created:     Fri Jul  7 17:21:23 EDT 2000
// $Id: T_DualTrackHelices.cc,v 1.1.1.1 2001/01/30 23:57:19 llh14 Exp $
//
// Revision history
//
// $Log: T_DualTrackHelices.cc,v $
// Revision 1.1.1.1  2001/01/30 23:57:19  llh14
// imported DualTrackHelices sources
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DualTrackHelices/DualTrackHelices.h"

#include "FrameAccess/FAPtrTable.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_MANY( DualTrackHelices )
//Use the following if 'DualTrackHelices' contains a ','
// also make sure there are no blank spaces within 'DualTrackHelices'
/* INSTANTIATE_MANY_1_COMMA( DualTrackHelices ) */

//Uncomment the following if DualTrackHelices ::Identifier is not
// some type of unsigned integer
//#define INSTANTIATE_FATABLE_MAP
#include "FrameAccess/instantiate_many.h"

#include "DualTrackHelices/ManyDualTrackHelicesStorageHelper.h"
SM_REGISTER_STORAGE(ManyDualTrackHelicesStorageHelper)
