#if !defined(TRACKROOT_TRTRACKELECTRONFITQUALITY_H)
#define TRACKROOT_TRTRACKELECTRONFITQUALITY_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackElectronFitQuality
// 
// Description: typedef for TRTrackCandidateFitQuality< DBCandidate::kElectron >
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon Nov 22 18:53:07 EST 1999
// $Id: TRTrackElectronFitQuality.h,v 1.1 2000/04/17 18:47:33 marsh Exp $
//
// Revision history
//
// $Log: TRTrackElectronFitQuality.h,v $
// Revision 1.1  2000/04/17 18:47:33  marsh
// Re-named candidate quality typedef's
//
// Revision 1.1  1999/11/23 00:01:31  wsun
// First submission.
//
//
//

// user include files
#include "CleoDB/DBCandidate.h"
#include "TrackRoot/TRTrackCandidateFitQuality.h"

typedef
TRTrackCandidateFitQuality< DBCandidate::kElectron > TRTrackElectronFitQuality;

#endif /* TRACKROOT_TRTRACKELECTRONFITQUALITY_H */
