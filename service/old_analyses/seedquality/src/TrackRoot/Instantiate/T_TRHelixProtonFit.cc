// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRHelixCandidateFit
// 
// Description: Instantiate TRHelixCandidateFit< kProton >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Michael Marsh
// Created:     Fri June 26  1998
// $Id: T_TRHelixProtonFit.cc,v 1.1.1.1 1998/07/20 21:16:10 marsh Exp $
//
// Revision history
//
// $Log: T_TRHelixProtonFit.cc,v $
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
#include "TrackRoot/TRHelixProtonFit.h"

#include "TrackRoot/Template/TRHelixCandidateFit.cc"

//
// static data member definitions
//

//const Stream::Type& TRHelixProtonFit::kStream = Stream::kEvent ;
//const IfdStrKey TRHelixProtonFit::kAnchor( "" ) ;

#if ( defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX) )
// for AXP
#pragma define_template TRHelixCandidateFit< DBCandidate::kProton >
#else
template class TRHelixCandidateFit< DBCandidate::kProton > ;
#endif /* (defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX)) */
