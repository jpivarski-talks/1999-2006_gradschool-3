#if !defined(TRACKROOT_TRTRACKPROTONFITQUALITY_H)
#define TRACKROOT_TRTRACKPROTONFITQUALITY_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackProtonFitQuality
// 
// Description: typedef for TRTrackCandidateFitQuality< DBCandidate::kProton >
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon Nov 22 18:53:07 EST 1999
// $Id: TRTrackProtonFitQuality.h,v 1.1 2000/04/17 18:47:35 marsh Exp $
//
// Revision history
//
// $Log: TRTrackProtonFitQuality.h,v $
// Revision 1.1  2000/04/17 18:47:35  marsh
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
TRTrackCandidateFitQuality< DBCandidate::kProton > TRTrackProtonFitQuality;

#endif /* TRACKROOT_TRTRACKPROTONFITQUALITY_H */
