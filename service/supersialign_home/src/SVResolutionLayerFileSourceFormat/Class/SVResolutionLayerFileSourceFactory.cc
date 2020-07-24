// -*- C++ -*-
//
// Package:     <SVResolutionLayerFileSourceFormat>
// Module:      SVResolutionLayerFileSourceFormatFactory
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri Mar 12 13:32:39 EST 1999
// $Id: SVResolutionLayerFileSourceFactory.cc,v 1.1.1.1 2001/05/22 16:30:01 mccann Exp $
//
// Revision history
//
// $Log: SVResolutionLayerFileSourceFactory.cc,v $
// Revision 1.1.1.1  2001/05/22 16:30:01  mccann
// FileSourceFormat for layer-dependant resolutions in silicon
//
// Revision 1.1  1999/03/13 22:44:09  cdj
// First submission
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "SVResolutionLayerFileSourceFormat/SVResolutionLayerInfo.h"
#include "ConstantsFileDelivery/CFSourceFormat.h"

// STL classes
#include <set>

//
// constants, enums and typedefs
//

// --------------------------- factory method -------------------
extern "C" { 
   SourceFormat* makeSourceFormat( void ); 
}

SourceFormat* 
makeSourceFormat( void )
{
   return new CFSourceFormat<SVResolutionLayerInfo>;
}
// ------------------------ end factory method ------------------
