// -*- C++ -*-
//
// Package:     <DRWeight2LayerPhiFileSourceFormat>
// Module:      T_Binder
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri Mar 12 13:56:38 EST 1999
// $Id: T_Binder.cc,v 1.1 1999/03/13 22:44:25 cdj Exp $
//
// Revision history
//
// $Log: T_Binder.cc,v $
// Revision 1.1  1999/03/13 22:44:25  cdj
// First submission
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DRWeight2LayerPhiFileSourceFormat/DRWeight2LayerPhiInfo.h"
#include "ConstantsFileDelivery/CFSourceBinder.h"

// STL classes
#include <string>
#include <map>

//
// instantiate
//
typedef CFSourceBinder<DRWeight2LayerPhiInfo> _binder_;
#include "DataDelivery/instantiateNotChainableBinder.h"

