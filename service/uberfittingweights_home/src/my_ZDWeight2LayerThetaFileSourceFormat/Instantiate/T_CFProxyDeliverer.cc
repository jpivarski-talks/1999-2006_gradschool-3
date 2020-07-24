// -*- C++ -*-
//
// Package:     <ZDWeight2LayerThetaFileSourceFormat>
// Module:      T_CFProxyDeliverer
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri Mar 12 10:45:14 EST 1999
// $Id: T_CFProxyDeliverer.cc,v 1.2 1999/05/08 21:42:48 cdj Exp $
//
// Revision history
//
// $Log: T_CFProxyDeliverer.cc,v $
// Revision 1.2  1999/05/08 21:42:48  cdj
// now include <vector> needed by cxx
//
// Revision 1.1  1999/03/13 22:44:26  cdj
// First submission
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "ZDWeight2LayerThetaFileSourceFormat/ZDWeight2LayerThetaInfo.h"
#include "ConstantsFileDelivery/Template/CFProxyDeliverer.cc"

// STL classes

//
// instantiate
//
template class CFProxyDeliverer< ZDWeight2LayerThetaInfo >;

