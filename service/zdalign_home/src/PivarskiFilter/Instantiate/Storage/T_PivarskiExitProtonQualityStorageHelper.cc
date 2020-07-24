// -*- C++ -*-
//
// Package:     PivarskiFilter
// Module:      T_PivarskiExitProtonQualityStorageHelper.cc
// 
// Description: Instantiate storage helpers for
//              PivarskiCandidateQuality< DBCandidate::kElectron >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 01:18:19 EDT 2000
// $Id: T_PivarskiExitProtonQualityStorageHelper.cc,v 1.1 2000/08/16 20:06:27 wsun Exp $
//
// Revision history
//
// $Log: T_PivarskiExitProtonQualityStorageHelper.cc,v $
// Revision 1.1  2000/08/16 20:06:27  wsun
// First submission.
//
//

// system include files

// user include files

//
// static data member definitions
//

#include "PivarskiFilter/Storage/instantiate_quality_store.h"
INSTANTIATE_QUALITY_STORE( DBCandidate::kExitProton )

#include "StorageManagement/instantiate_one_typetag.h"  

