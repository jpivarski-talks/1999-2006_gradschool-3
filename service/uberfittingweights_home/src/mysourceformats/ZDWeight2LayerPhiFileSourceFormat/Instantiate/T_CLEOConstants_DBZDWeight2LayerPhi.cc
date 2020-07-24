// -*- C++ -*-
//
// Package:     <ZDWeight2LayerPhiFileSourceFormat>
// Module:      T_CLEOConstants_DBZDWeight2LayerPhi
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Sep 22 15:20:01 EDT 2003
// $Id: T_Binder.cc,v 1.0 2003/09/22 15:20:01 mccann Exp $
//
// Revision history
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "ZDWeight2LayerPhiFileSourceFormat/ZDWeight2LayerPhiInfo.h"
#include "CLEOConstants.hxx"

// STL classes

//
// instantiate
//
#include "FrameAccess/instantiate_macros.h" 
INSTANTIATE_ONE( CLEOConstants< DBZDWeight2LayerPhi > ) 

#include "FrameAccess/instantiate_one.h"

