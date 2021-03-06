// -*- C++ -*-
//
// Package:     McCannFilter
// Module:      instantiate_helix_store
// 
// Description: CPP macros to instantiate all the templates needed to
//               store helicies
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 00:32:04 EDT 2000
// $Id: instantiate_quality_store.h,v 1.2 2002/06/03 20:08:31 cleo3 Exp $
//
// Revision history
//
// $Log: instantiate_quality_store.h,v $
// Revision 1.2  2002/06/03 20:08:31  cleo3
// changed cpp switch from SUNCC to MULTIPLE_INSTANCE_VIRTUAL_TABLE_BUG
//
// Revision 1.1  2000/08/16 20:07:29  wsun
// First submission.
//
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
//avoid accidental instantiation of default HCTypeTagTemplate::className()
#define HETEROCONTAINER_HCTYPETAGTEMPLATE_CC
#include "McCannFilter/McCannCandidateQuality.h"
#include "McCannFilter/Storage/Template/McCannCandidateQualityStorageHelper.cc"

//
// static data member definitions
//
#include "FrameAccess/instantiate_macros.h"
#include "StorageManagement/SMDeliverer.h"
#include "StorageManagement/Template/SMStorageHelper.cc"

// part_type = DBCandidate::kChargedPion, etc.
// part_name = Electron, Muon, Pion, Kaon, Proton,
//                 ExitElectron, ExitMuon, ExitPion, ExitKaon, ExitProton

#if defined(MULTIPLE_INSTANCE_VIRTUAL_TABLE_BUG)
//if manaually instantiate SMStorageHelper on Sun, compiler complains about
//  multiple instances of SMStorageHelper's virtual table
#define INSTANTIATE_QUALITY_STORE(part_type)\
typedef McCannCandidateQuality< part_type > _one_element_; \
template class McCannCandidateQualityStorageHelper< part_type > ;

#else
#define INSTANTIATE_QUALITY_STORE(part_type)\
template class SMStorageHelper<McCannCandidateQuality<part_type> >; \
typedef McCannCandidateQuality< part_type > _one_element_; \
template class McCannCandidateQualityStorageHelper< part_type > ;
#endif


