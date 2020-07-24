// -*- C++ -*-
//
// Package:     PivarskiFilter
// Module:      T_PivarskiMuonQualityStorageHelper.cc
// 
// Description: Instantiate storage helpers for
//              PivarskiCandidateQuality< DBCandidate::kElectron >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 01:18:19 EDT 2000
// $Id: T_PivarskiMuonQualityStorageHelper.cc,v 1.1 2000/08/16 20:06:28 wsun Exp $
//
// Revision history
//
// $Log: T_PivarskiMuonQualityStorageHelper.cc,v $
// Revision 1.1  2000/08/16 20:06:28  wsun
// First submission.
//
//

// system include files

// user include files

//
// static data member definitions
//

#include "PivarskiFilter/Storage/instantiate_quality_store.h"
INSTANTIATE_QUALITY_STORE( DBCandidate::kMuon )

#include "StorageManagement/instantiate_one_typetag.h"  
