// -*- C++ -*-
//
// Package:     <ZDWeight2LayerThetaFileSourceFormat>
// Module:      T_CFSourceController
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri Mar 12 10:45:29 EST 1999
// $Id: T_CFSourceController.cc,v 1.1.1.1 2003/09/30 17:11:57 mccann Exp $
//
// Revision history
//
// $Log: T_CFSourceController.cc,v $
// Revision 1.1.1.1  2003/09/30 17:11:57  mccann
// imported ZDWeight2LayerThetaFileSourceFormat sources
//
// Revision 1.2  1999/05/08 21:42:49  cdj
// now include <vector> needed by cxx
//
// Revision 1.1  1999/03/13 22:44:27  cdj
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
#include "ConstantsFileDelivery/Template/CFSourceController.cc"

//
// instantiate
//
template class CFSourceController< ZDWeight2LayerThetaInfo >;

