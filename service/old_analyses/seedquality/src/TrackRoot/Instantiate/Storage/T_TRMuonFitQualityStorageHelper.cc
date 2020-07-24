// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      T_TRElectronFitQualityStorageHelper.cc
// 
// Description: Instantiate storage helpers for
//              TRTrackCandidateFitQuality< DBCandidate::kElectron >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Werner Sun
// Created:     Thu Feb 24 02:11:37 EST 2000
// $Id: T_TRMuonFitQualityStorageHelper.cc,v 1.1 2000/02/25 01:07:25 wsun Exp $
//
// Revision history
//
// $Log: T_TRMuonFitQualityStorageHelper.cc,v $
// Revision 1.1  2000/02/25 01:07:25  wsun
// First submission.
//
//

// system include files

// user include files

//
// static data member definitions
//

#include "TrackRoot/Storage/instantiate_quality_store.h"
INSTANTIATE_QUALITY_STORE( DBCandidate::kMuon )

#include "StorageManagement/instantiate_one_typetag.h"  

