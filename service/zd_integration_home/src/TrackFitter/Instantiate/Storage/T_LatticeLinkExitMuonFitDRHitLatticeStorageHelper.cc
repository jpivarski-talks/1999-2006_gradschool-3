// -*- C++ -*-
//
// Package:     <TrackFitter>
// Module:      LatticeExitMuonFitDRHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Feb  7 02:35:33 EST 2000
// $Id: T_LatticeLinkExitMuonFitDRHitLatticeStorageHelper.cc,v 1.3 2002/06/15 01:59:06 cdj Exp $
//
// Revision history
//
// $Log: T_LatticeLinkExitMuonFitDRHitLatticeStorageHelper.cc,v $
// Revision 1.3  2002/06/15 01:59:06  cdj
// fixed template instantiation problem with new code on Solaris 2.6
//
// Revision 1.2  2002/02/14 20:06:40  cleo3
// changed needed for Solaris8
//
// Revision 1.1  2000/02/24 17:29:01  wsun
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

INSTANTIATESTORE_TRACKHITLATTICELINK(DBCandidate::kExitMuon,ExitMuon,DR)

#include "StorageManagement/instantiate_one_typetag.h"

#if !defined(MULTIPLE_INSTANCE_VIRTUAL_TABLE_BUG) || (__SUNPRO_CC >= 0x520)
#include "StorageManagement/instantiate_latticelinkhelper.h"
#endif
