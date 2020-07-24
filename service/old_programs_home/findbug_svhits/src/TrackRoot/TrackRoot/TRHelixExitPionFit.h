#if !defined(TRACKROOT_TRHELIXEXITPIONFIT_H)
#define TRACKROOT_TRHELIXEXITPIONFIT_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRHelixExitPionFit
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Michael Marsh
// Created:     Wed Jun 24 10:41:15 EDT 1998
// $Id: TRHelixExitPionFit.h,v 1.1.1.1 1998/07/20 21:16:11 marsh Exp $
//
// Revision history
//
// $Log: TRHelixExitPionFit.h,v $
// Revision 1.1.1.1  1998/07/20 21:16:11  marsh
// New library for base-level track objects
//
// Revision 1.1.1.1  1998/07/13 23:28:03  marsh
// New library for track objects
//

// system include files

// user include files
#include "TrackRoot/TRHelixCandidateFit.h"

typedef
TRHelixCandidateFit< DBCandidate::kExitPion > TRHelixExitPionFit;

#endif /* TRACKROOT_TRHELIXEXITPIONFIT_H */
