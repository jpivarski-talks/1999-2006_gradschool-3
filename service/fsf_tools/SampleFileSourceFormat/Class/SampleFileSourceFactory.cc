// -*- C++ -*-
//
// Package:     <REPLACEMIXEDCASEFileSourceFormat>
// Module:      REPLACEMIXEDCASEFileSourceFormatFactory
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      REPLACEUSERNAME
// Created:     REPLACETIMEB
// $Id: REPLACEMIXEDCASEFileSourceFactory.cc,v 1.0 REPLACETIMEA REPLACEUSERID Exp $
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
#include "REPLACEMIXEDCASEFileSourceFormat/REPLACEMIXEDCASEInfo.h"
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
   return new CFSourceFormat<REPLACEMIXEDCASEInfo>;
}
// ------------------------ end factory method ------------------
