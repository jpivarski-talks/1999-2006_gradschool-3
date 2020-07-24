#if !defined(TRACKROOT_TRTRACKEXITPIONFITQUALITY_H)
#define TRACKROOT_TRTRACKEXITPIONFITQUALITY_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackExitPionFitQuality
// 
// Description: typedef for TRTrackCandidateFitQuality< DBCandidate::kExitPion >
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon Nov 22 18:53:07 EST 1999
// $Id: TRTrackExitPionFitQuality.h,v 1.2 2000/04/25 17:12:41 marsh Exp $
//
// Revision history
//
// $Log: TRTrackExitPionFitQuality.h,v $
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
TRTrackCandidateFitQuality< DBCandidate::kExitPion > TRTrackExitPionFitQuality;

#endif /* TRACKROOT_TRTRACKEXITPIONFITQUALITY_H */
