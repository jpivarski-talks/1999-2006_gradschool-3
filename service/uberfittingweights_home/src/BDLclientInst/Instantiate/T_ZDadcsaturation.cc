// -*- C++ -*-
//
// Package:     <BDLclientInst>
// Module:      TZDadcsaturation
// 
// Description: Instantiate templates needed to extract a DBZDadcsaturation from
//              the Frame
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed May 12 16:30:39 EDT 1999
// $Id: T_constname.cc.skel,v 1.3 1999/06/09 18:32:08 cdj Exp $
//
// Revision history
//
// $Log: T_constname.cc.skel,v $
// Revision 1.3  1999/06/09 18:32:08  cdj
// now instantiates a vector of the constants type
//
// Revision 1.2  1999/05/12 23:35:50  cdj
// added info about sequential nature of a constant
//
// Revision 1.1.1.1  1999/05/12 22:08:13  cdj
// importing code
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DBZDadcsaturation.hh"

// STL classes
#include <vector>

//
// instantiate
//
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_CONSTANTS(DBZDadcsaturation)

#include "FrameAccess/instantiate_constant.h"
#include "FrameAccess/instantiate_nonseqconstant.h"

//also need a vector of the constants
typedef DBZDadcsaturation _vector_contents_;
#include "STLUtility/instantiate_vector.h"

