// -*- C++ -*-
//
// Package:     KalmanFilter
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
// $Id: instantiate_quality_store.h,v 1.1 2000/08/16 20:07:29 wsun Exp $
//
// Revision history
//
// $Log: instantiate_quality_store.h,v $
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
#include "KalmanFilter/KalmanCandidateQuality.h"
#include "KalmanFilter/Storage/Template/KalmanCandidateQualityStorageHelper.cc"

//
// static data member definitions
//
#include "FrameAccess/instantiate_macros.h"
#include "StorageManagement/SMDeliverer.h"
#include "StorageManagement/Template/SMStorageHelper.cc"

// part_type = DBCandidate::kChargedPion, etc.
// part_name = Electron, Muon, Pion, Kaon, Proton,
//                 ExitElectron, ExitMuon, ExitPion, ExitKaon, ExitProton

#if defined(SUNCC)
//if manaually instantiate SMStorageHelper on Sun, compiler complains about
//  multiple instances of SMStorageHelper's virtual table
#define INSTANTIATE_QUALITY_STORE(part_type)\
typedef KalmanCandidateQuality< part_type > _one_element_; \
template class KalmanCandidateQualityStorageHelper< part_type > ;

#else
#define INSTANTIATE_QUALITY_STORE(part_type)\
template class SMStorageHelper<KalmanCandidateQuality<part_type> >; \
typedef KalmanCandidateQuality< part_type > _one_element_; \
template class KalmanCandidateQualityStorageHelper< part_type > ;
#endif


