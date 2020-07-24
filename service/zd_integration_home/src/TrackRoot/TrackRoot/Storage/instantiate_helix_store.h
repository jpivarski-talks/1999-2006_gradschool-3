// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      instantiate_helix_store
// 
// Description: CPP macros to instantiate all the templates needed to
//               store helicies
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Michael Marsh
// Created:     Fri June 26  1998
// $Id: instantiate_helix_store.h,v 1.2 2002/06/03 20:14:36 cleo3 Exp $
//
// Revision history
//
// $Log: instantiate_helix_store.h,v $
// Revision 1.2  2002/06/03 20:14:36  cleo3
// changed cpp switch from SUNCC to MULTIPLE_INSTANCE_VIRTUAL_TABLE_BUG
//
// Revision 1.1  2000/01/05 18:51:26  cdj
// first submission
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
//avoid accidental instantiation of default HCTypeTagTemplate::className()
#define HETEROCONTAINER_HCTYPETAGTEMPLATE_CC
#include "TrackRoot/TRHelixCandidateFit.h"
#include "TrackRoot/TRHelixElectronFit.h"
#include "TrackRoot/Storage/TRHelixCandidateFitStorageHelper.h"
#include "TrackRoot/Storage/Template/TRHelixCandidateFitStorageHelper.cc"

//
// static data member definitions
//
#include "FrameAccess/instantiate_macros.h"
#include "StorageManagement/SMDeliverer.h"
#include "StorageManagement/Template/SMStorageHelper.cc"

#if defined(MULTIPLE_INSTANCE_VIRTUAL_TABLE_BUG)
//if manaually instantiate SMStorageHelper on Sun, compiler complains about
//  multiple instantices of SMStorageHelper's virtual table
#define INSTANTIATE_HELIX_STORE(instantiate_particles_type, particle_name)\
typedef TRHelixCandidateFit< instantiate_particles_type > _one_element_; \
template class TRHelixCandidateFitStorageHelper< instantiate_particles_type > ;

#else
#define INSTANTIATE_HELIX_STORE(instantiate_particles_type, particle_name)\
template class SMStorageHelper<TRHelixCandidateFit<instantiate_particles_type> >; \
typedef TRHelixCandidateFit< instantiate_particles_type > _one_element_; \
template class TRHelixCandidateFitStorageHelper< instantiate_particles_type > ;
#endif


