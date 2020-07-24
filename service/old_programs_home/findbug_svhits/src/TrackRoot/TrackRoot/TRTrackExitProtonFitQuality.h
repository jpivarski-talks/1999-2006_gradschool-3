#if !defined(TRACKROOT_TRTRACKEXITPROTONFITQUALITY_H)
#define TRACKROOT_TRTRACKEXITPROTONFITQUALITY_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackExitProtonFitQuality
// 
// Description: typedef for TRTrackCandidateFitQuality< DBCandidate::kExitProton >
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon Nov 22 18:53:07 EST 1999
// $Id: TRTrackExitProtonFitQuality.h,v 1.2 2000/04/25 17:12:41 marsh Exp $
//
// Revision history
//
// $Log: TRTrackExitProtonFitQuality.h,v $
// Revision 1.2  2000/04/25 17:12:41  marsh
// Missing colon in template arg.
//
// Revision 1.1  2000/04/17 18:47:34  marsh
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
TRTrackCandidateFitQuality< DBCandidate::kExitProton >
   TRTrackExitProtonFitQuality;

#endif /* TRACKROOT_TRTRACKEXITPROTONFITQUALITY_H */
