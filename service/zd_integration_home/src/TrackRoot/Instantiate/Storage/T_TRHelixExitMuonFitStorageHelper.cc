// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRHelixCandidateFit
// 
// Description: Instantiate TRHelixCandidateFit< kExitMuon >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Michael Marsh
// Created:     Fri June 26  1998
// $Id: T_TRHelixExitMuonFitStorageHelper.cc,v 1.1 2000/01/05 18:47:04 cdj Exp $
//
// Revision history
//
// $Log: T_TRHelixExitMuonFitStorageHelper.cc,v $
// Revision 1.1  2000/01/05 18:47:04  cdj
// first submission
//
// Revision 1.1.1.1  1998/07/20 21:16:11  marsh
// New library for base-level track objects
//
// Revision 1.1.1.1  1998/07/13 23:27:59  marsh
// New library for track objects
//
//

#include "TrackRoot/Storage/instantiate_helix_store.h"
INSTANTIATE_HELIX_STORE(DBCandidate::kExitMuon,ExitMuon)
#include "StorageManagement/instantiate_one_typetag.h"  
