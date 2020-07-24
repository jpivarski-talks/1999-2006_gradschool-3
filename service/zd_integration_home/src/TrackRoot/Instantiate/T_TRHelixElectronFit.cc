// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRHelixCandidateFit
// 
// Description: Instantiate TRHelixCandidateFit< kElectron >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Michael Marsh
// Created:     Fri June 26  1998
// $Id: T_TRHelixElectronFit.cc,v 1.1.1.1 1998/07/20 21:16:11 marsh Exp $
//
// Revision history
//
// $Log: T_TRHelixElectronFit.cc,v $
// Revision 1.1.1.1  1998/07/20 21:16:11  marsh
// New library for base-level track objects
//
// Revision 1.1.1.1  1998/07/13 23:27:59  marsh
// New library for track objects
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "TrackRoot/TRHelixCandidateFit.h"
#include "TrackRoot/TRHelixElectronFit.h"

#include "TrackRoot/Template/TRHelixCandidateFit.cc"

//
// static data member definitions
//

//const Stream::Type& TRHelixElectronFit::kStream = Stream::kEvent ;
//const IfdStrKey TRHelixElectronFit::kAnchor( "" ) ;

#if ( defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX) )
// for AXP
#pragma define_template TRHelixCandidateFit< DBCandidate::kElectron >
#else
template class TRHelixCandidateFit< DBCandidate::kElectron > ;
#endif /* (defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX)) */
