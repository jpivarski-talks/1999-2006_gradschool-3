// -*- C++ -*-
//
// Package:     <SVResolutionLayerFileSourceFormat>
// Module:      T_CLEOConstants_DBSVResolutionLayer
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri Mar 12 15:33:00 EST 1999
// $Id: T_CLEOConstants_DBSVResolutionLayer.cc,v 1.1.1.1 2001/05/22 16:30:01 mccann Exp $
//
// Revision history
//
// $Log: T_CLEOConstants_DBSVResolutionLayer.cc,v $
// Revision 1.1.1.1  2001/05/22 16:30:01  mccann
// FileSourceFormat for layer-dependant resolutions in silicon
//
// Revision 1.2  1999/05/08 21:42:49  cdj
// now include <vector> needed by cxx
//
// Revision 1.1  1999/04/09 21:53:18  cdj
// now use CLEOConstants<> instead of DBConstants<>
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "SVResolutionLayerFileSourceFormat/SVResolutionLayerInfo.h"
#include "CLEOConstants.hxx"

// STL classes

//
// instantiate
//
#include "FrameAccess/instantiate_macros.h" 
INSTANTIATE_ONE( CLEOConstants< DBSVResolutionLayer > ) 

#include "FrameAccess/instantiate_one.h"

