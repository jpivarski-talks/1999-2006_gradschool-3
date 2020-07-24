#if !defined(TRACKROOT_TRHELIXEXITPROTONFIT_H)
#define TRACKROOT_TRHELIXEXITPROTONFIT_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRHelixExitProtonFit
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Michael Marsh
// Created:     Wed Jun 24 10:41:15 EDT 1998
// $Id: TRHelixExitProtonFit.h,v 1.1 1999/05/07 22:38:31 marsh Exp $
//
// Revision history
//
// $Log: TRHelixExitProtonFit.h,v $
// Revision 1.1  1999/05/07 22:38:31  marsh
// Added remaining outgoing hypotheses.
//
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
TRHelixCandidateFit< DBCandidate::kExitProton > TRHelixExitProtonFit;

#endif /* TRACKROOT_TRHELIXEXITPROTONFIT_H */
