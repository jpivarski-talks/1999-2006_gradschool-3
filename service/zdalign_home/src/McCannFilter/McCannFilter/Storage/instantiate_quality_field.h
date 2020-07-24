// -*- C++ -*-
//
// Package:     McCannFilter
// Module:      instantiate_helix_field
// 
// Description: CPP macro used to do the setup necessary to instantiate 
//  a particle hypothesis
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 00:32:04 EDT 2000
// $Id: instantiate_quality_field.h,v 1.1 2000/08/16 20:07:28 wsun Exp $
//
// Revision history
//
// $Log: instantiate_quality_field.h,v $
// Revision 1.1  2000/08/16 20:07:28  wsun
// First submission.
//
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
#include "McCannFilter/Storage/McCannCandidateQualityStorageHelper.h"
#include "McCannFilter/McCannCandidateQuality.h"
#include "FrameAccess/instantiate_macros.h" 

// part_type = DBCandidate::kChargedPion, etc.
// part_name = Electron, Muon, Pion, Kaon, Proton,
//                 ExitElectron, ExitMuon, ExitPion, ExitKaon, ExitProton

#define INSTANTIATE_QUALITY_FIELD(part_type, part_name)\
typedef McCannCandidateQuality< part_type > McCann ## part_name ## Quality;\
INSTANTIATE_MANY( McCann ## part_name ## Quality )  \
INSTANTIATE_ONE( McCann ## part_name ## Quality )  \
static SMFAPtrTableStorageHelper<McCann ## part_name ## Quality > \
s_ ## FATableMcCann ## part_name ## Quality;\
static McCannCandidateQualityStorageHelper<part_type > \
s_ ## part_name ## McCannFitQuality ;

//NOTE: I directly create the static objects instead of using 
//  SM_REGISTER_STORAGE so I can avoid doing a typedef of the templates
//  because the typedef can cause the template to be instantiated


