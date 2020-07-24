// -*- C++ -*-
//
// Package:     DRWeightLayerDriftEntranceAngle
// Module:      DRWeightLayerDriftEntranceAngleFileSourceFormatFactory
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri Sep 12 15:15:41 EDT 2003
// $Id$
//
// Revision history
//
// $Log$

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

// user include files
//#include "Experiment/report.h"
#include "DRWeightLayerFileSourceFormat/DRWeightLayerInfo.h"
#include "ConstantsFileDelivery/CFSourceFormat.h"

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
   return new CFSourceFormat<DRWeightLayerDriftEntranceAngleInfo>;
}
// ------------------------ end factory method ------------------
