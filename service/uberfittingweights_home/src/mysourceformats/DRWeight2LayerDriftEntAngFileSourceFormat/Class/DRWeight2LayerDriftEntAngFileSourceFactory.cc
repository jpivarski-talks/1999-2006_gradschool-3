// -*- C++ -*-
//
// Package:     <DRWeight2LayerDriftEntAngFileSourceFormat>
// Module:      DRWeight2LayerDriftEntAngFileSourceFormatFactory
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Sep 22 15:02:26 EDT 2003
// $Id: DRWeight2LayerDriftEntAngFileSourceFactory.cc,v 1.0 2003/09/22 15:02:26 mccann Exp $
//
// Revision history
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DRWeight2LayerDriftEntAngFileSourceFormat/DRWeight2LayerDriftEntAngInfo.h"
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
   return new CFSourceFormat<DRWeight2LayerDriftEntAngInfo>;
}
// ------------------------ end factory method ------------------
