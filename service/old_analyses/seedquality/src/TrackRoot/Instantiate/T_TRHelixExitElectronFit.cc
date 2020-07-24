// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRHelixCandidateFit
// 
// Description: Instantiate TRHelixCandidateFit< kExitElectron >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Michael Marsh
// Created:     Fri June 26  1998
// $Id: T_TRHelixExitElectronFit.cc,v 1.1 1999/05/07 22:38:13 marsh Exp $
//
// Revision history
//
// $Log: T_TRHelixExitElectronFit.cc,v $
// Revision 1.1  1999/05/07 22:38:13  marsh
// Added remaining outgoing hypotheses.
//
// Revision 1.1.1.1  1998/07/20 21:16:10  marsh
// New library for base-level track objects
//
// Revision 1.1.1.1  1998/07/13 23:28:00  marsh
// New library for track objects
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "TrackRoot/TRHelixCandidateFit.h"
#include "TrackRoot/TRHelixExitElectronFit.h"

#include "TrackRoot/Template/TRHelixCandidateFit.cc"

//
// static data member definitions
//

//const Stream::Type& TRHelixExitElectronFit::kStream = Stream::kEvent ;
//const IfdStrKey TRHelixExitElectronFit::kAnchor( "" ) ;

#if ( defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX) )
// for AXP
#pragma define_template TRHelixCandidateFit< DBCandidate::kExitElectron >
#else
template class TRHelixCandidateFit< DBCandidate::kExitElectron > ;
#endif /* (defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX)) */
