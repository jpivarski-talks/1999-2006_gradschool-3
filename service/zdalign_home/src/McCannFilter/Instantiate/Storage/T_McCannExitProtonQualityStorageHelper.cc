// -*- C++ -*-
//
// Package:     McCannFilter
// Module:      T_McCannExitProtonQualityStorageHelper.cc
// 
// Description: Instantiate storage helpers for
//              McCannCandidateQuality< DBCandidate::kElectron >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 01:18:19 EDT 2000
// $Id: T_McCannExitProtonQualityStorageHelper.cc,v 1.1 2000/08/16 20:06:27 wsun Exp $
//
// Revision history
//
// $Log: T_McCannExitProtonQualityStorageHelper.cc,v $
// Revision 1.1  2000/08/16 20:06:27  wsun
// First submission.
//
//

// system include files

// user include files

//
// static data member definitions
//

#include "McCannFilter/Storage/instantiate_quality_store.h"
INSTANTIATE_QUALITY_STORE( DBCandidate::kExitProton )

#include "StorageManagement/instantiate_one_typetag.h"  

