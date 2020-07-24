// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      instantiatestore_trackhitlattice
// 
// Description: CPP macros to instantiate all the templates needed to
//               store helicies
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Werner Sun
// Created:     Sun Feb  6 23:39:18 EST 2000
// $Id: instantiatestore_trackhitlattice.h,v 1.1 2000/02/24 17:26:20 wsun Exp $
//
// Revision history
//
// $Log: instantiatestore_trackhitlattice.h,v $
// Revision 1.1  2000/02/24 17:26:20  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
//avoid accidental instantiation of default HCTypeTagTemplate::className()
#define HETEROCONTAINER_HCTYPETAGTEMPLATE_CC

//
// static data member definitions
//
#include "FrameAccess/instantiate_macros.h"
#include "StorageManagement/SMLatticeStorageHelper.h"

// particle_name = Electron, Muon, Pion, Kaon, Proton,
//                 ExitElectron, ExitMuon, ExitPion, ExitKaon, ExitProton
// hit_name = DR, SVR, SVZ, Cathode

#define INSTANTIATESTORE_TRACKHITLATTICE(particle_name,hit_name)\
static SMLatticeStorageHelper< particle_name ## Fit ## hit_name ## HitLattice>\
s_ ## particle_name ## Fit ## hit_name ## HitLatticeStorageHelper ; \
typedef particle_name ## Fit ## hit_name ## HitLattice _lattice_element_ ;
