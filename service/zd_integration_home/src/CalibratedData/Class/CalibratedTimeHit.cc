// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      CalibratedTimeHit
// 
// Description:     Code for CalibratedTimeHit class.
// Implementation:  See CalibratedTimeHit.h
//
// Author:      Jon Thaler
// Created:     Tue Dec 16 12:12:41 EST 1997
//              Thu Apr 2 1998 replace Calib* with Calibrated*
//
// Revision history
// Revision  3/13/98 Inga Karliner
//   remove all RawHit information 
//   take out device from Constructor
//   add leftDistance(), rightDistance(), resolution
//   eliminate multiple TDC/CalibratedHit
//   add pointer to a vector of CalibratedHit::id from the same
//     RawHit, many TDCs
//   add singleTDC = true for single TDC hit

#include <assert.h>
#include "Experiment/Experiment.h"
#include "Experiment/types.h"
//----------------------+
// system include files |
//----------------------+
#include <stdlib.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include "C++Std/iostream.h"

//--------------------+
// user include files |
//--------------------+
#include "CalibratedData/CalibratedTimeHit.h"

//-------------------------------+
// constants, enums and typedefs |
//-------------------------------+
static const char* const kFacilityString = "CalibratedData.CalibratedTimeHit";

//-----------------------------+
// Constructors and destructor |

//-----------------------------+
//CalibratedTimeHit::CalibratedTimeHit(const Identifier id)
CalibratedTimeHit::CalibratedTimeHit(Identifier id)
             : CalibratedHit(id)
{}

CalibratedTimeHit::~CalibratedTimeHit()
{}

//-------+
// print |
//-------+
void CalibratedTimeHit::printCTH(ostream& os) const
{ CalibratedHit::printCH(os);
  //os << "CalibratedTimeHit " << endl; 
}















