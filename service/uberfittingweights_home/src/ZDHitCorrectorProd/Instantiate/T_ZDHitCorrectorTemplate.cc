// -*- C++ -*-
//
// Package:     ZDHitCorrectorProd
// Module:      T_ZDHitCorrectorTemplate
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Aug 28 04:40:14 EDT 2003
// $Id: T_ZDHitCorrectorTemplate.cc,v 1.1.1.1 2003/08/30 00:17:09 wsun Exp $
//
// Revision history
//
// $Log: T_ZDHitCorrectorTemplate.cc,v $
// Revision 1.1.1.1  2003/08/30 00:17:09  wsun
// Imported ZDHitCorrectorProd sources.
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "WireHitCorrection/WHCDeviceHitCorrector.h"
#include "WireHitCorrection/Template/WHCDeviceHitCorrector.cc"

#include "ZDDriftFunctionProd/ZDDriftFunction.h"
#include "AZDGeom/AZDSenseWireStore.h"

#if ( defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX) )
// for AXP
#pragma define_template WHCDeviceHitCorrector< ZDDriftFunction, AZDSenseWireStore >
#else
template class WHCDeviceHitCorrector< ZDDriftFunction, AZDSenseWireStore> ;
#endif /* (defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX)) */
