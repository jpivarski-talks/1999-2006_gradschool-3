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
// Author:      Chris D. Jones
// Created:     Wed Jun 24 14:06:59 EDT 1998
// $Id: instantiate_helix_field.h,v 1.1 2000/01/05 18:51:25 cdj Exp $
//
// Revision history
//
// $Log: instantiate_helix_field.h,v $
// Revision 1.1  2000/01/05 18:51:25  cdj
// first submission
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
#include "TrackRoot/Storage/TRHelixCandidateFitStorageHelper.h"

#include "TrackRoot/TRHelixCandidateFit.h"
#include "FrameAccess/instantiate_macros.h" 
#define INSTANTIATE_HELIX_FIELD(instantiate_particles_type, particle_name)\
typedef TRHelixCandidateFit< instantiate_particles_type > TRHelix ## particle_name ## Fit; \
INSTANTIATE_MANY( TRHelix ## particle_name ## Fit )  \
INSTANTIATE_ONE( TRHelix ## particle_name ## Fit ) \
static SMFAPtrTableStorageHelper<TRHelix ## particle_name ## Fit > s_ ## FATableTRHelix ## particle_name ## Fit;\
static TRHelixCandidateFitStorageHelper<instantiate_particles_type > s_ ## Helix ## particle_name ## Fit;

//NOTE: I directly create the static objects instead of using 
//  SM_REGISTER_STORAGE so I can avoid doing a typedef of the templates
//  because the typedef can cause the template to be instantiated


