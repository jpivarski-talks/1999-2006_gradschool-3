// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      T_ADRCathodeStore
// 
// Description: Instantiate Classes for ADRCathodeStore
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Wed Apr  8 11:03:01 EDT 1998
// $Id: T_ADRCathodeStore.cc,v 1.4 2000/01/06 19:24:19 cdj Exp $
//
// Revision history
//
// $Log: T_ADRCathodeStore.cc,v $
// Revision 1.4  2000/01/06 19:24:19  cdj
// updated to use INSTANTIATE_ONE macro
//
// Revision 1.3  1999/12/08 18:53:21  inga
// add back T_ADRCathodeStore.cc
//
// Revision 1.1  1999/05/27 19:46:12  inga
// add Cathode Instantiation file
//
// Revision 1.1.1.1  1998/09/29 22:33:56  inga
// imported ADRGeom sources
//
// Revision 1.1.1.1  1998/05/22 16:53:02  marsh
// New DR3 library
// 
// 9/10/98 I.Karliner add pair instantiation

#include "Experiment/Experiment.h"

// system include files
#include <utility>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ADRGeom/ADRCathodeStore.h"

#include "FrameAccess/instantiate_macros.h"

INSTANTIATE_ONE(ADRCathodeStore)

#include "FrameAccess/instantiate_one.h"
