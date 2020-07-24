// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackCandidateFitQuality
// 
// Description: Instantiate TRTrackCandidateFitQuality< kExitKaon >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Michael Marsh
// Created:     Fri Dec 18  1998
// $Id: T_TRTrackExitKaonFitQuality.cc,v 1.1 1999/05/07 22:38:21 marsh Exp $
//
// Revision history
//
// $Log: T_TRTrackExitKaonFitQuality.cc,v $
// Revision 1.1  1999/05/07 22:38:21  marsh
// Added remaining outgoing hypotheses.
//
// Revision 1.1  1998/12/18 19:37:13  marsh
// Added templated fit track quality object to provide type-differentiation
// for Frame storage.
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "TrackRoot/TRTrackCandidateFitQuality.h"
#include "TrackRoot/TRHelixElectronFit.h"

#include "TrackRoot/Template/TRTrackCandidateFitQuality.cc"

//
// static data member definitions
//

#if ( defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX) )
// for AXP
#pragma define_template TRTrackCandidateFitQuality< DBCandidate::kExitKaon >
#else
template class TRTrackCandidateFitQuality< DBCandidate::kExitKaon > ;
#endif /* (defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX)) */
