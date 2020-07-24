// -*- C++ -*-
//
// Package:     <TrackFitter>
// Module:      LatticeExitProtonFitCathodeHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Feb  7 02:35:33 EST 2000
// $Id: T_LatticeLinkExitProtonFitCathodeHitLatticeStorageHelper.cc,v 1.1 2000/02/24 17:29:05 wsun Exp $
//
// Revision history
//
// $Log: T_LatticeLinkExitProtonFitCathodeHitLatticeStorageHelper.cc,v $
// Revision 1.1  2000/02/24 17:29:05  wsun
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
#include "TrackFitter/TrackFitCathodeHitLink.h"
#include "TrackFitter/Storage/Template/LatticeLinkTrackFitCathodeHitLatticeStorageHelper.cc"

INSTANTIATESTORE_TRACKHITLATTICELINK(DBCandidate::kExitProton,ExitProton,Cathode)

#include "StorageManagement/instantiate_one_typetag.h"

#if !defined(SUNCC)
#include "StorageManagement/instantiate_latticelinkhelper.h"
#endif
