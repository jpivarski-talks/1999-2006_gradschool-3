// -*- C++ -*-
//
// Package:     <TrackFitter>
// Module:      LatticeExitKaonFitDRHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Feb  7 02:35:33 EST 2000
// $Id: T_LatticeLinkExitKaonFitDRHitLatticeStorageHelper.cc,v 1.1 2000/02/24 17:28:58 wsun Exp $
//
// Revision history
//
// $Log: T_LatticeLinkExitKaonFitDRHitLatticeStorageHelper.cc,v $
// Revision 1.1  2000/02/24 17:28:58  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "TrackFitter/Storage/instantiatestore_trackhitlatticelink.h"
#include "TrackFitter/TrackFitDRHitLink.h"
#include "TrackFitter/Storage/Template/LatticeLinkTrackFitDRHitLatticeStorageHelper.cc"

INSTANTIATESTORE_TRACKHITLATTICELINK(DBCandidate::kExitKaon,ExitKaon,DR)

#include "StorageManagement/instantiate_one_typetag.h"

#if !defined(SUNCC)
#include "StorageManagement/instantiate_latticelinkhelper.h"
#endif
