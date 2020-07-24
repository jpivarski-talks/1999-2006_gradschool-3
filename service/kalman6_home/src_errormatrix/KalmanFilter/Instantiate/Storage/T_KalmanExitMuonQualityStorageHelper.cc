// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      T_KalmanExitMuonQualityStorageHelper.cc
// 
// Description: Instantiate storage helpers for
//              KalmanCandidateQuality< DBCandidate::kElectron >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 01:18:19 EDT 2000
// $Id: T_KalmanExitMuonQualityStorageHelper.cc,v 1.1 2000/08/16 20:06:25 wsun Exp $
//
// Revision history
//
// $Log: T_KalmanExitMuonQualityStorageHelper.cc,v $
// Revision 1.1  2000/08/16 20:06:25  wsun
// First submission.
//
//

// system include files

// user include files

//
// static data member definitions
//

#include "KalmanFilter/Storage/instantiate_quality_store.h"
INSTANTIATE_QUALITY_STORE( DBCandidate::kExitMuon )

#include "StorageManagement/instantiate_one_typetag.h"  

