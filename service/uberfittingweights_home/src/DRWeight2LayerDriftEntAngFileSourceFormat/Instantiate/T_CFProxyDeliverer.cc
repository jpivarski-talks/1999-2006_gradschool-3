// -*- C++ -*-
//
// Package:     <DRWeight2LayerDriftEntAngFileSourceFormat>
// Module:      T_CFProxyDeliverer
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri Mar 12 10:45:14 EST 1999
// $Id: T_CFProxyDeliverer.cc,v 1.1.1.1 2003/09/30 17:09:48 mccann Exp $
//
// Revision history
//
// $Log: T_CFProxyDeliverer.cc,v $
// Revision 1.1.1.1  2003/09/30 17:09:48  mccann
// imported DRWeight2LayerDriftEntAngFileSourceFormat sources
//
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
#include "DRWeight2LayerDriftEntAngFileSourceFormat/DRWeight2LayerDriftEntAngInfo.h"
#include "ConstantsFileDelivery/Template/CFProxyDeliverer.cc"

// STL classes

//
// instantiate
//
template class CFProxyDeliverer< DRWeight2LayerDriftEntAngInfo >;

