// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      instantiatestore_trackhitlatticelink
// 
// Description: CPP macro used to do the setup necessary to instantiate 
//  a particle hypothesis
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Feb  6 20:47:59 EST 2000
// $Id: instantiatestore_trackhitlatticelink.h,v 1.3 2002/06/12 20:41:27 cleo3 Exp $
//
// Revision history
//
// $Log: instantiatestore_trackhitlatticelink.h,v $
// Revision 1.3  2002/06/12 20:41:27  cleo3
// updated to new template specialization syntax
//
// Revision 1.2  2000/02/25 22:40:08  cdj
// added space before template arg name and '>'
//
// Revision 1.1  2000/02/24 17:26:21  wsun
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
//avoid accidental instantiation of default HCTypeTagTemplate::className()
#define HETEROCONTAINER_HCTYPETAGTEMPLATE_CC

#include "StorageManagement/SMLatticeStorageHelper.h"
#include "StorageManagement/SMDeliverer.h"
#include "FrameAccess/instantiate_macros.h"

// part_type = DBCandidate::kChargedPion, etc.
// part_name = Electron, Muon, Pion, Kaon, Proton,
//                 ExitElectron, ExitMuon, ExitPion, ExitKaon, ExitProton
// hit_name = DR, SVR, SVZ, Cathode

// CC can't parse typedef for storage helper in SM_REGISTER_VERSION_DEF
// properly, so must use original class name!!!!!!!!!!!!!

// If manaually instantiate SMStorageHelper on Sun, compiler complains about
// multiple instances of SMStorageHelper's virtual table

//INSTANTIATE_ONE( SMLatticeLinkInfoBase<part_name ## Fit ## hit_name ## HitLattice> ) \

#define INSTANTIATESTORE_TRACKHITLATTICELINK(part_type,part_name,hit_name) \
template<> SM_REGISTER_VERSION_DEF( LatticeLinkTrackFit ## hit_name ## HitLatticeStorageHelper<part_type >, SMLatticeLinkInfoBase<part_name ## Fit ## hit_name ## HitLattice > ) \
template class \
LatticeLinkTrackFit ## hit_name ## HitLatticeStorageHelper<part_type > ; \
static LatticeLinkTrackFit ## hit_name ## HitLatticeStorageHelper<part_type > \
s_ ## part_name ## Fit ## hit_name ## HitLatticeLinkStorageHelper ; \
typedef SMLatticeLinkInfoBase<part_name ## Fit ## hit_name ## HitLattice > \
part_name ## Fit ## hit_name ## HitLatticeLink ; \
INSTANTIATE_ONE( part_name ## Fit ## hit_name ## HitLatticeLink ) \
typedef part_name ## Fit ## hit_name ## HitLattice _lattice_element_ ;

//NOTE: I directly create the static objects instead of using 
//  SM_REGISTER_STORAGE so I can avoid doing a typedef of the templates
//  because the typedef can cause the template to be instantiated


