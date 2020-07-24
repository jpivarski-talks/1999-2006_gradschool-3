// -*- C++ -*-
//
// Package:     <DRWeight2LayerPhiFileSourceFormat>
// Module:      DRWeight2LayerPhiFileSourceFormatFactory
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri Mar 12 13:32:39 EST 1999
// $Id: DRWeight2LayerPhiFileSourceFactory.cc,v 1.1 1999/03/13 22:44:09 cdj Exp $
//
// Revision history
//
// $Log: DRWeight2LayerPhiFileSourceFactory.cc,v $
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
#include "DRWeight2LayerPhiFileSourceFormat/DRWeight2LayerPhiInfo.h"
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
   return new CFSourceFormat<DRWeight2LayerPhiInfo>;
}
// ------------------------ end factory method ------------------
