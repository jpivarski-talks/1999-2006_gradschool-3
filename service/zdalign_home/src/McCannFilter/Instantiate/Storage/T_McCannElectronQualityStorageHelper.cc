// -*- C++ -*-
//
// Package:     McCannFilter
// Module:      T_McCannElectronQualityStorageHelper.cc
// 
// Description: Instantiate storage helpers for
//              McCannCandidateQuality< DBCandidate::kElectron >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 01:18:19 EDT 2000
// $Id: T_McCannElectronQualityStorageHelper.cc,v 1.1 2000/08/16 20:06:23 wsun Exp $
//
// Revision history
//
// $Log: T_McCannElectronQualityStorageHelper.cc,v $
// Revision 1.1  2000/08/16 20:06:23  wsun
// First submission.
//
//

// system include files

// user include files

//
// static data member definitions
//

#include "McCannFilter/Storage/instantiate_quality_store.h"
INSTANTIATE_QUALITY_STORE( DBCandidate::kElectron )

#include "StorageManagement/instantiate_one_typetag.h"  
