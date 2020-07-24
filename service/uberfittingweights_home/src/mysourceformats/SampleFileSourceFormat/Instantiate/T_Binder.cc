// -*- C++ -*-
//
// Package:     <REPLACEMIXEDCASEFileSourceFormat>
// Module:      T_Binder
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      REPLACEUSERNAME
// Created:     REPLACETIMEB
// $Id: T_Binder.cc,v 1.0 REPLACETIMEA REPLACEUSERID Exp $
//
// Revision history
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "REPLACEMIXEDCASEFileSourceFormat/REPLACEMIXEDCASEInfo.h"
#include "ConstantsFileDelivery/CFSourceBinder.h"

// STL classes
#include <string>
#include <map>

//
// instantiate
//
typedef CFSourceBinder<REPLACEMIXEDCASEInfo> _binder_;
#include "DataDelivery/instantiateNotChainableBinder.h"

