// -*- C++ -*-
//
// Package:     <ZDWeight2LayerThetaFileSourceFormat>
// Module:      ZDWeight2LayerThetaFileSourceFormatFactory
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri Mar 12 13:32:39 EST 1999
// $Id: ZDWeight2LayerThetaFileSourceFactory.cc,v 1.1.1.1 2003/09/30 17:11:57 mccann Exp $
//
// Revision history
//
// $Log: ZDWeight2LayerThetaFileSourceFactory.cc,v $
// Revision 1.1.1.1  2003/09/30 17:11:57  mccann
// imported ZDWeight2LayerThetaFileSourceFormat sources
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
#include "ZDWeight2LayerThetaFileSourceFormat/ZDWeight2LayerThetaInfo.h"
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
   return new CFSourceFormat<ZDWeight2LayerThetaInfo>;
}
// ------------------------ end factory method ------------------
