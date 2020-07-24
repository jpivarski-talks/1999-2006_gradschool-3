// -*- C++ -*-
//
// Package:     KaonMonitorProc
// Module:      STL_vectorOfKTHelix
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Nov 20 10:33:48 EST 2003
// $Id: STL_vectorOfKTHelix.cc,v 1.1 2003/11/20 15:43:39 mccann Exp $
//
// Revision history

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "KinematicTrajectory/KTHelix.h"

// STL classes
#include <vector>


typedef KTHelix _vector_contents_;

#include "STLUtility/instantiate_vector.h"

