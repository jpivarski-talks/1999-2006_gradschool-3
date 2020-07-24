// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      instantiate_helix_field
// 
// Description: CPP macro used to do the setup necessary to instantiate 
//  a particle hypothesis
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Feb 24 01:40:27 EST 2000
// $Id: instantiate_quality_field.h,v 1.1 2000/02/25 01:08:22 wsun Exp $
//
// Revision history
//
// $Log: instantiate_quality_field.h,v $
// Revision 1.1  2000/02/25 01:08:22  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <vector>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#undef _vector_contents_ 
#undef STLUTILITY_INSTANTIATE_VECTOR_H
#define SMSTORAGEHELPER_DO_NOT_INSTANTIATE_MAKE
//avoid accidental instantiation of default HCTypeTagTemplate::className()
#define HETEROCONTAINER_HCTYPETAGTEMPLATE_CC
#include "StorageManagement/SMFAPtrTableStorageHelper.h"
#include "TrackRoot/Storage/TRTrackCandidateFitQualityStorageHelper.h"
#include "TrackRoot/TRTrackCandidateFitQuality.h"
#include "FrameAccess/instantiate_macros.h" 

// part_type = DBCandidate::kChargedPion, etc.
// part_name = Electron, Muon, Pion, Kaon, Proton,
//                 ExitElectron, ExitMuon, ExitPion, ExitKaon, ExitProton

#define INSTANTIATE_QUALITY_FIELD(part_type, part_name)\
typedef TRTrackCandidateFitQuality< part_type > TR ## part_name ## Quality;\
INSTANTIATE_MANY( TR ## part_name ## Quality )  \
INSTANTIATE_ONE( TR ## part_name ## Quality )  \
static SMFAPtrTableStorageHelper<TR ## part_name ## Quality > \
s_ ## FATableTR ## part_name ## Quality;\
static TRTrackCandidateFitQualityStorageHelper<part_type > \
s_ ## part_name ## FitQuality ;

//NOTE: I directly create the static objects instead of using 
//  SM_REGISTER_STORAGE so I can avoid doing a typedef of the templates
//  because the typedef can cause the template to be instantiated


