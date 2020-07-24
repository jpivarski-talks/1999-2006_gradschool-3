#if !defined(TRACKROOT_TRHELIXELECTRONFIT_H)
#define TRACKROOT_TRHELIXELECTRONFIT_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRHelixElectronFit
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Michael Marsh
// Created:     Wed Jun 24 10:41:15 EDT 1998
// $Id: TRHelixElectronFit.h,v 1.1.1.1 1998/07/20 21:16:11 marsh Exp $
//
// Revision history
//
// $Log: TRHelixElectronFit.h,v $
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
TRHelixCandidateFit< DBCandidate::kElectron > TRHelixElectronFit;

#endif /* TRACKROOT_TRHELIXELECTRONFIT_H */
