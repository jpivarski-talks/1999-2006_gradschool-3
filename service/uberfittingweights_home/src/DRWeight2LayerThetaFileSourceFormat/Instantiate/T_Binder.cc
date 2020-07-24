// -*- C++ -*-
//
// Package:     <DRWeight2LayerThetaFileSourceFormat>
// Module:      T_Binder
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri Mar 12 13:56:38 EST 1999
// $Id: T_Binder.cc,v 1.1.1.1 2003/09/30 17:10:50 mccann Exp $
//
// Revision history
//
// $Log: T_Binder.cc,v $
// Revision 1.1.1.1  2003/09/30 17:10:50  mccann
// imported DRWeight2LayerThetaFileSourceFormat sources
//
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
#include "DRWeight2LayerThetaFileSourceFormat/DRWeight2LayerThetaInfo.h"
#include "ConstantsFileDelivery/CFSourceBinder.h"

// STL classes
#include <string>
#include <map>

//
// instantiate
//
typedef CFSourceBinder<DRWeight2LayerThetaInfo> _binder_;
#include "DataDelivery/instantiateNotChainableBinder.h"

