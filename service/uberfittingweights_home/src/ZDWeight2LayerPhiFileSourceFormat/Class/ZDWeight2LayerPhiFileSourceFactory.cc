// -*- C++ -*-
//
// Package:     <ZDWeight2LayerPhiFileSourceFormat>
// Module:      ZDWeight2LayerPhiFileSourceFormatFactory
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri Mar 12 13:32:39 EST 1999
// $Id: ZDWeight2LayerPhiFileSourceFactory.cc,v 1.1.1.1 2003/09/30 17:11:38 mccann Exp $
//
// Revision history
//
// $Log: ZDWeight2LayerPhiFileSourceFactory.cc,v $
// Revision 1.1.1.1  2003/09/30 17:11:38  mccann
// imported ZDWeight2LayerPhiFileSourceFormat sources
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
#include "ZDWeight2LayerPhiFileSourceFormat/ZDWeight2LayerPhiInfo.h"
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
   return new CFSourceFormat<ZDWeight2LayerPhiInfo>;
}
// ------------------------ end factory method ------------------
