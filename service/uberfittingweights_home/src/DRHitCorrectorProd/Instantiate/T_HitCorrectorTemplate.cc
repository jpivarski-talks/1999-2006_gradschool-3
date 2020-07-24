// -*- C++ -*-
//
// Package:     DRHitCorrectorProd
// Module:      T_HitCorrectorTemplate
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Aug 28 04:40:14 EDT 2003
// $Id: T_HitCorrectorTemplate.cc,v 1.1 2003/08/30 00:03:34 wsun Exp $
//
// Revision history
//
// $Log: T_HitCorrectorTemplate.cc,v $
// Revision 1.1  2003/08/30 00:03:34  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "WireHitCorrection/WHCDeviceHitCorrector.h"
#include "WireHitCorrection/Template/WHCDeviceHitCorrector.cc"

#include "CDOffCal/DriftFunction.h"
#include "ADRGeom/ADRSenseWireStore.h"

#if ( defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX) )
// for AXP
#pragma define_template WHCDeviceHitCorrector< DriftFunction, ADRSenseWireStore >
#else
template class WHCDeviceHitCorrector< DriftFunction, ADRSenseWireStore > ;
#endif /* (defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX)) */
