// -*- C++ -*-
//
// Package:     CalibratedData
// Module:      T_CalibratedCathodeHit
// 
// Description: Instantiate Classes for CalibratedCathodeHit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Dawn Isabel
// Created:     Wed Jul  1 17:32:34 EDT 1998
// $Id: T_CalibratedCathodeHit.cc,v 1.4 2000/03/03 15:15:04 lyon Exp $
//
// Revision history
//
// $Log: T_CalibratedCathodeHit.cc,v $
// Revision 1.4  2000/03/03 15:15:04  lyon
// Lattice for connecting DRCACorrect hits (cathode raw) to cathode clusters
//
// Revision 1.3  1998/09/30 18:59:58  pg
//   Modified CalibratedSV*Hit classes.
//
// Revision 1.2  1998/09/30 17:03:41  cdj
// use new instantiate macro
//
// Revision 1.1  1998/08/07 19:05:25  isabel
// for use in CalibratedCathodeHitProxy
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "CalibratedData/CalibratedCathodeHit.h"
// STL include files
#include <map>

// Instantiate the storage helper
#include "CalibratedData/ManyCalibratedCathodeHitStorageHelper.h"
SM_REGISTER_STORAGE(ManyCalibratedCathodeHitStorageHelper)

#include "FrameAccess/instantiate_macros.h" 
INSTANTIATE_MANY( CalibratedCathodeHit ) 

#include "FrameAccess/instantiate_many.h"
